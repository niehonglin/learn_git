#ifndef _AIRKISS_CMDS_PARSE_H_
#define _AIRKISS_CMDS_PARSE_H_

#define AIRKISS_CMD_NOFOUND -2
#define AIRKISS_CMD_ERROR -1
#define AIRKISS_CMD_OK 0

#define IPV4_PTF(addr) printf("%d.%d.%d.%d", (addr) >> 24 & 0xFF, (addr) >> 16 & 0xFF, (addr) >> 8 & 0xFF, (addr) & 0xFF)
#endif

