#ifndef MY_CURL_BUFFERED_RESPONSE_PRINTER_H
#define MY_CURL_BUFFERED_RESPONSE_PRINTER_H

#include <stdbool.h>

#include "socket.h"

typedef struct buffered_response_printer BufferedResponsePrinter;

struct buffered_response_printer
{
    void (*print)(void);
};

extern const struct buffered_response_printer_class
{
    BufferedResponsePrinter *(*fromSocket)(Socket *socket);
} BufferedResponsePrinterClass;

#endif
