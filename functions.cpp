//
// Created by martin on 27.10.24.
//

#include "functions.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

std::vector<Command> pipe_cmds;

// füge Argument an letzten Command an
void add_argument(char *arg){
    pipe_cmds.back().cmdline.emplace_back(arg);
}

//Füge Command in Liste der Commands ein
void add_command(char *cmd){
    pipe_cmds.emplace_back(std::vector<const char*> {cmd},0,1);
}

// setzt input für erstes Command auf Ursprung von redirect input
bool set_input(char* file){
    if (file == nullptr){
        std::cerr << "error: inputfile is NULL" << std::endl;
        return false;
    }

    int input_fd = open(file, O_RDONLY);
    std::cout << "Input FD:" << input_fd << std::endl;

    if(input_fd < 0) {
        std::cerr << "error opening file: " << strerror(errno) << ": " << file << std::endl;
        return false;
    } else {
        pipe_cmds.back().input = input_fd;
        return true;
    }

}

// setzt output für ketztes Command auf Ziel von redirect output
bool set_output(char* file){
    if(file == nullptr) {
        std::cerr << "error: output file is null" << std::endl;
        return false;
    }

    int out_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    std::cout << "Output FD:" << out_fd << std::endl;


    if(out_fd < 0) {
        std::cerr << "error opening file: " << strerror(errno) << ": " << file << std::endl;
        return false;
    } else {
        pipe_cmds.back().output = out_fd;
        return true;
    }
}



void print_cmds(){
    for (const auto& arg : pipe_cmds){
        std::cout << arg << std::endl;
    }
}


void executeCommands(const std::vector<Command>& commands) {
    int numCommands = commands.size();
    int pipefd[2 * (numCommands - 1)]; // Array to hold pipe file descriptors

    // Create pipes
    for (int i = 0; i < numCommands - 1; ++i) {
        if (pipe(pipefd + i * 2) == -1) {
            perror("pipe");
            return;
        }
    }

    // Execute commands
    for (int i = 0; i < numCommands; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return;
        }

        if (pid == 0) { // Child process
            // Set input from the previous pipe if not the first command


            if (i > 0) {
                dup2(pipefd[(i - 1) * 2], STDIN_FILENO);
            }

            // Set output to the next pipe if not the last command
            if (i < numCommands - 1) {
                dup2(pipefd[i * 2 + 1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors
            for (int j = 0; j < 2 * (numCommands - 1); ++j) {
                close(pipefd[j]);
            }

            // Execute the command
            execvp(commands[i].cmdline[0], const_cast<char * const *>(commands[i].cmdline.data()));
            perror("execvp"); // If execvp fails
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipe file descriptors in the parent
    for (int i = 0; i < 2 * (numCommands - 1); ++i) {
        close(pipefd[i]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < numCommands; ++i) {
        wait(nullptr);
    }
}
