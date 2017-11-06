#include "hilink_osadapter.h"
#include "ww_dset.h"
#include "ww_hlink.h"
#define WW_DSETID_NVRAM_HLINK 10005
int hilink_save_flash(char* buf, unsigned int len)
{
	DEBUG;
	return ww_write_nv((unsigned char *)buf, len, WW_DSETID_NVRAM_HLINK);
}
int hilink_read_flash(char* buf, unsigned int len)
{
	DEBUG;
	return ww_read_nv((unsigned char *)buf, len, WW_DSETID_NVRAM_HLINK);
}

