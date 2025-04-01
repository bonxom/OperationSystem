#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

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
    printf("test");
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

//use an array of struct to store the information of the process
typedef struct {
    int pid;
    char name[256];
    int status; // 0: running, 1: stopped
} Process;
Process processes[100]; // Array to store processes
int processCount = 0; // Number of processes in the array

void addProcess(int pid, const char *name, int status) {
    processes[processCount].pid = pid;
    strncpy(processes[processCount].name, name, sizeof(processes[processCount].name) - 1);
    processes[processCount].status = status;
    processCount++;
}

void list(){
    if (processCount == 0) {
        printf("No background processes found.\n");
        return;
    }
    printf("Background processes:\n");
    for (int i = 0; i < processCount; i++) {
        if (kill(processes[i].pid, 0) == -1) {
            printf("PID: %d, Name: %s, Status: Terminated\n", processes[i].pid, processes[i].name);
        } else {
            printf("PID: %d, Name: %s, Status: %s\n", 
                processes[i].pid, 
                processes[i].name, 
                processes[i].status == 0 ? "Running" : "Stopped");
        }
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
    if (type == 1) { 
        int pid = atoi(id);
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == pid) {
                if (kill(pid, SIGSTOP) == 0) {
                    processes[i].status = 1;  
                    printf("Process %d stopped.\n", pid);
                } else {
                    perror("Failed to stop process");
                }
                return;
            }
        }
        printf("Process with PID %d not found.\n", pid);
    } else { 
        for (int i = 0; i < processCount; i++) {
            if (strcmp(processes[i].name, id) == 0) {
                if (kill(processes[i].pid, SIGSTOP) == 0) {
                    processes[i].status = 1;  
                    printf("Process %s stopped.\n", processes[i].name);
                } else {
                    perror("Failed to stop process");
                }
                return;
            }
        }
        printf("Process with name %s not found.\n", id);
    }
}

void resume(int type, char *id){
    if (type == 1) { 
        int pid = atoi(id);
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == pid) {
                if (kill(pid, SIGCONT) == 0) {
                    processes[i].status = 0;  
                    printf("Process %d resumed.\n", pid);
                } else {
                    perror("Failed to resume process");
                }
                return;
            }
        }
        printf("Process with PID %d not found.\n", pid);
    } else { 
        for (int i = 0; i < processCount; i++) {
            if (strcmp(processes[i].name, id) == 0) {
                if (kill(processes[i].pid, SIGCONT) == 0) {
                    processes[i].status = 0;  
                    printf("Process %s resumed.\n", processes[i].name);
                } else {
                    perror("Failed to resume process");
                }
                return;
            }
        }
        printf("Process with name %s not found.\n", id);
    }
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
        else if (strcmp(s, "child") == 0) { //test
            pid_t pid = fork();
            addProcess(pid, "child", 0);
            if (pid == 0) {
                printf("Child process is running\n");
            
                continue;
            }
        }
        else if (strcmp(s, "list") == 0) {
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
