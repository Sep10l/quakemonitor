#include "websocket.h"
#include <stdint.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

typedef uint8_t uint8;
typedef uint64_t uint64;
typedef uint16_t uint16;
