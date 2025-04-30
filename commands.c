#include "commands.h"
#include "shell.h"
#include "process.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//handler
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
    createChild();
    fg_command_name[0] = '\0';
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

void handle_execfile(char **args, int arg_count, int is_background) {
    if (arg_count >= 2) {
        execute_file(args[1], is_background);
    } else {
        printf("Usage: execfile <path>\n");
    }
}

// command mapping table
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
    {"execfile", handle_execfile}, // Thêm lệnh mới để thực thi file
    {NULL, NULL} // Kết thúc bảng
};

void handleCommand(char *input) {
    char *args[100];
    int i = 0;
    char s_copy[1000];
    strcpy(s_copy, input);
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
        return;
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
        printf("Invalid command, try again!\n");
    }
}