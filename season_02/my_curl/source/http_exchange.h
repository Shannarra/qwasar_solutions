#ifndef MY_CURL_HTTP_EXCHANGE_H
#define MY_CURL_HTTP_EXCHANGE_H

#include "http_parse.h"

extern const struct http_exchange
{
    void (*withUri)(Uri *uri);
} HttpExchange;

#endif
