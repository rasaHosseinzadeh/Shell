#include "builtin.h"

int rsh_cd(char **args);
int rsh_exit(char **args);

const char *builtin_str[] = {
    "cd",
    "help",
    "exit",
};
const char *builtin_desc[] = {
    "Changes directory.",
    "Prints this help message.",
    "Exits the shell.",
};
int (*builtin_func[]) (char **) = {
    &rsh_cd,
    &rsh_help,
    &rsh_exit,
};
const unsigned int builtin_cnt = sizeof(builtin_str)/sizeof(char*);

int rsh_exit(char **args)
{
  exit(EXIT_SUCCESS);
}

int rsh_cd(char **args){
    if (args[1]==NULL){
        fprintf(stderr, "cd needs a directory.\n");
        return EXIT_FAILURE;
    }
    if (chdir(args[1]) != 0){
        perror("rsh cd");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}

int rsh_help(char **args){
    printf( "######          #####                #     #\n"
            "#     #   ##   #     #   ##    ####  #     # ###### #      #\n"
            "#     #  #  #  #        #  #  #      #     # #      #      #\n"
            "######  #    #  #####  #    #  ####  ####### #####  #      #\n"
            "#   #   ######       # ######      # #     # #      #      #\n"
            "#    #  #    # #     # #    # #    # #     # #      #      #\n"
            "#     # #    #  #####  #    #  ####  #     # ###### ###### ######\n"
            "------------------------------------------------------------------\n"
            "Built-in commands:\n");
    for(int i=0; i<builtin_cnt; ++i){
        printf("\t%s - %s\n", builtin_str[i], builtin_desc[i]);
    }
    printf("You can call other commands that are in PATH and use pipes.\n");
    return EXIT_SUCCESS;
}

int match_with_builtin(char **args){
    for (int i=0; i<builtin_cnt; i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    return -1;
}

