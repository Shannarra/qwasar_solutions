
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "socket.h"

const struct socket_class SocketClass = {.fromAddrInfo = &from_addr_info};

static struct internals
{
    int fd;
} socket_;

static Socket socket_interface = {
    .getFd = &get_fd,
    .connect = &connect_,
    .read = &read_,
    .write = &write_,
    .close = &close_
};

Socket *from_addr_info(struct addrinfo *addrInfo)
{
    int socket_fd = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
    if (socket_fd == -1)
        return NULL;
    socket_.fd = socket_fd;
    return &socket_interface;
}

int get_fd(void)
{
    return socket_.fd;
}

int connect_(struct addrinfo *addr_info)
{
    return connect(socket_.fd, addr_info->ai_addr, addr_info->ai_addrlen);
}

ssize_t read_(char *buffer, size_t num)
{
    return read(socket_.fd, buffer, num);
}

ssize_t write_(char *text)
{
    return write(socket_.fd, text, strlen(text));
}

void close_(void)
{
    if (close(socket_.fd) == -1)
        fprintf(stderr, "my_curl: error when closing socket: %s\n", strerror(errno));
}
