//
// Created by martin on 27.10.24.
//

#include "functions.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>


struct Command {
    std::vector<const char *> cmdline;
    int input{};
    int output{};

    Command(std::vector<const char *>cmdline, int input, int output) : cmdline(std::move(cmdline)), input(input), output(output) {};
    // Overload the << operator for printing

    friend std::ostream& operator<<(std::ostream& os, const Command& command) {
        os << "Command: ";
        for (const auto &arg: command.cmdline) {
            if (arg) {
                os << arg << " ";
            }
        }
        os << "\nInput: " << command.input << ", Output: " << command.output << "\n";
        return os;
    }
};

std::vector<Command> pipe_cmds;


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


int main() {

    pipe_cmds.push_back(Command({"ls","-l", nullptr}, 0,0));
    pipe_cmds.push_back(Command({"grep","pattern", nullptr}, 0,0));
    pipe_cmds.push_back(Command({"wc","-l", nullptr}, 0,0));


    for (const auto& arg : pipe_cmds){
        std::cout << arg << std::endl;
    }

    std::cout << "leggo" << std::endl;
    std::vector<Command> commands = {
            Command({"ls", "-l", nullptr}, 0, 1),
            Command({"grep", "txt", nullptr}, 0, 1),
            Command({"wc", "-l", nullptr}, 0, 1)
    };

    executeCommands(commands);
    executeCommands(pipe_cmds);



    return 0;
}