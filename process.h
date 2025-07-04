#include <sys/types.h>
#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int pid;
    char name[256];
    int status; // 0: running, 1: stopped
} Process;

extern Process processes[100]; // Array to store processes
extern int processCount; // Number of processes in the array
extern pid_t fg_pid; // tracking foreground process
extern char fg_command_name[256]; // foreground process name

void addProcess(int pid, const char *name, int status);
void createChild(int is_background);
void list();
void kil(int type, char *id);
void stop(int type, char *id);
void resume(int type, char *id);
void fg(int type, char *id);
void handle_sigint(int sig);
void handle_sigtstp(int sig);
void handle_sigchld(int sig);
void shfile(char *path);
void change_dir(char *dir);
void set_env(char *var, char *value);
void append_env(char *var, char *value);
void print_env();
void print_path();
#endif