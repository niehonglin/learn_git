/*
 * Copyright (c) 2013-2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */


#ifndef __QCOM__UTILS_H__
#define __QCOM__UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "qcom/stdint.h"
//#include "stdarg.h"

int atoi(const char *buf);
int printf(const char *fmt, ...);
int sscanf(const char *str, const char *fmt, ...);
int snprintf(char *s, unsigned int n, const char *fmt, ...);
int qcom_snprintf(char *s, unsigned int n, const char *fmt, ...);
int sprintf(char *s, const char *fmt, ...);
int atoul(char *buf);
int tolower(int c);

//extern int cmnos_printf(const char *fmt, ...);
//#ifdef printf
//#undef printf
//#endif
//#define printf cmnos_printf
//char *strcpy(char *dest, const char *src);
//char *strncpy(char *dest, const char *src, size_t n);
//size_t strlen (const char *);
//int strcmp(const char *s1, const char *s2);
//int strncmp (const char *, const char *, size_t);
//char *strchr(const char *str, int chr);
char *strstr (const char *, const char *);
//int strcasecmp(const char *s1, const char *s2);
//int strncasecmp (const char *, const char *, size_t);

#define A_MEMSET  memset
#define A_MEMCPY  memcpy
#define A_MEMZERO(addr, size) memset((char *)(addr), (int)0, (int)(size))
#define A_MEMCMP(p1, p2, nbytes) memcmp((void *)(p1), (void *)(p2), (int)(nbytes))
#define A_MEMMOVE(dst, src, size)  memmove(dst, src, size)

#define A_STRLEN   strlen
#define A_STRCPY   strcpy
#define A_STRNCPY  strncpy
#define A_STRCMP   strcmp
#define A_STRNCMP  strncmp
#define A_STRSTR(str1,str2) strstr(str1,str2) 

#define A_LE_READ_4(p)                                              \
        ((A_UINT32)(                                                \
        (((A_UINT8 *)(p))[0]      ) | (((A_UINT8 *)(p))[1] <<  8) | \
        (((A_UINT8 *)(p))[2] << 16) | (((A_UINT8 *)(p))[3] << 24)))

#define A_LE_READ_2(p)                                              \
        ((A_UINT16)(                                                \
        (((A_UINT8 *)(p))[0]) | (((A_UINT8 *)(p))[1] <<  8)))


extern int sscanf(const char *s, const char *format, ... );
#define A_SSCANF                  sscanf


#define qcom_sprintf sprintf
#define qcom_printf printf

void qcom_thread_msleep(unsigned long ms);

#ifdef __cplusplus
}
#endif

#endif

