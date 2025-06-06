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
    if (str == NULL || *str == '\0') return 0; // Check for NULL or empty string
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0; // If any character is not a digit, return false
    }
    return 1; // All characters are digits
}

void dir(){
    struct dirent *dir;
    DIR *d = opendir("."); // open current directory
    
    if (d == NULL) {
        printf("Fail to open current folder"); // open current directory failed
        return;
    }
    
    int cnt = 0;
    while ((dir = readdir(d)) != NULL) {
        if (cnt % 2 == 0){
            printf("%-50s", dir->d_name); // print all files in current directory
        }
        else printf("%s\n", dir->d_name);
        cnt++;
    }
    printf("\n");
    
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

void openCalculator(int is_background) {
    pid_t pid = fork();
    if (pid == 0) {
        freopen("/dev/null", "w", stderr);

        // // Dùng để test trên máy không có GUI (mặc định)
        // char *args[] = {"sleep", "5", NULL}; // Thay gnome-calculator bằng sleep 5 giây
        // execvp("sleep", args);
        // printf("Invalid command: sleep\n");

        // Phần code này dành cho máy có GUI (như máy của Dũng)
        // Bỏ comment các dòng dưới đây khi test trên máy có cài gnome-calculator
        char *args[] = {"gnome-calculator", NULL};
        execvp("gnome-calculator", args);
        printf("Invalid command: gnome-calculator\n");

        exit(1);
    } else if (pid > 0) {
        if (!is_background) {
            fg_pid = pid;
            strcpy(fg_command_name, "Calculator");
            int status = 0;
            waitpid(pid, &status, WUNTRACED);
            fg_pid = -1;
        } else {
            addProcess(pid, "Calculator", 0);
            printf("Calculator opened in background with PID %d\n", pid);
            }
    } else {
        perror("Fork failed");
    }
} 

// void openCalculator(int is_background) { // Code cua Dung
//     pid_t pid = fork();
//     if (pid == 0) {
//         // Child process
//         freopen("/dev/null", "w", stderr); // avoid waning message in the console
//         execlp("gnome-calculator", "gnome-calculator", NULL); //if success skip the next 2 lines
//         perror("execlp failed");
//         exit(EXIT_FAILURE);
//     } else if (pid > 0) {
//         if (!is_background) {
//             // Parent process
//             printf("Calculator opened in foreground with PID %d\n", pid);
//             signal(SIGINT, SIG_IGN); // Ignore Ctrl+C in the parent process
//             waitpid(pid, NULL, 0); // Wait for the child process to finish
//             signal(SIGINT, SIG_DFL); // Restore default signal handler
//         } else {
//             addProcess(pid, "Calculator", 0);
//             printf("Calculator opened in background with PID %d\n", pid);        }
//     } else {
//         perror("Fork failed");
//     }

//     //basically fg process can be call: system("gnome-calculator");
//     //either bg process:                system("gnome-calculator &");
// }

//clock cooldown
void countdown(int seconds) {
    for (int i = seconds; i >= 0; i--) {
        system("clear"); // Linux terminal clear
        printf("***********************************\n");
        printf("*        Countdown Timer         *\n");
        printf("***********************************\n");
        printf("*          %2d seconds           *\n", i);
        printf("***********************************\n");
        sleep(1); // 1 second pause
    }

    system("clear");
    printf("***********************************\n");
    printf("*           Time's up!           *\n");
    printf("***********************************\n");
}
void timer(int seconds) { 
    pid_t pid = fork();
    if (pid == 0) {
        printf("Timer started for %d seconds\n", seconds);
        sleep(seconds);
        countdown(seconds);
        printf("Timer finished\n");
        exit(0);
    } else if (pid > 0) {
        fg_pid = pid;
        int status = 0;
        waitpid(pid, &status, WUNTRACED);
        fg_pid = -1;
    } else {
        perror("Fork failed");
    }
}


void execute_command(char *command, char *args[], int is_background) { // chạy lệnh fg hoặc bg thay vì chạy hardcore
    pid_t pid = fork();
    if (pid == 0) {
        execvp(command, args);
        exit(1);
    } else if (pid > 0) {
        if (!is_background) {
            fg_pid = pid; // Lưu PID của tiến trình foreground
            int status = 0;
            waitpid(pid, &status, WUNTRACED);
            fg_pid = -1;
        } else {
            addProcess(pid, command, 0); // Thêm vào danh sách background
        }
    } else {
        perror("Fork failed");
    }
}

void execute_file(char *path, int is_background) { // Thêm hàm thực thi file
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {path, NULL}; // Gọi file thực thi với đường dẫn
        execvp(path, args);
        printf("Failed to execute file: %s\n", path);
        exit(1);
    } else if (pid > 0) {
        if (!is_background) {
            fg_pid = pid;
            int status = 0;
            waitpid(pid, &status, WUNTRACED);
            fg_pid = -1;
        } else {
            addProcess(pid, path, 0);
        }
    } else {
        perror("Fork failed");
    }
}