#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "jsmn.h"
//#include "cJSON.h"
#include "qcom_mem.h"
#include "qcom_utils.h"
#include "hilink_osadapter.h"
#include "ww_hlink.h"

/**
 *@ingroup hilink_osadapter
 *@brief 将字符串形式的json数据解析为json结构体类型的数据。
 *
 *@par 描述:
 *<ul>
 *<li>根据用户提供的字符串形式的json数据，从中解析出json结构体类型的数据。</li>
 *</ul>
 *@attention
 *<ul>
 *<li>无</li>
 *</ul>
 *
 *@param value   [IN]     字符串形式的json数据
 *
 *@retval NULL 解析失败
 *@retval 非NULL 解析成功，返回值为json结构体指针
 *@par 依赖:
 *<ul><li>hilink_osadapter.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_json_delete
 *@since Huawei Liteos V100R002C00
 */

#define TOKENS_NUM 10
char *json_string = NULL;
jsmntok_t  *tokens;

void print_json()
{
	DEBUG;
	char ptr[30];
	int i = 0;
	for(i = 0;i<TOKENS_NUM;i++)
	{
		if(tokens[i].type != JSMN_UNDEFINED )
		{
			memset(ptr,0,30);
			strncpy(ptr,json_string+tokens[i].start,tokens[i].end-tokens[i].start);
			printf(" i = %d,%s\t tokens[%d].size = %d\n",i,ptr,i,tokens[i].size);
		}		
	}
}

void* hilink_json_parse(const char *value)
{
	DEBUG;
	json_string = value;
	int ret = -1;
	jsmn_parser parser;
	tokens = qcom_mem_calloc(TOKENS_NUM, sizeof(jsmntok_t));
	if(!tokens)
	{
		printf("mem_calloc failure \n");
		return NULL;
	}
	memset(tokens,0,sizeof(jsmntok_t)*TOKENS_NUM);
	
	jsmn_init(&parser);
	ret = jsmn_parse(&parser, value,strlen(value), tokens,TOKENS_NUM);
	if(ret < 0)
		return NULL;
	else
		return tokens;
}

/**
 *@ingroup hilink_osadapter
 *@brief 获取json结构体中的字符串value。
 *
 *@par 描述:
 *<ul>
 *<li>根据用户提供的json结构体指针object和需要获取的item的名字，从json结构体中获取对应的字符串value。</li>
 *</ul>
 *@attention
 *<ul>
 *<li>无</li>
 *</ul>
 *
 *@param object		[IN]     json结构体指针
 *@param name		[IN]     指定需要获取的item的名字
 *@param len		[OUT] 获取的字符串的长度
 *
 *@retval NULL      获取失败
 *@retval 非NULL    获取成功，返回值为字符串value首地址
 *@par 依赖:
 *<ul><li>hilink_osadapter.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_json_get_number_value
 *@since Huawei Liteos V100R002C00
 */

char* hilink_json_get_string_value(void* object, char* name, unsigned int *len) 
{
	DEBUG;
	char ptr[30];
	jsmntok_t  *json_pointer = NULL;
	if(object == NULL)
		return NULL;
	
	for(json_pointer = (jsmntok_t *)object;json_pointer->type != 0;json_pointer++)
	{
		memcpy(ptr, json_string+json_pointer->start, json_pointer->end - json_pointer->start);
		if(strcmp(ptr,name) == 0)
		{
			json_pointer++;
			*len = json_pointer->end - json_pointer->start;
			return json_string + (json_pointer)->start;
		}
	}
	
	return NULL;
}

/**
 *@ingroup hilink_osadapter
 *@brief 获取json结构体中的数值value。
 *
 *@par 描述:
 *<ul>
 *<li>根据用户提供的json结构体指针object和需要获取的item的名字，从json结构体中获取对应的数值value。</li>
 *</ul>
 *@attention
 *<ul>
 *<li>无</li>
 *</ul>
 *
 *@param object		[IN]     json结构体指针
 *@param name		[IN]     指定需要获取的item的名字
 *@param value		[OUT] 保存获取的item的值
 *
 *@retval 0    获取成功
 *@retval -1   获取失败
 *@par 依赖:
 *<ul><li>hilink_osadapter.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_json_get_string_value
 *@since Huawei Liteos V100R002C00
 */

int hilink_json_get_number_value(void* object, 	char* name, int *value)
{
	DEBUG;
	unsigned int len;
	char *ptr;
	char  buffer[20];
	ptr = hilink_json_get_string_value(object,name,&len);
	if(ptr)
	{
		strncpy(buffer,ptr,len);
		*value = atoi(buffer);
		return 0;
	}else
		return -1;
}

/**
 *@ingroup hilink_osadapter
 *@brief 释放json结构体所占用的空间。
 *
 *@par 描述:
 *<ul>
 *<li>根据用户提供的json结构体指针object，释放json数据所占用的空间。json结构体指针必须指向合法内存。</li>
 *</ul>
 *@attention
 *<ul>
 *<li>无</li>
 *</ul>
 *
 *@param object   [IN]     json结构体指针
 *
 *@retval 无
 *@par 依赖:
 *<ul><li>hilink_osadapter.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_json_parse
 *@since Huawei Liteos V100R002C00
 */
void hilink_json_delete(void* object) 
{
	DEBUG;
	mem_free(tokens);
}

int hilink_bi_get_cr(char *buf, unsigned int size) 
{
	DEBUG;
#if 1
    char *rsa_cipher =
    "A0FA503EBCB122EA7B49CAFC0C9B6452D9BC2F0F3CDE6E1EB539A784B61D7E89"\
    "714F75DF272F1D2F9B4FCB5340718841141D9E8CC35B400ACC71F914E8552EB5"\
    "1D55928BF73373B83124B0989003EDFC8616D71ACD704436F3515BE7C5D5F0D6"\
    "B567AB024CB3237B044B87B077BFE77D94CB5031C2F250A86A436CA9276D0CF1"\
    "F2FAE556E794A531674197A5E455CB2A5192D82A092DF0C5C8997B526C746CD1"\
    "986F06DEE818FF5AC7774A294BE15347F3B8028B1C6664169A84A0FC7753D828"\
    "372750FD328A06AB7AD9AE387B5B94BDF5C79930568FC94794EC4C119F8F6FBF"\
    "BCA57045D50FA2A02F14C0469105A3E900C9D8D48FE2C7F264B639F991A811B8";

	unsigned int cr_len = strlen(rsa_cipher) + 1;
	if (cr_len <= size)
	{
		memcpy(buf, rsa_cipher, cr_len);
		return 0;
	}
	else 
	{
		return -1;
	}
#endif
}
