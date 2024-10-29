#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "parse.tab.h"

//extern int yyparse();     // Function generated by Bison
//extern FILE *yyin;        // Input file pointer for Bison
//extern int parse_string(const char* in); // Declare the function from parse.y

int main(void) {
    std::cout << "leggo" << std::endl;
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
        return yyparse();


// Execute commands based on 'input' here
// Remember to fork a new process and use execve

    }


    free(input);
    return (0);
}