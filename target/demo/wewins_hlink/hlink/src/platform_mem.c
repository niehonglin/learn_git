#include "qcom/qcom_mem.h"
#include "qcom/base.h"
#include "ww_hlink.h"

void* hilink_memset(void* dst, int c, unsigned int len)
{
	DEBUG;
	return memset(dst, c, len);
}
void* hilink_memcpy(void* dst, const void* src, unsigned int len)
{
	DEBUG;
	return memcpy(dst,src,len);
}
int hilink_memcmp(const void* buf1, const void* buf2, unsigned int len)
{
	DEBUG;
	return memcmp(buf1,buf2,len);
}
void hilink_free(void* pt)
{
	DEBUG;
	mem_free(pt);
}
