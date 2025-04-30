#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "process.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

// ASCII Art for TinyShell
const char *tinyShellArt[] = {
    "___________________________________________________________________________________",
    "|                                                                                 |",
    "|     ████████╗██╗███╗   ██╗██╗   ██╗███████╗██╗  ██╗███████╗██╗     ██╗          |",
    "|     ╚══██╔══╝██║████╗  ██║██║   ██║██╔════╝██║  ██║██╔════╝██║     ██║          |",
    "|        ██║   ██║██╔██╗ ██║██║   ██║███████╗███████║█████╗  ██║     ██║          |",
    "|        ██║   ██║██║╚██╗██║██║   ██║╚════██║██╔══██║██╔══╝  ██║     ██║          |",
    "|        ██║   ██║██║ ╚████║╚██  ██╔╝███████║██║  ██║███████╗███████╗███████╗     |",
    "|        ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚████╔╝ ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝     |",
    "|                              ██╔╝                                               |",
    "|                              ██║                                                |",
    "|                              ╚═╝                                                |",
    "|_________________________________________________________________________________|",
};

void printAnimatedText() {
    int numLines = sizeof(tinyShellArt) / sizeof(tinyShellArt[0]);
    
    for (int i = 0; i < numLines; i++) {
        printf("%s\n", tinyShellArt[i]);
        usleep(150000); // Sleep for 150 milliseconds to create animation effect
    }
}

void introduction(){
    system("clear");
    printf("\n\n");
    printAnimatedText();
    printf("\nWelcome to TinyShell!\n");
    printf("\nThis shell is programmed for Unix as a project for the Operating Systems course at HUST.\nTeam members:\n");
    printf("\tNguyen Quang Duc\n");
    printf("\tLe Duc Chinh\n");
    printf("\tHo Minh Dung \n");
    printf("\tNguyen Ba Duc Anh\n");
    printf("\n_________________________________________________________________________________\n");
    help();
}

void help(){
    printf("\ntinyShell supports the following commands:\n\n");
    printf("clear       : Clear screen\n");
    printf("child       : Run the child process\n");
    printf("list        : List all background processes\n");
    //kill -> completely stop
    printf("kill <PID>  : Kill the process with the given PID\n");
    printf("kill <name> : Kill the process with the given name\n");
    //stop -> temporary stop
    printf("stop <PID>  : Stop the process with the given PID\n");
    printf("stop <name> : Stop the process with the given name\n");
    //resume-> continue a process (which was stopped)
    printf("resume <PID>: Resume a stopped process with the given PID\n");
    printf("resume <name>: Resume a stopped process with the given name\n");

    printf("fg <PID>    : Bring a background process to foreground by PID\n");
    printf("fg <name>   : Bring a background process to foreground by name\n");
    printf("calc        : Open calculator (add & to run in background)\n");
    printf("timer <sec> : Run a timer for specified seconds\n");
    printf("prog <cmd>  : Run a program (e.g., prog calc)\n");
    printf("dir         : List the contents of the current directory\n");
    printf("date        : Display the system date\n");
    printf("time        : Display the system time\n");
    printf("execfile <path> : Execute a file (e.g., execfile ./sum_ab)\n");
    printf("exit        : Exit my shell\n");
    printf("help        : Print this help\n");
    printf("<cmd> [&]   : Run any command, add & to run in background\n");
    printf("\n");
}