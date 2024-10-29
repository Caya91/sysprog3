//
// Created by martin on 27.10.24.
//
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <unistd.h>


#ifndef SYSPROG3_FUNCTIONS_H
#define SYSPROG3_FUNCTIONS_H

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


void add_argument(char *arg);
void add_command(char *cmd);
void print_cmds();


#endif //SYSPROG3_FUNCTIONS_H
