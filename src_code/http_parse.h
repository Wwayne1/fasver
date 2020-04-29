#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

#include "http.h"

#define CR '\r'
#define LF '\n'

// http请求行解析
int http_parse_request_line(http_request_t *request);
// http请求体解析
int http_parse_request_body(http_request_t *request);

#endif
