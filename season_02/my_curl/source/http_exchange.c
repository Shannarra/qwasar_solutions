#include <stdbool.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "http_exchange.h"
#include "socket.h"
#include "buffered_response_printer.h"

static void make_exchange(Uri *uri);
const struct http_exchange HttpExchange = {.withUri = &make_exchange};

static struct exchange
{
    Socket *socket;
    Uri *uri;
    bool failed;
} exchange = {
    .socket = NULL,
    .uri = NULL,
    .failed = false
};

static void send_request(void);
static void print_response(void);

void make_exchange(Uri *uri)
{
    exchange.uri = uri;
    send_request();
    if (!exchange.failed)
        print_response();
    close_();
}

static void create_connected_socket(void);
static void write_to_socket(void);

void send_request(void)
{
    create_connected_socket();
    if (!exchange.failed)
        write_to_socket();
}

void print_response(void)
{
    BufferedResponsePrinter *printer = BufferedResponsePrinterClass.fromSocket(exchange.socket);
    printer->print();
}

static void attempt_connection(struct addrinfo *addr_list);

void create_connected_socket(void)
{
    static const struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    struct addrinfo *addr_info;
    int getaddrinfo_return_value;
    Uri *uri = exchange.uri;

    if ((getaddrinfo_return_value = getaddrinfo(uri->host, uri->port ? uri->port : "80", &hints, &addr_info)) != 0)
    {
        fprintf(stderr, "my_curl: %s\n", gai_strerror(getaddrinfo_return_value));
        exchange.failed = true;
        return;
    }

    attempt_connection(addr_info);

    if (!exchange.socket)
    {
        fprintf(stderr, "Could not connect to %s\n", uri->host);
        exchange.failed = true;
    }
}

void write_to_socket(void)
{
    char request_text[BUFSIZ];
    Uri *uri = exchange.uri;
    int num_printed = snprintf(request_text, BUFSIZ,
                               "GET /%s HTTP/1.1\r\n"
                               "Host: %s\r\n\r\n",
                               uri->resource ? uri->resource : "", uri->host);
    if (num_printed == -1)
    {
        fprintf(stderr, "%s\n", "my_curl: encoding error in snprintf");
        exchange.failed = true;
    }
    else if (num_printed == BUFSIZ)
    {
        fprintf(stderr, "%s\n", "my_curl: cannot send a get request: URI too long");
        exchange.failed = true;
    }

    Socket *socket = exchange.socket;

    if (socket->write(request_text) == -1)
    {
        fprintf(stderr, "my_curl: error from write: %s\n", strerror(errno));
        exchange.failed = true;
    }
}

void attempt_connection(struct addrinfo *addr_list)
{
    for (struct addrinfo *addr_info = addr_list; addr_info; addr_info = addr_info->ai_next)
    {
        Socket *socket = SocketClass.fromAddrInfo(addr_info);

        if (!socket)
            continue;

        if (socket->connect(addr_info) != -1)
        {
            exchange.socket = socket;
            break;
        }

        socket->close();
    }
    freeaddrinfo(addr_list);
}
