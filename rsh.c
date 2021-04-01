#include "builtin.h"
#include "read_line.h"
#include "parser.h"
#include "prompt.h"
#include "exe.h"

int main(int argc, char **argv){
    char *line;
    char ***pipes;
    int stat=0;
    rsh_help(NULL);
    do{
        char *prompt = shell_prompt('$', stat);
        printf("%s", prompt);
        free(prompt);
        line = read_line();
        pipes = parse_line(line);
        stat = rsh_exe_pipes(pipes);
        free(line);
        for(int i=0; pipes[i]!=NULL; ++i){
            free(pipes[i]);
        }
        free(pipes);
    }while(1);
    return EXIT_SUCCESS;
}

