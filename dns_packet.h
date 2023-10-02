#pragma once

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

/* RECORD TYPE */
enum {
  type_A = 1,
  type_NS = 2,
  type_CNAME = 5,
  type_SOA = 6,
  type_PTR = 12,
  type_MX = 15,
  type_TXT = 16,
  type_AAAA = 28,
  type_SRV = 33
};

#define class_IN 1 // Для INTERNET
#define IPv4_size 4 // Размер адреса IPv4 

struct DNS_HEADER {
    unsigned short id; // уникальный id пакета
 
    unsigned char rd :1; // флаг рекурсии
    unsigned char tc :1; // флаг усеченного сообщения
    unsigned char aa :1; // флаг авторитетного ответа
    unsigned char opcode :4; // код типа запроса
    unsigned char qr :1; // флаг типа сообщения запрос/ответ
 
    unsigned char rcode :4; // код результата обработки
    unsigned char z :3; // резерв
    unsigned char ra :1; // флаг поддержки рекурсии
 
    unsigned short q_count; // кол-во записей в секц запросов
    unsigned short ans_count; // кол-во записей в секц ответов
    unsigned short auth_count; // кол-во записей в секц authority
    unsigned short add_count; //  кол-во записей в секц additional
};

struct QUESTION_INFO {
    unsigned short qtype; // тип записи DNS, которую ищем
    unsigned short qclass; // класс запроса (IN для INTERNET)
};

struct ANSWER_INFO {
    unsigned short type :16; // тип записи DNS
    unsigned short class :16; // класс ресурсной записи
    unsigned int ttl :32; // допустимое время жизни записи в кэше
    unsigned short data_len :16; // длина поля данных, где содержится ip
};

ssize_t compose_dns_blockanswer(char buf[], const char* ip_);

char* parse_name_from_dnsquery(const unsigned char buf[]);



