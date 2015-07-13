#pragma once

#include <cstdio>
#include <ctime>
#include <sys/prctl.h>

// Macros to shield unused vars
#ifdef __GNUC__
    #define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
    #define UNUSED(x) UNUSED_ ## x
#endif

// Compiler hints
#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

// A wrapper around printf(). It adds threadname and a timestamp to the output.
#define INFORM(d,s,...) {                                                                           \
  char name[16];                                                                                    \
  prctl(PR_GET_NAME, name, 0, 0, 0);                                                                \
  std::time_t t = std::time(NULL);                                                                  \
  char strtime[32];                                                                                 \
  std::strftime(strtime, sizeof(strtime), "%F %T", std::localtime(&t));                             \
  printf("%s | %s | " d " | %s:%03d -- " s "\n", strtime, name, __FILE__, __LINE__, ##__VA_ARGS__); \
  fflush(stdout); }
#define WRN(...)  do { INFORM("WARN", ##__VA_ARGS__);                                                        } while(0)
#define INF(...)  do { INFORM("INFO", ##__VA_ARGS__);                                                        } while(0)
#define DBG1(...) do { if(unlikely(DEBUG>0)) INFORM("DBG1", ##__VA_ARGS__); } while(0)  //dbg level 1. Can be enabled with full packet load
#define DBG2(...) do { if(unlikely(DEBUG>1)) INFORM("DBG2", ##__VA_ARGS__); } while(0)  //dbg level 2. Can be enabled with full packet load, but will degrade on error paths
#define DBG3(...) do { if(unlikely(DEBUG>2)) INFORM("DBG3", ##__VA_ARGS__); } while(0)  //dbg level 3. Will degrade in normal path
