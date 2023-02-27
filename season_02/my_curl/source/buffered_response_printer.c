#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h> // fd_set
#include <stdlib.h>

#include "buffered_response_printer.h"

#define CONTENT_LENGTH_LABEL "content-length:"
#define CHUNKED_ENCODING_LABEL "transfer-encoding:"

#include <stddef.h>

typedef enum transfer_encoding
{
    REGULAR,
    CHUNKED
} TransferEncoding;

typedef struct
{
    TransferEncoding transfer_encoding;
    size_t content_length;
} Headers;

extern const struct headers_class
{
    Headers *(*fromRawString)(char *raw_headers);
} HeadersClass;

static Headers *from_raw_string(char *raw_headers);
const struct headers_class HeadersClass = {.fromRawString = &from_raw_string};

static Headers headers = {.transfer_encoding = REGULAR, .content_length = 0};

size_t min(size_t n1, size_t n2)
{
    return n1 <= n2 ? n1 : n2;
}

Headers *from_raw_string(char *raw_headers)
{
    static size_t content_length_len = 15;   // strlen(CONTENT_LENGTH_LABEL)
    static size_t chunked_encoding_len = 18; // strlen(CHUNKED_ENCODING_LABEL);
    size_t header_len = strlen(raw_headers);
    for (size_t i = min(content_length_len, chunked_encoding_len); i < header_len; i++)
    {
        if (i >= content_length_len && !strncasecmp(&raw_headers[i - content_length_len], CONTENT_LENGTH_LABEL, content_length_len))
        {
            headers.content_length = strtol(&raw_headers[i], NULL, 10);
            break;
        }
        if (i >= chunked_encoding_len && !strncasecmp(&raw_headers[i - chunked_encoding_len], CHUNKED_ENCODING_LABEL, chunked_encoding_len))
        {
            headers.transfer_encoding = CHUNKED;
            break;
        }
    }
    free(raw_headers);
    return &headers;
}

#define CAPACITY 1 << 13 // 8 k

static BufferedResponsePrinter *from_socket(Socket *socket);
const struct buffered_response_printer_class BufferedResponsePrinterClass = {.fromSocket = &from_socket};

static struct internals
{
    Socket *socket;
    char buffer[CAPACITY];
    size_t index;
    size_t length;
    size_t capacity;
} printer = {
    .socket = NULL,
    .buffer = {0},
    .index = 0,
    .length = 0,
    .capacity = CAPACITY
};

static void print(void);
static BufferedResponsePrinter printer_interface = {.print = &print};

BufferedResponsePrinter *from_socket(Socket *socket)
{
    printer.socket = socket;
    return &printer_interface;
}

static bool is_full(void);
static size_t load(void);
static char *get_raw_headers(void);
static void print_chunked_transfer(void);
static void print_chunk(size_t chunk_size);

void print(void)
{
    char *raw_headers = NULL;
    while (!raw_headers && !is_full())
    {
        load();
        raw_headers = get_raw_headers();
    }
    if (!raw_headers)
    {
        fprintf(stderr, "%s\n", "my_curl: Can't parse headers; too large");
    }
    else
    {
        Headers *headers = HeadersClass.fromRawString(raw_headers);
        if (headers->transfer_encoding == CHUNKED)
        {
            print_chunked_transfer();
        }
        else
        {
            print_chunk(headers->content_length);
        }
    }
}

void print_chunked_transfer(void)
{
    fprintf(stderr, "%s\n", "my_curl: Transfer-Encoding header not yet supported; Content-Length must be specified");
}

static bool is_empty(void);
static size_t unload(void);

void print_chunk(size_t chunk_size)
{
    size_t write_count = 0, read_count = printer.length;
    int socket_fd = printer.socket->getFd();
    fd_set read_set, write_set;
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    while (write_count < chunk_size)
    {
        FD_SET(socket_fd, &read_set);
        FD_SET(STDOUT_FILENO, &write_set);
        select(socket_fd + 1, &read_set, &write_set, NULL, NULL);
        if (!is_empty() && FD_ISSET(STDOUT_FILENO, &write_set))
        {
            write_count += unload();
        }
        if (read_count < chunk_size && !is_full() && FD_ISSET(socket_fd, &read_set))
        {
            read_count += load();
        }
    }
}

size_t load(void)
{
    size_t free_capacity = printer.capacity - printer.length;
    size_t insert_index = (printer.index + printer.length) % printer.capacity;
    size_t read_potential = min(free_capacity, printer.capacity - insert_index);
    size_t read_count = printer.socket->read(&printer.buffer[insert_index], read_potential);

    if (read_count == read_potential && read_potential < free_capacity)
    {
        read_count += printer.socket->read(printer.buffer, free_capacity - read_potential);
    }
    printer.length += read_count;
    return read_count;
}

size_t unload(void)
{
    size_t print_potential = printer.index + printer.length <= printer.capacity ? printer.length : printer.capacity - printer.index;
    size_t print_count = write(STDOUT_FILENO, &printer.buffer[printer.index], print_potential);

    if (print_count == print_potential && print_potential < printer.length)
    {
        print_count += write(STDOUT_FILENO, printer.buffer, printer.length - print_potential);
    }
    printer.index = (printer.index + print_count) % printer.capacity;
    printer.length -= print_count;
    return print_count;
}

char *get_raw_headers(void)
{
    static size_t body_index = 4; // length of "\r\n\r\n"
    for (; body_index <= printer.length; body_index++)
    {
        if (!strncmp(&printer.buffer[body_index - 4], "\r\n\r\n", 4))
        {
            break;
        }
    }
    if (body_index > printer.length)
        return NULL;
    printer.index = (printer.index + body_index) % printer.capacity;
    printer.length -= body_index;
    return strndup(printer.buffer, body_index);
}

bool is_empty(void)
{
    return printer.length == 0;
}

bool is_full(void)
{
    return printer.length == printer.capacity;
}
