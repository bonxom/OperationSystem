#include "process.h"
#include "utils.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

Process processes[100]; // Array to store processes
int processCount = 0; // Number of processes in the array
pid_t fg_pid = -1; // tracking foreground process
char fg_command_name[256] = {0}; // foreground process name

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

void createChild(int is_background){
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child process is running\n");
        sleep(10000);
        printf("Child process finished\n");
        exit(0);
    } else if (pid > 0) {
        if (is_background) {
            printf("Started child process in background (PID %d)\n", pid);
            addProcess(pid, "child", 0);  // Ghi nhận tiến trình chạy nền
        } else {
            printf("Started child process in foreground (PID %d)\n", pid);
            fg_pid = pid;
            strcpy(fg_command_name, "child");
            int status = 0;
            waitpid(pid, &status, WUNTRACED);
            fg_pid = -1;
        }
    } else {
        perror("Fork failed");
    }
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

void fg(int type, char *id) { // move background process to foreground
    int pid = (type == 1) ? atoi(id) : -1;
    for (int i = 0; i < processCount; i++) {
        if ((type == 1 && processes[i].pid == pid) || (type == 0 && strcmp(processes[i].name, id) == 0)) {
            printf("Bringing process %d (%s) to foreground\n", processes[i].pid, processes[i].name);
            kill(processes[i].pid, SIGCONT);
            processes[i].status = 0;
            fg_pid = processes[i].pid;
            int status;
            waitpid(pid, &status, WUNTRACED);
         //   if (waitpid(fg_pid, NULL, 0) == -1) {
          //      perror("waitpid failed");
          //  }
          //  fg_pid = -1;
            // delete from bg processes array
         //   for (int j = i; j < processCount - 1; j++) {
         //       processes[j] = processes[j + 1];
         //   }
         //   processCount--;
            if (WIFSTOPPED(status)) {
                //    printf("Process %d stopped again\n", pid);
                    processes[i].status = 1;
                } else {
                    for (int j = i; j < processCount - 1; j++) {
                        processes[j] = processes[j + 1];
                    }
                    processCount--;
                }
                fg_pid = -1;
            return;
        }
    }
    printf("Process not found: %s\n", id);
}

void shfile(char *path){
    //this function is used to execute a shell script file
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        handleCommand(line);
    }
    printf("File %s executed\n", path);
    fclose(file);
}

//set environment variable, example set PATH to /usr/local/bin
//this setenv function only set env_var during the running time of this TinyShell
void set_env(char *var, char *value) {
    char *old_path = getenv(var);
    if (setenv(var, value, 1) != 0) {
        perror("Failed to set environment variable");
    } else {
        printf("Old value of %s: %s\n", var, old_path);
        printf("Environment variable %s set to %s\n", var, value);
    }
}

//append value to environment variable, example append PATH with /usr/local/bin
void append_env(char *var, char *value) {
    char *old_value = getenv(var);
    if (old_value == NULL) {
        printf("Environment variable %s does not exist. Creating it.\n", var);
        setenv(var, value, 1);
    } else {
        char new_value[1024];
        snprintf(new_value, sizeof(new_value), "%s:%s", old_value, value);
        if (setenv(var, new_value, 1) != 0) {
            perror("Failed to append to environment variable");
        } else {
            printf("Appended %s to %s. \nNew value: %s\n", value, var, new_value);
        }
    }
}

void change_dir(char *dir){
    if (dir == NULL){//move to Home
        dir = getenv("HOME");
    }

    if (chdir(dir) != 0){
        perror("Change directory failed");
    }
}

//print all environment variables
void print_env() {
    extern char **environ;
    for (char **env = environ; *env != 0; env++) {
        char *thisEnv = *env;
        printf("%s\n", thisEnv);
    }
}
//print PATH
void print_path() {
    char *path = getenv("PATH");
    if (path != NULL) {
        printf("PATH=%s\n", path);
    } else {
        printf("PATH variable not found.\n");
    }
}
void handle_sigint(int sig) { // (Ctrl+C): kill foreground process
    if (fg_pid > 0) {
        kill(fg_pid, SIGKILL);
        printf("\nProcess %d terminated\n", fg_pid);
        fg_pid = -1;
    }
}

void handle_sigtstp(int sig) { // (Ctrl+Z): move foreground process to background
    if (fg_pid > 0) {
        kill(fg_pid, SIGSTOP);
        addProcess(fg_pid, fg_command_name[0] ? fg_command_name : "unknown", 1); 
        printf("\nProcess %d stopped and moved to background\n", fg_pid);
        fg_pid = -1;
        rl_reset_line_state(); // reset readline's state
        rl_on_new_line();      // notice to readline that start a new line
    }
}

void handle_sigchld(int sig) { // clear background process
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < processCount; i++) {
            if (processes[i].pid == pid) {
                // printf("\nProcess %d (%s) exited\n", pid, processes[i].name);
                // for (int j = i; j < processCount - 1; j++) {
                //     processes[j] = processes[j + 1];
                // }
                // processCount--;
                // break;
            

                //////
                //Clear current input line
                rl_save_prompt();
                rl_replace_line("", 0);
                rl_redisplay();

                // Print message
                printf("\nProcess %d (%s) exited\n", pid, processes[i].name);

                // Restore prompt
                rl_restore_prompt();
                rl_redisplay();

                // Remove process from list
                for (int j = i; j < processCount - 1; j++) {
                    processes[j] = processes[j + 1];
                }
                processCount--;
                break;
            }
        }
        // rl_on_new_line(); // notice to readline that start a new line
        // rl_redisplay();   // reset readline's state
    }
}