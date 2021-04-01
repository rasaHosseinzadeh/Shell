#include "exe.h"

int rsh_remote_exe(char** args);
int rhs_exe(char** args);

int rsh_remote_exe(char** args){
    pid_t pid;
    int stat=0;
    pid = fork();
    if (pid == 0) {
        stat = execvp(args[0], args);
        if (stat == -1){
            perror("rsh");
        }
        exit(stat);
    } else if (pid < 0) {
        perror("rsh");
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &stat, WUNTRACED);
    }
    return stat;
}

int rsh_exe(char** args){
    if (args[0] == NULL) {
        return EXIT_SUCCESS;
    }
    int res = match_with_builtin(args);
    if (res == -1){
        res = rsh_remote_exe(args);
    }
    return res;
}

int rsh_exe_pipes(char*** pipes){
    int stat=0;
    if (pipes[0] == NULL){
        return EXIT_SUCCESS;
    }
    else if (pipes[1] == NULL){
        stat = rsh_exe(pipes[0]);
        return EXIT_SUCCESS;
    }
    else{
        int pipefd[2];
        pid_t pid1, pid2;
        if (pipe(pipefd) < 0) { 
            perror("rsh: Pipe could not be initialized."); 
            return EXIT_FAILURE; 
        }
        
        pid1 = fork(); 
        if (pid1 < 0) { 
            perror("rsh: Could not fork."); 
        return EXIT_FAILURE; 
        }
        else if (pid1 == 0) {
            close(pipefd[0]); 
            dup2(pipefd[1], STDOUT_FILENO);
            stat = rsh_exe(pipes[0]);
            exit(stat);
        } else {
            pid2 = fork();
            if (pid2 < 0) { 
                perror("rsh: Could not fork."); 
                return EXIT_FAILURE; 
            }
            else if (pid2 == 0) {
                close(pipefd[1]); 
                dup2(pipefd[0], STDIN_FILENO);
                stat = rsh_exe_pipes(++pipes);
                exit(stat);
            } else {
                waitpid(pid1, &stat, WUNTRACED);
                close(pipefd[1]);
                close(pipefd[0]);
                waitpid(pid2, &stat, WUNTRACED);
            }
        }

    }
    return stat;
}
