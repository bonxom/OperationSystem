# OperationSystem
PracticeOS

## TinyShell
A simple shell implementation for Unix as a project for the Operating Systems course at HUST.

### Team Members
- Nguyen Quang Duc
- Le Duc Chinh
- Ho Minh Dung
- Nguyen Ba Duc Anh

### Project Structure
TinyShell/
│
├── main.c          // Contains the main() function and the command processing loop
├── shell.h         // Declares functions for shell interface (introduction, help)
├── shell.c         // Implements shell interface functions (introduction, help)
├── process.h       // Declares the Process struct and process management functions
├── process.c       // Implements process management functions (addProcess, list, kill, stop, resume, fg, signal handlers)
├── utils.h         // Declares utility functions (isNumeric, dir, date, time_, execute_command, etc.)
├── utils.c         // Implements utility functions
├── commands.h      // Declares command mapping table and command handling functions
├── commands.c      // Implements command handling using a mapping table
└── Makefile        // Automates the compilation of all files into an executable


### Features
- **Basic Commands**: `help`, `dir`, `date`, `time`, `clear`, `exit`
- **Process Management**: `child`, `list`, `kill <PID|name>`, `stop <PID|name>`, `resume <PID|name>`, `fg <PID|name>`
- **Program Execution**: `calc`, `timer <sec>`, `prog <cmd>`, `execfile <path>` (supports background execution with `&`)
- **Signal Handling**: Supports `Ctrl+C` (terminate foreground process), `Ctrl+Z` (stop foreground process and move to background)
- **Command History**: Uses `readline` library for command input with history support

### Build and Run
1. **Compile the program**:
   ```bash
   make
2. Run the shell:
    ./tinyshell
3. Clean up (optional): To remove object files and the executable, run:
    make clean


### Example Usage
1. Display help:
    help
2. List directory contents:
    dir
3. Run a timer for 5 seconds:
    timer 5
4. Execute a file (e.g., a program to sum two numbers):
    execfile ./sum_ab
5. Run a command in the background:
    calc &
6. Stop a process:
    stop 1234
7. Bring a background process to foreground:
    fg 1234

### Notes
Ensure the readline library is installed on your system to support command history and input.
    On Ubuntu, install it with:
        sudo apt-get install libreadline-dev
    The execfile command requires the target file to be executable. For example, to create a simple program sum_ab:
    1. Create sum_ab.c: 
#include <stdio.h>
int main() {
    int a, b;
    printf("Enter two numbers (a b): ");
    scanf("%d %d", &a, &b);
    printf("Sum of %d and %d is: %d\n", a, b, a + b);
    return 0;
}
    2. Compile and make it executable:
gcc -o sum_ab sum_ab.c
chmod +x sum_ab
    3. Run in the shell:
execfile ./sum_ab
    The calc command has two implementations in the openCalculator function in utils.c:
        Default (for machines without GUI): Uses sleep 5 to simulate the calculator.
        For machines with GUI (e.g., Dũng's machine): Uses gnome-calculator. To enable this, open utils.c, find the openCalculator function, and:
            1. Comment out the sleep lines:
// char *args[] = {"sleep", "5", NULL};
// execvp("sleep", args);
// printf("Invalid command: sleep\n");
            2. Uncomment the gnome-calculator lines:
char *args[] = {"gnome-calculator", NULL};
execvp("gnome-calculator", args);
printf("Invalid command: gnome-calculator\n");

### Known Issues
Some commands (e.g., calc) use sleep for testing purposes by default due to the lack of a GUI in some environments. Follow the instructions in the "Notes" section to enable gnome-calculator on systems with GUI support.