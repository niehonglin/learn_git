#include <stdio.h>
#include <string.h>
#include "qcom/qcom_utils.h"
#include "vsn_printf.h"
#include "hilink_osadapter.h"
#include "ww_hlink.h"

unsigned int hilink_strlen(const char* src)
{
	DEBUG;
	return (unsigned int)strlen(src);
}
char* hilink_strncpy(char* dst, const char* src, unsigned int len)
{
	DEBUG;
	return strncpy(dst, src, len);
}
char* hilink_strncat(char *dst, const char *src, unsigned int len)
{
	DEBUG;
	char* tail = dst;
	while(*tail) 
	{
		tail++;
	}

	strncpy(tail,src,len);
	return dst;
}
int hilink_strncmp(const char* str1, const char* str2, unsigned int len)
{
	DEBUG;
	return strncmp(str1, str2, len);
}
char* hilink_strchr(char* str, char ch)
{
	DEBUG;
	while(*str != '\0' && *str != ch)
	{
		++str;
	}

	return *str== ch?str:NULL;
}
char* hilink_strrchr(const char* str, char c)
{
	DEBUG;
	int len = strlen(str);  
	const char *last=str+len-1;
	while((*last!=c)&&(last != str))
	{  
		last--;  
	}  
	if(*last == c)
		return last;
	else
    		return NULL;
}
int hilink_atoi(const char* str)
{
	DEBUG;
	return atoi(str);
}
int hilink_snprintf(char* buf, unsigned int size, const char* format, ...)
{
	DEBUG;
	int len = 0;
	va_list args;
	va_start(args, format);
	len = vsnprintf(buf, size, format, args);
	va_end(args);  
	return len;
   
}
int hilink_sprintf(char* buf, const char* format, ...)
{
	DEBUG;
	int len = 0;
	va_list args;
	va_start(args, format);
	len = vsnprintf(buf, 0xffffffff, format, args);
	va_end(args);  
	return len;
}
int hilink_printf(const char* format, ...)
{
	DEBUG;
	char buffer[128];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer, 128, format, ap);
	va_end(ap); 
	//printf("%s",buffer);
	return 0;
  
}

