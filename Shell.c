#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

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

void help() {
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

void introduction() {
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

//use an array of struct to store the information of the process
typedef struct {
    int pid;
    int status; // 0: stopped, 1: running
} Process;
#define MAX_PROCESSES 100
Process processes[MAX_PROCESSES];
int processCount = 0;
pid_t fg_pid = -1;

void addProcess(pid_t pid) {
    if (processCount < MAX_PROCESSES) {
        processes[processCount].pid = pid;
        processes[processCount].status = 1;
        processCount++;
        printf("Process %d added to the list.\n", pid);
    } else {
        printf("Process list is full.\n");
    }
}

void list() {
    printf("PID\tSTATUS\n");
    for (int i = 0; i < processCount; i++) {
        printf("%d\t%s\n", processes[i].pid, processes[i].status ? "Running" : "Stopped");
    }
}

void kil(int type, char *id) {
    if (type == 1) {
        pid_t pid = atoi(id);
        if (kill(pid, SIGKILL) == 0) {
            printf("Process %d killed successfully.\n", pid);
        } else {
            perror("Failed to kill process");
        }
    } else {
        char command[256];
        snprintf(command, sizeof(command), "pkill %s", id);
        if (system(command) == 0) {
            printf("Process %s killed successfully.\n", id);
        } else {
            printf("Failed to kill process %s.\n", id);
        }
    }
}

void stop(int type, char *id) {
    pid_t pid = (type == 1) ? atoi(id) : -1;
    if (pid > 0) {
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == pid) {
                kill(pid, SIGSTOP);
                processes[i].status = 0;
                printf("Process %d stopped.\n", pid);
                return;
            }
        }
        printf("Process not found.\n");
    } else {
        printf("Invalid PID.\n");
    }
}

void resume(int type, char *id) {
    pid_t pid = (type == 1) ? atoi(id) : -1;
    if (pid > 0) {
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == pid) {
                kill(pid, SIGCONT);
                processes[i].status = 1;
                printf("Process %d resumed.\n", pid);
                return;
            }
        }
        printf("Process not found.\n");
    } else {
        printf("Invalid PID.\n");
    }
}

void fg(int type, char *id) {
    pid_t pid = (type == 1) ? atoi(id) : -1;
    if (pid > 0) {
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == pid) {
                kill(pid, SIGCONT);
                processes[i].status = 1;
                fg_pid = pid;
                waitpid(pid, NULL, 0);
                fg_pid = -1;
                return;
            }
        }
        printf("Process not found.\n");
    } else {
        printf("Invalid PID.\n");
    }
}

void dir() {
    struct dirent *dir;
    DIR *d = opendir(".");
    if (d == NULL) {
        perror("opendir");
        return;
    }
    while ((dir = readdir(d)) != NULL) {
        printf("%s\n", dir->d_name);
    }
    closedir(d);
}

void date() {
    system("date");
}

void time_() {
    setenv("TZ", "Asia/Ho_Chi_Minh", 1);
    tzset();
    time_t t;
    struct tm *tm_info;
    char buffer[20];
    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Current time: %s\n", buffer);
}

int isNumeric(char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

void handle_ctrl_c(int sig) {
    if (fg_pid > 0) {
        kill(fg_pid, SIGKILL);
        printf("\nProcess %d terminated.\n", fg_pid);
        fg_pid = -1;
    }
}

void handle_ctrl_z(int sig) {
    if (fg_pid > 0) {
        kill(fg_pid, SIGSTOP);
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == fg_pid) {
                processes[i].status = 0;
                printf("\nProcess %d moved to background.\n", fg_pid);
                fg_pid = -1;
                return;
            }
        }
        addProcess(fg_pid);
        printf("\nProcess %d moved to background.\n", fg_pid);
        fg_pid = -1;
    }
}

void execute_command(char *command, char *args[], int is_background) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(command, args);
        printf("Invalid command: %s\n", command);
        exit(1);
    } else if (pid > 0) {
        if (!is_background) {
            fg_pid = pid;
            waitpid(pid, NULL, 0);
            fg_pid = -1;
        } else {
            addProcess(pid);
        }
    } else {
        perror("Fork failed");
    }
}

int main() {
    signal(SIGINT, handle_ctrl_c);
    signal(SIGTSTP, handle_ctrl_z);
    introduction();
    while (1) {
        printf("ShellHehe> ");
        char s[1000];
        fgets(s, 1000, stdin);
        s[strcspn(s, "\n")] = 0;

        char *args[100];
        int i = 0;
        char s_copy[1000];
        strcpy(s_copy, s);
        char *token = strtok(s_copy, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        int is_background = 0;
        if (i > 0 && strcmp(args[i-1], "&") == 0) {
            is_background = 1;
            args[i-1] = NULL;
            i--;
        }

        if (i == 0) continue;
        if (strcmp(args[0], "exit") == 0) break;
        else if (strcmp(args[0], "help") == 0) help();
        else if (strcmp(args[0], "dir") == 0) dir();
        else if (strcmp(args[0], "date") == 0) date();
        else if (strcmp(args[0], "time") == 0) time_();
        else if (strcmp(args[0], "list") == 0) list();
        else if (strcmp(args[0], "clear") == 0) system("clear");
        else if (strcmp(args[0], "kill") == 0 && i >= 2) {
            int type = isNumeric(args[1]);
            kil(type, args[1]);
        }
        else if (strcmp(args[0], "stop") == 0 && i >= 2) {
            int type = isNumeric(args[1]);
            stop(type, args[1]);
        }
        else if (strcmp(args[0], "resume") == 0 && i >= 2) {
            int type = isNumeric(args[1]);
            resume(type, args[1]);
        }
        else if (strcmp(args[0], "fg") == 0 && i >= 2) {
            int type = isNumeric(args[1]);
            fg(type, args[1]);
        }
        else if (strcmp(args[0], "prog") == 0 && i >= 2) {
            execute_command(args[1], &args[1], is_background);
        }
        else {
            execute_command(args[0], args, is_background);
        }
    }
    return 0;
}
