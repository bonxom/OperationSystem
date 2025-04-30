# OperationSystem
PracticeOS

## TinyShell
A simple shell implementation for Unix as a project for the Operating Systems course at HUST.

### Team Members
- Nguyen Quang Duc
- Le Duc Chinh
- Ho Minh Dung
- Nguyen Ba Duc Anh

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
```text
TinyShell/
│
├── main.c         // Main function and command processing loop
├── shell.h        // Shell interface declarations
├── shell.c        // Shell interface implementations
├── process.h      // Process struct and management declarations
├── process.c      // Process management implementations
├── utils.h        // Utility functions declarations
├── utils.c        // Utility functions implementations
├── commands.h     // Command mapping table declarations
├── commands.c     // Command handling implementations
└── Makefile       // Build automation


### Features
- **Basic Commands**: `help`, `dir`, `date`, `time`, `clear`, `exit`
- **Process Management**: `child`, `list`, `kill <PID|name>`, `stop <PID|name>`, `resume <PID|name>`, `fg <PID|name>`
- **Program Execution**: `calc`, `timer <sec>`, `prog <cmd>`, `execfile <path>` (supports background execution with `&`)
- **Signal Handling**: Supports `Ctrl+C` (terminate foreground process), `Ctrl+Z` (stop foreground process and move to background)
- **Command History**: Uses `readline` library for command input with history support

### Build and Run
1. **Compile the program**:
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
- Ensure the readline library is installed on your system to support command history and input.
On Ubuntu, install it with:
bash
sudo apt-get install libreadline-dev

- The execfile command requires the target file to be executable. For example, to create a simple program sum_ab:
1. Create sum_ab.c:
c

#include <stdio.h>
int main() {
    int a, b;
    printf("Enter two numbers (a b): ");
    scanf("%d %d", &a, &b);
    printf("Sum of %d and %d is: %d\n", a, b, a + b);
    return 0;
}
2. Compile and make it executable:
bash

gcc -o sum_ab sum_ab.c
chmod +x sum_ab
Run in the shell:
bash

execfile ./sum_ab

### Known Issues
Some commands (e.g., calc) use sleep for testing purposes by default due to the lack of a GUI in some environments. Follow the instructions in the "Notes" section to enable gnome-calculator on systems with GUI support.