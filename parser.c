#include "parser.h"

#define ARG_BUF_LEN 64
#define PIPE_LIST_LEN 16

#define PARSER_STATE_BEG 0
#define PARSER_STATE_SQ 1
#define PARSER_STATE_DQ 2
#define PARSER_STATE_NT 3

char*** parse_line(char *line){
    size_t size = ARG_BUF_LEN;
    size_t pipe_size = PIPE_LIST_LEN;
    int pipe_index = 0;
    char *** pipes = malloc(pipe_size*sizeof(char**));
    pipes[pipe_index] = malloc(size*sizeof(char*));
    int state = PARSER_STATE_BEG;
    int tok_cnt=0, pos=0;
    while(line[pos]!='\0'){
        if(size <= tok_cnt + 2){
            size *= 2;
            pipes[pipe_index] = realloc(pipes[pipe_index], size * sizeof(char*));
        }
        switch(state){
            case PARSER_STATE_BEG:
                switch (line[pos]){
                    case '|':
                        line[pos] = '\0';
                        pipes[pipe_index] = realloc(pipes[pipe_index], (tok_cnt+1)*sizeof(char*));
                        size = ARG_BUF_LEN;
                        tok_cnt=0;
                        if(pipe_size <= ++pipe_index + 2){
                            pipe_size *= 2;
                            pipes = realloc(pipes, pipe_size * sizeof(char**));
                        }
                        pipes[pipe_index] = malloc(size*sizeof(char*));
                        break;
                    case ' ':
                    case '\t':
                    case '\n':
                        line[pos] = '\0';
                        break;
                    case '\'': 
                        line[pos] = '\0';
                        if (line[++pos] != '\''){
                            pipes[pipe_index][tok_cnt++] = &line[pos];
                            state = PARSER_STATE_SQ;
                        }
                        break;
                    case '"':
                        line[pos]='\0';
                        if (line[++pos] != '"'){
                            pipes[pipe_index][tok_cnt++] = &line[pos];
                            state = PARSER_STATE_DQ;
                        }
                        break;
                    default:
                        pipes[pipe_index][tok_cnt++] = &line[pos];
                        state = PARSER_STATE_NT;
                }
                break;
            case PARSER_STATE_NT:
                switch (line[pos]){
                    case '|':
                        line[pos] = '\0';
                        pipes[pipe_index] = realloc(pipes[pipe_index], (tok_cnt+1)*sizeof(char*));
                        size = ARG_BUF_LEN;
                        tok_cnt=0;
                        if(pipe_size <= ++pipe_index + 2){
                            pipe_size *= 2;
                            pipes = realloc(pipes, pipe_size * sizeof(char**));
                        }
                        pipes[pipe_index] = malloc(size*sizeof(char*));
                        state = PARSER_STATE_BEG;
                        break;
                    case ' ':
                    case '\t':
                    case '\n':
                        line[pos] = '\0';
                        state = PARSER_STATE_BEG;
                        break;
                    case '\'':
                        state = PARSER_STATE_BEG;
                        --pos;
                        break;
                    case '"':
                        state = PARSER_STATE_BEG;
                        --pos;
                        break;
                }
                break;
            case PARSER_STATE_SQ:
                if (line[pos] == '\''){
                    line[pos] = '\0';
                    state = PARSER_STATE_BEG;
                }
                break;
            case PARSER_STATE_DQ: 
                if (line[pos] == '"'){
                    line[pos] = '\0';
                    state = PARSER_STATE_BEG;
                }
                break;
        }
        ++pos;
    }
    pipes[++pipe_index] = NULL;
    pipes = realloc(pipes, (pipe_index+1) * sizeof(char**));
    return pipes;
}

