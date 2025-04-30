#include <stdlib.h>
#include "shell.h"
#include "process.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

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

        if (strcmp(s, "exit") == 0) {
            free(s);
            break;
        }

        handleCommand(s);
        free(s); // Giải phóng bộ nhớ cho readline
    }
    return 0;
}