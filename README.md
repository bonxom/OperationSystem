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
text
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
- **Program Execution**: `calc`, `timer <sec>`, `prog <cmd>`, `execfile <path>`, `shfile <path>` (supports background execution with `&`)
- **Directory Management**: `cd <dir>` (change directory), dynamic prompt showing current directory (e.g., `ShellHehe//home/user>` with `ShellHehe` in green and directory path in blue)
- **Environment Management**: `setenv <var> <value>`, `appendenv <var> <value>`, `printenv` (set, append, or print environment variables)
- **Signal Handling**: Supports `Ctrl+C` (terminate foreground process), `Ctrl+Z` (stop foreground process and move to background)
- **Command History**: Uses `readline` library for command input with history support

### Build and Run
1. **Compile the program**:
   ```bash
   make
2. Run the shell:
    ```bash
    ./tinyshell
3. Clean up (optional): To remove object files and the executable, run:
    ```bash
    make clean

### Example Usage
1. Display help:
    ```bash
    help
2. List directory contents:
    ```bash
    dir
3. Run a timer for 5 seconds:
    ```bash
    timer 5
4. Execute a file (e.g., a program to sum two numbers):
    ```bash
    execfile ./sum_ab

5. Run a command in the background:
    ```bash
    calc &
6. Stop a process:
    ```bash
    stop 1234
7. Bring a background process to foreground:
    ```bash
    fg 1234
8. Run a shell script:
    ```bash
    shfile script.sh
9. Change directory:
    ```bash
    cd /tmp
10. Set an environment variable:
    ```bash
    setenv MY_VAR hello
11. Append to an environment variable:
    ```bash
    appendenv PATH /usr/local/bin
12. Print all environment variables:
    ```bash
    printenv
### Notes
- Ensure the readline library is installed on your system to support command history and input.
On Ubuntu, install it with:
    ```bash
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