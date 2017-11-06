/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"
#include "swat_parse.h"
#include "qcom/socket_api.h"
#include "swat_bench_core.h"
#include "swat_bench_iniche_1.1.2.h"
#include "swat_wmiconfig_p2p.h"
#include "qcom/qcom_ssl.h"
#include "qcom/qcom_scan.h"
#include "qcom/qcom_gpio.h"
#include "qcom/qcom_security.h"

extern int atoul(char *buf);
extern SSL_ROLE_T ssl_role;
A_UINT8 currentDeviceId = 0;

//#define RSA_VERIFY_DEMO 1
#ifdef RSA_VERIFY_DEMO
unsigned char rsa_out_buf[512]= {0};
char rsa_in_buf[] = "aaaaaaaaaaaaaaaa";
unsigned char sharkSslPrivRSAKey_ylcert_qc[] =
{
   0x30, 0x82, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00,
   0x00, 0x01, 0x00, 0x01, 0xE3, 0x84, 0x09, 0x73,
   0x93, 0x34, 0x94, 0xCD, 0x3E, 0xE9, 0xD2, 0x47,
   0xE6, 0xD6, 0x7E, 0xB8, 0xDA, 0xC3, 0x59, 0x1A,
   0x6A, 0x1E, 0x6A, 0x4A, 0xF2, 0x9A, 0x9F, 0xC0,
   0x8A, 0xCB, 0x33, 0x99, 0xAB, 0xEF, 0x20, 0x2E,
   0xCC, 0x5C, 0x2E, 0xE6, 0x2F, 0x58, 0x4B, 0xFD,
   0xE6, 0x1E, 0x02, 0x75, 0xA6, 0x8E, 0x29, 0x05,
   0xA9, 0xBA, 0x69, 0xFB, 0xC3, 0x99, 0x04, 0x7F,
   0x41, 0xCB, 0x6D, 0x86, 0x6F, 0x30, 0xFA, 0xBA,
   0xA8, 0x62, 0x23, 0x5D, 0x4B, 0xE5, 0x7F, 0xDF,
   0xB1, 0x71, 0x85, 0xB5, 0x72, 0x9A, 0x7B, 0x93,
   0x81, 0x50, 0x6D, 0x3C, 0x38, 0x98, 0xC8, 0x46,
   0x20, 0x66, 0x23, 0x1A, 0x8B, 0xFA, 0x48, 0xEC,
   0xF2, 0x31, 0x3F, 0x95, 0x9E, 0x31, 0x19, 0x18,
   0x75, 0xBF, 0xE9, 0x35, 0x97, 0x98, 0x33, 0x07,
   0xB2, 0x19, 0x5A, 0x39, 0x72, 0x47, 0x46, 0x32,
   0x3F, 0x47, 0x1F, 0x08, 0xD7, 0x4A, 0xC7, 0x2F,
   0x81, 0x25, 0xCF, 0xCF, 0x12, 0xF8, 0x43, 0x04,
   0xC0, 0x70, 0xF8, 0xC1, 0x0C, 0xA0, 0x55, 0x36,
   0xF9, 0xAC, 0xB8, 0x68, 0xB2, 0x01, 0xAC, 0x5E,
   0x0E, 0x12, 0x28, 0xEB, 0xAC, 0x7E, 0xEF, 0xC5,
   0x5D, 0x5C, 0x16, 0x9B, 0xD0, 0xC1, 0xB8, 0xCF,
   0xB0, 0x7A, 0x64, 0x4C, 0x1A, 0x58, 0x7F, 0x84,
   0xEF, 0xED, 0x68, 0xC7, 0x1D, 0x4F, 0x44, 0xF8,
   0x8E, 0x0F, 0xCA, 0x63, 0x1C, 0x35, 0x4A, 0xBB,
   0x53, 0x64, 0xB3, 0xEA, 0x48, 0x88, 0xEF, 0x5F,
   0x62, 0xAE, 0x69, 0x20, 0xB9, 0xC8, 0xA4, 0xA9,
   0x1A, 0xA4, 0x7C, 0x82, 0x06, 0x9B, 0x64, 0x48,
   0x3D, 0xE7, 0xB3, 0xA7, 0x85, 0x3F, 0x13, 0xA6,
   0xF0, 0x20, 0x0C, 0xD3, 0x7D, 0x4F, 0x48, 0x6B,
   0x39, 0x96, 0xBA, 0x2E, 0xD4, 0x5A, 0x8C, 0x1D,
   0xCD, 0xDB, 0xDE, 0xA8, 0xE4, 0xB3, 0xF9, 0xD6,
   0x7F, 0xBB, 0x79, 0x0D, 0xF3, 0x7F, 0xBB, 0xD8,
   0x39, 0xE2, 0xAA, 0x84, 0x5F, 0xBD, 0x47, 0xA1,
   0x91, 0xBF, 0x47, 0xC6, 0x7F, 0xD0, 0x57, 0x9C,
   0xDC, 0x68, 0x98, 0x42, 0x89, 0x2E, 0x88, 0x43,
   0x1A, 0x9D, 0xFA, 0x07, 0x15, 0x29, 0x5F, 0x50,
   0x71, 0xEA, 0xF3, 0x47, 0xAB, 0x47, 0xA2, 0x91,
   0x84, 0xEF, 0x82, 0x66, 0xB2, 0x0C, 0xB9, 0x6F,
   0x83, 0xA2, 0xCB, 0x8E, 0xE0, 0x35, 0x96, 0xC7,
   0x08, 0x1E, 0x43, 0xC9, 0x85, 0xF0, 0xBC, 0xBB,
   0xF6, 0xC1, 0x4E, 0x41, 0x58, 0x9C, 0xF6, 0xF8,
   0x8B, 0xF7, 0xBE, 0xC6, 0x69, 0xD4, 0xC3, 0x48,
   0x40, 0x69, 0x4D, 0x14, 0x23, 0x9A, 0xC7, 0x86,
   0x7E, 0xB1, 0x3F, 0x31, 0x24, 0x12, 0x80, 0xAC,
   0xF5, 0x95, 0x92, 0xD5, 0x9E, 0x54, 0x47, 0xEC,
   0x6D, 0xED, 0x78, 0xCF, 0x30, 0x79, 0x22, 0xB6,
   0xD8, 0x39, 0x49, 0xA4, 0xB1, 0x17, 0xA7, 0xF5,
   0x15, 0x05, 0x06, 0x41, 0xEF, 0x32, 0x3D, 0x1E,
   0x0C, 0xD4, 0x84, 0x11, 0xE1, 0x5F, 0xC5, 0x57,
   0xDC, 0x92, 0x43, 0x6F, 0x59, 0x3E, 0x8A, 0x2F,
   0xC9, 0xCD, 0x7F, 0x5C, 0xC7, 0x79, 0xEF, 0xE3,
   0x8D, 0xB5, 0x60, 0xF7, 0x54, 0x39, 0x27, 0x63,
   0x50, 0x21, 0x70, 0xF3, 0xBE, 0x47, 0x5E, 0x51,
   0xAF, 0x3D, 0x1C, 0xA4, 0x2A, 0x0D, 0x8A, 0xDE,
   0xB8, 0x16, 0x9F, 0xC3, 0x79, 0x34, 0xE3, 0xBF,
   0xFC, 0x11, 0x59, 0xB5, 0x2C, 0xAF, 0x45, 0x42,
   0xDF, 0xC9, 0x95, 0x61, 0x66, 0x75, 0x30, 0x40,
   0x1B, 0x1F, 0x4B, 0xE8, 0xE3, 0xEA, 0x58, 0x7D,
   0x81, 0xF2, 0xE5, 0x81, 0x7B, 0xB8, 0x78, 0xFF,
   0x64, 0x82, 0x18, 0x41, 0x4D, 0x02, 0x8F, 0x86,
   0x19, 0x94, 0x3F, 0x40, 0xB2, 0x8A, 0xED, 0x9C,
   0xF7, 0x82, 0x78, 0x19, 0x13, 0x60, 0x03, 0x26,
   0x39, 0xD5, 0xE1, 0x90, 0xD4, 0xF8, 0x77, 0xCF,
   0xA5, 0xFD, 0xB7, 0xCD, 0xBC, 0xD5, 0x6A, 0x4F,
   0x9D, 0x54, 0x72, 0xF8, 0x85, 0x66, 0xBC, 0x8E,
   0x48, 0x01, 0x17, 0x20, 0x91, 0x2E, 0x79, 0x43,
   0x64, 0x8A, 0x98, 0xC9, 0xC0, 0xA9, 0x1D, 0x73,
   0x2C, 0xC9, 0x26, 0x85, 0x27, 0x9C, 0x16, 0x16,
   0xC4, 0xC1, 0x78, 0x39, 0x34, 0x53, 0x31, 0x77,
   0xB8, 0xA8, 0x25, 0x70, 0x8E, 0xD3, 0xDF, 0x5A,
   0xE3, 0xB2, 0xF4, 0x39, 0xB1, 0x16, 0xDD, 0xFF,
   0x2C, 0x04, 0x8C, 0x7D, 0x54, 0x2A, 0x05, 0xB3,
   0x9F, 0x00, 0xFA, 0x37, 0x23, 0x09, 0x57, 0x5E,
   0x18, 0xED, 0xE0, 0x3C, 0xEF, 0x05, 0xD9, 0x0B,
   0xC7, 0x27, 0xE2, 0x66, 0xA3, 0xD5, 0xFC, 0xB6,
   0x08, 0xBA, 0x5C, 0x4F, 0x37, 0xB2, 0x41, 0xA7,
   0xE0, 0x2C, 0x46, 0xDA, 0xF3, 0x16, 0x59, 0x50,
   0xB9, 0x4F, 0x58, 0x49, 0x15, 0x46, 0x5B, 0xED,
   0x80, 0x71, 0xAC, 0xF2, 0xD3, 0x83, 0x8C, 0xF5,
   0x3B, 0x7F, 0xD2, 0x01, 0xD4, 0x6B, 0xE0, 0x3D,
   0x04, 0x95, 0xE2, 0x67, 0xE2, 0xA7, 0x63, 0xA1,
   0x20, 0x80, 0x40, 0x24, 0xED, 0xB3, 0x8D, 0x92,
   0xDC, 0xFB, 0x83, 0x65, 0x08, 0xBD, 0x70, 0x5D,
   0xBC, 0x66, 0x5C, 0xA3, 0xA9, 0x07, 0x4C, 0xC4,
   0x74, 0xD0, 0x0D, 0xF8, 0x58, 0x7E, 0x16, 0xF4,
   0x21, 0xD4, 0xFA, 0xCF, 0x8B, 0x8D, 0x15, 0x4B,
   0xAF, 0x5E, 0x60, 0x29, 0x7F, 0x45, 0x8A, 0x10,
   0xE0, 0x31, 0x24, 0x62, 0xB2, 0x39, 0x41, 0x20,
   0x1F, 0x8F, 0x6C, 0xFA, 0xDF, 0xBB, 0xA8, 0xCA,
   0xF5, 0x8B, 0x38, 0xA5, 0xAD, 0x6A, 0x5B, 0x85,
   0xB2, 0x56, 0xDC, 0x9C, 0xBE, 0x1B, 0xD8, 0x50,
   0xAE, 0x98, 0x9E, 0x23, 0xB7, 0xD9, 0x53, 0x4E,
   0x7A, 0xFE, 0xDD, 0xA1, 0x7E, 0x2D, 0xC4, 0xD1,
   0x06, 0x39, 0xB6, 0xA0, 0x46, 0x45, 0x98, 0xE6,
   0x64, 0xAC, 0x5D, 0xB2, 0x7A, 0x7F, 0xE2, 0x1E,
   0xE5, 0x41, 0xC9, 0xED, 0x43, 0x88, 0xA1, 0xC7,
   0x82, 0x05, 0x8B, 0x7D, 0x7A, 0xC7, 0xC9, 0xC5,
   0x34, 0xED, 0x25, 0x0B, 0xB0, 0xFC, 0x0F, 0xF6,
   0x0F, 0x7F, 0x3F, 0xE8, 0x51, 0x8A, 0xEB, 0x36,
   0x0A, 0xE5, 0x7A, 0x1F, 0x93, 0xA9, 0x08, 0xB3,
   0x68, 0x16, 0xB7, 0xF0, 0x83, 0xD6, 0xAE, 0x6F,
   0x84, 0x37, 0xA2, 0x1B, 0x40, 0xB5, 0x67, 0xD5,
   0x24, 0xEC, 0x7F, 0xEF, 0x0D, 0xB7, 0x2F, 0x43,
   0x33, 0xB1, 0x05, 0x3A, 0x08, 0x49, 0xD6, 0xAC,
   0x51, 0x48, 0xF8, 0x69, 0x1D, 0xD9, 0xA3, 0xF4,
   0xE9, 0x48, 0xBF, 0xF7, 0x17, 0x54, 0x24, 0xB7,
   0xB3, 0x2B, 0xE8, 0x52, 0xBA, 0x4C, 0x68, 0x62,
   0x5A, 0x52, 0xD4, 0x46, 0x98, 0x71, 0xCA, 0x7E,
   0x97, 0xEC, 0x0B, 0x55, 0xD8, 0x46, 0x36, 0xF5,
   0x44, 0xC9, 0xE4, 0xD9, 0x96, 0x62, 0xDE, 0x78,
   0x22, 0xFD, 0xF6, 0x0A, 0xA9, 0xB6, 0x2C, 0x6F,
   0xEF, 0xC3, 0x8E, 0x28, 0xFF, 0xFF, 0xFF, 0xFF
};

