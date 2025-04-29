#include "utils.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int isNumeric(char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

// Các hàm dir, date, time_, openCalculator_fg, openCalculator_bg như bạn đã viết

void dir(){
    struct dirent *dir;
    DIR *d = opendir("."); // open current directory
    
    if (d == NULL) {
        printf("Fail to open current folder"); // open current directory failed
        return;
    }
    
    while ((dir = readdir(d)) != NULL) {
        printf("%s\n", dir->d_name); // print all files in current directory
    }
    
    closedir(d); // close folder
}

void date(){
    system("date");
}

void time_() {
    setenv("TZ", "Asia/Ho_Chi_Minh", 1); // Set Vietnam timezone
    tzset(); // Apply the new timezone

    time_t t = time(NULL);
    if (t == (time_t)-1) {
        perror("Failed to get the current time");
        return;
    }

    struct tm tm_info;
    if (localtime_r(&t, &tm_info) == NULL) {
        perror("Failed to convert time to local time");
        return;
    }

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_info);
    printf("Current time: %s\n", buffer);
}

void openCalculator_fg() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        freopen("/dev/null", "w", stderr); // avoid waning message in the console
        execlp("gnome-calculator", "gnome-calculator", NULL); //if success skip the next 2 lines
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        printf("Calculator opened in foreground with PID %d\n", pid);
        signal(SIGINT, SIG_IGN); // Ignore Ctrl+C in the parent process
        waitpid(pid, NULL, 0); // Wait for the child process to finish
        signal(SIGINT, SIG_DFL); // Restore default signal handler
    } else {
        perror("Fork failed");
    }
    
    //similar to the background process function, this can be done with system command
    //system("gnome-calculator");
}
void openCalculator_bg() {
    //in Linux, we can use fork and execlp to run a program in the background
    //fork is used to create a new process, the new process will be a child of the current process
    //execlp is used to execute a program, it will replace the current process (fork create a child as a copy of TinyShell) with the new process (calculator)
    //the child process (calculator) will run the program, and the parent process (TinyShell) will continue to run
    pid_t pid = fork();
    //here both Shell and Calculator are running at the same time
    //so both (pid == 0) and (pid > 0) will excecute (magic :)))
    if (pid == 0) {
        // Child process
        freopen("/dev/null", "w", stderr); // avoid waning message in the console
        //printf("child process: Calculator\n");
        execlp("gnome-calculator", "gnome-calculator", NULL); //if success skip the next 2 lines
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        addProcess(pid, "Calculator", 0);
        printf("Calculator opened in background with PID %d\n", pid);
    } else {
        perror("Fork failed");
    }


    //however this can be easily done by call the system command (But I want to make it complicated)
    //system("gnome-calculator &");
    //addProcess(pid, "Calculator", 0);
    //printf("Calculator opened in background with PID %d\n", pid);
}