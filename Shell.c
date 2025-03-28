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
    int status; // 0: running, 1: stopped
} Process;
#define MAX_PROCESSES 100 
Process processes[MAX_PROCESSES]; // Array to store processes
int processCount = 0; // Number of processes in the array

void addProcess(pid_t pid) {
    if (processCount < MAX_PROCESSES) {
        processes[processCount].pid = pid;
        processes[processCount].status = 1; // Đánh dấu là đang chạy
        processCount++;
        printf("Process %d added to the list.\n", pid);
    } else {
        printf("Process list is full, cannot track new processes.\n");
    }
}

void list(){
    printf("PID\tSTATUS\n");
    for (int i = 0; i < processCount; i++) {
        printf("%d\t%s\n", processes[i].pid, processes[i].status ? "Running" : "Stopped");
    }
}

void kil(int type, char *id){ //type 1: PID, type 0: name
    if (type == 1) { // Kill by PID
        pid_t pid = atoi(id);
        if (kill(pid, SIGKILL) == 0) {
            printf("Process %d killed successfully.\n", pid);
        } else {
            perror("Failed to kill process");
        }
    } else { // Kill by name
        char command[256];
        snprintf(command, sizeof(command), "pkill %s", id);
        int status = system(command);
        if (status == 0) {
            printf("Process %s killed successfully.\n", id);
        } else {
            printf("Failed to kill process %s.\n", id);
        }
    }
}

void stop(int type, char *id){
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

void resume(int type, char *id){
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

void dir(){
    struct dirent *dir;
    DIR *d = opendir("."); // Mở thư mục hiện tại (.)

    if (d == NULL) {
        perror("opendir"); // Hiển thị lỗi nếu không mở được
        return;
    }

    while ((dir = readdir(d)) != NULL) {
        printf("%s\n", dir->d_name); // In ra tên file/thư mục
    }

    closedir(d); // Đóng thư mục
}

void time_(){
    setenv("TZ", "Asia/Ho_Chi_Minh", 1); // Đặt múi giờ Việt Nam
    tzset();  // Cập nhật múi giờ mới
    
    time_t t;
    struct tm *tm_info;
    char buffer[20];

    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Current time: %s\n", buffer);
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
                while (1);
            } else if (pid > 0) {
                addProcess(pid);
            } else {
                perror("Fork failed");
            }
        }
        else if (strcmp(s, "list") == 0) {
            printf("List of background processes:\n");
            list();
        }
        else if (strcmp(s, "clear") == 0) system("clear");
        else{// Xử lý các lệnh có tham số (kill, stop, resume)
            char s_copy[1000];
            strcpy(s_copy, s); // Sao chép chuỗi gốc để không làm thay đổi s
            char *token = strtok(s_copy, " ");
            char *args[3];
            int i = 0;
            while (token != NULL && i < 3) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            if (i == 2) {
                int type = isNumeric(args[1]); // Kiểm tra số
                if (strcmp(args[0], "kill") == 0) kil(type, args[1]);
                else if (strcmp(args[0], "resume") == 0) resume(type, args[1]);
                else if (strcmp(args[0], "stop") == 0) stop(type, args[1]);
                else printf("Invalid command, try again!\n");
            } else {
                printf("Invalid command, try again!\n");
            }

    }
    return 0;
}
