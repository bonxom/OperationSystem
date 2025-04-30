#ifndef UTILS_H
#define UTILS_H

int isNumeric(char *str);
void dir();
void date();
void time_();
void openCalculator(int is_background);
void timer(int seconds);
void execute_command(char *command, char *args[], int is_background);
void execute_file(char *path, int is_background);

#endif