#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/wait.h> // Thêm để dùng waitpid()
#include <readline/readline.h> // Thêm để dùng readline()
#include <readline/history.h> // Thêm để hỗ trợ lịch sử lệnh

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

    printf("fg <PID>    : Bring a background process to foreground by PID\n");
    printf("fg <name>   : Bring a background process to foreground by name\n");
    printf("calc        : Open calculator (add & to run in background)\n");
    printf("timer <sec> : Run a timer for specified seconds\n");
    printf("prog <cmd>  : Run a program (e.g., prog calc)\n");
    printf("dir         : List the contents of the current directory\n");
    printf("date        : Display the system date\n");
    printf("time        : Display the system time\n");
    printf("exit        : Exit my shell\n");
    printf("help        : Print this help\n");
    printf("<cmd> [&]   : Run any command, add & to run in background\n");
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
    char name[256];
    int status; // 0: running, 1: stopped
} Process;
Process processes[100]; // Array to store processes
int processCount = 0; // Number of processes in the array
pid_t fg_pid = -1; // Thêm biến để theo dõi tiến trình foreground
char fg_command_name[256] = {0}; // Thêm biến để lưu tên lệnh foreground

void addProcess(int pid, const char *name, int status) {
    if (processCount >= 100) { // tránh tràn
        printf("Process list is full, cannot track new processes.\n");
        return;
    }
    processes[processCount].pid = pid;
    strncpy(processes[processCount].name, name, sizeof(processes[processCount].name) - 1);
    processes[processCount].status = status;
    processCount++;
    printf("Process %d (%s) added to background.\n", pid, name); // test
}

void list(){
    if (processCount == 0) {
        printf("No background processes found.\n");
        return;
    }
    printf("Background processes:\n");
    for (int i = 0; i < processCount; i++) {
        printf("PID: %d, Name: %s, Status: %s\n", processes[i].pid, processes[i].name, processes[i].status == 0 ? "Running" : "Stopped");
    }
}

void kil(int type, char *id){ //type 1: PID, type 0: name
    if (type == 1) {
        int pid = atoi(id);
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == pid) {
                if (kill(pid, SIGTERM) == 0) {  // Send termination signal
                    printf("Process %d terminated.\n", pid);
                } else {
                    perror("Failed to kill process");
                    return;
                }
                // Remove the process from the array
                for (int j = i; j < processCount - 1; j++) {
                    processes[j] = processes[j + 1];
                }
                processCount--;
                return;
            }
        }
        printf("Process with PID %d not found.\n", pid);
    } 
    else {
        for (int i = 0; i < processCount; i++) {
            if (strcmp(processes[i].name, id) == 0) {
                if (kill(processes[i].pid, SIGTERM) == 0) {  // Send termination signal
                    printf("Process %s terminated.\n", processes[i].name);
                } else {
                    perror("Failed to kill process");
                    return;
                }
                // Remove the process from the array
                for (int j = i; j < processCount - 1; j++) {
                    processes[j] = processes[j + 1];
                }
                processCount--;
                return;
            }
        }
        printf("Process with name %s not found.\n", id);
    }
}

void stop(int type, char *id){
    int pid = (type == 1) ? atoi(id) : -1;

    for (int i = 0; i < processCount; i++) {
        if ((type == 1 && processes[i].pid == pid) || (type == 0 && strcmp(processes[i].name, id) == 0)) {
            printf("Stopping process %d (%s)\n", processes[i].pid, processes[i].name);
            kill(processes[i].pid, SIGSTOP);
            processes[i].status = 1; 
            return;
        }
    }
    printf("Process not found: %s\n", id);
}

void resume(int type, char *id){
    int pid = (type == 1) ? atoi(id) : -1;

    for (int i = 0; i < processCount; i++) {
        if ((type == 1 && processes[i].pid == pid) || (type == 0 && strcmp(processes[i].name, id) == 0)) {
            printf("Resuming process %d (%s)\n", processes[i].pid, processes[i].name);
            kill(processes[i].pid, SIGCONT);
            processes[i].status = 0; 
            return;
        }
    }
    printf("Process not found: %s\n", id);
}

