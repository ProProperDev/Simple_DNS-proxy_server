#include <stdio.h> 
#include <string.h>  
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>

#include "net_functions.h"
#include "set.h"
#include "load_config.h"
#include "dns_packet.h"

//gcc -c dns-proxy_main.c net_functions.c set.c load_config.c dns_packet.c

#define DEFAULT_LOCAL_PORT  53
#define MAX_BUF_SIZE 65536

const int SENIOR_SERV_PORT = DEFAULT_LOCAL_PORT;
char* SENIOR_SERV_ADDR;

const int LISTEN_PORT = 12345; // стнд. порт 53; 12345 порт устанавливаем для проверки
unsigned long LISTEN_ADDR = INADDR_ANY; // Принимаем запросы от любого ip

char* CONFIG_FILE = "dns_proxy_conf.txt";

char* RDATA_FOR_BLOCK;

// Для хранения "чёрного списка" доменов будем использовать
// реализацию set из сторонней библиотеки для быстрого поиска
// Недостаток: блокировка домена не гарантирует блокировку субдомена
SimpleSet DOMAINS_BLACKLIST; 


int main( int argc , char *argv[]) {

    load_config(CONFIG_FILE);
    printf("The configuration has been downloaded and set successfully!\n");

    // Открываем сокет (IPv4 family, UDP, default)
    int udp_listen_socket = create_socket(AF_INET, SOCK_DGRAM, 0); 
    printf("UDP listen socket has been created successfully!\n");

    struct sockaddr_in listen_addr, senior_serv_addr;
      
    socklen_t listen_addr_len = fill_sockaddr_in(&listen_addr, AF_INET,
                                                 htons(LISTEN_PORT), LISTEN_ADDR);
   
    socklen_t senior_serv_addr_len = fill_sockaddr_in(&senior_serv_addr, AF_INET,
                                                      htons(SENIOR_SERV_PORT), inet_addr(SENIOR_SERV_ADDR));

    bind_socket(udp_listen_socket, (struct sockaddr*)&listen_addr, listen_addr_len);
    printf("UDP listen socket has been bound successfully!\n");

    unsigned char buffer[MAX_BUF_SIZE];

    // Структура для записи и хранения адреса клиента, от которого пришёл dns запрос
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_size = sizeof(struct sockaddr_in);

    while(true) {

        memset(&buffer, '\0', sizeof(buffer)); // Очищаем buffer
        
        memset(&client_addr, 0, client_addr_size); // Очищаем структуру, хранящую адрес клиента

        printf("\nWaiting message...\n");

        ssize_t msg_recv_bytes = recvive_from(udp_listen_socket, (char*)&buffer, sizeof(buffer),
                                              0,(struct sockaddr*)&client_addr , &client_addr_size);
        printf("Message from client recieved! Bytes recieved: %ld\n", msg_recv_bytes);
        
        char* query_domain = parse_name_from_dnsquery(buffer);
        printf("DNS request from client for \"%s\"\n", query_domain);

        if (set_contains(&DOMAINS_BLACKLIST, query_domain) == SET_TRUE) {          
            printf("Domain \"%s\" BANNED!\n", query_domain);

            ssize_t dnspacket_size = compose_dns_blockanswer(buffer, RDATA_FOR_BLOCK);
            
            ssize_t sent_bytes = send_to(udp_listen_socket, &buffer , dnspacket_size, 0,
                                         (struct sockaddr *)&client_addr, client_addr_size);
            printf("Sending DNS answer to client is done! Bytes sent = %ld\n", sent_bytes);
            
            free(query_domain);

            continue;
        }

        printf("DNS requst is forwarding to senior DNS server!\n");

        // Дополняем запрос для перенаправления вышестоящему серверу
        char* frwd_query = malloc(msg_recv_bytes + 3);
        frwd_query[0] = 0;
        frwd_query[1] = msg_recv_bytes;
        memcpy(frwd_query + 2, &buffer, msg_recv_bytes);

        // Открываем сокет (IPv4 family, TCP, default)
        int tcp_frwd_socket = create_socket(AF_INET, SOCK_STREAM, 0);
        printf("TCP forward socket has been created successfully!\n");

        connect_to(tcp_frwd_socket, (struct sockaddr*)&senior_serv_addr, senior_serv_addr_len);
        printf("TCP forward socket has been connected with senior DNS server successfully!\n");

        ssize_t sent_bytes = send(tcp_frwd_socket, frwd_query, msg_recv_bytes + 2, 0);

        memset(&buffer, '\0', sizeof(buffer)); // Очищаем buffer

        msg_recv_bytes = recv(tcp_frwd_socket, &buffer, sizeof(buffer), 0);
        printf("Answer from senior DNS server recieved! Bytes recieved = %ld\n", msg_recv_bytes);
        
        // Закрываем сокет для связи с вышестоящим сервером 
        close(tcp_frwd_socket);

        // Пересылаем ответ от вышестоящего сервера клиенту
        sent_bytes = send_to(udp_listen_socket, &buffer[2] , msg_recv_bytes - 2 , 0,
                             (struct sockaddr *)&client_addr, client_addr_size);
        printf("Sending DNS answer to client is done! Bytes sent = %ld\n", sent_bytes);
        
        free(query_domain);
        free(frwd_query);
    }

    close(udp_listen_socket);

    set_destroy(&DOMAINS_BLACKLIST);

    free(SENIOR_SERV_ADDR);

    free(RDATA_FOR_BLOCK);

    return 0;
}



