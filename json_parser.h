/*
 * -----------------------------------------------------------
 * <vvolkanunal@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff.
 * -----------------------------------------------------------
*/

#ifndef __JSON_PARSER_
#define __JSON_PARSER_

#ifdef __cplusplus
extern "C" {
#endif

#define JSON_DEBUG
#define MAX_NUM_OF_TOKEN 64

#if defined JSON_DEBUG
#define JSON_LOG(...) do{ Serial.println(__VA_ARGS__); }while(0);
#else
#define JSON_LOG(...) do{ }while(0);
#endif


typedef void (*ParseCb)(void *data);

typedef enum 
{
    CHAR = 0x01,
    INT,
    STRING = 0x03,

}eget_token_type;

int 
json_validate(const char *json_string);

void*
get_json_value(const char *json_string, const char *key, eget_token_type type_of_token);

void 
set_fill_struct_from_json_cb(ParseCb Cb);

void 
fill_struct_from_json(void *data);

#ifdef __cplusplus
}
#endif

#endif
