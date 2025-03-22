#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

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

void introduction(){
    system("clear");
    printf("\n\n");
    printAnimatedText();
    printf("\nWelcome to TinyShell!\n");
    printf("\nThis shell is programmed for Unix as a project for the Operating Systems course at HUST.\nTeam members:\n");
    printf("\tHo Minh Dung 20235050\n");
    printf("\tHo Minh Dung 20235050\n");
    printf("\tHo Minh Dung 20235050\n");
    printf("\tHo Minh Dung 20235050\n");
    printf("\tHo Minh Dung 20235050\n");
    printf("\n_________________________________________________________________________________\n");
    help();
}

void kil(int type, char *id){ //type 1: PID, type 0: name

}

void stop(int type, char *id){

}

void resume(int type, char *id){

}

void list(){
    
}

void dir(){

}

void date(){

}

void time_(){

}


int isNumeric(char *str) {
    if (str == NULL || *str == '\0') return 0; // Check for NULL or empty string

    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) return 0; // If any character is not a digit, return false
    }
    return 1; // All characters are digits
}

int main() {
    introduction();

    while (1){
        printf("ShellHehe> ");
        char s[1000];
        fgets(s, 1000, stdin);
        s[strcspn(s, "\n")] = 0;

        if (strcmp(s, "exit") == 0) break;
        else if (strcmp(s, "help") == 0) help();
        else if (strcmp(s, "dir") == 0) dir();
        else if (strcmp(s, "date") == 0) date();
        else if (strcmp(s, "time") == 0) time_();
        else if (strcmp(s, "child") == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                printf("Child process is running\n");
                exit(0);
            }
        }
        else if (strcmp(s, "list") == 0) {
            printf("List of background processes:\n");
            list();
        }
        else if (strcmp(s, "clear") == 0) system("clear");
        else{//kill, stop, resume
            //input should split by space, example kill 1234, so we need to split kill and 1234
            //then check if 1234 is a number or not, if it is a number, then it is a PID, otherwise, it is a name
            char **str = malloc(4 * sizeof(char *));
            int i = 0;
            str[i] = strtok(s, " ");
            while (str[i] != NULL) {
                i++;
                if (i >= 4) break; // Prevent buffer overflow
                str[i] = strtok(NULL, " ");
            }

            if (i != 2) {
                printf("Invalid command, try again!\n");
                continue;
            }
            if (str[0] != NULL && str[1] != NULL) {
                int type = isNumeric(str[1]);
                if (strcmp(str[0], "kill") == 0) kil(type, str[1]);
                else if (strcmp(str[0], "resume") == 0) resume(type, str[1]);
                else if (strcmp(str[0], "stop") == 0) stop(type, str[1]);
                else printf("Invalid command, try again!\n");
            }

        }
    }
    return 0;
}
