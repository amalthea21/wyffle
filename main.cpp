#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

char getch() {
    char buf = 0;
    struct termios old = {0};

    if (tcgetattr(STDIN_FILENO, &old) < 0) {
        perror("tcgetattr()");
        return -1;
    }

    struct termios raw = old;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
        perror("tcsetattr");
        return -1;
    }

    if (read(STDIN_FILENO, &buf, 1) < 0) {
        perror("read()");
    }

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &old) < 0) {
        perror("tcsetattr restore");
    }

    return buf;
}

struct termios initializeTerminal() {
    struct termios term = {0};

    term.c_lflag &= ~ICANON;   // Disable canonical mode (line buffering)
    term.c_lflag &= ~ECHO;     // Disable echo
    term.c_cc[VMIN] = 1;       // Minimum chars
    term.c_cc[VTIME] = 0;      // Timeout for reading

    if (tcsetattr(0, TCSANOW, &term) < 0)
        perror("tcsetattr ICANON");

    return term;
}

int main(int argc, char* argv[]) {
    printf("Press any key (q to quit): ");
    fflush(stdout);

    return 0;
}