#include <stdlib.h>
#include "shell.h"
#include "process.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h> // Thêm để sử dụng getcwd

int main() {
    signal(SIGINT, handle_sigint); // Bắt Ctrl+C
    signal(SIGTSTP, handle_sigtstp); // Bắt Ctrl+Z
    signal(SIGCHLD, handle_sigchld); // Xử lý tiến trình con kết thúc
    introduction();

    char cwd[1024]; // Buffer để lưu đường dẫn thư mục hiện tại
    while (1) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            strcpy(cwd, "unknown"); // Nếu không lấy được thư mục, dùng "unknown"
        }
        char prompt[1059]; // Buffer đủ lớn để chứa prompt
        snprintf(prompt, sizeof(prompt), "\033[1;32mShellHehe:\033[0m\033[1;34m%s\033[0m> ", cwd);
        char *s = readline(prompt); // Sử dụng prompt động với màu sắc
        if (!s) break; // Thoát nếu gặp EOF (Ctrl+D)
        if (*s) add_history(s); // Lưu lệnh vào lịch sử
        s[strcspn(s, "\n")] = 0;

        if (strcmp(s, "exit") == 0) {
            free(s);
            break;
        }

        handleCommand(s);
        free(s); // Giải phóng bộ nhớ
    }
    return 0;
}