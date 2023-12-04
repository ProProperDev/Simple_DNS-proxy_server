#pragma once

#include <stdio.h> 
#include <string.h>  
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "set.h"

extern char* UPSTREAM_SERV_ADDR;

extern SimpleSet DOMAINS_BLACKLIST;

extern char* RDATA_FOR_BLOCK;

char* get_value(char* line);

char* string_value(char* value);

void load_config(char* file);
