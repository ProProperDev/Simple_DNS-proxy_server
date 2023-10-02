#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "dns_packet.h" 

ssize_t compose_dns_blockanswer(char buf[], const char* ip_) {
    //Редактируем DNS_HEADER    
    unsigned int current_pos = 0;
    struct DNS_HEADER* dns_header = (struct DNS_HEADER*)&buf[current_pos];
    dns_header->ans_count = htons(1); // Один ответ

    //Заполняем имя домена в секции ответа, копируя имя из секции запроса
    current_pos += sizeof(struct DNS_HEADER);
    unsigned char* q_name = (unsigned char*)&buf[current_pos];
    current_pos += (strlen((const char*)q_name) + 1) + sizeof(struct QUESTION_INFO);
    unsigned char* ans_name = &buf[current_pos];
    strcpy(ans_name, q_name);

    //Заполняем инфо секции ответа
    current_pos += (strlen((const char*)ans_name)) + 1;
    struct ANSWER_INFO* answ_flags = (struct ANSWER_INFO*)&buf[current_pos];
    answ_flags->type = htons(type_A);
    answ_flags->class = htons(class_IN);
    answ_flags->ttl = 0; 
    answ_flags->data_len = htons(IPv4_size);

    //Заполняем поле данных ответом,
    //предусмотренным для заблокированных доменов
    current_pos += sizeof(struct ANSWER_INFO) - 2;
    long* rdata = (long*)&buf[current_pos];
    *rdata = inet_addr(ip_);

    return (ssize_t)current_pos + sizeof(long);    
}

// Выделяем имя домена из запроса и конвертируем из 3www6google3com0
//в формат "www.google.com" и возвращаем его
char* parse_name_from_dnsquery(const unsigned char buf[]) {
    unsigned char* qname = (unsigned char*)&buf[sizeof(struct DNS_HEADER)];
    char* name = malloc(strlen(qname));
    strcpy(name, qname);
    int p, i, j = 0;

    for(i = 0;i < (int)strlen((const char*)name);i++) {
        p = name[i];
        for( j = 0;j < (int)p;j++) {
            name[i] = name[i+1];
            i = i + 1;
        }
        name[i] = '.';
    }
    name[i - 1] = '\0';

    return name;
}



