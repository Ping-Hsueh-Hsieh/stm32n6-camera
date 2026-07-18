#ifndef UTIL_H_
#define UTIL_H_

#define DEV_ASSERT(cond, ...) \
  do                          \
  {                           \
    while (!(cond))           \
    {                         \
      __NOP();                \
    }                         \
  } while (0)

#define UNREACHABLE(...) DEV_ASSERT(0, __VA_ARGS__)
#define TODO(...) UNREACHABLE(__VA_ARGS__)

#endif  // UTIL_H_
