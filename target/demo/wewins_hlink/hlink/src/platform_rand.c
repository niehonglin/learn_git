#include "hilink_osadapter.h"
#include "ww_hlink.h"

static unsigned long next = 1;

int hilink_rand(void) 
{
	DEBUG;
	 next = next * 1103515245 + 12345;
	 return((unsigned)(next/65536) % 32768);
}
void hilink_srand(unsigned int value)
{
	DEBUG;
	next = value;
}
