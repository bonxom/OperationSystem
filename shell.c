#include "shell.h"
#include "process.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


void handleCommand(char *input) {
    if (strcmp(input, "help") == 0) help();
    else if (strcmp(input, "dir") == 0) dir();
    else if (strcmp(input, "date") == 0) date();
    else if (strcmp(input, "time") == 0) time_();
    else if (strcmp(input, "calc") == 0) openCalculator_fg();
    else if (strcmp(input, "calc &") == 0) openCalculator_bg();
    else if (strcmp(input, "child") == 0) createChild();
    else if (strcmp(input, "list") == 0) list();
    else if (strcmp(input, "clear") == 0) system("clear");
    else {
        parseAndHandleProcessCommand(input);
    }
}

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
    printf("resume<name>: Resume a stopped process with the given name\n");

    printf("dir         : List the contents of the current directory\n");
    printf("date        : Display the system date\n");
    printf("time        : Display the system time\n");
    printf("exit        : Exit my shell\n");
    printf("help        : Print this help\n");
    printf("\n");
}