const unsigned char sharkSSL_ylcert_qc[] = {
0x30, 0x82, 0x04, 0x14, 0x30, 0x82, 0x02
, 0xFC, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x01
, 0x02, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48
, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00
, 0x30, 0x55, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03
, 0x55, 0x04, 0x06, 0x13, 0x02, 0x63, 0x6E, 0x31
, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08
, 0x0C, 0x02, 0x73, 0x68, 0x31, 0x0B, 0x30, 0x09
, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0C, 0x02, 0x73
, 0x68, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55
, 0x04, 0x0A, 0x0C, 0x02, 0x71, 0x63, 0x31, 0x0B
, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C
, 0x02, 0x71, 0x63, 0x31, 0x12, 0x30, 0x10, 0x06
, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01
, 0x09, 0x01, 0x16, 0x03, 0x73, 0x40, 0x73, 0x30
, 0x1E, 0x17, 0x0D, 0x31, 0x36, 0x30, 0x37, 0x30
, 0x37, 0x31, 0x33, 0x32, 0x30, 0x30, 0x35, 0x5A
, 0x17, 0x0D, 0x34, 0x36, 0x30, 0x36, 0x33, 0x30
, 0x31, 0x33, 0x32, 0x30, 0x30, 0x35, 0x5A, 0x30
, 0x62, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55
, 0x04, 0x06, 0x13, 0x02, 0x63, 0x6E, 0x31, 0x0B
, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0C
, 0x02, 0x73, 0x68, 0x31, 0x0B, 0x30, 0x09, 0x06
, 0x03, 0x55, 0x04, 0x07, 0x0C, 0x02, 0x73, 0x68
, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04
, 0x0A, 0x0C, 0x02, 0x71, 0x63, 0x31, 0x0B, 0x30
, 0x09, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x0C, 0x02
, 0x71, 0x63, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03
, 0x55, 0x04, 0x03, 0x0C, 0x02, 0x71, 0x63, 0x31
, 0x12, 0x30, 0x10, 0x06, 0x09, 0x2A, 0x86, 0x48
, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01, 0x16, 0x03
, 0x73, 0x40, 0x73, 0x30, 0x82, 0x01, 0x22, 0x30
, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7
, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82
, 0x01, 0x0F, 0x00, 0x30, 0x82, 0x01, 0x0A, 0x02
, 0x82, 0x01, 0x01, 0x00, 0xE3, 0x84, 0x09, 0x73
, 0x93, 0x34, 0x94, 0xCD, 0x3E, 0xE9, 0xD2, 0x47
, 0xE6, 0xD6, 0x7E, 0xB8, 0xDA, 0xC3, 0x59, 0x1A
, 0x6A, 0x1E, 0x6A, 0x4A, 0xF2, 0x9A, 0x9F, 0xC0
, 0x8A, 0xCB, 0x33, 0x99, 0xAB, 0xEF, 0x20, 0x2E
, 0xCC, 0x5C, 0x2E, 0xE6, 0x2F, 0x58, 0x4B, 0xFD
, 0xE6, 0x1E, 0x02, 0x75, 0xA6, 0x8E, 0x29, 0x05
, 0xA9, 0xBA, 0x69, 0xFB, 0xC3, 0x99, 0x04, 0x7F
, 0x41, 0xCB, 0x6D, 0x86, 0x6F, 0x30, 0xFA, 0xBA
, 0xA8, 0x62, 0x23, 0x5D, 0x4B, 0xE5, 0x7F, 0xDF
, 0xB1, 0x71, 0x85, 0xB5, 0x72, 0x9A, 0x7B, 0x93
, 0x81, 0x50, 0x6D, 0x3C, 0x38, 0x98, 0xC8, 0x46
, 0x20, 0x66, 0x23, 0x1A, 0x8B, 0xFA, 0x48, 0xEC
, 0xF2, 0x31, 0x3F, 0x95, 0x9E, 0x31, 0x19, 0x18
, 0x75, 0xBF, 0xE9, 0x35, 0x97, 0x98, 0x33, 0x07
, 0xB2, 0x19, 0x5A, 0x39, 0x72, 0x47, 0x46, 0x32
, 0x3F, 0x47, 0x1F, 0x08, 0xD7, 0x4A, 0xC7, 0x2F
, 0x81, 0x25, 0xCF, 0xCF, 0x12, 0xF8, 0x43, 0x04
, 0xC0, 0x70, 0xF8, 0xC1, 0x0C, 0xA0, 0x55, 0x36
, 0xF9, 0xAC, 0xB8, 0x68, 0xB2, 0x01, 0xAC, 0x5E
, 0x0E, 0x12, 0x28, 0xEB, 0xAC, 0x7E, 0xEF, 0xC5
, 0x5D, 0x5C, 0x16, 0x9B, 0xD0, 0xC1, 0xB8, 0xCF
, 0xB0, 0x7A, 0x64, 0x4C, 0x1A, 0x58, 0x7F, 0x84
, 0xEF, 0xED, 0x68, 0xC7, 0x1D, 0x4F, 0x44, 0xF8
, 0x8E, 0x0F, 0xCA, 0x63, 0x1C, 0x35, 0x4A, 0xBB
, 0x53, 0x64, 0xB3, 0xEA, 0x48, 0x88, 0xEF, 0x5F
, 0x62, 0xAE, 0x69, 0x20, 0xB9, 0xC8, 0xA4, 0xA9
, 0x1A, 0xA4, 0x7C, 0x82, 0x06, 0x9B, 0x64, 0x48
, 0x3D, 0xE7, 0xB3, 0xA7, 0x85, 0x3F, 0x13, 0xA6
, 0xF0, 0x20, 0x0C, 0xD3, 0x7D, 0x4F, 0x48, 0x6B
, 0x39, 0x96, 0xBA, 0x2E, 0xD4, 0x5A, 0x8C, 0x1D
, 0xCD, 0xDB, 0xDE, 0xA8, 0xE4, 0xB3, 0xF9, 0xD6
, 0x7F, 0xBB, 0x79, 0x0D, 0x02, 0x03, 0x01, 0x00
, 0x01, 0xA3, 0x81, 0xE1, 0x30, 0x81, 0xDE, 0x30
, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16
, 0x04, 0x14, 0xD6, 0x37, 0x20, 0x80, 0xEB, 0xEA
, 0xCB, 0xF3, 0xB2, 0x52, 0x16, 0xA8, 0xEF, 0x3C
, 0x7C, 0xF3, 0x61, 0x98, 0x3A, 0xB9, 0x30, 0x81
, 0x85, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x7E
, 0x30, 0x7C, 0x80, 0x14, 0x2F, 0x50, 0xF5, 0x71
, 0xAF, 0x40, 0x73, 0x3E, 0x2D, 0x89, 0x35, 0x1A
, 0x77, 0xEE, 0xE7, 0xCF, 0x3E, 0xA5, 0x7A, 0xA4
, 0xA1, 0x59, 0xA4, 0x57, 0x30, 0x55, 0x31, 0x0B
, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13
, 0x02, 0x63, 0x6E, 0x31, 0x0B, 0x30, 0x09, 0x06
, 0x03, 0x55, 0x04, 0x08, 0x0C, 0x02, 0x73, 0x68
, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04
, 0x07, 0x0C, 0x02, 0x73, 0x68, 0x31, 0x0B, 0x30
, 0x09, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x02
, 0x71, 0x63, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03
, 0x55, 0x04, 0x03, 0x0C, 0x02, 0x71, 0x63, 0x31
, 0x12, 0x30, 0x10, 0x06, 0x09, 0x2A, 0x86, 0x48
, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01, 0x16, 0x03
, 0x73, 0x40, 0x73, 0x82, 0x09, 0x00, 0xC4, 0x8C
, 0x8E, 0xFF, 0xE5, 0xC4, 0x50, 0x69, 0x30, 0x09
, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x04, 0x02, 0x30
, 0x00, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x1D, 0x0F
, 0x04, 0x04, 0x03, 0x02, 0x03, 0xA8, 0x30, 0x1D
, 0x06, 0x03, 0x55, 0x1D, 0x25, 0x04, 0x16, 0x30
, 0x14, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05
, 0x07, 0x03, 0x02, 0x06, 0x08, 0x2B, 0x06, 0x01
, 0x05, 0x05, 0x07, 0x03, 0x01, 0x30, 0x0D, 0x06
, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01
, 0x01, 0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01
, 0x00, 0x6C, 0xC3, 0xC6, 0xED, 0xBD, 0x08, 0x1A
, 0x75, 0xCB, 0x71, 0x6C, 0x34, 0x01, 0x61, 0xBE
, 0xCA, 0x2F, 0x3A, 0x8A, 0x3D, 0xCE, 0xD8, 0x40
, 0xE8, 0xCC, 0xD4, 0xE7, 0xAA, 0x9F, 0x92, 0x91
, 0x22, 0x08, 0x8B, 0x4F, 0xDF, 0xB6, 0xD3, 0x2B
, 0x8F, 0x1C, 0x08, 0xF7, 0x27, 0x27, 0x75, 0xF7
, 0x21, 0xB2, 0xFD, 0x8E, 0x85, 0x81, 0xB6, 0x8B
, 0x96, 0xA3, 0x9E, 0x4B, 0x93, 0x63, 0x4C, 0x53
, 0x44, 0x93, 0x82, 0x2A, 0x4F, 0x30, 0x60, 0xAD
, 0xA7, 0xEF, 0xAF, 0xE2, 0x3D, 0xBF, 0x18, 0x1B
, 0x1E, 0x24, 0xF5, 0xD6, 0x24, 0x67, 0x63, 0xFE
, 0xCE, 0x91, 0xD7, 0x13, 0x28, 0x29, 0xF2, 0x65
, 0xD9, 0xF2, 0x98, 0x8A, 0x32, 0xA5, 0x2B, 0x66
, 0x5E, 0x23, 0x71, 0x88, 0x81, 0x95, 0x7D, 0x15
, 0x24, 0xA9, 0x98, 0xC1, 0xB0, 0xFB, 0x3B, 0x6B
, 0xA3, 0xDE, 0xF9, 0xD0, 0x1A, 0x6B, 0xE6, 0xF6
, 0x5D, 0xD7, 0x29, 0x20, 0x7A, 0x48, 0x8E, 0xCC
, 0x31, 0xE8, 0xF1, 0x51, 0x7B, 0xAE, 0x81, 0x09
, 0xB4, 0xC6, 0x7A, 0xFE, 0x10, 0x60, 0xE8, 0x87
, 0x57, 0x92, 0xA4, 0x57, 0xE6, 0x97, 0xCC, 0x5E
, 0xBA, 0x2D, 0xD0, 0xB1, 0x62, 0x2B, 0xBB, 0x31
, 0x36, 0x7F, 0x6C, 0x68, 0x80, 0x3D, 0x06, 0xCB
, 0x61, 0xAA, 0x9F, 0xC0, 0xC9, 0x20, 0x98, 0xEB
, 0x10, 0x10, 0x8A, 0xA8, 0xE9, 0xE5, 0x55, 0x6D
, 0xD9, 0x7A, 0x29, 0xB4, 0x61, 0xA0, 0x70, 0x7A
, 0xA0, 0xFE, 0x27, 0x25, 0x01, 0xA3, 0x26, 0x7E
, 0xF2, 0x64, 0xB2, 0x12, 0x87, 0xBE, 0x0A, 0xCC
, 0x33, 0x93, 0x4B, 0x05, 0xAA, 0xF0, 0xEB, 0x8C
, 0x4F, 0x85, 0x14, 0x72, 0x10, 0xF8, 0x5C, 0x36
, 0x5C, 0x57, 0xC8, 0xE4, 0xD7, 0x15, 0xB4, 0x34
, 0x1C, 0x73, 0x15, 0x97, 0x81, 0x8F, 0xAE, 0x45
, 0x2A, 0xAC, 0x11, 0x82, 0x68, 0xBE, 0x56, 0x02
, 0xC1, 0x00, 0x04, 0x01, 0x00, 0x00, 0x01, 0x00
, 0x01, 0xE3, 0x84, 0x09, 0x73, 0x93, 0x34, 0x94
, 0xCD, 0x3E, 0xE9, 0xD2, 0x47, 0xE6, 0xD6, 0x7E
, 0xB8, 0xDA, 0xC3, 0x59, 0x1A, 0x6A, 0x1E, 0x6A
, 0x4A, 0xF2, 0x9A, 0x9F, 0xC0, 0x8A, 0xCB, 0x33
, 0x99, 0xAB, 0xEF, 0x20, 0x2E, 0xCC, 0x5C, 0x2E
, 0xE6, 0x2F, 0x58, 0x4B, 0xFD, 0xE6, 0x1E, 0x02
, 0x75, 0xA6, 0x8E, 0x29, 0x05, 0xA9, 0xBA, 0x69
, 0xFB, 0xC3, 0x99, 0x04, 0x7F, 0x41, 0xCB, 0x6D
, 0x86, 0x6F, 0x30, 0xFA, 0xBA, 0xA8, 0x62, 0x23
, 0x5D, 0x4B, 0xE5, 0x7F, 0xDF, 0xB1, 0x71, 0x85
, 0xB5, 0x72, 0x9A, 0x7B, 0x93, 0x81, 0x50, 0x6D
, 0x3C, 0x38, 0x98, 0xC8, 0x46, 0x20, 0x66, 0x23
, 0x1A, 0x8B, 0xFA, 0x48, 0xEC, 0xF2, 0x31, 0x3F
, 0x95, 0x9E, 0x31, 0x19, 0x18, 0x75, 0xBF, 0xE9
, 0x35, 0x97, 0x98, 0x33, 0x07, 0xB2, 0x19, 0x5A
, 0x39, 0x72, 0x47, 0x46, 0x32, 0x3F, 0x47, 0x1F
, 0x08, 0xD7, 0x4A, 0xC7, 0x2F, 0x81, 0x25, 0xCF
, 0xCF, 0x12, 0xF8, 0x43, 0x04, 0xC0, 0x70, 0xF8
, 0xC1, 0x0C, 0xA0, 0x55, 0x36, 0xF9, 0xAC, 0xB8
, 0x68, 0xB2, 0x01, 0xAC, 0x5E, 0x0E, 0x12, 0x28
, 0xEB, 0xAC, 0x7E, 0xEF, 0xC5, 0x5D, 0x5C, 0x16
, 0x9B, 0xD0, 0xC1, 0xB8, 0xCF, 0xB0, 0x7A, 0x64
, 0x4C, 0x1A, 0x58, 0x7F, 0x84, 0xEF, 0xED, 0x68
, 0xC7, 0x1D, 0x4F, 0x44, 0xF8, 0x8E, 0x0F, 0xCA
, 0x63, 0x1C, 0x35, 0x4A, 0xBB, 0x53, 0x64, 0xB3
, 0xEA, 0x48, 0x88, 0xEF, 0x5F, 0x62, 0xAE, 0x69
, 0x20, 0xB9, 0xC8, 0xA4, 0xA9, 0x1A, 0xA4, 0x7C
, 0x82, 0x06, 0x9B, 0x64, 0x48, 0x3D, 0xE7, 0xB3
, 0xA7, 0x85, 0x3F, 0x13, 0xA6, 0xF0, 0x20, 0x0C
, 0xD3, 0x7D, 0x4F, 0x48, 0x6B, 0x39, 0x96, 0xBA
, 0x2E, 0xD4, 0x5A, 0x8C, 0x1D, 0xCD, 0xDB, 0xDE
, 0xA8, 0xE4, 0xB3, 0xF9, 0xD6, 0x7F, 0xBB, 0x79
, 0x0D, 0xF3, 0x7F, 0xBB, 0xD8, 0x39, 0xE2, 0xAA
, 0x84, 0x5F, 0xBD, 0x47, 0xA1, 0x91, 0xBF, 0x47
, 0xC6, 0x7F, 0xD0, 0x57, 0x9C, 0xDC, 0x68, 0x98
, 0x42, 0x89, 0x2E, 0x88, 0x43, 0x1A, 0x9D, 0xFA
, 0x07, 0x15, 0x29, 0x5F, 0x50, 0x71, 0xEA, 0xF3
, 0x47, 0xAB, 0x47, 0xA2, 0x91, 0x84, 0xEF, 0x82
, 0x66, 0xB2, 0x0C, 0xB9, 0x6F, 0x83, 0xA2, 0xCB
, 0x8E, 0xE0, 0x35, 0x96, 0xC7, 0x08, 0x1E, 0x43
, 0xC9, 0x85, 0xF0, 0xBC, 0xBB, 0xF6, 0xC1, 0x4E
, 0x41, 0x58, 0x9C, 0xF6, 0xF8, 0x8B, 0xF7, 0xBE
, 0xC6, 0x69, 0xD4, 0xC3, 0x48, 0x40, 0x69, 0x4D
, 0x14, 0x23, 0x9A, 0xC7, 0x86, 0x7E, 0xB1, 0x3F
, 0x31, 0x24, 0x12, 0x80, 0xAC, 0xF5, 0x95, 0x92
, 0xD5, 0x9E, 0x54, 0x47, 0xEC, 0x6D, 0xED, 0x78
, 0xCF, 0x30, 0x79, 0x22, 0xB6, 0xD8, 0x39, 0x49
, 0xA4, 0xB1, 0x17, 0xA7, 0xF5, 0x15, 0x05, 0x06
, 0x41, 0xEF, 0x32, 0x3D, 0x1E, 0x0C, 0xD4, 0x84
, 0x11, 0xE1, 0x5F, 0xC5, 0x57, 0xDC, 0x92, 0x43
, 0x6F, 0x59, 0x3E, 0x8A, 0x2F, 0xC9, 0xCD, 0x7F
, 0x5C, 0xC7, 0x79, 0xEF, 0xE3, 0x8D, 0xB5, 0x60
, 0xF7, 0x54, 0x39, 0x27, 0x63, 0x50, 0x21, 0x70
, 0xF3, 0xBE, 0x47, 0x5E, 0x51, 0xAF, 0x3D, 0x1C
, 0xA4, 0x2A, 0x0D, 0x8A, 0xDE, 0xB8, 0x16, 0x9F
, 0xC3, 0x79, 0x34, 0xE3, 0xBF, 0xFC, 0x11, 0x59
, 0xB5, 0x2C, 0xAF, 0x45, 0x42, 0xDF, 0xC9, 0x95
, 0x61, 0x66, 0x75, 0x30, 0x40, 0x1B, 0x1F, 0x4B
, 0xE8, 0xE3, 0xEA, 0x58, 0x7D, 0x81, 0xF2, 0xE5
, 0x81, 0x7B, 0xB8, 0x78, 0xFF, 0x64, 0x82, 0x18
, 0x41, 0x4D, 0x02, 0x8F, 0x86, 0x19, 0x94, 0x3F
, 0x40, 0xB2, 0x8A, 0xED, 0x9C, 0xF7, 0x82, 0x78
, 0x19, 0x13, 0x60, 0x03, 0x26, 0x39, 0xD5, 0xE1
, 0x90, 0xD4, 0xF8, 0x77, 0xCF, 0xA5, 0xFD, 0xB7
, 0xCD, 0xBC, 0xD5, 0x6A, 0x4F, 0x9D, 0x54, 0x72
, 0xF8, 0x85, 0x66, 0xBC, 0x8E, 0x48, 0x01, 0x17
, 0x20, 0x91, 0x2E, 0x79, 0x43, 0x64, 0x8A, 0x98
, 0xC9, 0xC0, 0xA9, 0x1D, 0x73, 0x2C, 0xC9, 0x26
, 0x85, 0x27, 0x9C, 0x16, 0x16, 0xC4, 0xC1, 0x78
, 0x39, 0x34, 0x53, 0x31, 0x77, 0xB8, 0xA8, 0x25
, 0x70, 0x8E, 0xD3, 0xDF, 0x5A, 0xE3, 0xB2, 0xF4
, 0x39, 0xB1, 0x16, 0xDD, 0xFF, 0x2C, 0x04, 0x8C
, 0x7D, 0x54, 0x2A, 0x05, 0xB3, 0x9F, 0x00, 0xFA
, 0x37, 0x23, 0x09, 0x57, 0x5E, 0x18, 0xED, 0xE0
, 0x3C, 0xEF, 0x05, 0xD9, 0x0B, 0xC7, 0x27, 0xE2
, 0x66, 0xA3, 0xD5, 0xFC, 0xB6, 0x08, 0xBA, 0x5C
, 0x4F, 0x37, 0xB2, 0x41, 0xA7, 0xE0, 0x2C, 0x46
, 0xDA, 0xF3, 0x16, 0x59, 0x50, 0xB9, 0x4F, 0x58
, 0x49, 0x15, 0x46, 0x5B, 0xED, 0x80, 0x71, 0xAC
, 0xF2, 0xD3, 0x83, 0x8C, 0xF5, 0x3B, 0x7F, 0xD2
, 0x01, 0xD4, 0x6B, 0xE0, 0x3D, 0x04, 0x95, 0xE2
, 0x67, 0xE2, 0xA7, 0x63, 0xA1, 0x20, 0x80, 0x40
, 0x24, 0xED, 0xB3, 0x8D, 0x92, 0xDC, 0xFB, 0x83
, 0x65, 0x08, 0xBD, 0x70, 0x5D, 0xBC, 0x66, 0x5C
, 0xA3, 0xA9, 0x07, 0x4C, 0xC4, 0x74, 0xD0, 0x0D
, 0xF8, 0x58, 0x7E, 0x16, 0xF4, 0x21, 0xD4, 0xFA
, 0xCF, 0x8B, 0x8D, 0x15, 0x4B, 0xAF, 0x5E, 0x60
, 0x29, 0x7F, 0x45, 0x8A, 0x10, 0xE0, 0x31, 0x24
, 0x62, 0xB2, 0x39, 0x41, 0x20, 0x1F, 0x8F, 0x6C
, 0xFA, 0xDF, 0xBB, 0xA8, 0xCA, 0xF5, 0x8B, 0x38
, 0xA5, 0xAD, 0x6A, 0x5B, 0x85, 0xB2, 0x56, 0xDC
, 0x9C, 0xBE, 0x1B, 0xD8, 0x50, 0xAE, 0x98, 0x9E
, 0x23, 0xB7, 0xD9, 0x53, 0x4E, 0x7A, 0xFE, 0xDD
, 0xA1, 0x7E, 0x2D, 0xC4, 0xD1, 0x06, 0x39, 0xB6
, 0xA0, 0x46, 0x45, 0x98, 0xE6, 0x64, 0xAC, 0x5D
, 0xB2, 0x7A, 0x7F, 0xE2, 0x1E, 0xE5, 0x41, 0xC9
, 0xED, 0x43, 0x88, 0xA1, 0xC7, 0x82, 0x05, 0x8B
, 0x7D, 0x7A, 0xC7, 0xC9, 0xC5, 0x34, 0xED, 0x25
, 0x0B, 0xB0, 0xFC, 0x0F, 0xF6, 0x0F, 0x7F, 0x3F
, 0xE8, 0x51, 0x8A, 0xEB, 0x36, 0x0A, 0xE5, 0x7A
, 0x1F, 0x93, 0xA9, 0x08, 0xB3, 0x68, 0x16, 0xB7
, 0xF0, 0x83, 0xD6, 0xAE, 0x6F, 0x84, 0x37, 0xA2
, 0x1B, 0x40, 0xB5, 0x67, 0xD5, 0x24, 0xEC, 0x7F
, 0xEF, 0x0D, 0xB7, 0x2F, 0x43, 0x33, 0xB1, 0x05
, 0x3A, 0x08, 0x49, 0xD6, 0xAC, 0x51, 0x48, 0xF8
, 0x69, 0x1D, 0xD9, 0xA3, 0xF4, 0xE9, 0x48, 0xBF
, 0xF7, 0x17, 0x54, 0x24, 0xB7, 0xB3, 0x2B, 0xE8
, 0x52, 0xBA, 0x4C, 0x68, 0x62, 0x5A, 0x52, 0xD4
, 0x46, 0x98, 0x71, 0xCA, 0x7E, 0x97, 0xEC, 0x0B
, 0x55, 0xD8, 0x46, 0x36, 0xF5, 0x44, 0xC9, 0xE4
, 0xD9, 0x96, 0x62, 0xDE, 0x78, 0x22, 0xFD, 0xF6
, 0x0A, 0xA9, 0xB6, 0x2C, 0x6F, 0xEF, 0xC3, 0x8E
, 0x28
};
#endif

