#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "net_functions.h"

int create_socket(int domain, int type, int protocol) {
    int result = socket(domain, type, protocol);
    if (result == INCORRECT_VALUE) {
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }
    return result; // Возвращаем дескриптор
}

void bind_socket(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int result = bind(sockfd, addr, addrlen);
    if (result == INCORRECT_VALUE) {
        perror("Can't associates a socket with an address. Bind failed!\n");
        exit(EXIT_FAILURE);
    }
}

// В этой функции, возможно, есть смысл не аварийно завершать всю программу
// в случае ошибки,а обработать ошибку и рекурсивно вызвать опять эту же
// функцию для получения сообщений от других клиентов (повысить устойчивость системы)
ssize_t recvive_from(int sockfd, void *buf, size_t len, int flags,
                     struct sockaddr *src_addr, socklen_t *addrlen) {
    ssize_t recv_bytes = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);    
    
    if (recv_bytes == INCORRECT_VALUE) {
        perror("Reciving message failed!\n");
        exit(EXIT_FAILURE);
    }

    return recv_bytes; // Возвращаем кол-во полученных байт
}

ssize_t send_to(int sockfd, const void *buf, size_t len, int flags,
                const struct sockaddr *dest_addr, socklen_t addrlen) {
    ssize_t sent_bytes = sendto(sockfd, buf, len, flags, dest_addr, addrlen);

    if (sent_bytes == INCORRECT_VALUE) {
        perror("Recieve failed!\n");
        exit(EXIT_FAILURE);
    }

    return  sent_bytes; // Возвращаем кол-во отправленных байт
}

void connect_to(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int result_code = connect(sockfd, addr, addrlen);
    
    if (result_code == INCORRECT_VALUE) {
        perror("Connection failed!\n");
        exit(EXIT_FAILURE);
    }
}

socklen_t fill_sockaddr_in(struct sockaddr_in* addr_struct,
                      short domain, unsigned short  port, unsigned long addr) {

    addr_struct->sin_family = domain;
    addr_struct->sin_port = port;
    addr_struct->sin_addr.s_addr = addr;

    return sizeof(struct sockaddr_in); 
} 




