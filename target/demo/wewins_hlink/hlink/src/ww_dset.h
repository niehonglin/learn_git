#ifndef __WW_DSET_H__
#define __WW_DSET_H__

#define WW_NV_START_ADDR    0x0007E000
#define WW_NV_BLOCK_SZ      4*1024 //4*1024

/*#define  WW_DSETID_NVRAM_LICENSE 0x00010001
#define  WW_DSETID_NVRAM_SN      0x00010002
#define  WW_DSETID_NVRAM_PUBKEY  0x00010003
#define  WW_DSETID_NVRAM_DHKEY   0x00010004
#define  WW_DSETID_NVRAM_WIFI    0x00010005
#define  WW_DSETID_NVRAM_AP      0x00010006*/

#define  WW_DSETID_NVRAM_PRODUCT 0x00010001
#define  WW_DSETID_NVRAM_ALINK      0x00010002


#define  WW_WIFI_SSID_LEN 64
#define  WW_WIFI_KEY_LEN  64


int ww_write_nv(unsigned char *buf, unsigned int count,unsigned int dset_id);
int ww_read_nv(unsigned char *buf, unsigned int count,unsigned int dset_id);
void ww_erase_nv(unsigned int dset_id);

#endif