A_INT32
swat_iwconfig_scan_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_UCHAR wifi_state = 0;
    A_CHAR* pssid;
    QCOM_BSS_SCAN_INFO* pOut;
    A_UINT16 count;


    qcom_get_state(currentDeviceId, &wifi_state);

    //if(wifi_state!=2)
    {
        //if not connected, scan
        if (argc == 2) {
            pssid = "";
        } else if (argc == 3) {
            pssid = argv[2];
        } else {
        	A_CHAR ssid[32];
			A_UINT8 i, j;

			j = 0;
			A_MEMSET(ssid, 0, 32);
			for (i=2; i<argc; i++)
			{
				if ((j + strlen(argv[i])) > 32)
				{
					//SWAT_PTF("ERROR: ssid > 32\n");
					return SWAT_ERROR;
				}
				else
				{
					do
					{
						ssid[j] = *argv[i];
						j++;
					} while (*argv[i]++);
				}

				ssid[j-1] = 0x20; //space
			}
			ssid[j-1] = 0;
            pssid = ssid;
        }
    }

    /*Set the SSID*/
    qcom_set_ssid(currentDeviceId, pssid);
    /*Start scan*/

#ifndef USE_QCOM_REL_3_3_API
    qcom_start_scan_params_t scanParams;
    scanParams.forceFgScan  = 1;
    scanParams.scanType     = WMI_LONG_SCAN;
    scanParams.numChannels  = 0;
    scanParams.forceScanIntervalInMs = 1;
    scanParams.homeDwellTimeInMs = 0;
    qcom_set_scan(currentDeviceId, &scanParams);
#else
    qcom_set_scan(currentDeviceId);
#endif

    /*Get scan results*/
    if(qcom_get_scan(currentDeviceId, &pOut, &count) == A_OK){
        QCOM_BSS_SCAN_INFO* info = pOut;
        int i,j;

        tx_thread_sleep(1000);
        for (i = 0; i < count; i++)
        {
            printf("ssid = ");
            {
                for(j = 0;j < info[i].ssid_len;j++)
                {
                    printf("%c",info[i].ssid[j]);
                }
                printf("\n");
            }
#if 1
            //tx_thread_sleep(10);
            printf("bssid = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",info[i].bssid[0],info[i].bssid[1],info[i].bssid[2],info[i].bssid[3],info[i].bssid[4],info[i].bssid[5]);

        //   tx_thread_sleep(10);
            printf("channel = %d\n",info[i].channel);

            printf("indicator = %d\n",info[i].rssi);

            printf("security = ");
            if(info[i].security_enabled){
                printf("\n");
                if(info[i].rsn_auth || info[i].rsn_cipher){
                    printf("RSN/WPA2= ");

                if(info[i].rsn_auth){
                    printf("{");
                    if(info[i].rsn_auth & SECURITY_AUTH_1X){
                        printf("802.1X ");
                    }
                    if(info[i].rsn_auth & SECURITY_AUTH_PSK){
                        printf("PSK ");
                    }
                    printf("}");
                }
                if(info[i].rsn_cipher){
                    printf("{");
                    if(info[i].rsn_cipher & ATH_CIPHER_TYPE_WEP){
                        printf("WEP ");
                    }
                    if(info[i].rsn_cipher & ATH_CIPHER_TYPE_TKIP){
                        printf("TKIP ");
                    }
                    if(info[i].rsn_cipher & ATH_CIPHER_TYPE_CCMP){
                        printf("AES ");
                    }
                    printf("}");
                }
                printf("\n");
            }
            if(info[i].wpa_auth || info[i].wpa_cipher){
                printf("WPA= ");

                if(info[i].wpa_auth){
                    printf("{");
                    if(info[i].wpa_auth & SECURITY_AUTH_1X){
                        printf("802.1X ");
                    }
                    if(info[i].wpa_auth & SECURITY_AUTH_PSK){
                        printf("PSK ");
                    }
                    printf("}");
                }

             if(info[i].wpa_cipher){
                    printf("{");
                    if(info[i].wpa_cipher & ATH_CIPHER_TYPE_WEP){
                        printf("WEP ");
                    }
                    if(info[i].wpa_cipher & ATH_CIPHER_TYPE_TKIP){
                        printf("TKIP ");
                    }
                    if(info[i].wpa_cipher & ATH_CIPHER_TYPE_CCMP){
                        printf("AES ");
                    }
                    printf("}");
                }
                printf("\n");
            }
            if(info[i].rsn_cipher == 0 && info[i].wpa_cipher == 0){
                printf("WEP \n");
            }
        }else{
            printf("NONE! \n");
        }
		if (i != (count-1))
		printf("\n");
#endif
    }
	SWAT_PTF("\nshell> ");
    }
    return 0;
}

A_INT32
swat_wmiconfig_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = 0;

    if (argc == 1) {
        swat_wmiconfig_information(currentDeviceId);
        return 0;
    }

    ret += swat_wmiconfig_connect_handle(argc, argv);

#if defined(SWAT_WMICONFIG_SOFTAP)
	ret += swat_wmiconfig_softAp_handle(argc,argv);
#endif
#if defined(SWAT_WMICONFIG_WEP)
	ret += swat_wmiconfig_wep_handle(argc,argv);
#endif
#if defined(SWAT_WMICONFIG_WPA)
    ret += swat_wmiconfig_wpa_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_WPS)
     ret += swat_wmiconfig_wps_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_P2P)
     ret += swat_wmiconfig_p2p_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_IP)
    ret += swat_wmiconfig_ip_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_MISC)
    ret += swat_wmiconfig_misc_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_MISC_EXT)
    ret += swat_wmiconfig_misc_ext_handle(argc, argv);
#endif
    if (ret == 0) {
        SWAT_PTF("Unknown wmiconfig command!\n");
    }else if (ret == SWAT_ERROR){
        SWAT_PTF("Unknown/Invalid command\n");
    }

}


A_INT32
swat_wmiconfig_connect_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--connect")) {
        if (argc == 3) {
            swat_wmiconfig_connect_ssid(currentDeviceId, (char *)argv[2]);
        } else {
            A_INT8 ssid[32];
            A_UINT8 i, j;

            j = 0;
            A_MEMSET(ssid, 0, 32);
            for (i=2; i<argc; i++)
            {
                if ((j + strlen(argv[i])) > 32)
                {
                    //SWAT_PTF("ERROR: ssid > 32\n");
                    return SWAT_ERROR;
                }
                else
                {
                    do
                    {
                        ssid[j] = *argv[i];
                        j++;
                    } while (*argv[i]++);
                }

				ssid[j-1] = 0x20; //space
            }
			ssid[j-1] = 0;

            swat_wmiconfig_connect_ssid(currentDeviceId, (char *) ssid);
        }

        return SWAT_OK;
    }

	if (!A_STRCMP(argv[1], "--connectbssid")) {
	    int rt;
            int aidata[6];
	    unsigned char mac[6];

	    rt = sscanf(argv[2], "%2X:%2X:%2X:%2X:%2X:%2X", \
                                &aidata[0], &aidata[1], &aidata[2], \
                                &aidata[3], &aidata[4], &aidata[5]);
            if (rt < 0) {
		SWAT_PTF("wrong mac format\n");
		return SWAT_ERROR;
            }

	    for (rt = 0; rt < 6; rt++){
		mac[rt] = (unsigned char)aidata[rt];
	    }
        
		if (argc == 4) {	
    	    swat_wmiconfig_connect_bssid(currentDeviceId,(char *)argv[3], mac);
        } else {
            A_INT8 ssid[32];
            A_UINT8 i, j;

            j = 0;
            A_MEMSET(ssid, 0, 32);
            for (i=3; i<argc; i++)
            {
                if ((j + strlen(argv[i])) > 32)
                {
                    //SWAT_PTF("ERROR: ssid > 32\n");
                    return SWAT_ERROR;
                }
                else
                {
                    do
                    {
                        ssid[j] = *argv[i];
                        j++;
                    } while (*argv[i]++);
                }

				ssid[j-1] = 0x20; //space
            }
			ssid[j-1] = 0;

            swat_wmiconfig_connect_bssid(currentDeviceId, (char *) ssid, mac);
        } 

        return SWAT_OK;
    }
	
	if (!A_STRCMP(argv[1], "--connect2"))
		{
		swat_wmiconfig_connect_ssid_2(currentDeviceId, (char *)argv[2]);
		return SWAT_OK;
		}

	if (!A_STRCMP(argv[1], "--adhoc"))
		{

        //SWAT_PTF("Disabling Power Save\n");

        qcom_power_set_mode(0, MAX_PERF_POWER);

		swat_wmiconfig_connect_adhoc(currentDeviceId, (char *)argv[2]);

		return SWAT_OK;
		}

    if (!A_STRCMP(argv[1], "--disc")) {
        swat_wmiconfig_connect_disc(currentDeviceId);
        return SWAT_OK;

    }

	 if (!A_STRCMP(argv[1], "--stacountry")) {
	 	A_CHAR country_code[3];
        if (3 == argc){
            country_code[0] = argv[2][0];
            country_code[1] = argv[2][1];
            country_code[2] = 0x20;

            /* Removed country code QAPI. However, retain this application
             * API so as to not break CST scripts/tests
             */
            swat_wmiconfig_sta_country_code_set(currentDeviceId, country_code);
        }else{
            //SWAT_PTF("Invalid paramter format\n");
            return SWAT_ERROR;
        }
        return SWAT_OK;

    }
#ifdef RSA_VERIFY_DEMO
	 if (!A_STRCMP(argv[1], "--rsa")) {
		 A_PRINTF("before encrypt data: %s	 \r\n",rsa_in_buf);

		 A_PRINTF("before encrypt data1:	 \r\n");
		 if(qcom_rsa_public_encrypt((unsigned char *)rsa_in_buf,16,(const const char *)sharkSSL_ylcert_qc,(unsigned char *)rsa_out_buf,1) == -1)
		 {
		  A_PRINTF("public key encrypt failed \n");
		  return -1 ;
		 }
		 A_PRINTF("after encrypt data1:	 \r\n");
		 int i=0;
		 for(i=0;i<256;i++)
		 {
		 	 if(i%16==0)
		 	 A_PRINTF("\r\n");
		 	 else
		 	 {
				 A_PRINTF("0x%2X ",rsa_out_buf[i]);
			}
		 }
		 A_PRINTF("\r\n");

		 if(qcom_rsa_private_decrypt((unsigned char *)rsa_out_buf,16,(const const char *)sharkSslPrivRSAKey_ylcert_qc,(unsigned char *)rsa_out_buf,1) == -1 ){
			 A_PRINTF("verify failed \n");
			 return  -1;
		 }else {
			 A_PRINTF(" data after decrypt:  %s!!\n",rsa_out_buf);
		 }

		 return SWAT_OK;

		 }

#endif
#ifdef RSA_VERIFY_DEMO

if (!A_STRCMP(argv[1], "--rsa_sign_verify")) {

#ifdef  RSA_SIGN_DO_ENABLE
        unsigned int sign_data_len;
        qcom_rsa_sign_do(sharkSslPrivRSAKey_ylcert_qc,rsa_out_buf,&sign_data_len);
#endif

        qcom_rsa_sign_deinit();

        if(qcom_rsa_sign_init(sharkSSL_ylcert_qc) == -1)
        {
         A_PRINTF("rsa_sign_init failed \n");
         return SWAT_OK ;
        }

        //rsa_sim_data[128] = 0xff;
        if(qcom_rsa_sign_verify(rsa_out_buf,256) <0 ){
            A_PRINTF("vrify failed \n");
            return  -1;
        }else {
            A_PRINTF("DONE!!\n");
        }

        qcom_rsa_sign_deinit();
        return SWAT_OK;

    }
#endif

    return SWAT_NOFOUND;
}

/* Disable this if not needed to test GPIO separately */
#define GPIO_TEST

#ifdef GPIO_TEST
void gpio_pin_int_handler_test(void *pContext)
{
    return;
}
#endif /* GPIO_TEST */

