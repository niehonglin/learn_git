#include "qcom/qcom_time.h"
#include "hilink_osadapter.h"
#include "ww_hlink.h"
int hilink_gettime(unsigned long* ms) 
{
	DEBUG;
	unsigned long curr = time_ms();
	
	*ms=curr;
	
	return 0;
}
