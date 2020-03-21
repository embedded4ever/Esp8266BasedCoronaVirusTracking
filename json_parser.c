/*
 * -----------------------------------------------------------
 * <vvolkanunal@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff.
 * -----------------------------------------------------------
*/

#include "json_parser.h"
#include "jsmn.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

static jsmn_parser p;
static jsmntok_t t[MAX_NUM_OF_TOKEN];
static int num_of_token;
static ParseCb ParseCallBack;

static int 
jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

static int 
get_json_value_as_int (const char *json_string, int num_of_token, const char *key)
{
    int ret = 0;
    char ret_arr[10];

    for(int i = 0; i < num_of_token; ++i)
    {
        if (jsoneq (json_string, &t[i], key) == 0)
        {
            snprintf(ret_arr, sizeof(ret_arr), "%.*s", t[i + 1].end - t[i + 1].start,
            json_string + t[i + 1].start);
            ret = atol(ret_arr);
            
            break;
        }
    }

    return ret;
}

static char* 
get_json_value_as_char (const char *json_string, int num_of_token, const char *key)
{
    static char ret[1];
    
    for(int i = 0; i < num_of_token; ++i)
    {
        if (jsoneq (json_string, &t[i], key) == 0)
        {
            snprintf(ret, sizeof(ret), "%.*s", t[i + 1].end - t[i + 1].start,
            json_string + t[i + 1].start);

            break;
        }
    }

    return ret;
}

static char* 
get_json_value_as_string (const char *json_string, int num_of_token, const char *key)
{
    static char ret[30];

    for (int i = 0; i < num_of_token; ++i)
    {
        if (jsoneq (json_string, &t[i], key) == 0)
        {
            snprintf(ret, sizeof(ret), "%.*s", t[i + 1].end - t[i + 1].start,
            json_string + t[i + 1].start);
            
            break;
        }
    }

    return ret;
}

int 
json_validate (const char *json_string)
{
    int ret = 0;
    num_of_token = 0;
    jsmn_init(&p);

    num_of_token = jsmn_parse(&p, json_string, strlen(json_string), t, 256);

    if (num_of_token < 0)
    {
        return (ret = 0);
    } 

    else 
    { 
       return (ret = 1);
    }
}

void* 
get_json_value (const char *json_string, const char *key, eget_token_type type_of_token)
{
    void *vret = NULL;
    
    switch (type_of_token)
    {
        case CHAR:
            vret = (void *)get_json_value_as_char(json_string, num_of_token, key);
            break;

        case INT:
            vret = (void *)get_json_value_as_int(json_string, num_of_token, key);
            break;

        case STRING:
            vret = (void *)get_json_value_as_string(json_string, num_of_token, key);
            break;
        
        default:
            break;
    }

    return vret;
}

void 
set_fill_struct_from_json_cb(ParseCb Cb)
{
    ParseCallBack = Cb;
}

void 
fill_struct_from_json(void *data)
{
    ParseCallBack(data);
}
