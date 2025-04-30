#include "process.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

Process processes[100];
int processCount = 0;


void addProcess(int pid, const char *name, int status) {
    processes[processCount].pid = pid;
    strncpy(processes[processCount].name, name, sizeof(processes[processCount].name) - 1);
    processes[processCount].status = status;
    processCount++;
}

void createChild(){
    pid_t pid = fork();
    addProcess(pid, "child", 0);
    if (pid == 0) {
        printf("Child process is running\n");
    
        return;
    }
}

void parseAndHandleProcessCommand(char *input){
    // kill, stop, resume
    // input should split by space, example kill 1234, so we need to split kill and 1234
    // then check if 1234 is a number or not, if it is a number, then it is a PID, otherwise, it is a name
    char **str = malloc(4 * sizeof(char *));
    int i = 0;
    str[i] = strtok(input, " ");
    while (str[i] != NULL) {
        i++;
        if (i >= 4) break; // Prevent buffer overflow
        str[i] = strtok(NULL, " ");
    }

    if (i != 2) {
        printf("Invalid command, try again!\n");
        return;
    }
    if (str[0] != NULL && str[1] != NULL) {
        int type = isNumeric(str[1]);
        if (strcmp(str[0], "kill") == 0) kil(type, str[1]);
        else if (strcmp(str[0], "resume") == 0) resume(type, str[1]);
        else if (strcmp(str[0], "stop") == 0) stop(type, str[1]);
        else printf("Invalid command, try again!\n");
    }
    free(str); // Free the allocated memory
    str = NULL; // Set to NULL to avoid dangling pointer
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
