#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define INCORRECT_VALUE -1

int create_socket(int domain, int type, int protocol);

void bind_socket(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ssize_t recvive_from(int sockfd, void *buf, size_t len, int flags,
                     struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t send_to(int sockfd, const void *buf, size_t len, int flags,
                const struct sockaddr *dest_addr, socklen_t addrlen);

void connect_to(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

socklen_t fill_sockaddr_in(struct sockaddr_in* addr_struct,
                           short domain, unsigned short  port, unsigned long addr);


