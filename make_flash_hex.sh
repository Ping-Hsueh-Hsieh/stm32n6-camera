#!/usr/bin/env bash
# Sign a CubeIDE app build as an STM32N6 SSBL and emit an Intel HEX at 0x70100000.
# With --merge, also combine FSBL (0x70000000) + signed app (0x70100000) +
# network weights (0x70380000) into a single flashable hex like the reference
# Binary/NUCLEO-N657X0-Q/USB-UVC-Display/*-uvc.hex.

set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"

# --- defaults ----------------------------------------------------------------
APP_BIN="$ROOT/Application/NUCLEO-N657X0-Q/STM32CubeIDE/UVCL/stm32n6-camera.bin"
OUT_HEX=""
DO_MERGE=0
FSBL_HEX="${FSBL_HEX:-$ROOT/FSBL/ai_fsbl.hex}"
NETWORK_HEX="${NETWORK_HEX:-$ROOT/Model/NUCLEO-N657X0-Q/network_atonbuf.hex}"

usage() {
  cat <<EOF
Usage: $(basename "$0") [-b <app.bin|app.elf>] [-o <out.hex>] [--merge]

Signs a NUCLEO-N657X0-Q app build as an SSBL (header v2.3, payload aligned to
0x400 like the ST prebuilt) and writes an Intel HEX starting at 0x70100000.

  --merge   also merge FSBL (0x70000000), the signed app (0x70100000) and the
            network weights (0x70380000) into one flashable hex, matching the
            structure of the reference *-uvc.hex. Default output:
            <app>-uvc.hex (override with -o).
  -b <bin|elf>   app input (default: $APP_BIN)
  -o <out.hex>   final output (sign hex, or merged hex with --merge)

Optional env overrides: FSBL_HEX, NETWORK_HEX, STM32_SIGNING_TOOL.

To program the board from dev mode (external flash loader required):
  STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el \$STLDR -hardRst -w <out.hex>
EOF
}

while (($#)); do
  case "$1" in
    -b) APP_BIN="$2"; shift 2 ;;
    -o) OUT_HEX="$2"; shift 2 ;;
    --merge) DO_MERGE=1; shift ;;
    -h|--help) usage; exit 0 ;;
    *) echo "unknown option: $1" >&2; usage >&2; exit 1 ;;
  esac
done

# --- locate STM32 signing tool ------------------------------------------------
SIGNER="${STM32_SIGNING_TOOL:-}"
if [[ -z "$SIGNER" || ! -x "$SIGNER" ]]; then
  for c in \
    "/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer.app/Contents/Resources/bin/STM32_SigningTool_CLI" \
    /usr/local/STM32CubeProgrammer/bin/STM32_SigningTool_CLI \
    /opt/ST/STM32CubeProgrammer/bin/STM32_SigningTool_CLI
  do
    if [[ -x "$c" ]]; then SIGNER="$c"; break; fi
  done
fi
if [[ -z "$SIGNER" ]]; then
  echo "error: STM32_SigningTool_CLI not found (set STM32_SIGNING_TOOL)" >&2
  exit 1
fi

# --- input -------------------------------------------------------------------
if [[ ! -e "$APP_BIN" ]]; then
  echo "error: app input not found: $APP_BIN" >&2
  exit 1
fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

case "$APP_BIN" in
  *.elf|*.axf)
    BIN="$WORK/app.bin"
    arm-none-eabi-objcopy -O binary "$APP_BIN" "$BIN" ;;
  *.bin)
    BIN="$APP_BIN" ;;
  *)
    echo "error: unsupported input type: $APP_BIN" >&2
    exit 1 ;;
esac

# --- sign + emit hex at 0x70100000 -------------------------------------------
SIGN_BIN="$WORK/app_sign.bin"
"$SIGNER" -bin "$BIN" -nk -t ssbl -hv 2.3 -align -o "$SIGN_BIN"

APP_HEX="$WORK/app_sign.hex"
arm-none-eabi-objcopy -I binary -O ihex --change-addresses 0x70100000 \
  "$SIGN_BIN" "$APP_HEX"

if ((DO_MERGE)); then
  # --- merge FSBL + signed app + network weights into one hex ------------------
  if [[ -z "$OUT_HEX" ]]; then
    OUT_HEX="${APP_BIN%.bin}-uvc.hex"
  fi
  for f in "$FSBL_HEX" "$NETWORK_HEX"; do
    if [[ ! -e "$f" ]]; then
      echo "error: missing input for merge: $f" >&2
      exit 1
    fi
  done
  python3 - "$APP_HEX" "$OUT_HEX" "$FSBL_HEX" "$NETWORK_HEX" <<'PYEOF'
import sys

def read_hex(path):
    base = 0
    out = {}
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line[0] != ':':
                continue
            bb = bytes.fromhex(line[1:])
            n = bb[0]
            addr = int.from_bytes(bb[1:3], 'big')
            rtype = bb[3]
            data = bb[4:4 + n]
            if rtype == 0:
                a = base + addr
                for i, b in enumerate(data):
                    if a + i in out and out[a + i] != b:
                        raise SystemExit(f"conflicting data at 0x{a + i:08X}")
                    out[a + i] = b
            elif rtype == 4:
                base = int.from_bytes(data, 'big') << 16
            elif rtype in (1, 3, 5):
                pass  # EOF / start segment / start linear address: no payload data
            else:
                raise SystemExit(f"unsupported record type {rtype} in {path}")
    return out

def write_hex(mapping, path):
    if not mapping:
        raise SystemExit("empty image")
    with open(path, 'w') as f:
        base = None
        addrs = sorted(mapping)
        i = 0
        while i < len(addrs):
            a = addrs[i]
            j = i
            while j + 1 < len(addrs) and addrs[j + 1] == addrs[j] + 1:
                j += 1
            cur = a
            while cur <= addrs[j]:
                if (cur >> 16) != base:
                    base = cur >> 16
                    rec = bytes([2, 0, 0, 4, (base >> 8) & 0xff, base & 0xff])
                    ck = (256 - (sum(rec) & 0xff)) & 0xff
                    f.write(':' + rec.hex().upper() + f'{ck:02X}\n')
                end = min(cur + 15, addrs[j])
                data = bytes(mapping[x] for x in range(cur, end + 1))
                rec = bytes([len(data), (cur >> 8) & 0xff, cur & 0xff, 0]) + data
                ck = (256 - (sum(rec) & 0xff)) & 0xff
                f.write(':' + rec.hex().upper() + f'{ck:02X}\n')
                cur = end + 1
            i = j + 1
        f.write(':00000001FF\n')

app, fsbl, net = sys.argv[1], sys.argv[3], sys.argv[4]
merged = {}
for src in (fsbl, app, net):
    merged.update(read_hex(src))
write_hex(merged, sys.argv[2])
print(f"merged {fsbl} + {app} + {net} -> {sys.argv[2]}")
PYEOF
else
  if [[ -z "$OUT_HEX" ]]; then
    OUT_HEX="${APP_BIN%.bin}-sign.hex"
  fi
  cp "$APP_HEX" "$OUT_HEX"
  echo "wrote $OUT_HEX (app at 0x70100000)"
  echo "  - Signed with: $SIGNER"
  echo "  - To boot from flash also program FSBL/ai_fsbl.hex and"
  echo "    Model/NUCLEO-N657X0-Q/network_atonbuf.hex (or rerun with --merge)."
fi
