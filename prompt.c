#include "prompt.h"

#define DIR_LEN_MAX 1024

#define GRN   "\x1b[32m"
#define BLU   "\x1b[34m"
#define RESET "\x1b[0m"


char* shell_prompt(char symbol, int stat){
    size_t size = DIR_LEN_MAX;
    char *cwd = malloc(sizeof(char)*size);
    if (getcwd(cwd, sizeof(char)*size) != NULL) {
        char *prompt = malloc(sizeof(char)*(size+10));
        sprintf(prompt, "%s%s\n%s%c(%d)%s", GRN, cwd, BLU, symbol, stat, RESET);
        free(cwd);
        return prompt;
    } else {
       perror("rsh: getcwd() error");
       return "$";
   }
}

