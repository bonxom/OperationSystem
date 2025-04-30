#ifndef COMMANDS_H
#define COMMANDS_H

typedef void (*CommandHandler)(char **args, int arg_count, int is_background);
typedef struct {
    const char *name;
    CommandHandler handler;
} CommandEntry;

extern CommandEntry command_table[];

void handleCommand(char *input);

#endif