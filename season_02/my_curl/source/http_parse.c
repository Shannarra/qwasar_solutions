/* Taken from https://www.w3.org/Library/src/HTParse.html */

#include <string.h>
#include <ctype.h>

#include "http_parse.h"

void scan(char *raw_uri, Uri *uri)
{
    char *p;
    char *after_scheme = raw_uri;
    memset(uri, '\0', sizeof(Uri));

    /* Look for fragment identifier */
    if ((p = strchr(raw_uri, '#')) != NULL)
    {
        *p++ = '\0';
        uri->fragment = p;
    }

    if ((p = strchr(raw_uri, ' ')) != NULL)
        *p++ = '\0';

    for (p = raw_uri; *p; p++)
    {
        // Look for any whitespace. This is very bad for pipelining as it makes the request invalid
        
        if (isspace((int)*p))
        {
            char *orig = p, *dest = p + 1;
            while ((*orig++ = *dest++))
                ;
            p = p - 1;
        }
        if (*p == '/' || *p == '#' || *p == '?')
            break;
        if (*p == ':' && p[1] == '/' && p[2] == '/')
        {
            *p = 0;
            uri->scheme = after_scheme; /* Scheme has been specified */

            after_scheme = p + 3;

            if (0 == strcasecmp("URL", uri->scheme))
            {
                uri->scheme = NULL; /* Ignore IETF's URL: pre-prefix */
            }
            else
                break;
        }
    }

    p = after_scheme;
    uri->host = p;              /* host has been specified 	*/
    p = strchr(uri->host, '/'); /* look for end of host raw_uri if any */
    if (p)
    {
        *p = 0;                /* Terminate host */
        uri->resource = p + 1; /* Root has been found */
    }
    for (p = uri->host; *p && *p != ':'; p++);
    
    if (*p)
    {
        *p = 0;
        uri->port = p + 1;
    }
}