void fg(int type, char *id) { // Hàm này đưa tiến trình bg lên fg.
    int pid = (type == 1) ? atoi(id) : -1;
    for (int i = 0; i < processCount; i++) {
        if ((type == 1 && processes[i].pid == pid) || (type == 0 && strcmp(processes[i].name, id) == 0)) {
            printf("Bringing process %d (%s) to foreground\n", processes[i].pid, processes[i].name);
            kill(processes[i].pid, SIGCONT);
            processes[i].status = 0;
            fg_pid = processes[i].pid;
            waitpid(fg_pid, NULL, 0);
            fg_pid = -1;
            // Xóa tiến trình khỏi danh sách sau khi hoàn thành
            for (int j = i; j < processCount - 1; j++) {
                processes[j] = processes[j + 1];
            }
            processCount--;
            return;
        }
    }
    printf("Process not found: %s\n", id);
}

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

void execute_command(char *command, char *args[], int is_background) { // chạy lệnh fg hoặc bg thay vì chạy hardcore
    pid_t pid = fork();
    if (pid == 0) {
        execvp(command, args);
        printf("Invalid command: %s\n", command);
        exit(1);
    } else if (pid > 0) {
        if (!is_background) {
            fg_pid = pid; // Lưu PID của tiến trình foreground
            //waitpid(pid, NULL, 0);
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

void openCalculator(int is_background) { // Test thay gnome-calculator bằng sleep 5 giây vì docker không có GUI
    pid_t pid = fork();
    if (pid == 0) {
        freopen("/dev/null", "w", stderr);
        char *args[] = {"sleep", "5", NULL}; // Thay gnome-calculator bằng sleep 5 giây
        execvp("sleep", args);
        printf("Invalid command: sleep\n");
        exit(1);
    } else if (pid > 0) {
        if (!is_background) {
            fg_pid = pid;
            waitpid(pid, NULL, 0);
            fg_pid = -1;
        } else {
            addProcess(pid, "sleep", 0);
        }
    } else {
        perror("Fork failed");
    }
}

void timer(int seconds) { // test hàm chạy đồng hồ hẹn giờ
    pid_t pid = fork();
    if (pid == 0) {
        printf("Timer started for %d seconds\n", seconds);
        sleep(seconds);
        printf("Timer finished\n");
        exit(0);
    } else if (pid > 0) {
        fg_pid = pid;
        waitpid(pid, NULL, 0);
        fg_pid = -1;
    } else {
        perror("Fork failed");
    }
}

int isNumeric(char *str) {
    if (str == NULL || *str == '\0') return 0; // Check for NULL or empty string

    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) return 0; // If any character is not a digit, return false
    }
    return 1; // All characters are digits
}

void handle_sigint(int sig) { // (Ctrl+C): Hủy tiến trình foreground mà không thoát shell.
    if (fg_pid > 0) {
        kill(fg_pid, SIGKILL);
        printf("\nProcess %d terminated\n", fg_pid);
        fg_pid = -1;
    }
}

void handle_sigtstp(int sig) { // (Ctrl+Z): Tạm dừng tiến trình foreground và chuyển vào background
    if (fg_pid > 0) {
        kill(fg_pid, SIGSTOP);
        addProcess(fg_pid, fg_command_name[0] ? fg_command_name : "unknown", 1); // Dùng fg_command_name
        printf("\nProcess %d stopped and moved to background\n", fg_pid);
        fg_pid = -1;
        rl_reset_line_state(); // Đặt lại trạng thái dòng lệnh của readline
        rl_on_new_line();      // Báo cho readline rằng đã xuống dòng mới
        //rl_redisplay();        // Yêu cầu readline in lại prompt
    }
}

void handle_sigchld(int sig) { // Dọn dẹp tiến trình background khi kết thúc.
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == pid) {
                printf("\nProcess %d (%s) exited\n", pid, processes[i].name);
                for (int j = i; j < processCount - 1; j++) {
                    processes[j] = processes[j + 1];
                }
                processCount--;
                break;
            }
        }
        rl_on_new_line(); // Báo cho readline rằng đã xuống dòng mới
        rl_redisplay();   // Yêu cầu readline in lại prompt
    }
}

// Định nghĩa kiểu hàm xử lý lệnh
typedef void (*CommandHandler)(char **args, int arg_count, int is_background);

// Cấu trúc cho bảng ánh xạ lệnh
typedef struct {
    const char *name;
    CommandHandler handler;
} CommandEntry;

