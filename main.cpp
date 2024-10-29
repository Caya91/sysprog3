#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lex.yy.c"

int main(void) {
    char *input = NULL;
    size_t len = 0;
    ssize_t read;

    while (1) {
        printf("$ ");
        read = getline(&input, &len, stdin);


        if (read == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }

// Execute commands based on 'input' here
// Remember to fork a new process and use execve

    }


    free(input);
    return (0);
}