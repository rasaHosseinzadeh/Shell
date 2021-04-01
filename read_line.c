#include "read_line.h"

#define LINE_LEN 2048

char *read_line(){
    char *line = NULL;
    size_t len = LINE_LEN;
    if (getline(&line, &len, stdin)==-1){
        if (!feof(stdin)){
            perror("readline");
            exit(EXIT_FAILURE);
        } else {
            exit(EXIT_SUCCESS);
        }
    }
    return line;
}