// Các hàm xử lý lệnh
void handle_help(char **args, int arg_count, int is_background) {
    help();
}

void handle_dir(char **args, int arg_count, int is_background) {
    dir();
}

void handle_date(char **args, int arg_count, int is_background) {
    date();
}

void handle_time(char **args, int arg_count, int is_background) {
    time_();
}

void handle_calc(char **args, int arg_count, int is_background) {
    strcpy(fg_command_name, "calc");
    openCalculator(is_background);
    fg_command_name[0] = '\0';
}

void handle_timer(char **args, int arg_count, int is_background) {
    if (arg_count >= 2) {
        strcpy(fg_command_name, "timer");
        timer(atoi(args[1]));
        fg_command_name[0] = '\0';
    } else {
        printf("Usage: timer <seconds>\n");
    }
}

void handle_child(char **args, int arg_count, int is_background) {
    strcpy(fg_command_name, "child");
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child process is running\n");
        sleep(5);
        printf("Child process finished\n");
        exit(0);
    } else if (pid > 0) {
        addProcess(pid, "child", 0);
    } else {
        perror("Fork failed");
    }
}

void handle_list(char **args, int arg_count, int is_background) {
    list();
}

void handle_clear(char **args, int arg_count, int is_background) {
    system("clear");
}

void handle_kill(char **args, int arg_count, int is_background) {
    if (arg_count >= 2) {
        int type = isNumeric(args[1]);
        kil(type, args[1]);
    } else {
        printf("Usage: kill <PID|name>\n");
    }
}

void handle_stop(char **args, int arg_count, int is_background) {
    if (arg_count >= 2) {
        int type = isNumeric(args[1]);
        stop(type, args[1]);
    } else {
        printf("Usage: stop <PID|name>\n");
    }
}

void handle_resume(char **args, int arg_count, int is_background) {
    if (arg_count >= 2) {
        int type = isNumeric(args[1]);
        resume(type, args[1]);
    } else {
        printf("Usage: resume <PID|name>\n");
    }
}

void handle_fg(char **args, int arg_count, int is_background) {
    if (arg_count >= 2) {
        int type = isNumeric(args[1]);
        fg(type, args[1]);
    } else {
        printf("Usage: fg <PID|name>\n");
    }
}

void handle_prog(char **args, int arg_count, int is_background) {
    if (arg_count >= 2) {
        execute_command(args[1], &args[1], is_background);
    } else {
        printf("Usage: prog <command> [args...]\n");
    }
}

// Bảng ánh xạ lệnh
CommandEntry command_table[] = {
    {"help", handle_help},
    {"dir", handle_dir},
    {"date", handle_date},
    {"time", handle_time},
    {"calc", handle_calc},
    {"timer", handle_timer},
    {"child", handle_child},
    {"list", handle_list},
    {"clear", handle_clear},
    {"kill", handle_kill},
    {"stop", handle_stop},
    {"resume", handle_resume},
    {"fg", handle_fg},
    {"prog", handle_prog},
    {NULL, NULL} // Kết thúc bảng
};

int main() {
    signal(SIGINT, handle_sigint); // Bắt Ctrl+C
    signal(SIGTSTP, handle_sigtstp); // Bắt Ctrl+Z
    signal(SIGCHLD, handle_sigchld); // Xử lý tiến trình con kết thúc
    introduction();

    while (1) {
        char *s = readline("ShellHehe> "); // Thay fgets bằng readline
        if (!s) break; // Thoát nếu gặp EOF (Ctrl+D)
        if (*s) add_history(s); // Lưu lệnh vào lịch sử
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

        if (i == 0) {
            free(s);
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            free(s);
            break;
        }

        // Duyệt bảng ánh xạ để tìm và thực thi lệnh
        int found = 0;
        for (int j = 0; command_table[j].name != NULL; j++) {
            if (strcmp(args[0], command_table[j].name) == 0) {
                command_table[j].handler(args, i, is_background);
                found = 1;
                break;
            }
        }

        // Nếu không tìm thấy trong bảng, chạy như lệnh hệ thống
        if (!found) {
            strcpy(fg_command_name, args[0]);
            execute_command(args[0], args, is_background);
            fg_command_name[0] = '\0';
        }

        free(s); // Giải phóng bộ nhớ cho readline
    }
    return 0;
}