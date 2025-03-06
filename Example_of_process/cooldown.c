#include <stdio.h>
#include <windows.h>

void countdown(int seconds) {
    for (int i = seconds; i >= 0; i--) {
        system("cls"); // Clear screen for UI effect
        printf("***********************************\n");
        printf("*        Countdown Timer         *\n");
        printf("***********************************\n");
        printf("*          %2d seconds           *\n", i);
        printf("***********************************\n");
        Sleep(1000); // Pause for 1 second
    }
    system("cls");
    printf("***********************************\n");
    printf("*        Time's up!              *\n");
    printf("***********************************\n");
}

int main() {
    countdown(10);
    return 0;
}
