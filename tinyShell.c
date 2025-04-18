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
        //list of program
        else if (strcmp(s, "calc") == 0) openCalculator_fg();
        else if (strcmp(s, "calc &") == 0) openCalculator_bg();
        //
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