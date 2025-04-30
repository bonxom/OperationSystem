#include "shell.h"
#include "display.h"
#include <stdio.h>
#include <string.h>

int main() {
    introduction();

    while (1) {
        printf("ShellHehe> ");
        char s[1000];
        fgets(s, 1000, stdin);
        s[strcspn(s, "\n")] = 0;

        if (strcmp(s, "exit") == 0) break;
        else handleCommand(s);
    }
    printf("Exiting TinyShell. Goodbye!\n");
    return 0;
}
