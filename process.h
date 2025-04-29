#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int pid;
    char name[256];
    int status;
} Process;

void addProcess(int pid, const char *name, int status);
void list();
void kil(int type, char *id);
void stop(int type, char *id);
void resume(int type, char *id);
void createChild();
void parseAndHandleProcessCommand(char *input);

#endif
