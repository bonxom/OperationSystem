#include "shell.h"
#include "process.h"
#include "utils.h"
#include "display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


void handleCommand(char *input) {
    if (strcmp(input, "help") == 0) help();
    else if (strcmp(input, "dir") == 0) dir();
    else if (strcmp(input, "date") == 0) date();
    else if (strcmp(input, "time") == 0) time_();
    else if (strcmp(input, "calc") == 0) openCalculator_fg();
    else if (strcmp(input, "calc &") == 0) openCalculator_bg();
    else if (strcmp(input, "child") == 0) createChild();
    else if (strcmp(input, "list") == 0) list();
    else if (strcmp(input, "clear") == 0) system("clear");
    else {
        parseAndHandleProcessCommand(input);
    }
}

