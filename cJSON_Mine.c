//
// Created by alin2 on 02-Feb-20.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON_Mine.h"
static const char *ep;
cJSON *cJSON_CreateObject(void){
    cJSON *item=cJSON_New_Item();
    if(item)item->type=cJSON_Object;
    return item;
}
cJSON *cJSON_CreateString(const char *string)	{
    cJSON *item=cJSON_New_Item();
    if(item){item->type=cJSON_String;
        item->valuestring= cJSON_stringAllocation(string);
    }
    return item;
}
cJSON *cJSON_CreateArray(void)					{
    cJSON *item=cJSON_New_Item();
    if(item)item->type=cJSON_Array;
    return item;
}
static cJSON *cJSON_New_Item(void)
{
    cJSON* node = (cJSON*)cJSON_malloc(sizeof(cJSON));
    if (node) memset(node,0,sizeof(cJSON));
    return node;
}
/*void cJSON_Delete(cJSON *c)
{
    cJSON *next;
    while (c)
    {
        next=c->next;
        if (!(c->type&cJSON_IsReference) && c->child) cJSON_Delete(c->child);
        if (!(c->type&cJSON_IsReference) && c->valuestring) cJSON_free(c->valuestring);
        if (!(c->type&cJSON_StringIsConst) && c->string) cJSON_free(c->string);
        cJSON_free(c);
        c=next;
    }
}*/
static char* cJSON_stringAllocation(const char* str)
{
    size_t len;
    char* copy;

    len = strlen(str) + 1;
    if (!(copy = (char*)cJSON_malloc(len))) return 0;
    memcpy(copy,str,len);
    return copy;
}
void   cJSON_AddItemToArray(cJSON *array, cJSON *item){
    cJSON *c = array->child;
    if (!item) return;
    if (!c) {
        array->child=item;
    } else {
        while (c && c->next)
            c = c->next;
        addItemToChild(c,item);
    }
}
void   cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item)	{
    if (!item)
        return;
    if (item->string)
        cJSON_free(item->string);
    item->string= cJSON_stringAllocation(string);
    cJSON_AddItemToArray(object,item);
}
static void addItemToChild(cJSON *previousItem, cJSON *item) {
    previousItem->next=item;
    item->prev=previousItem;
}
char *cJSON_Print(cJSON *item)				{return print_value(item,0,1,0);}
static char *print_value(cJSON *item,int depth,int fmt,printbuffer *p){
    char *out=0;
    if (!item) return 0;

        switch ((item->type)&255)
        {
            case cJSON_NULL:	out= cJSON_stringAllocation("null");break;
            case cJSON_False:	out= cJSON_stringAllocation("false");break;
            case cJSON_True:	out= cJSON_stringAllocation("true");break;
            case cJSON_String:	out=print_string(item,0);break;
            case cJSON_Array:	out=print_array(item,depth,fmt,0);break;
            case cJSON_Object:	out=print_object(item,depth,fmt,0);break;
        }
    return out;
}
/*static char *print_number(cJSON *item,printbuffer *p){
    char *str=0;
    double d=item->valuedouble;
    if (d==0)
    {
        if (p)	str=ensure(p,2);
        else	str=(char*)cJSON_malloc(2);	*//* special case for 0. *//*
        if (str) strcpy(str,"0");
    }
    else if (fabs(((double)item->valueint)-d)<=DBL_EPSILON && d<=INT_MAX && d>=INT_MIN)
    {
        if (p)	str=ensure(p,21);
        else	str=(char*)cJSON_malloc(21);	*//* 2^64+1 can be represented in 21 chars. *//*
        if (str)	sprintf(str,"%d",item->valueint);
    }
    else
    {
        if (p)	str=ensure(p,64);
        else	str=(char*)cJSON_malloc(64);	*//* This is a nice tradeoff. *//*
        if (str)
        {
            if (fabs(floor(d)-d)<=DBL_EPSILON && fabs(d)<1.0e60)sprintf(str,"%.0f",d);
            else if (fabs(d)<1.0e-6 || fabs(d)>1.0e9)			sprintf(str,"%e",d);
            else												sprintf(str,"%f",d);
        }
    }
    return str;
}*/
static char* ensure(printbuffer *p,int needed){
    char *newbuffer;int newsize;
    if (!p || !p->buffer) return 0;
    needed+=p->offset;
    if (needed<=p->length) return p->buffer+p->offset;
    newsize=pow2gt(needed);
    newbuffer=(char*)cJSON_malloc(newsize);
    if (!newbuffer) {cJSON_free(p->buffer);p->length=0,p->buffer=0;return 0;}
    if (newbuffer) memcpy(newbuffer,p->buffer,p->length);
    cJSON_free(p->buffer);
    p->length=newsize;
    p->buffer=newbuffer;
    return newbuffer+p->offset;
}
static char *print_string_ptr(const char *str,printbuffer *p){
    const char *ptr;char *ptr2,*out;int len=0,flag=0;unsigned char token;

    for (ptr=str;*ptr;ptr++) flag|=((*ptr>0 && *ptr<32)||(*ptr=='\"')||(*ptr=='\\'))?1:0;
    if (!flag)
    {
        len=ptr-str;
        if (p) out=ensure(p,len+3);
        else		out=(char*)cJSON_malloc(len+3);
        if (!out) return 0;
        ptr2=out;*ptr2++='\"';
        strcpy(ptr2,str);
        ptr2[len]='\"';
        ptr2[len+1]=0;
        return out;
    }

    if (!str)
    {
        if (p)	out=ensure(p,3);
        else	out=(char*)cJSON_malloc(3);
        if (!out) return 0;
        strcpy(out,"\"\"");
        return out;
    }
    ptr=str;while ((token=*ptr) && ++len) {if (strchr("\"\\\b\f\n\r\t",token)) len++; else if (token<32) len+=5;ptr++;}

    if (p)	out=ensure(p,len+3);
    else	out=(char*)cJSON_malloc(len+3);
    if (!out) return 0;

    ptr2=out;ptr=str;
    *ptr2++='\"';
    while (*ptr)
    {
        if ((unsigned char)*ptr>31 && *ptr!='\"' && *ptr!='\\') *ptr2++=*ptr++;
        else
        {
            *ptr2++='\\';
            switch (token=*ptr++)
            {
                case '\\':	*ptr2++='\\';	break;
                case '\"':	*ptr2++='\"';	break;
                case '\b':	*ptr2++='b';	break;
                case '\f':	*ptr2++='f';	break;
                case '\n':	*ptr2++='n';	break;
                case '\r':	*ptr2++='r';	break;
                case '\t':	*ptr2++='t';	break;
                default: sprintf(ptr2,"u%04x",token);ptr2+=5;	break;	/* escape and print */
            }
        }
    }
    *ptr2++='\"';*ptr2++=0;
    return out;
}
static char *print_array(cJSON *item,int depth,int fmt,printbuffer *p)
{
    char **entries;
    char *out=0,*ptr,*ret;int len=5;
    cJSON *child=item->child;
    int numentries=0,i=0,fail=0;
    size_t tmplen=0;

    /* How many entries in the array? */
    while (child) numentries++,child=child->next;
    /* Explicitly handle numentries==0 */
    if (!numentries)
    {
        if (p)	out=ensure(p,3);
        else	out=(char*)cJSON_malloc(3);
        if (out) strcpy(out,"[]");
        return out;
    }

    if (p)
    {
        /* Compose the output array. */
        i=p->offset;
        ptr=ensure(p,1);if (!ptr) return 0;	*ptr='[';	p->offset++;
        child=item->child;
        while (child && !fail)
        {
            print_value(child,depth+1,fmt,p);
            p->offset=update(p);
            if (child->next) {len=fmt?2:1;ptr=ensure(p,len+1);if (!ptr) return 0;*ptr++=',';if(fmt)*ptr++=' ';*ptr=0;p->offset+=len;}
            child=child->next;
        }
        ptr=ensure(p,2);if (!ptr) return 0;	*ptr++=']';*ptr=0;
        out=(p->buffer)+i;
    }
    else
    {
        /* Allocate an array to hold the values for each */
        entries=(char**)cJSON_malloc(numentries*sizeof(char*));
        if (!entries) return 0;
        memset(entries,0,numentries*sizeof(char*));
        /* Retrieve all the results: */
        child=item->child;
        while (child && !fail)
        {
            ret=print_value(child,depth+1,fmt,0);
            entries[i++]=ret;
            if (ret) len+=strlen(ret)+2+(fmt?1:0); else fail=1;
            child=child->next;
        }

        /* If we didn't fail, try to malloc the output string */
        if (!fail)	out=(char*)cJSON_malloc(len);
        /* If that fails, we fail. */
        if (!out) fail=1;

        /* Handle failure. */
        if (fail)
        {
            for (i=0;i<numentries;i++) if (entries[i]) cJSON_free(entries[i]);
            cJSON_free(entries);
            return 0;
        }

        /* Compose the output array. */
        *out='[';
        ptr=out+1;*ptr=0;
        for (i=0;i<numentries;i++)
        {
            tmplen=strlen(entries[i]);memcpy(ptr,entries[i],tmplen);ptr+=tmplen;
            if (i!=numentries-1) {*ptr++=',';if(fmt)*ptr++=' ';*ptr=0;}
            cJSON_free(entries[i]);
        }
        cJSON_free(entries);
        *ptr++=']';*ptr++=0;
    }
    return out;
}
static int update(printbuffer *p)
{
    char *str;
    if (!p || !p->buffer) return 0;
    str=p->buffer+p->offset;
    return p->offset+strlen(str);
}
static char *print_object(cJSON *item,int depth,int fmt,printbuffer *p)
{
    char **entries=0,**names=0;
    char *out=0,*ptr,*ret,*str;int len=7,i=0,j;
    cJSON *child=item->child;
    int numentries=0,fail=0;
    size_t tmplen=0;
    /* Count the number of entries. */
    while (child) numentries++,child=child->next;
    /* Explicitly handle empty object case */
    if (!numentries)
    {
        if (p) out=ensure(p,fmt?depth+4:3);
        else	out=(char*)cJSON_malloc(fmt?depth+4:3);
        if (!out)	return 0;
        ptr=out;*ptr++='{';
        if (fmt) {*ptr++='\n';for (i=0;i<depth-1;i++) *ptr++='\t';}
        *ptr++='}';*ptr++=0;
        return out;
    }
    if (p)
    {
        /* Compose the output: */
        i=p->offset;
        len=fmt?2:1;	ptr=ensure(p,len+1);	if (!ptr) return 0;
        *ptr++='{';	if (fmt) *ptr++='\n';	*ptr=0;	p->offset+=len;
        child=item->child;depth++;
        while (child)
        {
            if (fmt)
            {
                ptr=ensure(p,depth);	if (!ptr) return 0;
                for (j=0;j<depth;j++) *ptr++='\t';
                p->offset+=depth;
            }
            print_string_ptr(child->string,p);
            p->offset=update(p);

            len=fmt?2:1;
            ptr=ensure(p,len);	if (!ptr) return 0;
            *ptr++=':';if (fmt) *ptr++='\t';
            p->offset+=len;

            print_value(child,depth,fmt,p);
            p->offset=update(p);

            len=(fmt?1:0)+(child->next?1:0);
            ptr=ensure(p,len+1); if (!ptr) return 0;
            if (child->next) *ptr++=',';
            if (fmt) *ptr++='\n';*ptr=0;
            p->offset+=len;
            child=child->next;
        }
        ptr=ensure(p,fmt?(depth+1):2);	 if (!ptr) return 0;
        if (fmt)	for (i=0;i<depth-1;i++) *ptr++='\t';
        *ptr++='}';*ptr=0;
        out=(p->buffer)+i;
    }
    else
    {
        /* Allocate space for the names and the objects */
        entries=(char**)cJSON_malloc(numentries*sizeof(char*));
        if (!entries) return 0;
        names=(char**)cJSON_malloc(numentries*sizeof(char*));
        if (!names) {cJSON_free(entries);return 0;}
        memset(entries,0,sizeof(char*)*numentries);
        memset(names,0,sizeof(char*)*numentries);

        /* Collect all the results into our arrays: */
        child=item->child;depth++;if (fmt) len+=depth;
        while (child)
        {
            names[i]=str=print_string_ptr(child->string,0);
            entries[i++]=ret=print_value(child,depth,fmt,0);
            if (str && ret) len+=strlen(ret)+strlen(str)+2+(fmt?2+depth:0); else fail=1;
            child=child->next;
        }

        /* Try to allocate the output string */
        if (!fail)	out=(char*)cJSON_malloc(len);
        if (!out) fail=1;

        /* Handle failure */
        if (fail)
        {
            for (i=0;i<numentries;i++) {if (names[i]) cJSON_free(names[i]);if (entries[i]) cJSON_free(entries[i]);}
            cJSON_free(names);cJSON_free(entries);
            return 0;
        }

        /* Compose the output: */
        *out='{';ptr=out+1;if (fmt)*ptr++='\n';*ptr=0;
        for (i=0;i<numentries;i++)
        {
            if (fmt) for (j=0;j<depth;j++) *ptr++='\t';
            tmplen=strlen(names[i]);memcpy(ptr,names[i],tmplen);ptr+=tmplen;
            *ptr++=':';if (fmt) *ptr++='\t';
            strcpy(ptr,entries[i]);ptr+=strlen(entries[i]);
            if (i!=numentries-1) *ptr++=',';
            if (fmt) *ptr++='\n';*ptr=0;
            cJSON_free(names[i]);cJSON_free(entries[i]);
        }

        cJSON_free(names);cJSON_free(entries);
        if (fmt) for (i=0;i<depth-1;i++) *ptr++='\t';
        *ptr++='}';*ptr++=0;
    }
    return out;
}
cJSON *cJSON_Parse(const char *value) {return cJSON_ParseWithOpts(value,0,0);}
cJSON *cJSON_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated)
{
    const char *end=0;
    cJSON *c=cJSON_New_Item();
    ep=0;
    if (!c) return 0;       /* memory fail */

    end=parse_value(c,skip(value));
    if (!end)	{cJSON_Delete(c);return 0;}	/* parse failure. ep is set. */

    /* if we require null-terminated JSON without appended garbage, skip and then check for a null terminator */
    if (require_null_terminated) {end=skip(end);if (*end) {cJSON_Delete(c);ep=end;return 0;}}
    if (return_parse_end) *return_parse_end=end;
    return c;
}
cJSON *cJSON_GetObjectItem(cJSON *object,const char *string)	{
    cJSON *c=object->child;
    while (c && cJSON_strcasecmp(c->string,string)) c=c->next;
    return c;
}
static int cJSON_strcasecmp(const char *s1,const char *s2){
    if (!s1) return (s1==s2)?0:1;if (!s2) return 1;
    for(; tolower(*s1) == tolower(*s2); ++s1, ++s2)	if(*s1 == 0)	return 0;
    return tolower(*(const unsigned char *)s1) - tolower(*(const unsigned char *)s2);
}
int cJSON_GetArraySize(cJSON *array){
    cJSON *c=array->child;
    int i=0;
    while(c)i++,c=c->next;
    return i;
}
cJSON *cJSON_GetArrayItem(cJSON *array,int item){
    cJSON *c=array->child;
    while (c && item>0) item--,c=c->next;
    return c;
}
static const char *parse_value(cJSON *item,const char *value){
    if (!value)						return 0;	/* Fail on null. */
    if (!strncmp(value,"null",4))	{ item->type=cJSON_NULL;  return value+4; }
    if (!strncmp(value,"false",5))	{ item->type=cJSON_False; return value+5; }
    if (!strncmp(value,"true",4))	{ item->type=cJSON_True; item->valueint=1;	return value+4; }
    if (*value=='\"')				{ return parse_string(item,value); }
    if (*value=='-' || (*value>='0' && *value<='9'))	{ return parse_number(item,value); }
    if (*value=='[')				{ return parse_array(item,value); }
    if (*value=='{')				{ return parse_object(item,value); }

    ep=value;return 0;	/* failure. */
}
static const char *parse_string(cJSON *item,const char *str)
{
    const char *ptr=str+1;char *ptr2;char *out;int len=0;unsigned uc,uc2;
    if (*str!='\"') {ep=str;return 0;}	/* not a string! */

    while (*ptr!='\"' && *ptr && ++len) if (*ptr++ == '\\') ptr++;	/* Skip escaped quotes. */

    out=(char*)cJSON_malloc(len+1);	/* This is how long we need for the string, roughly. */
    if (!out) return 0;

    ptr=str+1;ptr2=out;
    while (*ptr!='\"' && *ptr)
    {
        if (*ptr!='\\') *ptr2++=*ptr++;
        else
        {
            ptr++;
            switch (*ptr)
            {
                case 'b': *ptr2++='\b';	break;
                case 'f': *ptr2++='\f';	break;
                case 'n': *ptr2++='\n';	break;
                case 'r': *ptr2++='\r';	break;
                case 't': *ptr2++='\t';	break;
                case 'u':	 /* transcode utf16 to utf8. */
                    uc=parse_hex4(ptr+1);ptr+=4;	/* get the unicode char. */

                    if ((uc>=0xDC00 && uc<=0xDFFF) || uc==0)	break;	/* check for invalid.	*/

                    if (uc>=0xD800 && uc<=0xDBFF)	/* UTF16 surrogate pairs.	*/
                    {
                        if (ptr[1]!='\\' || ptr[2]!='u')	break;	/* missing second-half of surrogate.	*/
                        uc2=parse_hex4(ptr+3);ptr+=6;
                        if (uc2<0xDC00 || uc2>0xDFFF)		break;	/* invalid second-half of surrogate.	*/
                        uc=0x10000 + (((uc&0x3FF)<<10) | (uc2&0x3FF));
                    }

                    len=4;if (uc<0x80) len=1;else if (uc<0x800) len=2;else if (uc<0x10000) len=3; ptr2+=len;

                    switch (len) {
                        case 4: *--ptr2 =((uc | 0x80) & 0xBF); uc >>= 6;
                        case 3: *--ptr2 =((uc | 0x80) & 0xBF); uc >>= 6;
                        case 2: *--ptr2 =((uc | 0x80) & 0xBF); uc >>= 6;
                        case 1: *--ptr2 =(uc | firstByteMark[len]);
                    }
                    ptr2+=len;
                    break;
                default:  *ptr2++=*ptr; break;
            }
            ptr++;
        }
    }
    *ptr2=0;
    if (*ptr=='\"') ptr++;
    item->valuestring=out;
    item->type=cJSON_String;
    return ptr;
}
static const char *parse_array(cJSON *item,const char *value){
    cJSON *child;
    if (*value!='['){
        ep=value;return 0;
    }	/* not an array! */

    item->type=cJSON_Array;
    value= skip(value+1);
    if (*value==']') return value+1;	/* empty array. */

    item->child=child=cJSON_New_Item();
    if (!item->child) return 0;		 /* memory fail */
    value=skip(parse_value(child,skip(value)));	/* skip any spacing, get the value. */
    if (!value) return 0;

    while (*value==',')
    {
        cJSON *new_item;
        if (!(new_item=cJSON_New_Item())) return 0; 	/* memory fail */
        child->next=new_item;new_item->prev=child;child=new_item;
        value=skip(parse_value(child,skip(value+1)));
        if (!value) return 0;	/* memory fail */
    }

    if (*value==']') return value+1;	/* end of array */
    ep=value;return 0;	/* malformed. */
}
static const char *parse_object(cJSON *item,const char *value)
{
    cJSON *child;
    if (*value!='{')	{ep=value;return 0;}	/* not an object! */

    item->type=cJSON_Object;
    value=skip(value+1);
    if (*value=='}') return value+1;	/* empty array. */

    item->child=child=cJSON_New_Item();
    if (!item->child) return 0;
    value=skip(parse_string(child,skip(value)));
    if (!value) return 0;
    child->string=child->valuestring;child->valuestring=0;
    if (*value!=':') {
        ep=value;
        return 0;
    }	/* fail! */
    value=skip(parse_value(child,skip(value+1)));	/* skip any spacing, get the value. */
    if (!value) return 0;
    while (*value==',')
    {
        cJSON *new_item;
        if (!(new_item=cJSON_New_Item()))	return 0; /* memory fail */
        child->next=new_item;new_item->prev=child;child=new_item;
        value=skip(parse_string(child,skip(value+1)));
        if (!value) return 0;
        child->string=child->valuestring;child->valuestring=0;
        if (*value!=':') {ep=value;return 0;}	/* fail! */
        value=skip(parse_value(child,skip(value+1)));	/* skip any spacing, get the value. */
        if (!value) return 0;
    }

    if (*value=='}') return value+1;	/* end of array */
    ep=value;return 0;	/* malformed. */
}
static const char *parse_number(cJSON *item,const char *num)
{
    double n=0,sign=1,scale=0;int subscale=0,signsubscale=1;

    if (*num=='-') sign=-1,num++;	/* Has sign? */
    if (*num=='0') num++;			/* is zero */
    if (*num>='1' && *num<='9')	do	n=(n*10.0)+(*num++ -'0');	while (*num>='0' && *num<='9');	/* Number? */
    if (*num=='.' && num[1]>='0' && num[1]<='9') {num++;		do	n=(n*10.0)+(*num++ -'0'),scale--; while (*num>='0' && *num<='9');}	/* Fractional part? */
    if (*num=='e' || *num=='E')		/* Exponent? */
    {	num++;if (*num=='+') num++;	else if (*num=='-') signsubscale=-1,num++;		/* With sign? */
        while (*num>='0' && *num<='9') subscale=(subscale*10)+(*num++ - '0');	/* Number? */
    }

    n=sign*n*pow(10.0,(scale+subscale*signsubscale));	/* number = +/- number.fraction * 10^+/- exponent */

    item->valuedouble=n;
    item->valueint=(int)n;
    item->type=cJSON_Number;
    return num;
}
