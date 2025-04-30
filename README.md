# OperationSystem
PracticeOS

TinyShell/
│
├── main.c          // Chứa hàm main() và loop xử lý lệnh
├── shell.h         // Chứa khai báo các hàm
├── shell.c         // Chứa phần hiện thực (implementation) các hàm lệnh (help, dir, date, time, kill, stop, resume,...)
├── process.h       // Khai báo struct Process và các hàm quản lý process
├── process.c       // Cài đặt các hàm quản lý process (addProcess, list, kill, stop, resume)
├── utils.h         // Các hàm tiện ích nhỏ (isNumeric, clearScreen, etc.)
├── utils.c         // Cài đặt các hàm tiện ích
└── Makefile        // Để biên dịch tất cả các file thành 1 file executable


make

./tinyShell
