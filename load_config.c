#include "load_config.h"

char* get_value(char* line) {
    char* token, *tmp;
    token = strtok(line, " ");
    for (;;) {
        if ((tmp = strtok(NULL, " ")) == NULL) {
            break;
        } else {
            token = tmp;
        }
    }
    return token;
}

char* string_value(char* value) {
    char *tmp = (char*)malloc(strlen(value)+1); // Вызвать free()!
    strcpy(tmp, value);
    value = tmp;
    if (value[strlen(value)-1] == '\n') {
        value[strlen(value)-1] = '\0';
    }
    
    return value;
}

void load_config(char* file) {
    char read_line[255];
    FILE *open_file = fopen(file, "r"); //open_file=fopen("c:\\c.txt","r");при указании пути
    if (!open_file) {
        perror("Error! Error can't open configuration file!\n");
    }

    // Инициализация set, в котором будем хранить чёрный список доменов
    set_init(&DOMAINS_BLACKLIST); 

    while (fgets(read_line, sizeof(read_line), open_file) != NULL) {
        //Если это коммент, то переходим на след строчку        
        if (read_line[0] == '#') { 
            memset(read_line, '\0', sizeof(read_line));            
            continue;
        }

        if(strstr(read_line, "UPSTREAM_SERV_ADDR") != NULL) {
            UPSTREAM_SERV_ADDR = string_value(get_value(read_line));
           
        } else if (strstr(read_line,"RDATA_FOR_BLOCK") != NULL) {
            RDATA_FOR_BLOCK = string_value(get_value(read_line));
        } else {
            // обрезаем строку так, чтобы /n, считанный из файла не учитывался   
            read_line[strlen(read_line)-2] = '\0';
            
            set_add(&DOMAINS_BLACKLIST, read_line);   
        }
        memset(read_line, '\0', sizeof(read_line));
    }

    if (fclose(open_file)) {
    perror("Error closing config_file\n");
    } 
}



