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
    "\033[1;31m█████████████████████████████████████████████████████████████████████████████████████\033[0m",
    "\033[1;31m██                                                                                 ██\033[0m",
    "\033[1;31m██\033[0m     \033[1;33m████████╗██╗███╗   ██╗██╗   ██╗███████╗██╗  ██╗███████╗██╗     ██╗\033[0m          \033[1;31m██\033[0m",
    "\033[1;31m██\033[0m     \033[1;33m╚══██╔══╝██║████╗  ██║██║   ██║██╔════╝██║  ██║██╔════╝██║     ██║\033[0m          \033[1;31m██\033[0m",
    "\033[1;31m██\033[0m        \033[1;33m██║   ██║██╔██╗ ██║██║   ██║███████╗███████║█████╗  ██║     ██║\033[0m          \033[1;31m██\033[0m",
    "\033[1;31m██\033[0m        \033[1;33m██║   ██║██║╚██╗██║██║   ██║╚════██║██╔══██║██╔══╝  ██║     ██║\033[0m          \033[1;31m██\033[0m",
    "\033[1;31m██\033[0m        \033[1;33m██║   ██║██║ ╚████║╚██  ██╔╝███████║██║  ██║███████╗███████╗███████╗\033[0m     \033[1;31m██\033[0m",
    "\033[1;31m██\033[0m        \033[1;33m╚═╝   ╚═╝╚═╝  ╚═══╝ ╚████╔╝ ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\033[0m     \033[1;31m██\033[0m",
    "\033[1;31m██\033[0m                              \033[1;33m██╔╝\033[0m                                               \033[1;31m██\033[0m",
    "\033[1;31m██\033[0m                              \033[1;33m██║\033[0m                                                \033[1;31m██\033[0m",
    "\033[1;31m██\033[0m                              \033[1;33m╚═╝\033[0m                                                \033[1;31m██\033[0m",
    "\033[1;31m██                                                                                 ██\033[0m",
    "\033[1;31m█████████████████████████████████████████████████████████████████████████████████████\033[0m",
};

void printAnimatedText(){
    int numLines = sizeof(tinyShellArt) / sizeof(tinyShellArt[0]);

    for (int i = 0; i < numLines; i++)
    {
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
    printf("\t\033[1;35mNguyen Quang Duc\033[0m \n");
    printf("\t\033[1;35mLe Duc Chinh\033[0m \n");
    printf("\t\033[1;35mHo Minh Dung\033[0m \n");
    printf("\t\033[1;35mNguyen Ba Duc Anh\033[0m \n");
    printf("\n\033[1;31m_________________________________________________________________________________\033[0m\n");
    help();
}

void help(){
    printf("\ntinyShell supports the following commands:\n\n");
    printf("clear       : Clear screen\n");
    printf("child       : Run the child process\n");
    printf("list        : List all background processes\n");
    // kill -> completely stop
    printf("kill <PID>  : Kill the process with the given PID\n");
    printf("kill <name> : Kill the process with the given name\n");
    // stop -> temporary stop
    printf("stop <PID>  : Stop the process with the given PID\n");
    printf("stop <name> : Stop the process with the given name\n");
    // resume-> continue a process (which was stopped)
    printf("resume <PID>: Resume a stopped process with the given PID\n");
    printf("resume <name>: Resume a stopped process with the given name\n");

    printf("fg <PID>    : Bring a background process to foreground by PID\n");
    printf("calc        : Open calculator (add & to run in background)\n");
    printf("timer <sec> : Run a timer for specified seconds\n");
    printf("dir         : List the contents of the current directory\n");
    printf("date        : Display the system date\n");
    printf("time        : Display the system time\n");
    printf("exit        : Exit my shell\n");
    printf("help        : Print this help\n");
    printf("<cmd> [&]   : Run any command, add & to run in background\n");
    printf("shfile <path> : Execute a shell script file (example: shfile script.sh)\n");
    printf("cd <dir>    : Change directory to <dir>\n");
    printf("setenv <var> <value> : Set an environment variable\n");
    printf("appendenv <var> <value> : Append a value to an environment variable\n");
    printf("printenv    : Print all environment variables\n");
    printf("printPATH   : Print environment variable PATH\n");
    printf("mlem        : Wanna see our introduction again 👉👈? (just for fun)\n");
    printf("\n");
}