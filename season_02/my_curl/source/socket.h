#ifndef MY_CURL_SOCKET_H
#define MY_CURL_SOCKET_H

#include <netdb.h>

typedef struct socket Socket;
struct socket {
    int (*getFd)(void);
    int (*connect)(struct addrinfo* addrInfo);
    ssize_t (*read)(char* buffer, size_t num);
    ssize_t (*write)(char* text);
    void (*close)(void);
};

extern const struct socket_class {
    Socket* (*fromAddrInfo)(struct addrinfo* addrInfo);
} SocketClass;

int get_fd(void);
int connect_(struct addrinfo *addr_info);
ssize_t read_(char *buffer, size_t num);
ssize_t write_(char *text);
void close_(void);

Socket *from_addr_info(struct addrinfo *addrInfo);

#endif