A_INT32
swat_wmiconfig_misc_ext_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 data = 0;
    A_INT32 data0 = 0;
    A_INT32 data1 = 0;

    if (!A_STRCMP(argv[1], "--pwm")) {
        if (argc < 3) {
			//SWAT_PTF("pwm parameters should be start or stop\n");
            return SWAT_ERROR;
        }
		if(!A_STRCMP(argv[2], "start")){
			data = 1;
		}
		else if(!A_STRCMP(argv[2], "stop")){
			data = 0;
		}
		else {
			//SWAT_PTF("pwm parameters should be start or stop\n");
			return SWAT_ERROR;
		}
		swat_wmiconfig_pwm_start(data);
		return SWAT_OK;
	}
	if (!A_STRCMP(argv[1], "--pwmclock")) {
        if (argc < 3) {
			//SWAT_PTF("wmiconfig --pwmclock divider\n");
            return SWAT_ERROR;
        }
		data = atoi(argv[2]);
		//SWAT_PTF("pwm: configure pwm frequency division as %d\n", data);
		swat_wmiconfig_pwm_clock(data);
		return SWAT_OK;
	}
	if (!A_STRCMP(argv[1], "--dutycycle")) {
        if (argc < 3) {
			//SWAT_PTF("wmiconfig --dutycycle duty_cycle\n");
            return SWAT_ERROR;
        }
		data = atoi(argv[2]);
		SWAT_PTF("pwm: configure pwm duty cycle as %d\n", data);
		swat_wmiconfig_pwm_duty(data);
		return SWAT_OK;
	}
    if (!A_STRCMP(argv[1], "--pwmset")) {
        if (argc < 4) {
			//SWAT_PTF("wmiconfig --pwmset freq duty_cycle\n");
            return SWAT_ERROR;
        }
		data = atoi(argv[2]);
		data1 = atoi(argv[3]);
		SWAT_PTF("pwm: configure pwm freq as %d, and duty cycle as %d\n", data, data1);
		swat_wmiconfig_pwm_set(data, data1);
		return SWAT_OK;
	}
	if (!A_STRCMP(argv[1], "--pwmdump")) {
		swat_wmiconfig_pwm_dump();
		return SWAT_OK;
	}
	if (!A_STRCMP(argv[1], "--pwmswstart")) {
		SWAT_PTF("pwm: sw pwm start\n");
		swat_wmiconfig_pwm_sw_start();
		return SWAT_OK;
	}
	if (!A_STRCMP(argv[1], "--pwmswstop")) {
		SWAT_PTF("pwm: sw pwm stop\n");
		swat_wmiconfig_pwm_sw_stop();
		return SWAT_OK;
	}
	if (!A_STRCMP(argv[1], "--pwmswconfig")) {
        if (argc < 5) {
			//SWAT_PTF("wmiconfig --pwmswconfig id freq duty_cycle\n");
            return SWAT_ERROR;
        }
		data = atoi(argv[2]);
		data0 = atoi(argv[3]);
		A_UINT32 data2=atoi(argv[4]);
		swat_wmiconfig_pwm_sw_config(data, data0, data2);
		SWAT_PTF("pwm: sw pwm config with id %d, freq %d, duty cycle %d\n", data, data0, data2);
        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--raw")) {
		A_UINT8 i;
		int aidata[6];
		int rt;
		A_UINT8 addr[4][6];

		// demo data configuration for probe request
		A_UINT8 probe_req_str[46]={
		                             /*SSID */
			                         0x00,0x00,
									 /*supported rates*/
									 0x01, 0x08, 0x82, 0x84 ,0x8B, 0x0C, 0x2, 0x96, 0x18, 0x24,
									 /*extended supported Rates*/
									  0x32 ,0xC8, 0x5C ,0x9B ,0x31, 0xB6, 0x16, 0x0D ,0xFC, 0xB2,
								      0xC0, 0x8B, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00 ,0x98 ,
									  0x0D ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xC0 ,0x06 ,0x00 ,0x88 ,0x01,0x66,0x15,0x0B
							     };
		QCOM_RAW_MODE_PARAM_t para;

        if (argc < 7) {
            goto raw_usage;
        }
		else if(argc < 12){
			;
		}
		else
		{
            goto raw_usage;
		}
        data = atoi(argv[2]);
        data0 = atoi(argv[3]);
        data1 = atoi(argv[4]);
        A_UINT32 data2=atoi(argv[5]);
        A_UINT32 data3=atoi(argv[6]);
		for (i=0; i<4; i++)
		memset(&addr[i][0], 0, sizeof(addr[i]));

		if (argc > 7){
			for (i=0; i<(argc-7); i++){
				memset(&aidata[0], 0, sizeof(aidata));
				rt = sscanf(argv[7+i], "%2X:%2X:%2X:%2X:%2X:%2X", \
                            &aidata[0], &aidata[1], &aidata[2], \
                            &aidata[3], &aidata[4], &aidata[5]);
				if (rt < 0)
                {
                    SWAT_PTF("wrong mac format.\n");
                    return SWAT_ERROR;
                }
				for (rt = 0; rt < 6; rt++)
                {
                    addr[i][rt] = (A_UINT8)aidata[rt];
                }
			}
		}

        if ( data2 > 13 ) {
            goto raw_usage;
        }

        if (argc == 7)
		{
			addr[0][0] = 0xff;
	        addr[0][1] = 0xff;
	        addr[0][2] = 0xff;
	        addr[0][3] = 0xff;
	        addr[0][4] = 0xff;
	        addr[0][5] = 0xff;
	        addr[1][0] = 0x00;
	        addr[1][1] = 0x03;
	        addr[1][2] = 0x7f;
	        addr[1][3] = 0xdd;
	        addr[1][4] = 0xdd;
	        addr[1][5] = 0xdd;
	        addr[2][0] = 0x00;
	        addr[2][1] = 0x03;
	        addr[2][2] = 0x7f;
	        addr[2][3] = 0xdd;
	        addr[2][4] = 0xdd;
	        addr[2][5] = 0xdd;
	        addr[3][0] = 0x00;
	        addr[3][1] = 0x03;
	        addr[3][2] = 0x7f;
	        addr[3][3] = 0xee;
	        addr[3][4] = 0xee;
	        addr[3][5] = 0xee;

			if (data3 == 1)  //these codes are not right, add here only for remain the old design
			{
				memcpy(addr[0], addr[1], ATH_MAC_LEN);
				addr[2][3] = 0xaa;
				addr[2][4] = 0xaa;
				addr[2][5] = 0xaa;
			}
        }

		para.rate_index = data;
	    para.tries = data0;
	    para.size = data1;
		para.chan = data2;
		para.header_type = data3;
		para.seq = 0;
		memcpy(&para.addr1.addr[0], addr[0], ATH_MAC_LEN);
		memcpy(&para.addr2.addr[0], addr[1], ATH_MAC_LEN);
		memcpy(&para.addr3.addr[0], addr[2], ATH_MAC_LEN);
		memcpy(&para.addr4.addr[0], addr[3], ATH_MAC_LEN);

		if(para.header_type == 3){
			para.buflen = sizeof(probe_req_str);
        	para.pdatabuf = probe_req_str;
		}
		else
		{
        	para.pdatabuf = NULL;
			para.buflen = 0;
		}

		if (!qcom_raw_mode_send_pkt(&para))
        return SWAT_OK;

        raw_usage:
        {
            //SWAT_PTF("raw input error\n");
            SWAT_PTF("usage : wmiconfig --raw rate num_tries num_bytes channel header_type [addr1 [addr2 [addr3 [add4]]]]\n");
#if 0
            SWAT_PTF("rate = rate index where 0==1mbps; 1==2mbps; 2==5.5mbps etc\n");
            SWAT_PTF("num_tries = number of transmits 1 - 14\n");
            SWAT_PTF("num_bytes = payload size 0 to 1400\n");
            SWAT_PTF("channel = 0 -- 11, 0: send on current channel\n");
            SWAT_PTF("header_type = 0==beacon frame; 1==QOS data frame; 2==4 address data frame; 3==probe request\n");
            SWAT_PTF("addrY = address Y of 802.11 header xx:xx:xx:xx:xx:xx\n");
            //SWAT_PTF("addr2 = address 2 of 802.11 header xx:xx:xx:xx:xx:xx\n");
            //SWAT_PTF("addr3 = address 3 of 802.11 header xx:xx:xx:xx:xx:xx\n");
            //SWAT_PTF("addr4 = address 4 of 802.11 header xx:xx:xx:xx:xx:xx\n");
#endif
            return SWAT_ERROR;
        }
    }
    if (!A_STRCMP(argv[1], "--wdt")) {
        if(argc < 4){
            //SWAT_PTF("Invalid params\n");
            return SWAT_ERROR;
        }
        data = atoi(argv[2]);
        data0 = atoi(argv[3]);

        qcom_watchdog(data,data0);
        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--wdttest")) {
        data = atoi(argv[2]);
        int curTime, startTime;
        startTime = curTime = swat_time_get_ms();

        /*block for specified time*/
        while(curTime < startTime + data*1000){
            curTime = swat_time_get_ms();
        }
        return SWAT_OK;
    }
      if (!A_STRCMP(argv[1], "--rssi")) {
        swat_wmiconfig_rssi_get(currentDeviceId);
        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--settxpower")) {
        if (argc != 3) {
            return SWAT_ERROR;
        }

        A_SSCANF(argv[2], "%d", &data);
        swat_wmiconfig_tx_power_set(currentDeviceId, (A_UINT32) data);

        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--lpl")) {
        if (argc != 3) {
            return SWAT_ERROR;
        }

        A_SSCANF(argv[2], "%d", &data);
        swat_wmiconfig_lpl_set(currentDeviceId, (A_UINT32) data);

        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--gtx")) {
        if (argc != 3) {
            return SWAT_ERROR;
        }

        A_SSCANF(argv[2], "%d", &data);
        swat_wmiconfig_gtx_set(currentDeviceId, (A_UINT32) data);

        return SWAT_OK;
    }
    /* --setrate <rate> , where rate can be 1,2,5,6,9,11,12,18,24,36,48,54 mbps or MCS */
    if (!A_STRCMP(argv[1], "--setrate"))
    {

        if(argc==3) {
            A_SSCANF(argv[2], "%d", &data);
            swat_wmiconfig_rate_set(currentDeviceId, 0, (A_UINT32) data);
        }
        if(argc==4) {
            if (!A_STRCMP(argv[2], "mcs")) {
                A_SSCANF(argv[3], "%d", &data);
                swat_wmiconfig_rate_set(currentDeviceId, 1, (A_UINT32) data);
            }
        }

        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--suspend")) {
        swat_wmiconfig_suspenable(currentDeviceId);
        return SWAT_OK;
    }


    if (!A_STRCMP(argv[1], "--suspflag")) {
        A_UINT8 flag;
		swat_wmiconfig_suspflag_get(currentDeviceId, &flag);
		if (flag == 1)
		{
			SWAT_PTF("reset from timer - software \n");
		}
		else if (flag == 2)
		{
			SWAT_PTF("reset from GPIO - hardware \n");
		}
		else
		{
			SWAT_PTF("normal reset - no suspend\n");
		}

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--suspstart")) {
        A_UINT32 susp_time;
        A_UINT32 wifiMode;

        swat_wmiconfig_devmode_get(currentDeviceId, &wifiMode);
        if (wifiMode == 0) {
            SWAT_PTF("NOT supported \n");
            return SWAT_ERROR;
        }

        if (argc == 3) {
            /* xiny */
             susp_time = swat_atoi(argv[2]);

            swat_wmiconfig_suspstart(susp_time);
        } else {
           // SWAT_PTF("    --suspstart <time>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

#ifdef GPIO_TEST
    if (!A_STRCMP(argv[1], "--gpiotest")) {
        A_UINT32    peripheral_id = 0;
        A_UINT32    peripheral_disable = 0;
        A_INT32    retVal = 0;
        A_UINT32    cmd_category;
        A_UINT32 pmap = 0;

        cmd_category = swat_atoi(argv[2]);

        if (cmd_category == 0)
        {
            peripheral_id = swat_atoi(argv[3]);
            peripheral_disable = swat_atoi(argv[4]);

            retVal = qcom_gpio_apply_peripheral_configuration(peripheral_id,
                                                        peripheral_disable);

            if (A_OK != retVal)
            {
                qcom_gpio_peripheral_pin_conflict_check(peripheral_id, &pmap);
            }
            SWAT_PTF("GPIO Test: PER ID %d, Disable %d, RETVAL %d, pmap %x\n",
                                peripheral_id, peripheral_disable, retVal, pmap);
        }
        else if (cmd_category == 1)
        {
            A_UINT32    gpio_pin;

            gpio_pin = swat_atoi(argv[3]);
            peripheral_disable = swat_atoi(argv[4]);

            peripheral_id = QCOM_PERIPHERAL_ID_GPIOn(gpio_pin);

            retVal = qcom_gpio_apply_peripheral_configuration(peripheral_id,
                                                        peripheral_disable);
            if (A_OK != retVal)
            {
                qcom_gpio_peripheral_pin_conflict_check(peripheral_id, &pmap);
            }

            SWAT_PTF("GPIO Test: PER ID %d, Pin %d, Disable %d, RETVAL %d, pmap %x\n", peripheral_id,
                                                    gpio_pin, peripheral_disable, retVal, pmap);
        }
        else if (cmd_category == 2)
        {
            A_UINT32 sw_pin = swat_atoi(argv[3]);
            A_UINT32 pin_num;

            retVal = qcom_gpio_get_interrupt_pin_num(sw_pin, &pin_num);

            SWAT_PTF("GPIO Test: SW PIN %d, PIN Num %d, retval %d\n", sw_pin, pin_num, retVal);
        }
        else if (cmd_category == 3)
        {
            A_UINT32 sw_pin = swat_atoi(argv[3]);
            A_UINT32 update_param = swat_atoi(argv[4]);
            A_UINT32 pin_num;

            retVal = qcom_gpio_get_interrupt_pin_num(sw_pin, &pin_num);

            if (retVal == -1)
            {
                SWAT_PTF("SW_INT %d to pin failed %d\n", sw_pin, pin_num);
                return SWAT_OK;
            }

            if (update_param == 1)
            {
                A_UINT32 pull_type = swat_atoi(argv[5]);
                A_UINT32 strength =  swat_atoi(argv[6]);
                A_UINT32 open_drain =  swat_atoi(argv[7]);
                qcom_gpio_pin_pad(pin_num, pull_type, strength, open_drain);

                SWAT_PTF("GPIO Test: SW PIN %d, PIN Num %d, retVal %d\n", sw_pin, pin_num, retVal);
                SWAT_PTF("GPIO Test: pull type %d, strength %d, open_drain %d\n", pull_type, strength, open_drain);
            }
            else if (update_param == 2)
            {
                A_UINT32 in_out =  swat_atoi(argv[5]);
                qcom_gpio_pin_dir(pin_num, in_out);
                SWAT_PTF("GPIO Test: SW PIN %d, PIN Num %d, inout %d\n", sw_pin, pin_num, in_out);
            }
            else if (update_param == 3)
            {
                A_UINT32 source =  swat_atoi(argv[5]);
                qcom_gpio_pin_source(pin_num, source);
                SWAT_PTF("GPIO Test: SW PIN %d, PIN Num %d, source %d\n", sw_pin, pin_num, source);
            }


        }
        else if (cmd_category == 4)
        {
            A_UINT32 sw_pin = swat_atoi(argv[3]);
            A_UINT32 update_param = swat_atoi(argv[4]);
            A_UINT32 pin_num;

            retVal = qcom_gpio_get_interrupt_pin_num(sw_pin, &pin_num);

            if (retVal == -1)
            {
                SWAT_PTF("SW_INT %d to pin failed %d\n", sw_pin, pin_num);
                return SWAT_OK;
            }

            qcom_gpio_interrupt_info_t  gpio_interrupt;

            gpio_interrupt.pin = pin_num;
            gpio_interrupt.gpio_pin_int_handler_fn = gpio_pin_int_handler_test;
            gpio_interrupt.arg = NULL;

            if (update_param == 1)
            {
                qcom_gpio_interrupt_register(&gpio_interrupt);
            }
            else if (update_param == 2)
            {
                qcom_gpio_interrupt_deregister(&gpio_interrupt);
            }
        }
        else if(cmd_category == 5)
        {
            A_UINT32 pin_num = swat_atoi(argv[3]);
            A_UINT32 regVal = 0, fn_ptr = 0;
            qcom_gpio_interrupt_info_t  *gpio_interrupt;

            typedef struct {
                gpio_interrupt_info_t  *gpio_info_list[GPIO_PIN_COUNT];
            } gpio_int_dispatcher_t;

            extern gpio_int_dispatcher_t *gpio_int_dispatcher;

            regVal = *(A_UINT32 *)(0x14028 + (4 * pin_num));
            gpio_interrupt = (qcom_gpio_interrupt_info_t *)gpio_int_dispatcher->gpio_info_list[pin_num];

            if (gpio_interrupt != NULL)
                fn_ptr = (A_UINT32)(gpio_interrupt->gpio_pin_int_handler_fn);

            SWAT_PTF("Pin %d, regVal %x, fnptr %d\n", pin_num, regVal, fn_ptr);
        }
        return SWAT_OK;
    }
#endif /* GPIO_TEST */

    if (!A_STRCMP(argv[1], "--help")) {
        swat_wmiconfig_help();
        return SWAT_OK;
    }

    if(!A_STRCMP(argv[1],"--setmainttimeout")){

        A_UINT32 timeout = swat_atoi(argv[2]);

        swat_wmiconfig_maint_timeout_set(&timeout,sizeof(timeout));
        return SWAT_OK;
    }

    if(!A_STRCMP(argv[1],"--aes_test")){

        A_UINT8  aes256Key[32] = "01234567890123456789012345678901";
        A_UINT8 inputBuffer[16] = "ssssssssssssssss";
        A_UINT8  outputBuffer[16];

        qcom_aes_encrypt_init(aes256Key, 32);
        qcom_aes_encrypt(NULL, inputBuffer, outputBuffer);
        qcom_aes_encrypt_deinit(NULL);


        qcom_aes_decrypt_init(aes256Key, 32);
        qcom_aes_decrypt(NULL, outputBuffer, outputBuffer);
        qcom_aes_decrypt_deinit(NULL);
        A_PRINTF("after decrypt %s \n", outputBuffer);

        return SWAT_OK;
    }
    return SWAT_NOFOUND;

}

A_INT32
swat_wmiconfig_misc_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 data = 0;
    A_INT32 data0 = 0;
    A_INT32 data1 = 0;

    if (!A_STRCMP(argv[1], "--pmparams")) {
        A_UINT32 i;
        A_UINT8 iflag = 0, npflag = 0, dpflag = 0, txwpflag = 0, ntxwflag = 0, pspflag = 0;
        A_INT32 idlePeriod;
        A_INT32 psPollNum;
        A_INT32 dtimPolicy;
        A_INT32 tx_wakeup_policy;
        A_INT32 num_tx_to_wakeup;
        A_INT32 ps_fail_event_policy;

        if (argc < 3) {
            return SWAT_ERROR;
        }

        for (i = 2; i < argc; i++) {
            if (!A_STRCMP(argv[i], "--idle")) {
                if ((i + 1) == argc) {
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%u", &idlePeriod);
                iflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--np")) {
                if ((i + 1) == argc) {
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &psPollNum);
                npflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--dp")) {
                if ((i + 1) == argc) {
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &dtimPolicy);
                dpflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--txwp")) {
                if ((i + 1) == argc) {
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &tx_wakeup_policy);
                txwpflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--ntxw")) {
                if ((i + 1) == argc) {
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &num_tx_to_wakeup);
                ntxwflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--psfp")) {
                if ((i + 1) == argc) {
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &ps_fail_event_policy);
                pspflag = 1;
                i++;
            } else if ((i % 2) == 0) {
                //SWAT_PTF("wmiconfig --pmparams --idle <time in ms>\n");
                //SWAT_PTF("                     --np < > \n");
                //SWAT_PTF("                     --dp <1=Ignore 2=Normal 3=Stick 4=Auto>\n");
                //SWAT_PTF("                     --txwp <1=wake 2=do not wake>\n");
                //SWAT_PTF("                     --ntxw < > \n");
                //SWAT_PTF("                     --psfp <1=send fail event 2=Ignore event>\n");
                return SWAT_ERROR;
            }
        }

        if (!iflag) {
            idlePeriod = 0;
        }

        if (!npflag) {
            psPollNum = 10;
        }

        if (!dpflag) {
            dtimPolicy = 3;
        }

        if (!txwpflag) {
            tx_wakeup_policy = 2;
        }

        if (!ntxwflag) {
            num_tx_to_wakeup = 1;
        }

        if (!pspflag) {
            ps_fail_event_policy = 2;
        }

        swat_wmiconfig_pmparams(currentDeviceId, idlePeriod, psPollNum, dtimPolicy, tx_wakeup_policy,
                                num_tx_to_wakeup, ps_fail_event_policy);
        return SWAT_OK;
    }

	if (!A_STRCMP(argv[1], "--pwrmode")) {
        if (argc < 3) {
            return SWAT_ERROR;
        }

        A_SSCANF(argv[2], "%d", &data);

        if (data == 0) {
            swat_wmiconfig_power_mode_set(currentDeviceId, 2);   //MAX_PERF_POWER
        } else if (data == 1) {
            A_UINT32 wifiMode;
            qcom_op_get_mode(currentDeviceId, &wifiMode);
            if (wifiMode == 1) {
                SWAT_PTF("Setting REC Power is not allowed MODE_AP \n");
                return SWAT_ERROR;
            }

            swat_wmiconfig_power_mode_set(currentDeviceId, 1);   //REC_POWER
        } else {
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--driver")) {
        if (argc != 3) {
            return SWAT_ERROR;
        }

        if (!A_STRCMP(argv[2], "up")) {
            swat_wmiconfig_reset();
        } else if (!A_STRCMP(argv[2], "down")) {
            ;
        } else {
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--reset")) {
        swat_wmiconfig_reset();
        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--version")) {
        swat_wmiconfig_version();
        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--print")) {
        A_UINT8 printEnable;
        if (argc != 3) {
           return SWAT_ERROR;
        }
        printEnable = atoi(argv[2]);
        qcom_enable_print(printEnable);
        //this is a test string. when firmware prints are disabled, this string is not shown on shell
        A_PRINTF("Firmware prints %sabled\n", (printEnable ? "en" : "dis"));
        SWAT_PTF("shell> ");
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--activeptn")) {
        A_UINT32 partition_index;
        if (argc != 3) {
           return SWAT_ERROR;
        }
        partition_index = atoi(argv[2]);
        swat_wmiconfig_active_partition(partition_index);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--allow_aggr")) {
        if (argc != 4) {
			//SWAT_PTF("usage: --allow_aggr <tx_tid_mask> <rx_tid_mask>\n");
            return SWAT_ERROR;
        }

        data0 = atoul(argv[2]);
        data1 = atoul(argv[3]);
        if (data0 > 0xff || data1 > 0xff) {
           SWAT_PTF("Invalid parameters, should no more than 0xFF\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_allow_aggr(currentDeviceId, (A_UINT16) data0, (A_UINT16) data1);
        return SWAT_OK;
    }

	if (!A_STRCMP(argv[1], "--ht40flag")) {
		if (argc != 3) {
			SWAT_PTF("wmiconfig --ht40flag <default, above, below>\n");
			return SWAT_ERROR;
		}

		if (0 == A_STRCMP(argv[2], "default")) {
			swat_wmiconfig_ht40_flag_set(0);
		} else if (0 == A_STRCMP(argv[2], "above")) {
			swat_wmiconfig_ht40_flag_set(1);
		} else if (0 == A_STRCMP(argv[2], "below")) {
			swat_wmiconfig_ht40_flag_set(2);
		} else {
			SWAT_PTF("Unknown flag, only support default/above/below\n");
			return SWAT_ERROR;
		}

		return SWAT_OK;
	}

	if (!strcmp(argv[1], "--promisc")) {
		if (argc == 4 || argc == 3) {
			A_UINT8 promiscuousEn;
			A_UINT8 promiscMode = 1;
			promiscuousEn = atoi(argv[2]);
			if (argc == 4)
				promiscMode = atoi(argv[3]);
			if(promiscMode >=  PROMISC_MODE_MAX)
				return SWAT_ERROR;
			if(promiscuousEn){
				swat_wmiconfig_promiscuous_test(TRUE,promiscMode);
			}
			else{
				swat_wmiconfig_promiscuous_test(FALSE,promiscMode);
			}
			return SWAT_OK;
		}
		else {
			//SWAT_PTF("	  --promisc <value>\n");
			return SWAT_ERROR;
		}
	}
	if (!strcmp(argv[1], "--device")) {
	    if (argc == 3) {
                extern A_UINT8 gNumOfWlanDevices;
                if (gNumOfWlanDevices > 1) {
                    currentDeviceId = atoi(argv[2]);
                    if(currentDeviceId < gNumOfWlanDevices){
                        SWAT_PTF("configure device switch to %d\n", currentDeviceId);
                        return SWAT_OK;
                    }
                    else{
                        SWAT_PTF("Max device id is %d\n", gNumOfWlanDevices-1);
                        return SWAT_ERROR;
                    }
                }
                else {
                    SWAT_PTF("Board is working on single device mode.\n");
                    return SWAT_OK;
                }
	    }
	    else {
	        //SWAT_PTF("    --device <value>\n");
	        return SWAT_ERROR;
	    }
	}

	if (!strcmp(argv[1], "--roaming")){
		if (argc == 3){
			if (!swat_strcmp((A_CHAR *) argv[2], "enable")){
				swat_wmiconfig_roaming_enable(currentDeviceId, 1);
				return SWAT_OK;
			}
			else if (!swat_strcmp((A_CHAR *) argv[2], "disable")){
				swat_wmiconfig_roaming_enable(currentDeviceId, 0);
				return SWAT_OK;
			}
		}

		//SWAT_PTF("    --roming <enable/disable>\n");
		return SWAT_ERROR;
	}

#ifdef SWAT_OTP
    if (!strcmp(argv[1], "--otp")){
        extern A_INT32 qcom_otp_write_mac(A_UINT8 mac[]);
        A_INT32 rt;
        A_INT32 aidata[6];
        A_UINT8 mac[6];

        if (argc < 4){
            SWAT_PTF("--otp mac <xx:xx:xx:xx:xx:xx>\n");
            return SWAT_ERROR;
        }

        sscanf(argv[3], "%2X:%2X:%2X:%2X:%2X:%2X", \
                            &aidata[0], &aidata[1], &aidata[2], \
                            &aidata[3], &aidata[4], &aidata[5]);
        for (rt = 0; rt < 6; rt++)
        {
            mac[rt] = (unsigned char)aidata[rt];
        }

        rt = qcom_otp_write_mac(mac);
        if (rt == -3) {
            SWAT_PTF("otp overlap\n");
        }
        else if (rt == -2) {
            SWAT_PTF("no space\n");
        }
        else if (rt == -1) {
            SWAT_PTF("otp fail\n");
        }
        else if (rt == 0) {
            SWAT_PTF("otp ok\n");
        }
        else {
            SWAT_PTF("unknow\n");
        }

        return SWAT_OK;
    }
#endif

    return SWAT_NOFOUND;
}

#if defined(SWAT_WMICONFIG_SOFTAP)

A_INT32
swat_wmiconfig_softAp_handle(A_INT32 argc, A_CHAR * argv[])
{

    A_INT32 data = 0;

    if (!A_STRCMP(argv[1], "--mode")) {
		if(argc <= 2)
		{
	  		SWAT_PTF("invalid parameters\n");
			return SWAT_ERROR;
		}
            if (!swat_strcmp((A_CHAR *) argv[2], "ap"))
              {
                  SWAT_PTF("Disabling PowerSave\n");

                  qcom_power_set_mode(currentDeviceId,MAX_PERF_POWER);

              }
            else if (!swat_strcmp((A_CHAR *) argv[2], "station"))
              {
                  SWAT_PTF("Enabling PowerSave\n");

                  qcom_power_set_mode(currentDeviceId,REC_POWER);

              }


        if (3 == argc) {

            swat_wmiconfig_dev_mode_set(currentDeviceId, (A_CHAR *) argv[2]);
        } else if (4 == argc) {
            if (!A_STRCMP(argv[2], "ap")) {
                if (!A_STRCMP(argv[3], "hidden")) {
                    swat_wmiconfig_ap_hidden_set(currentDeviceId);
#if defined(SWAT_WMICONFIG_WPS)
                } else if (!A_STRCMP(argv[3], "wps")) {
                    swat_wmiconfig_ap_wps_set(currentDeviceId);
#endif
                } else {
                    //SWAT_PTF("wmiconfig --mode ap <hidden|wps>\n");
                    return SWAT_ERROR;
                }


            swat_wmiconfig_dev_mode_set(currentDeviceId, (A_CHAR *) argv[2]);


            } else {
                //SWAT_PTF("wmiconfig --mode ap <hidden|wps>\n");
                return SWAT_ERROR;
            }
        } else {
            //SWAT_PTF("wmiconfig --mode <ap|station>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--channel")) {
        if (argc == 3) {
            A_SSCANF(argv[2], "%d", &data);
			if ((data < 1) || (data > 165))
			{
			  SWAT_PTF("invalid channel: should range between 1-165\n");
			  return SWAT_ERROR;
			}
            swat_wmiconfig_channel_set(currentDeviceId, (A_UINT32) data);
        } else {
            //SWAT_PTF("wmiconfig --channel <channel-id>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--wmode")) {
        if (argc != 3) {
            SWAT_PTF("wmiconfig --wmode <b,g,n,ht40>\n");
            return SWAT_ERROR;
        }

        if (0 == A_STRCMP(argv[2], "a")) {
                       swat_wmiconfig_wifi_mode_set(currentDeviceId, (A_UINT8)QCOM_11A_MODE);
                    } else if (0 == A_STRCMP(argv[2], "b")) {
                        swat_wmiconfig_wifi_mode_set(currentDeviceId, (A_UINT8)QCOM_11B_MODE);
                    } else if (0 == A_STRCMP(argv[2], "g")) {
                        swat_wmiconfig_wifi_mode_set(currentDeviceId, (A_UINT8)QCOM_11G_MODE);
                    } else if (0 == A_STRCMP(argv[2], "n")) {
                        swat_wmiconfig_wifi_mode_set(currentDeviceId, (A_UINT8)QCOM_11N_MODE);
                    } else if (0 == A_STRCMP(argv[2], "ht40")) {
                        swat_wmiconfig_wifi_mode_set(currentDeviceId, (A_UINT8)QCOM_HT40_MODE);
                    } else {
              SWAT_PTF("Unknown wmode, only support b/g/n/ht40\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }
#if !defined(REV74_TEST_ENV2)
    if (!A_STRCMP(argv[1], "--listen")) {
        if (3 == argc) {
            A_SSCANF(argv[2], "%d", &data);
            swat_wmiconfig_listen_time_set(currentDeviceId, (A_UINT32) data);
        } else {
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--scanctrl")) {
        if (3 == argc || 4 == argc) {
            A_UINT8 fgScan;
            A_SSCANF(argv[2], "%d", &data);
            fgScan = (A_UINT8) data;
            if (fgScan == 1)
            {
              swat_wmiconfig_scan_ctl_set(currentDeviceId, 0);
            }
            else
            {
              swat_wmiconfig_scan_ctl_set(currentDeviceId, 1);
            }
        } else {
            SWAT_PTF("Invalid paramter format\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--setscanpara")) {
        if (12 == argc) {
            qcom_scan_params_t scan_params;
            A_SSCANF(argv[2], "%d", &data);
            scan_params.maxActChDwellTimeInMs = (A_UINT16) data;
            A_SSCANF(argv[3], "%d", &data);
            scan_params.pasChDwellTimeInMs    = (A_UINT16) data;
            A_SSCANF(argv[4], "%d", &data);
            scan_params.fgStartPeriod = (A_UINT16) data;
            A_SSCANF(argv[5], "%d", &data);
            scan_params.fgEndPeriod = (A_UINT16) data;
            A_SSCANF(argv[6], "%d", &data);
            scan_params.bgPeriod = (A_UINT16) data;
            A_SSCANF(argv[7], "%d", &data);
            scan_params.shortScanRatio = (A_UINT8) data;
            A_SSCANF(argv[8], "%d", &data);
            scan_params.scanCtrlFlags = (A_UINT8) data;
            A_SSCANF(argv[9], "%d", &data);
            scan_params.minActChDwellTimeInMs = (A_UINT16) data;
            A_SSCANF(argv[10], "%d", &data);
            scan_params.maxActScanPerSsid = (A_UINT16) data;
            A_SSCANF(argv[11], "%d", &data);
            scan_params.maxDfsChActTimeInMs = (A_UINT32) data;
            swat_wmiconfig_scan_param_set(currentDeviceId, &scan_params);
        } else {
            if (4 == argc)
            {
              qcom_scan_params_t scan_params;
              A_SSCANF(argv[2], "%d", &data);
              scan_params.maxActChDwellTimeInMs = (A_UINT16) data;
              A_SSCANF(argv[3], "%d", &data);
              scan_params.pasChDwellTimeInMs    = (A_UINT16) data;
              scan_params.fgStartPeriod = 0;
              scan_params.fgEndPeriod   = 0;
              scan_params.bgPeriod      = 0;
              scan_params.shortScanRatio= 3;
              scan_params.scanCtrlFlags = 0x2f;
              scan_params.minActChDwellTimeInMs = 20;
              scan_params.maxActScanPerSsid = 1;
              scan_params.maxDfsChActTimeInMs = 2000;
              swat_wmiconfig_scan_param_set(currentDeviceId, &scan_params);
              return SWAT_OK;
            }
            SWAT_PTF("wmiconfig --setscanpara <max_act_ch_dwell_time_ms> <pas_act_chan_dwell_time_ms> <fg_start_period(in secs)> <fg_end_period (in secs)> <bg_period (in secs)> <short_scan_ratio> <scan_ctrl_flags>  <min_active_chan_dwell_time_ms> <max_act_scan_per_ssid> <max_dfs_ch_act_time_in_ms> \n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--setscan")) {
        QCOM_BSS_SCAN_INFO* pOut;
        A_UINT16 count;

        if (7 <= argc) {
            qcom_start_scan_params_t scan_params;
            A_UINT16 maxArgCount = 0, argI = 0;
            A_SSCANF(argv[2], "%d", &data);
            scan_params.forceFgScan = (A_BOOL) data;
            A_SSCANF(argv[3], "%d", &data);
            scan_params.homeDwellTimeInMs    = (A_UINT32) data;
            A_SSCANF(argv[4], "%d", &data);
            scan_params.forceScanIntervalInMs = (A_UINT32) data;
            A_SSCANF(argv[5], "%d", &data);
            scan_params.scanType = (A_UINT8) data;
            A_SSCANF(argv[6], "%d", &data);
            scan_params.numChannels = (A_UINT8) data;
            if (scan_params.numChannels > 12)
            {
               SWAT_PTF("cannot set more than 12 channels to scan\n");
            }
            maxArgCount = scan_params.numChannels + 7;
            argI = 7;
            if (argc != maxArgCount)
            {
               SWAT_PTF("wmiconfig --setscan <forceFgScan> <homeDwellTimeInMs> <forceScanIntervalInMs> <scanType> <numChannels> [<channelInMhz 1> <channelInMhz 2>...<channelInMhz N>]\n");
               return SWAT_ERROR;
            }
            while (argI < maxArgCount)
            {
               A_SSCANF(argv[argI], "%d", &data);
               scan_params.channelList[argI-7] = (A_UINT16) data;
               argI++;
            }
#ifndef USE_QCOM_REL_3_3_API
            qcom_set_scan(currentDeviceId, &scan_params);
#else
            qcom_set_scan(currentDeviceId);
#endif
        } else {
            SWAT_PTF("wmiconfig --setscan <forceFgScan> <homeDwellTimeInMs> <forceScanIntervalInMs> <scanType> <numChannels> [<channel> <channel>... upto numChannels]\n");
            return SWAT_ERROR;
        }
        if(qcom_get_scan(currentDeviceId, &pOut, &count) == A_OK){
           QCOM_BSS_SCAN_INFO* info = pOut;
           int i,j;

           tx_thread_sleep(1000);
           printf("count : %d\n", count);
           for (i = 0; i < count; i++)
           {
               printf("ssid = ");
               {
                   for(j = 0;j < info[i].ssid_len;j++)
                   {
                      printf("%c",info[i].ssid[j]);
                   }
                   printf("\n");
               }
               printf("bssid = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
                     info[i].bssid[0], info[i].bssid[1],
                     info[i].bssid[2], info[i].bssid[3],
                     info[i].bssid[4], info[i].bssid[5]);

               printf("channel = %d\n",info[i].channel);

               if (i != (count-1))
                  printf("\n");
           }
           SWAT_PTF("shell> ");
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ap")) {
        if (argc < 3) {
            SWAT_PTF("Missing parameter\n");
            return SWAT_ERROR;
        }

        if (!A_STRCMP(argv[2], "bconint")) {
            if (4 == argc) {
                A_SSCANF(argv[3], "%d", &data);
                swat_wmiconfig_bcon_int_set(currentDeviceId, (A_UINT16) data);
            } else {
                //SWAT_PTF("Invalid paramter format\n");
                return SWAT_ERROR;
            }

        }
        if (!A_STRCMP(argv[2], "country")) {
            A_CHAR country_code[3];

            if (4 == argc) {
                country_code[0] = argv[3][0];
                country_code[1] = argv[3][1];
                country_code[2] = 0x20;
                /* Removed country code QAPI. However, retain this application
                 * API so as to not break CST scripts/tests
                 */
                swat_wmiconfig_country_code_set(currentDeviceId, country_code);
            } else {
                //SWAT_PTF("Invalid paramter format\n");
                return SWAT_ERROR;
            }
        }

		if (!A_STRCMP(argv[2], "inact")) {
            if (4 == argc) {
                A_SSCANF(argv[3], "%d", &data);
                swat_wmiconfig_inact_set(currentDeviceId, (A_UINT16) data);
            } else {
                //SWAT_PTF("Invalid paramter format\n");
                return SWAT_ERROR;
            }

        }

		if (!A_STRCMP(argv[2], "stainfo")) {
            if (3 == argc) {
                swat_wmiconfig_sta_info(currentDeviceId);
            } else {
                //SWAT_PTF("Invalid paramter format\n");
                return SWAT_ERROR;
            }

        }

        return SWAT_OK;
    }
#endif
    return SWAT_NOFOUND;
}
#endif

#if defined(SWAT_WMICONFIG_WEP)

A_INT32
swat_wmiconfig_wep_handle(A_INT32 argc, A_CHAR * argv[])
{

    A_UINT32 key_index;
	A_UINT32 mode;

    if (!A_STRCMP(argv[1], "--wepkey")) {

        if (argc == 4) {
            A_SSCANF(argv[2], "%d", &key_index);

            if ((key_index > 4) || (key_index < 1)) {
                SWAT_PTF("key_index: 1-4 \n");
                return SWAT_ERROR;
            }

            swat_wmiconfig_wep_key_set(currentDeviceId, argv[3], (A_CHAR) key_index);
        } else {
            //SWAT_PTF("	  --wepkey <key_index> <key>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--wep")) {

        if (argc == 4) {
            A_SSCANF(argv[2], "%d", &key_index);
            if ((key_index > 4) || (key_index < 1)) {
                SWAT_PTF("key_index: 1-4 \n");
                return SWAT_ERROR;
            }
			if (!A_STRCMP(argv[3], "open"))
			{
			  mode = 0;
			}
			else if (!A_STRCMP(argv[3], "shared"))
			{
			  mode = 1;
			}
            	       else if (!A_STRCMP(argv[3], "auto"))
			{
			  mode = 2;
			}
			else
			{
			  //SWAT_PTF("<open> or <shared> \n");
			  return SWAT_ERROR;
			}
            /* support open mode now*/
            swat_wmiconfig_wep_key_index_set(currentDeviceId, key_index, mode);
        }else if (argc == 3){
        	A_SSCANF(argv[2], "%d", &key_index);
            if ((key_index > 4) || (key_index < 1)) {
                SWAT_PTF("key_index: 1-4 \n");
                return SWAT_ERROR;
            }
			swat_wmiconfig_wep_key_index_set(currentDeviceId, key_index, 2);
    	}
		else {
            //SWAT_PTF("	  --wep <def_keyix> <mode>\n");
            return SWAT_ERROR;
        }
        return SWAT_OK;
    }

		return SWAT_NOFOUND;

}
#endif

#if defined(SWAT_WMICONFIG_WPA)

A_INT32
swat_wmiconfig_wpa_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--p")) {
        if (3 == argc) {
            swat_wmiconfig_wep_passowrd_set(currentDeviceId, argv[2]);
        } else {
            //SWAT_PTF("	  wmiconfig --p <passphrase>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }
#if !defined(REV74_TEST_ENV2)
    if (!A_STRCMP(argv[1], "--wpa")) {
        if (argc == 5) {
            swat_wmiconfig_wpa_set(currentDeviceId, argv[2], argv[3], argv[4]);
        } else {
            //SWAT_PTF("	  --wpa <ver> <ucipher> <mcipher>\n");
            return SWAT_ERROR;
        }
        return SWAT_OK;
    }
#endif
    return SWAT_NOFOUND;
}
#endif

#if 1

#if defined(SWAT_WMICONFIG_WPS)
A_INT32 wps_config_state_mode;

A_INT32
swat_wmiconfig_wps_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--wps")) {
        A_UINT32 connect = 0;

        if (argc < 4) {
            //SWAT_PTF
              //  ("wmiconfig --wps <connect:0-not connect after wps 1:connect after wps> <mode:pin/push> [pin:<=8 characters] <*ssid> <*mac:xx:xx:xx:xx:xx:xx> <*channel>\n");
            SWAT_PTF("wmiconfig --wps config <0-not configured 1:configured>\n");
            return SWAT_ERROR;
        }

        if (!A_STRCMP(argv[2], "config")) {
            A_INT32 mode;
            A_SSCANF(argv[3], "%d", &mode);
            wps_config_state_mode = mode;
        } else {
            A_SSCANF(argv[2], "%d", &connect);
            if (!A_STRCMP(argv[3], "pin")) {
                if (argc < 5) {
                    SWAT_PTF("Parameter number is too less.\n");
                    return SWAT_ERROR;
                }

                if (strlen(argv[4]) > 8) {
                    SWAT_PTF("pin length must be less than 8.\n");
                    return SWAT_ERROR;
                }
                if(argc == 8)
                {
                    int aidata[6];
                    char ssid[WMI_MAX_SSID_LEN];
                    int rt;
                    int channel;
                    A_UINT8 macaddress[6];

                    if(strlen(argv[5]) > WMI_MAX_SSID_LEN)
    				{
    					SWAT_PTF("Invalid ssid length\n");
    					return A_ERROR;
    				}
                    strcpy(ssid,argv[5]);

                    rt = sscanf(argv[6], "%2X:%2X:%2X:%2X:%2X:%2X", \
                                &aidata[0], &aidata[1], &aidata[2], \
                                &aidata[3], &aidata[4], &aidata[5]);

                    if (rt < 0)
                    {
                        SWAT_PTF("wrong mac format.\n");
                        return A_ERROR;
                    }

                    for (rt = 0; rt < 6; rt++)
                    {
                        macaddress[rt] = (A_UINT8)aidata[rt];
                    }

                    rt = sscanf(argv[7], "%d", &channel);
                    if((channel < 1) || (channel > 165) || rt < 0)
                    {
                        SWAT_PTF("wrong channel.\n");
                        return A_ERROR;
                    }

                    swat_wmiconfig_wps_enable(currentDeviceId, 1);
                    swat_wmiconfig_wps_config_state_set(currentDeviceId, wps_config_state_mode);
                    swat_wmiconfig_wps_start_without_scan(currentDeviceId,connect,0,(A_INT8*)argv[4],(A_INT8*)ssid,channel,macaddress);

                }else if(argc > 5 && argc < 8)
                {
                    SWAT_PTF("Parameter number is too less.\n");
                }else{

                    swat_wmiconfig_wps_enable(currentDeviceId, 1);
                    swat_wmiconfig_wps_config_state_set(currentDeviceId, wps_config_state_mode);
                    swat_wmiconfig_wps_start(currentDeviceId, connect, 0, (A_INT8 *)argv[4]);
                }
            } else if (!A_STRCMP(argv[3], "push")) {
                swat_wmiconfig_wps_enable(currentDeviceId, 1);
                swat_wmiconfig_wps_config_state_set(currentDeviceId, wps_config_state_mode);
                swat_wmiconfig_wps_start(currentDeviceId, connect, 1, 0);
            } else {
                SWAT_PTF("Invalid wps mode.\n");
                return SWAT_ERROR;
            }
        }

        return SWAT_OK;
    }

    return SWAT_NOFOUND;
}
#endif

#if defined(SWAT_WMICONFIG_P2P)
A_INT32
swat_wmiconfig_p2p_handle(A_INT32 argc, A_CHAR * argv[])
{
    extern void swat_wmiconfig_p2p(int argc, char *argv[]);
    if (!A_STRCMP(argv[1], "--p2p")) {
#if defined(P2P_ENABLED)
         swat_wmiconfig_p2p(argc - 1, &argv[1]);
        return SWAT_OK;
#endif
    }

    return SWAT_NOFOUND;

}
#endif
#if defined(SWAT_WMICONFIG_IP)

A_INT32
swat_wmiconfig_ip_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--ipstatic")) {
        if (argc < 5) {
            //SWAT_PTF("--ipstatic x.x.x.x(ip) x.x.x.x(msk) x.x.x.x(gw)\n");
            return SWAT_ERROR;
        }

        swat_wmiconfig_ipstatic(currentDeviceId, argv[2], argv[3], argv[4]);

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ipdhcp")) {
        swat_wmiconfig_ipdhcp(currentDeviceId);
        return SWAT_OK;
    }

	if (!A_STRCMP(argv[1], "--ipauto")) {
		if(argc == 3)
		{
			swat_wmiconfig_ipauto(currentDeviceId, argv[2]);
		}
		else
		{
			swat_wmiconfig_ipauto(currentDeviceId, NULL);
		}
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ipdhcp_release")) {
        qcom_dhcps_release_pool(currentDeviceId);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--tcp_backoff_retry")) {
        A_INT32 retry = atoi(argv[2]);
        if(retry > 12 || retry < 4){
            //SWAT_PTF("TCP max retries between 4 and 12\n");
            return SWAT_ERROR;
        }
        qcom_tcp_set_exp_backoff(currentDeviceId, retry);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ipconfig")) {
        swat_wmiconfig_ipconfig(currentDeviceId);
        return SWAT_OK;
    }

#ifdef SWAT_DNS_ENABLED

    if (!A_STRCMP(argv[1], "--ip_gethostbyname")) {
        if (argc < 3) {
            SWAT_PTF("--ip_gethostbyname <hostname>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns(currentDeviceId, argv[2]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_gethostbyname2")) {
        if (argc < 4) {
            //SWAT_PTF("--ip_gethostbyname2 [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2 ipv6 =3)\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns2(currentDeviceId, argv[2], atoi(argv[3]), 1); /* 1:gethostbyname2, 2:resolvehostname */
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_resolve_hostname")) {
        if (argc < 4) {
            //SWAT_PTF("--ip_resolve_hostname [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2 ipv6 =3)\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns2(currentDeviceId, argv[2], atoi(argv[3]), 2); /* 1:gethostbyname2, 2:resolvehostname */
        return SWAT_OK;
    }
#endif

#ifdef BRIDGE_ENABLED
	if (!A_STRCMP(argv[1], "--ipbridgemode")) {
		swat_wmiconfig_bridge_mode_enable(currentDeviceId);
		return SWAT_OK;
	}
#endif

    if (!A_STRCMP(argv[1], "--pingId")) {
	   if (argc < 3) {
	    //SWAT_PTF("--pingId <new_pingId>\n");
		return SWAT_ERROR;
		}
		swat_wmiconfig_set_ping_id(currentDeviceId, atoi(argv[2]));
			return SWAT_OK;
	  }

   	if (!A_STRCMP(argv[1], "--ipv4route")) {
		swat_ipv4_route(currentDeviceId,argc,argv);
		return SWAT_OK;
	}
    if (!A_STRCMP(argv[1], "--ipv6route")) {
		swat_ipv6_route(currentDeviceId,argc,argv);
		return SWAT_OK;
	}
    if (!A_STRCMP(argv[1],"--ipv6")){
        if (argc < 3) {
            SWAT_PTF("--ipv6 enable/disable\n");
            return SWAT_ERROR;
        }
        swat_set_ipv6_status(currentDeviceId,argv[2]);
        return SWAT_OK;
    }

#ifdef SWAT_DNS_ENABLED
    if (!A_STRCMP(argv[1], "--ip_dns_client")) {
        if (argc < 3) {
            //SWAT_PTF("--ip_dns_client start/stop\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_enable(currentDeviceId, argv[2]);
        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--ip_dns_server")) {
        A_UINT32 dns_wifiMode;
        if (argc < 3) {
            //SWAT_PTF("--ip_dns_server start/stop\n");
            return SWAT_ERROR;
        }
        qcom_op_get_mode(currentDeviceId, &dns_wifiMode);
         if (dns_wifiMode == 0) {
             SWAT_PTF("DNS Server is not possible in station mode\n");
             return SWAT_ERROR;
            }
        swat_wmiconfig_dnss_enable(currentDeviceId, argv[2]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_dns_local_domain")) {
        if (argc < 3) {
            //SWAT_PTF("--ip_dns_local_domain local_domain\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_domain(currentDeviceId, argv[2]);
        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--ip_dns_entry_create")) {
        if (argc < 4) {
            //SWAT_PTF("--ip_dns_entry_create <hostname> <ipaddr>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_entry_add(currentDeviceId, argv[2], argv[3]);
        return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--ip_dns_entry_delete")) {
        if (argc < 4) {
            //SWAT_PTF("--ip_dns_entry_delete <hostname> <ipaddr>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_entry_del(currentDeviceId, argv[2], argv[3]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_dns_server_addr")) {
        if (argc < 3) {
            //SWAT_PTF("--ip_dns_server_addr <xx.xx.xx.xx>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_svr_add(currentDeviceId, argv[2]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_dns_delete_server_addr")) {
        if (argc < 3) {
            //SWAT_PTF("--ip_dns_delete_server_addr <xx.xx.xx.xx>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_svr_del(currentDeviceId, argv[2]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_dns_timeout")) {
        if (argc < 3) {
            //SWAT_PTF("--ip_dns_timeout <timeout (in sec)>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_set_timeout(currentDeviceId, atoi(argv[2]));
        return SWAT_OK;
    }
#endif
    if (!A_STRCMP(argv[1], "--ipdhcppool")) {
        A_INT32 data;

        if (argc != 5) {
            //SWAT_PTF("Missing parameter\n");
            return SWAT_ERROR;
        }

        /* data = A_ATOUL(argv[4]); */
        swat_wmiconfig_dhcp_pool(currentDeviceId, argv[2], argv[3], data);
        return SWAT_OK;
    }

    if(!A_STRCMP(argv[1],"--dhcpscb")){
        if(argc < 3)
        {
            SWAT_PTF("incomplete params:e.g wmiconfig --dhcpscb <start/stop>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dhcps_cb_enable(currentDeviceId, argv[2]);

        return SWAT_OK;
    }
    if(!A_STRCMP(argv[1],"--dhcpccb")){
         if(argc < 3)
         {
             SWAT_PTF("incomplete params:e.g wmiconfig --dhcpccb <start/stop>\n");
            return SWAT_ERROR;
         }
         swat_wmiconfig_dhcpc_cb_enable(currentDeviceId, argv[2]);

         return SWAT_OK;
    }
    if (!A_STRCMP(argv[1], "--iphostname")) {
        if (argc < 3) {
            //SWAT_PTF("--iphostname <new_host_name>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_set_hostname(currentDeviceId, argv[2]);
        return SWAT_OK;
    }
#ifdef SWAT_WMICONFIG_SNTP
    if (!A_STRCMP(argv[1], "--ip_sntp_client"))
    {
                if(argc < 3)
                {
                  //SWAT_PTF("incomplete params\n");
                  return SWAT_ERROR;
                }
        swat_wmiconfig_sntp_enable(currentDeviceId, argv[2]);
        return SWAT_OK;
    }

    if(!A_STRCMP(argv[1], "--ip_sntp_srvr"))
    {
               if(argc < 4)
               {
                 //SWAT_PTF("incomplete params\n");
                 return SWAT_ERROR;
               }
        swat_wmiconfig_sntp_srv(currentDeviceId, argv[2],argv[3]);
        return SWAT_OK;
    }

    if(!A_STRCMP(argv[1], "--ip_sntp_zone"))
    {
               if(argc < 5)
               {
                 //SWAT_PTF("incomplete params\n");
                 return SWAT_ERROR;
               }

               if(strlen(argv[3]) > 3)
               {
                 //SWAT_PTF("Error : Invalid string.only dse is valid\n");
                 return SWAT_ERROR;
               }
        swat_wmiconfig_sntp_zone(currentDeviceId, argv[2],argv[4]);
        return SWAT_OK;
    }

    if(!A_STRCMP(argv[1], "--ip_sntp_get_time"))
    {
        swat_wmiconfig_sntp_get_time(currentDeviceId);
        return SWAT_OK;
    }

    if(!A_STRCMP(argv[1], "--ip_sntp_get_time_of_day"))
    {
        swat_wmiconfig_sntp_get_time_of_day(currentDeviceId);
        return SWAT_OK;
    }

    if(!A_STRCMP(argv[1], "--ip_show_sntpconfig"))
    {
        swat_wmiconfig_sntp_show_config(currentDeviceId);
        return SWAT_OK;
    }
#endif /* SWAT_SNTP_CLIENT */
#ifdef ENABLE_HTTP_SERVER
    if (strcmp(argv[1],"--ip_http_server") == 0)
    {
        swat_http_server(argc, argv);
        return SWAT_OK;
    }
    if (strcmp(argv[1],"--ip_https_server") == 0)
    {
        swat_http_server(argc, argv);
        return SWAT_OK;
    }

    if (strcmp(argv[1],"--ip_http_post") == 0)
    {
        swat_http_server_post(argc, argv);
        return SWAT_OK;
    }
    if (strcmp(argv[1],"--ip_http_get") == 0)
    {
        swat_http_server_get(argc, argv);
        return SWAT_OK;
    }
#endif /* ENABLE_HTTP_SERVER */

#ifdef ENABLE_HTTP_CLIENT
    if(strcmp(argv[1], "--ip_http_client") == 0)
    {
        httpc_command_parser(argc, argv);
        return SWAT_OK;
    }
#endif /* ENABLE_HTTP_CLIENT */

#if !defined(REV74_TEST_ENV2)
 if(strcmp(argv[1], "--ip6rtprfx") == 0)
    {
        swat_ip6_set_router_prefix(argc,argv);
        return SWAT_OK;
    }
#endif
#ifdef SWAT_SSL
    if(strcmp(argv[1], "--ssl_start") == 0)
    {
        ssl_start(argc,argv);
        return SWAT_OK;
    }
    if(strcmp(argv[1], "--ssl_stop") == 0)
    {
        ssl_stop(argc,argv);
        return SWAT_OK;
    }
    if(strcmp(argv[1], "--ssl_config") == 0)
    {
        ssl_config(argc,argv);
        return SWAT_OK;
    }

    if(strcmp(argv[1], "--ssl_add_cert") == 0)
    {
        ssl_add_cert(argc,argv);
        return SWAT_OK;
    }
if(strcmp(argv[1], "--ssl_store_cert") == 0)
    {
        ssl_store_cert(argc,argv);
        return SWAT_OK;
    }
if(strcmp(argv[1], "--ssl_delete_cert") == 0)
    {
        ssl_delete_cert(argc,argv);
        return SWAT_OK;
    }
if(strcmp(argv[1], "--ssl_list_cert") == 0)
    {
        ssl_list_cert(argc,argv);
        return SWAT_OK;
    }

#endif
#ifdef SWAT_OTA
if(!A_STRCMP(argv[1],"--ota_upgrade"))
    {
       swat_ota_upgrade(currentDeviceId,argc,argv);
        return SWAT_OK;
    }
if(!A_STRCMP(argv[1],"--ota_read"))
   {
       swat_ota_read(argc,argv);
        return SWAT_OK;
   }

if(!A_STRCMP(argv[1],"--ota_done"))
   {
        swat_ota_done(argc,argv);
        return SWAT_OK;
   }
if(!A_STRCMP(argv[1],"--ota_cust"))
   {
        swat_ota_cust(argc,argv);
        return SWAT_OK;
   }
if(!A_STRCMP(argv[1],"--ota_format"))
   {
        swat_ota_format(argc,argv);
        return SWAT_OK;
   }
if(!A_STRCMP(argv[1],"--ota_ftp"))
   {
        swat_ota_ftp_upgrade(currentDeviceId, argc,argv);
        return SWAT_OK;
   }

#endif
if(!A_STRCMP(argv[1],"--settcptimeout"))
   {
         swat_tcp_conn_timeout(argc,argv);
        return SWAT_OK;
    }

    return SWAT_NOFOUND;
}
#endif


#if defined(SWAT_BENCH)

A_INT32
swat_benchtx_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = 0;
    A_UINT32 aidata[4];
    A_UINT32 ipAddress = 0;
    A_UINT32 local_ipAddress = 0;
    A_UINT32 port = 0;
    A_UINT32 pktSize = 0;
    A_UINT32 seconds = 0;
    A_UINT32 numOfPkts = 0;
    A_UINT32 mode = 0;
    A_UINT32 protocol = 0;
    A_UINT32 delay = 0;
    A_UINT32 ip_hdr_incl = 0;
    IP6_ADDR_T ip6Address;

    if (((void *) 0 == argv) || (argc < 8)) {
        goto ERROR;
    }

    A_MEMSET(&ip6Address,0,sizeof(IP6_ADDR_T));
    /* IP Address */
    if(v6_enabled){
        Inet6Pton(argv[1], &ip6Address);
    }else{
    ret = swat_sscanf(argv[1], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
    if (ret < 0) {
       goto ERROR;
    }

    }
    ipAddress = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

    /* Port */
    port = swat_atoi(argv[2]);

    /* TCP or UDP or raw */
    if ((0 != swat_strcmp(argv[3], "tcp"))
        && (0 != swat_strcmp(argv[3], "udp")) && (0 != swat_strcmp(argv[3],"raw"))
        &&(0 != swat_strcmp(argv[3], "ssl")))
    {
       goto ERROR;
    }
    ret = swat_strcmp(argv[3], "tcp");
    if (0 == ret) {
        protocol = 0;
    }
    ret = swat_strcmp(argv[3], "udp");
    if (0 == ret) {
        protocol = 1;
    }
#ifdef SWAT_BENCH_RAW
    ret = swat_strcmp(argv[3], "raw");
    if (0 == ret)
    {
        if(argc >= 9){
            /* IP Address */
            ret = swat_sscanf(argv[8], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0)
            {
                goto ERROR;
            }
            local_ipAddress = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];
        }
        protocol = 2;

        if((port == 6) || (port == 17) || (port < 0) || (port > 255))
        {
           goto ERROR;
        }

    }

    if(argc == 10)
    {
    ret = swat_strcmp(argv[9],"ip_hdr_inc");
    if(0 == ret)
        {
               ip_hdr_incl = 1;
        }
    else{
               goto ERROR;
        }
    }
#endif /*SWAT_BENCH_RAW*/
#ifdef SWAT_SSL
    if(swat_strcmp(argv[3], "ssl") == 0){

    if (ssl_role)
    {
        printf("ERROR: busy.\n");
        goto ERROR;
    }
    if (ssl_inst[SSL_CLIENT_INST].sslCtx == NULL || ssl_inst[SSL_CLIENT_INST].role != SSL_CLIENT)
    {
       printf("ERROR: SSL client not stated (Use 'wmiconfig --ssl_start client' first).\n");
       goto ERROR;
    }
    ssl_role = SSL_CLIENT;
    protocol = TEST_PROT_SSL;
    }
#endif

    /* Packet Size */
    pktSize = swat_atoi(argv[4]);
    if ((0 == pktSize) || (pktSize > 1400)) {
       goto ERROR;
    }

    /* Test Mode */
    mode = swat_atoi(argv[5]);
    if ((0 != mode)
        && (1 != mode)) {
        goto ERROR;
    }
    if (0 == mode) {
        seconds = swat_atoi(argv[6]);
        if (0 == seconds) {
            goto ERROR;
        }
    }
    if (1 == mode) {
        numOfPkts = swat_atoi(argv[6]);
        if (0 == numOfPkts) {
            goto ERROR;
        }
    }

    /* Do nothing */
    delay = swat_atoi(argv[7]);
    swat_bench_tx_test(ipAddress, ip6Address, port, protocol, pktSize, mode, seconds, numOfPkts, local_ipAddress, ip_hdr_incl, delay);

    return 0;
ERROR:
        SWAT_PTF("Invalid/missing param\n");
        SWAT_PTF("benchtx <Tx IP> <port> <protocol tcp/udp> <size> <test mode> <packets|time> <delay>\r\n");
        SWAT_PTF("Ex: benchtx 192.168.1.100 6000 tcp 64 1 1000 0\r\n");
        SWAT_PTF("Parameters:\r\n");
        SWAT_PTF("1 <Tx IP>       : IP address (v4 or v6)\r\n");
        SWAT_PTF("2 <port>        : Listening Port for tcp/udp\r\n");
        SWAT_PTF("3 <protocol>    : tcp/udp/ssl\r\n");
        SWAT_PTF("4 <size>        : Packet size in bytes\r\n");
        SWAT_PTF("5 <test mode>   : 0:Time 1:Packets\r\n");
        SWAT_PTF("6 <packets|time>: Seconds or Packets number\r\n");
        SWAT_PTF("7 <delay>       : Always 0\r\n");
#ifdef SWAT_BENCH_RAW
        SWAT_PTF("benchtx <Tx IP> <prot> <raw> <msg size> <test mode> <number of packets | time (sec)> <delay in msec> <local IP> [ip_hdr_inc*]\r\n");
#endif
        return -1;

}

A_INT32
swat_benchrx_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = 0;
    A_UINT32 aidata[4];
    A_UINT32 ipAddress = 0;
    A_UINT32 localIp = 0;
    A_UINT32 mcastIp = 0;
    A_UINT32 port = 0;
    A_UINT32 protocol = 0;
    A_UINT32 ip_hdr_inc = 0;
    A_UINT32 rawmode = 0;
    IP6_ADDR_T localIp6;
    IP6_ADDR_T mcastIp6;
    A_UINT8 mcastEnabled = 0;

    if (((void *) 0 == argv) || (argc > 5) || (argc < 3)) {
        goto ERROR;
    }

    A_MEMSET(&localIp6,0,sizeof(IP6_ADDR_T));
    A_MEMSET(&mcastIp6,0,sizeof(IP6_ADDR_T));

    /* TCP or UDP */
    if ((0 != swat_strcmp(argv[1], "tcp"))
        && (0 != swat_strcmp(argv[1], "udp"))
        && (0 != swat_strcmp(argv[1], "raw"))
        && (0 != swat_strcmp(argv[1], "ssl")))
    {
        goto ERROR;
    }
    ret = swat_strcmp(argv[1], "tcp");
    if (0 == ret) {
        protocol = 0;
    }
    ret = swat_strcmp(argv[1], "udp");
    if (0 == ret) {
        protocol = 1;
    }

#ifdef SWAT_BENCH_RAW

    ret = swat_strcmp(argv[1], "raw");
    if (0 == ret)
    {
        protocol = 2;
        ret = swat_strcmp(argv[2],"eapol");
        if(ret == 0)
                rawmode = ETH_RAW;
        else
                rawmode = IP_RAW;
    }
#endif
#ifdef SWAT_SSL
    if(swat_strcmp("ssl", argv[1]) == 0)
    {
        if (ssl_role)
        {
            printf("ERROR: busy.\n");
            goto ERROR;
        }
        if (ssl_inst[SSL_SERVER_INST].sslCtx == NULL || ssl_inst[SSL_SERVER_INST].role != SSL_SERVER)
        {
           printf("ERROR: SSL server not stated (Use 'wmiconfig --ssl_start server' first).\n");
           goto ERROR;
        }
        protocol = TEST_PROT_SSL;
        ssl_role = SSL_SERVER;
    }
#endif
    /* Port */
    port = swat_atoi(argv[2]);
    if(rawmode == ETH_RAW)
    {
        port = ATH_ETH_P_PAE;
    }

    /* IP Check */
    if (4 == argc) {
        if(strcasecmp("ip_hdr_inc",argv[3]) == 0)
		{
		    ip_hdr_inc = 1;
		}else{
            ret = swat_sscanf(argv[3], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0) {
                goto ERROR;
            }
            ipAddress = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

            /* Check Multicast IP Or Local IP */
            if ((aidata[0] & 0xf0) == 0xE0) //224.xxx.xxx.xxx - 239.xxx.xxx.xxx
            {
                mcastIp = ipAddress;
            } else {
                localIp = ipAddress;
            }
        }
    }

    if (5 <= argc)
    {
        ret = swat_strcmp(argv[4],"ip_hdr_inc");
        if(ret == 0)
        {
#ifdef SWAT_BENCH_RAW
            ip_hdr_inc = 1;
            ret = swat_sscanf(argv[3], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0)
            {
                goto ERROR;
            }
            localIp = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];
#endif
         }else{
#ifdef SWAT_BENCH_RAW
            if(argc == 6){
                ret = swat_strcmp(argv[5],"ip_hdr_inc");
                if(ret == 0)
                {
                    ip_hdr_inc = 1;
                }
            }
#endif
            if(v6_enabled){
                Inet6Pton(argv[3], &mcastIp6);
                Inet6Pton(argv[4], &localIp6);
            }else{
            ret = swat_sscanf(argv[3], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0)
            {
                   goto ERROR;
            }
            mcastIp = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

            if((aidata[0] & 0xf0) != 0xE0)
            {
                  goto ERROR;
            }

            ret = swat_sscanf(argv[4], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0)
            {
                  goto ERROR;
            }
            localIp = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];
        }
            mcastEnabled = 1;
        }
    }/* argc >= 5 */

    if(mcastEnabled)
        swat_bench_rx_test(protocol, port, localIp, mcastIp, &localIp6, &mcastIp6, ip_hdr_inc, rawmode);
    else
        swat_bench_rx_test(protocol, port, localIp, mcastIp, NULL, NULL, ip_hdr_inc, rawmode);

    return 0;
ERROR:
    SWAT_PTF("Incorrect/missing param\n");
    SWAT_PTF("benchrx <protocol> <port> <multicast IP*> <local IP*>\r\n");
    SWAT_PTF("Ex: benchrx tcp 6000 192.168.1.100 \r\n");
    SWAT_PTF("Parameters:\r\n");
    SWAT_PTF("1 <protocol>    : tcp/udp/ssl\r\n");
    SWAT_PTF("2 <port>        : Listening Port\r\n");
    SWAT_PTF("3 <multicast IP>: Multicast IP Address\r\n");
    SWAT_PTF("4 <local IP>    : Local IP Address\r\n");
#ifdef SWAT_BENCH_RAW
    SWAT_PTF("benchrx <protocol> <port> <multicast IP*> <local IP*> [ip_hdr_inc]\r\n");
#endif /*SWAT_BENCH_RAW*/

    return -1;
}

A_INT32
swat_benchquit_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (argc > 1) {
        SWAT_PTF("input benchquit\n");
        return -1;
    }

    swat_bench_quit_config();

#if 0
    qcom_msleep(1000);

    extern void qcom_task_kill_all(void);
    qcom_task_kill_all();

    extern void swat_socket_close_all(void);
    swat_socket_close_all();
#endif

    return 0;
}

A_INT32
swat_benchdbg_handle(A_INT32 argc, A_CHAR * argv[])
{

    /* swat_bench_dbg(); */
    return 0;
}

#endif

#if defined(SWAT_PING)
extern A_INT32 isdigit(A_INT32 c);

A_INT32
inet_aton(const A_CHAR *name,
          /* [IN] dotted decimal IP address */
          A_UINT32 * ipaddr_ptr
          /* [OUT] binary IP address */
    )
{                               /* Body */

    A_INT8 ipok = FALSE;

    A_UINT32 dots;

    A_UINT32 byte;

    A_UINT32 addr;

    addr = 0;

    dots = 0;

    for (;;) {

        if (!isdigit(*name))
            break;

        byte = 0;

        while (isdigit(*name)) {

            byte *= 10;

            byte += *name - '0';

            if (byte > 255)
                break;

            name++;

        }                       /* Endwhile */

        if (byte > 255)
            break;

        addr <<= 8;

        addr += byte;

        if (*name == '.') {

            dots++;

            if (dots > 3)
                break;

            name++;

            continue;

        }



      if ((*name == '\0') && (dots == 3)) {

         ipok = TRUE;

      } /* Endif */



      break;



   } /* Endfor */



   if (!ipok) {

      return 0;

   } /* Endif */



   if (ipaddr_ptr) {

      *ipaddr_ptr = addr;

   } /* Endif */



    return -1;

}                               /* Endbody */

A_INT32
swat_ping_handle(A_INT32 argc, A_CHAR *argv[], A_CHAR v6)
{
    A_UINT32 hostaddr;
#ifdef IP_V6
    A_UINT8 host6addr[16];
#endif
    A_INT32 error;
    A_UINT32 count, size, interval;
    A_UINT32 i;

    if (argc < 2) {
        SWAT_PTF("Usage: %s <host_ip> [ -c <count> -s <size> -i <interval> ]\n", argv[0]);

        return -1;
    }

    if(v6){
#ifdef IP_V6
        error = Inet6Pton(argv[1], &host6addr);
#else
        return -1;
#endif
    }else{
    error = inet_aton(argv[1], &hostaddr);
    }

    if (!v6 && error != -1) {
        if (strlen(argv[1]) > 32) {
            SWAT_PTF("host name cannot be more then 32 Bytes\n");

            return -1;
        } else {
#if !defined (REV74_TEST_ENV2)
            if (A_OK != qcom_dnsc_get_host_by_name(argv[1], &hostaddr) ) {
                SWAT_PTF("Can't get IP addr by host name %s\n", argv[1]);
                return -1;
            } else {
                SWAT_PTF("Get IP addr %s by host name %s\n", (char *)_inet_ntoa(hostaddr), argv[1]);
            }
#endif
        }
    }

    if (v6 && error !=0 ) {
        if (strlen(argv[1]) > 32) {
            SWAT_PTF("host name cannot be more then 32 Bytes\n");

            return -1;
        } else {
#if !defined (REV74_TEST_ENV2)
	  if (A_OK != qcom_dnsc_get_host_by_name2(argv[1], (A_UINT32*)(host6addr), AF_INET6, 2)) {
                SWAT_PTF("Can't get IPv6 addr by host name %s\n", argv[1]);
                return -1;
            } else {
	        A_UINT8 ip6_str[48];
	        inet6_ntoa((char *)&host6addr,(char *)ip6_str);
                SWAT_PTF("Get IPv6 addr %s by host name %s\n", ip6_str, argv[1]);
            }
#endif
        }
    }

    count = 1;
    size = 64;
    interval = 0;

    if (argc > 2 || argc <= 8) {
        for (i = 2; i < argc; i += 2) {
            if (!A_STRCMP(argv[i], "-c")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &count);
            } else if (!A_STRCMP(argv[i], "-s")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &size);
            } else if (!A_STRCMP(argv[i], "-i")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &interval);
            }
        }
    } else {
        SWAT_PTF("Usage: %s <host> [ -c <count> -s <size> -i <interval> ] \n", argv[0]);

        return -1;
    }

    if (size > 7000) {          /*CFG_PACKET_SIZE_MAX_TX */
        SWAT_PTF("Error: Invalid Parameter %s \n", argv[5]);
        return -1;
    }

    for (i = 0; i < count; i++) {
        if(v6){
#ifdef IP_V6
            if(qcom_ping6(host6addr, size) == A_OK){
                A_UINT8 ip6_str[48];
                inet6_ntoa((char *)&host6addr,(char *)ip6_str);
                SWAT_PTF("Ping reply from %s : time<1ms\r\n",ip6_str);
            }else{
                SWAT_PTF("Request timed out\r\n");
            }
#else
            return -1;
#endif
        }else{
            if(qcom_ping(hostaddr, size) == A_OK){
                SWAT_PTF("Ping reply from %d.%d.%d.%d: time<1ms\r\n",
                    hostaddr >> 24 & 0xFF, hostaddr >> 16 & 0xFF, hostaddr >> 8 & 0xFF, hostaddr & 0xFF);

            }else{
                SWAT_PTF("Request timed out\r\n");
            }
        }
        if ((count > 0) && (i < (count - 1))) {
            qcom_thread_msleep(interval); /*Sleep to wait for Reply packets */
        }

    }
    return 0;
}

#endif


A_INT32 swat_ip6_set_router_prefix(A_INT32 argc,char *argv[])
{

	unsigned char  v6addr[16];
	A_INT32 retval=-1;
	int prefixlen = 0;
    int prefix_lifetime = 0;
    int valid_lifetime = 0;
    A_UINT32 wifiMode;

	if(argc < 6)
	{
		printf("incomplete params\n");
		return A_ERROR;
	}

    qcom_op_get_mode(currentDeviceId, &wifiMode);
    if ((wifiMode != QCOM_WLAN_DEV_MODE_AP) && (wifiMode != QCOM_WLAN_DEV_MODE_ADHOC))
    {
		printf("ipv6 rt prfx support not possible in station \n");
		return A_ERROR;
	}

    retval = Inet6Pton(argv[2],v6addr);
    if(retval == 1)
	{
           printf("Invalid ipv6 prefix \n");
           return (A_ERROR);
	}
	prefixlen =  atoi(argv[3]);
    prefix_lifetime = atoi(argv[4]);
    valid_lifetime =  atoi(argv[5]);
    qcom_ip6config_router_prefix(currentDeviceId, v6addr,prefixlen,prefix_lifetime,valid_lifetime);
    return 0;
}

#endif
void
swat_time()
{
		  #if 0
          cmnos_printf("[%dh%dm%ds:]", (swat_time_get_ms() / 1000) / 3600,
                 ((swat_time_get_ms() / 1000) % 3600) / 60, (swat_time_get_ms() / 1000) % 60);
		  #endif
}


A_INT32
swat_http_server(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 enable;

    if(argc < 3)
    {
       SWAT_PTF("Incomplete params\n");
       return A_ERROR;
    }
    if(strcmp(argv[1], "--ip_http_server") == 0){
    if(strcmp(argv[2], "start") == 0)
    {
        enable = 1;
    }
    else if(strcmp(argv[2], "stop") == 0)
    {
        enable = 0;
    }
    else
    {
        SWAT_PTF("Supported commands: start/stop");
        return (A_ERROR);
    }
    }
    else if(strcmp(argv[1], "--ip_https_server") == 0){
        if(strcmp(argv[2], "start") == 0)
        {
            enable = 3;
        }
        else if(strcmp(argv[2], "stop") == 0)
        {
            enable = 2;
        }
        else
        {
            SWAT_PTF("Supported commands: start/stop");
            return (A_ERROR);
        }
    }

    swat_wmiconfig_http_server(currentDeviceId, enable);
}

A_INT32 swat_http_server_post(A_INT32 argc,char *argv[])
{
#if defined HTTP_ENABLED
    A_INT32 command = HTTP_POST_METHOD ; /* Command 1 for HTTP server post */
#endif
    A_INT32 objtype, objlen;

    if(argc < 7)
    {
       SWAT_PTF("Incomplete params\n");
       return A_ERROR;
    }

    objtype = atoi(argv[4]);
    objlen = atoi(argv[5]);
    if((objtype != 1) && (objtype != 2) && (objtype != 3))
    {
        SWAT_PTF("Error: <objtype> can be 1 ,2 or 3\n");
        return A_ERROR;
    }

#if defined HTTP_ENABLED
    if (A_OK != qcom_http_server_method(command, (A_UINT8*)argv[2], (A_UINT8*)argv[3], objtype, objlen, (A_UINT8*)argv[6]))
    {
        SWAT_PTF("Error! HTTP post failed\n");
    }
#endif
    return A_OK;
}



A_INT32 swat_http_server_get(A_INT32 argc,char *argv[])
{
#if defined HTTP_ENABLED
    A_INT32 command = HTTP_GET_METHOD ; /* Command 0 for HTTP server get */
#endif
    A_INT32 objtype, objlen;
    A_UINT8 *value;

    if(argc < 4)
    {
       SWAT_PTF("Incomplete params\n");
       return A_ERROR;
    }
    objtype = 0;
    objlen = 0;
    value = swat_mem_malloc(1500);
    if (!value)
        return A_ERROR;

#if defined HTTP_ENABLED
    qcom_http_server_method(command, (A_UINT8*)argv[2], (A_UINT8*)argv[3], objtype, objlen, value);

    SWAT_PTF("Value is %s\n", value);
#endif
    swat_mem_free(value);
    return A_OK;
}


HTTPC_PARAMS      httpc; // Since stack size is less

typedef PREPACK struct {
    A_UINT32    length;
    A_UINT32    resp_code;
    A_UINT32    more;
    A_UINT8     data[1];
} POSTPACK HTTPC_RESPONSE;

void httpc_method(HTTPC_PARAMS *p_httpc)
{
    A_UINT32  error = A_OK;
    HTTPC_RESPONSE *temp = NULL;
    int i,j;
    A_UINT8* value = swat_mem_malloc(1500); //TBD:change the number to micro
    A_UINT8 *p=NULL;

    if (!value) {
        SWAT_PTF("No memory for page\n");
        return;
    }
#if defined HTTP_ENABLED
    error = qcom_http_client_method(httpc.command, httpc.url, httpc.data, value);
#endif
    if(error != A_OK){
        SWAT_PTF("HTTPC Command failed\n");
    }else if(value != NULL)
    {
        temp = (HTTPC_RESPONSE *)value;

        /* For GET and POST alone, print the output */
        if ((p_httpc->command == 1) || (p_httpc->command == 2))
        {
            j = temp->length;
            SWAT_PTF("=========>Size:%u,%d,  Resp_code:%u\n\n", temp->length, j,temp->resp_code);

            if (temp->length)
            {
                p = (A_UINT8 *)temp->data;
                SWAT_PTF("Packet:\n");
                for(i = 0; i < j; i++)
                {
                	SWAT_PTF_NO_TIME("%c", *p++);
				}
            }
        }
    }

    swat_mem_free(value);
    return;
}


A_INT32 httpc_command_parser(A_INT32 argc, A_CHAR* argv[] )
{
    A_INT32           return_code = A_OK;

    if (argc < 3)
    {
        /*Incorrect number of params, exit*/
        return_code = A_ERROR;
    }
    else
    {
        memset((void *)&httpc, 0, sizeof(HTTPC_PARAMS));

        if(strlen((char*)(httpc.url)) >= 256)
        {
            SWAT_PTF("Maximum 256 bytes supported as argument\n");
            return A_ERROR;
        }

        if (argv[3])
            strcpy((char*)httpc.url, argv[3]);

        if (argv[4]){
            strcpy((char*)httpc.data, argv[4]);
        }else{
            httpc.data[0] =  '\0';
        }

#if defined HTTP_ENABLED
        if(strcmp(argv[2], "connect") == 0)
        {
            httpc.command = HTTPC_CONNECT_CMD;
            if(argc < 4){
                SWAT_PTF("wmiconfig --ip_http_client connect server_addr\n");
                return A_ERROR;
                }
            if(strncmp(argv[3], "https://", 8) == 0)
			{
				int size = sizeof(httpc.url) - 8;
				memmove(httpc.url, httpc.url + 8, size);
				httpc.command = HTTPC_CONNECT_SSL_CMD;
			}
			else if(strncmp(argv[3], "http://", 7) == 0)
			{
				int size = sizeof(httpc.url) - 7;
				memmove(httpc.url, httpc.url + 7, size);
			}
            if(argc >= 4){
               qcom_http_client_method(httpc.command, httpc.url, httpc.data, NULL);
            }else{
                return_code = A_ERROR;
            }
        }
        else if(strcmp(argv[2], "get") == 0)
        {
            httpc.command = HTTPC_GET_CMD;
            if(argc >= 4){
                httpc_method(&httpc);
            }else
                return_code = A_ERROR;
        }
        else if(strcmp(argv[2], "post") == 0)
        {
            httpc.command = HTTPC_POST_CMD;
            if(argc >= 4)
                httpc_method(&httpc);
            else
                return_code = A_ERROR;
        }
        else if(strcmp(argv[2], "query") == 0)
        {
            httpc.command = HTTPC_DATA_CMD;
            if(argc >= 5)
                httpc_method(&httpc);
            else
                return_code = A_ERROR;
        }
        else if(strcmp(argv[2], "disc") == 0)
        {
            int res;
            httpc.command = HTTPC_DISCONNECT_CMD;
            res = qcom_http_client_method(httpc.command, httpc.url, httpc.data, NULL);
            if(res == 0)
                SWAT_PTF("Disconnect success\n");
            else
                SWAT_PTF("Disconnect failed\n");
        }
        else
        {
            SWAT_PTF("Unknown Command \"%s\"\n", argv[2]);
            return_code = A_ERROR;
        }
#endif
    }
    if (return_code == A_ERROR)
    {
        SWAT_PTF ("USAGE: wmiconfig --ip_http_client [<connect/get/post/query> <data1> <data2>]\n");
    }
    return return_code;
} /* Endbody */

#if 0 //defined(P2P_ENABLED)

/* Printf to the serial port */
#if defined(P2P_PRINTS_ENABLED) /* { */
#define P2P_PRINTF(args...)  A_PRINTF(args)
#else /* } { */
#define P2P_PRINTF(args...)
#endif /* } */

int swat_wmiconfig_p2p(int argc, char *argv[])
{
      if(argc < 2){
#if defined(P2P_PRINTS_ENABLED)
          P2P_PRINTF("<on|off> | <find|stop> | <cancel> | <nodelist> | <auth|connect peer-mac push|display|keypad pin>\n");
          P2P_PRINTF("<set> <p2pmode p2pdevp|p2pclient|p2pgo> | <gointent val>\n");
          P2P_PRINTF("p2p invite <ssid> <mac> <wps-method>\n");
          P2P_PRINTF("join <mac> <push | keypad | display> <wpspin> \n");
          P2P_PRINTF("prov <mac> <display> \n");
#endif
          return -1;
      }
      if(!strcmp(argv[1], "on")) {
          extern void qca_p2p_enable(A_UINT8 device_id, int enable);
          qca_p2p_enable(currentDeviceId, 1);
      }
      else if(!strcmp(argv[1], "find")) {

       #define P2P_STANDARD_TIMEOUT 300

        typedef enum p2p_disc_type {
        P2P_DISC_START_WITH_FULL,
        P2P_DISC_ONLY_SOCIAL,
        P2P_DISC_PROGRESSIVE
        } P2P_DISC_TYPE;

        P2P_DISC_TYPE type;
        A_UINT32 timeout;

        extern void qca_p2p_device_discover(A_UINT8 device_id, P2P_DISC_TYPE type,A_UINT32 timeout);
        if(argc == 3)
        {
             if(strcmp(argv[2],"1") == 0)
             {
                   type    = P2P_DISC_START_WITH_FULL;
                   timeout = P2P_STANDARD_TIMEOUT;
             }
             else if(strcmp(argv[2],"2") == 0)
             {
                   type    = P2P_DISC_ONLY_SOCIAL;
                   timeout = P2P_STANDARD_TIMEOUT;
             }
             else if(strcmp(argv[2],"3") == 0)
             {
                   type    = P2P_DISC_PROGRESSIVE;
                   timeout = P2P_STANDARD_TIMEOUT;
             }
             else
             {
                   P2P_PRINTF("\n wrong option enter option 1,2 or 3\n");
                   return;
             }
        }
        else if(argc == 4)
        {
             if(strcmp(argv[2],"1") == 0)
             {
                   type    = P2P_DISC_START_WITH_FULL;
                   timeout = atoi(argv[3]);
             }
             else if(strcmp(argv[2],"2") == 0)
             {
                   type    = P2P_DISC_ONLY_SOCIAL;
                   timeout = atoi(argv[3]);
             }
             else if(strcmp(argv[2],"3") == 0)
             {
                   type    = P2P_DISC_PROGRESSIVE;
                   timeout = atoi(argv[3]);
             }
             else
             {
                   P2P_PRINTF("\n wrong option enter option 1,2 or 3\n");
                   return;
             }
        }
        else{
                   type    = P2P_DISC_ONLY_SOCIAL;
                   timeout = P2P_STANDARD_TIMEOUT;
        }
         qca_p2p_device_discover(currentDeviceId, type,timeout);
    }

    else if(!strcmp(argv[1], "nodelist")) {
         extern void qca_p2p_device_list_shown(A_UINT8 device_id);
         qca_p2p_device_list_shown(currentDeviceId);
    }
    else if(!strcmp(argv[1],"setconfig"))
    {
           extern void qca_p2p_set_config(A_UINT8 device_id, A_UINT8 go_intent,A_UINT8 listen_ch,A_UINT8 oper_ch,char *op_ch,A_UINT8 country[3],A_UINT32 node_age_to);
           A_UINT8 go_intent, listen_channel, oper_channel;
           A_UINT32 node_age_timeout;
           A_UINT8 country[3];
           if(argc < 7){
              P2P_PRINTF("error cmd \n");
              return -1;
           }
           go_intent        = atoi(argv[2]);
           listen_channel   = atoi(argv[3]);
           oper_channel     = atoi(argv[4]);
           strcpy((char *)country,argv[5]);
           node_age_timeout = atoi(argv[6]);
           qca_p2p_set_config(currentDeviceId, go_intent,listen_channel,oper_channel,argv[4],country,node_age_timeout);
    }
    else if(!strcmp(argv[1], "connect")) {
           extern void qca_p2p_connect_client(A_UINT8 device_id, int wps_method, unsigned char *peer_mac,unsigned char* wps_pin,A_UINT8 persistent_flag);
           int rt;
           int wps_method;
           int aidata[6];
           unsigned char peer_mac[6],wps_pin[9];
           A_UINT8 persistent_flag = 0;

           if(argc < 4){
              P2P_PRINTF("error cmd\n");
              return -1;
           }

           if(!strcmp(argv[3], "push")) {
              wps_method = 4;     // WPS_PBC
              if(argc == 5)
              {
                 if(!strcmp(argv[4],"persistent")) {
                        persistent_flag = 1;
              }
              strcpy((char *)wps_pin,(char *)"\0");
              }
           }
           else if(!strcmp(argv[3], "display")) {
              wps_method = 2;     // WPS_PIN_DISPLAY
              strcpy((char *)wps_pin,(char *)argv[4]);
           }
           else if(!strcmp(argv[3], "keypad")) {
              wps_method = 3;     // WPS_PIN_KEYPAD
              strcpy((char *)wps_pin,(char*)argv[4]);
           }
           else{
              P2P_PRINTF("wps mode error.\n");
              return -1;
           }

           rt = sscanf(argv[2], "%2x:%2x:%2x:%2x:%2x:%2x", \
                                &aidata[0], &aidata[1], &aidata[2], \
                                &aidata[3], &aidata[4], &aidata[5]);
           if (rt < 0)
           {
                P2P_PRINTF("wrong mac format.\n");
                return -1;
           }

           for (rt = 0; rt < 6; rt++)
           {
              peer_mac[rt] = (unsigned char)aidata[rt];
           }
        qca_p2p_connect_client(currentDeviceId, wps_method, peer_mac,(unsigned char*) wps_pin, persistent_flag);
    }
    else if(!strcmp(argv[1], "auth")) {
        extern void qca_p2p_auth(A_UINT8 device_id, int wps_method, unsigned char *peer_mac,unsigned char* wps_pin,A_UINT8 persistent_flag);
        int rt;
        int wps_method;
        int aidata[6];
        unsigned char peer_mac[6],wps_pin[9];
        A_UINT8 persistent_flag = 0;

        if(argc < 4){
            P2P_PRINTF("error cmd\n");
            return -1;
        }

        if(!strcmp(argv[3], "push")) {
            wps_method = 4;     // WPS_PBC
            if(argc == 5)
            {
                 if(!strcmp(argv[4],"persistent")) {
                        persistent_flag = 1;
                 }
                 strcpy((char *)wps_pin,(char *)"\0");
            }
        }
        else if(!strcmp(argv[3], "deauth")) {
            wps_method = 1;
        }
        else if(!strcmp(argv[3], "display")) {
            wps_method = 2;     // WPS_PIN_DISPLAY
            strcpy((char *)wps_pin,(char *)argv[4]);
        }
        else if(!strcmp(argv[3], "keypad")) {
            wps_method = 3;     // WPS_PIN_KEYPAD
            strcpy((char *)wps_pin,(char*)argv[4]);
        }
        else{
            P2P_PRINTF("wps mode error.\n");
            return -1;
        }

        rt = sscanf(argv[2], "%2x:%2x:%2x:%2x:%2x:%2x", \
                                &aidata[0], &aidata[1], &aidata[2], \
                                &aidata[3], &aidata[4], &aidata[5]);
        if (rt < 0)
        {
            P2P_PRINTF("wrong mac format.\n");
            return -1;
        }

        for (rt = 0; rt < 6; rt++)
        {
            peer_mac[rt] = (unsigned char)aidata[rt];
        }
        qca_p2p_auth(currentDeviceId, wps_method, peer_mac,(unsigned char*) wps_pin, persistent_flag);
    }
    else if(!strcmp(argv[1], "autogo")) {
        extern void qca_p2p_auto_go(A_UINT8 device_id, A_UINT8 flag);
        A_UINT8 persistent_flag = 0;
        if(argc > 2)
        {
                if(!strcmp(argv[2],"persistent")) {
                    persistent_flag = 1;
                }
        }
        qca_p2p_auto_go(currentDeviceId, persistent_flag);
    }
    else if(!strcmp(argv[1],"passphrase"))
    {
        extern void qca_set_passphrase(A_UINT8 device_id, char *pphrase,char *ssid) ;

        if(argc < 4)
        {
            P2P_PRINTF("\n Usage : wmiconfig --p2p passphrase <passphrase> <SSID> \n");
            return -1;
        }
        qca_set_passphrase(currentDeviceId, argv[2],argv[3]) ;
    }
    else if(!strcmp(argv[1], "prov")) {
        extern void qca_p2p_prov(A_UINT8 device_id, int wps_method, unsigned char *pmac);
        int rt;
        int wps_method;
        int aidata[6];
        unsigned char peer_mac[6];

        if(argc < 4){
            P2P_PRINTF("err or cmd\n");
            return -1;
        }

        rt = sscanf(argv[2], "%2X:%2X:%2X:%2X:%2X:%2X", \
                                &aidata[0], &aidata[1], &aidata[2], \
                                &aidata[3], &aidata[4], &aidata[5]);
        if (rt < 0)
        {
            P2P_PRINTF("wrong mac format.\n");
            return -1;
        }

        for (rt = 0; rt < 6; rt++)
        {
            peer_mac[rt] = (unsigned char)aidata[rt];
        }

        if(!strcmp(argv[3], "push")) {
            wps_method = 4;//WPS_PBC
        }
        else if(!strcmp(argv[3], "display")) {
            wps_method = 2;//WPS_PIN_DISPLAY
        }
        else if(!strcmp(argv[3], "keypad")) {
            wps_method = 3;//WPS_PIN_KEYPAD
        }
        else{
            P2P_PRINTF("wps mode error.\n");
            return -1;
        }
        qca_p2p_prov(currentDeviceId, wps_method, peer_mac);
    }
    else if(!strcmp(argv[1], "join")) {
        extern void qca_p2p_join(A_UINT8 device_id, int wps_method, unsigned char *pmac, unsigned char *wps_pin);
        int rt;
        int wps_method;
        int aidata[6];
        unsigned char peer_mac[6],wps_pin[9];

        if(argc < 4){
            P2P_PRINTF("error cmd\n");
            return -1;
        }

        rt = sscanf(argv[2], "%2X:%2X:%2X:%2X:%2X:%2X", \
                                &aidata[0], &aidata[1], &aidata[2], \
                                &aidata[3], &aidata[4], &aidata[5]);
        if (rt < 0)
        {
            P2P_PRINTF("wrong mac format.\n");
            return -1;
        }

        for (rt = 0; rt < 6; rt++)
        {
            peer_mac[rt] = (unsigned char)aidata[rt];
        }

        if(!strcmp(argv[3], "push")) {
            wps_method = 4;//WPS_PBC
            strcpy((char *)wps_pin,(char *)"\0");
        }
        else if(!strcmp(argv[3], "display")) {
            wps_method = 2;//WPS_PIN_DISPLAY
            strcpy((char *)wps_pin,(char*)argv[4]);
        }
        else if(!strcmp(argv[3], "keypad")) {
            wps_method = 3;//WPS_PIN_KEYPAD
            strcpy((char *)wps_pin,(char*)argv[4]);
        }
        else{
            P2P_PRINTF("wps mode error.\n");
            return -1;
        }
        qca_p2p_join(currentDeviceId, wps_method, peer_mac, wps_pin);
    }
    else if(!strcmp(argv[1], "invite")) {
        extern void qca_p2p_invite(A_UINT8 device_id, unsigned char *ssid, unsigned char *pmac, A_UINT8 method, unsigned char *wps_pin);
        int rt;
        A_UINT8 wps_method;
        int aidata[6];
        unsigned char peer_mac[6],wps_pin[9];
        unsigned char ssid[32];

        if(argc < 5){
            P2P_PRINTF("error cmd\n");
            return -1;
        }

        A_MEMSET(ssid, 0, 32);

        memcpy((char *)ssid,argv[2],strlen(argv[2]));

        rt = sscanf(argv[3], "%2X:%2X:%2X:%2X:%2X:%2X", \
                                &aidata[0], &aidata[1], &aidata[2], \
                                &aidata[3], &aidata[4], &aidata[5]);
        if (rt < 0)
        {
            P2P_PRINTF("wrong mac format.\n");
            return -1;
        }

        for (rt = 0; rt < 6; rt++)
        {
            peer_mac[rt] = (unsigned char)aidata[rt];
        }

        if(!strcmp(argv[4], "push")) {
            wps_method = 4;//WPS_PBC
            strcpy((char *)wps_pin,(char *)"\0");
        }
        else if(!strcmp(argv[4], "display")) {
            wps_method = 2;//WPS_PIN_DISPLAY
            strcpy((char *)wps_pin,(char*)argv[4]);
        }
        else if(!strcmp(argv[4], "keypad")) {
            wps_method = 3;//WPS_PIN_KEYPAD
            strcpy((char *)wps_pin,(char*)argv[4]);
        }
        else{
            P2P_PRINTF("wps mode error.\n");
            return -1;
        }
        qca_p2p_invite(currentDeviceId, ssid, peer_mac, wps_method, wps_pin);
    }
#if defined(P2P_HOSTLESS_PS_EN)
    else if(!strcmp(argv[1], "setnoa")) {
        A_UINT8 count_or_type;
        int duration = 0,interval = 0,start_or_offset =0;;
        extern void qca_setnoa(A_UINT8 count_or_type,int start_or_offset,int duration,int interval);
        if(argc < 3)
        {
            P2P_PRINTF("error cmd\n");
            return -1;
        }
        else{
            count_or_type   = atoi(argv[2]);
            if(argc > 3)
            {
               start_or_offset = atoi(argv[3]) * 1000;
               duration        = atoi(argv[4]) * 1000;
               interval        = atoi(argv[5]) * 1000;
            }
            qca_setnoa(count_or_type,start_or_offset,duration,interval);
        }

    }
#endif
    else{
           P2P_PRINTF("Error : P2P Command not found \n");
    }

      return 0;
}
#endif
