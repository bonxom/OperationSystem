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


### Notes
Ensure the readline library is installed on your system to support command history and input.
On Ubuntu, install it with:
    sudo apt-get install libreadline-dev