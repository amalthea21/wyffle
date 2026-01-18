#include <iostream>
#include <termios.h>
#include <unistd.h>

char getch(struct termios term) {
    char buf = 0;

    if (tcgetattr(0, &term) < 0)
        perror("tcgetattr()");

    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &term) < 0)
        perror("tcsetattr ICANON");

    if (read(0, &buf, 1) < 0)
        perror("read()");

    return buf;
}

struct termios initializeTerminal() {
    struct termios term = {0};

    if (tcgetattr(0, &term) < 0)
        perror("tcgetattr()");

    return term;
}

void restoreTerminal(struct termios term) {
    term.c_lflag |= ICANON;
    term.c_lflag |= ECHO;

    if (tcsetattr(0, TCSADRAIN, &term) < 0)
        perror("tcsetattr restore");
}

int main(int argc, char* argv[]) {
    struct termios term = initializeTerminal();



    restoreTerminal(term);
    return 0;
}