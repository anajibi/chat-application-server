//
// Created by alin2 on 02-Feb-20.
//

#ifndef CHAT_APPLICATION_PHASE1_CJSON_MINE_H
#define CHAT_APPLICATION_PHASE1_CJSON_MINE_H

#endif //CHAT_APPLICATION_PHASE1_CJSON_MINE_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6
#define cJSON_IsReference 256
#define cJSON_StringIsConst 512
#define DBL_EPSILON	__DBL_EPSILON__

typedef struct cJSON {
    struct cJSON *next,*prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct cJSON *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

    int type;					/* The type of the item, as above. */

    char *valuestring;			/* The item's string, if type==cJSON_String */
    int valueint;				/* The item's number, if type==cJSON_Number */
    double valuedouble;			/* The item's number, if type==cJSON_Number */
    char *string;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} cJSON;
typedef struct {
    char *buffer;
    int length;
    int offset;
} printbuffer;
static void *(*cJSON_malloc)(size_t sz) = malloc;
static void (*cJSON_free)(void *ptr) = free;
static int pow2gt (int x)	{	--x;	x|=x>>1;	x|=x>>2;	x|=x>>4;	x|=x>>8;	x|=x>>16;	return x+1;	}

static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static cJSON *cJSON_New_Item(void);
static char* cJSON_stringAllocation(const char* str);
cJSON *cJSON_CreateObject(void);
cJSON *cJSON_CreateString(const char *);
cJSON *cJSON_CreateArray(void);
cJSON *cJSON_Parse(const char *value);
cJSON *cJSON_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated);
cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);
static const char *skip(const char *in) {while (in && *in && (unsigned char)*in<=32) in++; return in;}
static cJSON *cJSON_New_Item(void);
void cJSON_Delete(cJSON *);
static char* cJSON_stringAllocation(const char* );
void   cJSON_AddItemToArray(cJSON *, cJSON *);
void   cJSON_AddItemToObject(cJSON *,const char *,cJSON *);
static void addItemToChild(cJSON *,cJSON *);
char *cJSON_Print(cJSON *);
static char *print_value(cJSON *,int ,int ,printbuffer *);
static char *print_number(cJSON *,printbuffer *);
static char* ensure(printbuffer *,int );
static char *print_string_ptr(const char *,printbuffer *);
static char *print_string(cJSON *item,printbuffer *p)	{return print_string_ptr(item->valuestring,p);}
static char *print_array(cJSON *item,int depth,int fmt,printbuffer *p);
static int update(printbuffer *p);
static char *print_object(cJSON *item,int depth,int fmt,printbuffer *p);
static const char *parse_value(cJSON *item,const char *value);
static int cJSON_strcasecmp(const char *s1,const char *s2);
int cJSON_GetArraySize(cJSON *array)	;
cJSON *cJSON_GetArrayItem(cJSON *array,int item);
static const char *parse_value(cJSON *item,const char *value);
static const char *parse_string(cJSON *item,const char *str);
static unsigned parse_hex4(const char *str){unsigned h=0;if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4;str++;if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4;str++;if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4;str++;if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;return h;}
static const char *parse_array(cJSON *item,const char *value);
static const char *parse_object(cJSON *item,const char *value);
static const char *parse_number(cJSON *item,const char *num);