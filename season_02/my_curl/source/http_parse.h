/* Taken from https://www.w3.org/Library/src/HTParse.html */

#ifndef MY_CURL_HTPARSE_H
#define MY_CURL_HTPARSE_H

typedef struct uri
{
    char *scheme;
    char *host;
    char *port;
    char *resource;
    char *fragment;
} Uri;

void scan(char *raw_uri, Uri *uri);

#endif
