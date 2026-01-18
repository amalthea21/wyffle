#include <chrono>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string>
#include <thread>
#include <vector>

#include "include/Formatting.h"

using namespace std;

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

    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) < 0) {
        perror("tcsetattr");
        return -1;
    }

    if (read(STDIN_FILENO, &buf, 1) < 0) {
        perror("read()");
    }

    if (tcsetattr(STDIN_FILENO, TCSANOW, &old) < 0) {
        perror("tcsetattr restore");
    }

    return buf;
}

string readKey() {
    char ch = getch();

    if (ch == 27) {
        string seq;
        seq += ch;

        char next;
        while (read(STDIN_FILENO, &next, 1) > 0) {
            seq += next;
            if (escapeSequences.find(seq) != escapeSequences.end()) {
                return escapeSequences.at(seq);
            }
            if (seq.length() > 6) break;
        }
        return "Escape";
    }

    if (singleByteKeys.find(ch) != singleByteKeys.end()) {
        return singleByteKeys.at(ch);
    }

    return string(1, ch);
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

string getBorder() {
    string border;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col;

    for(int i = 0; i < width; i++) {
        border += "━";
    }

    return border;
}

void printMenu() {
    cout << ansi::HOME;
    cout << ansi::BG_BRIGHT_RED << "QUIT: [F1]" << ansi::RESET;
    cout << endl;
    cout << getBorder() << endl;
    cout.flush();
}

void printText(vector<string> text, int pos) {
    for (int i = 0; i < text.size(); i++) {
        if (i == pos) {
            cout << ansi::BG_WHITE << ansi::BLACK << text.at(i) << ansi::RESET;
        } else cout << text.at(i);
    }
}

int main(int argc, char* argv[]) {
    fflush(stdout);
    string key;
    vector<string> text;
    int pos = 0;

    do {
        cout << ansi::CLEAR_SCREEN;
        printMenu();
        cout << ansi::moveTo(4, 1);
        printText(text, pos);
        cout.flush();

        key = readKey();

        if (key == "Left") {
            if (pos > 0) {
                pos--;
            }
        } else if (key == "Right") {
            pos++;
        } else if (key == "Up") {
        } else if (key == "Down") {
        } else if (key == "Home") {
            pos = 0;
        } else if (key == "End") {
            pos = text.size();
        } else if (key == "Backspace") {
            if (pos > 0) {
                pos--;
                if (pos < text.size()) {
                    text[pos] = "";
                }
            }
        } else if (key == "Enter") {
            while (pos >= text.size())
                text.push_back("");
            text[pos] += "\n";
            pos++;
        } else if (key == "Space") {
            while (pos >= text.size())
                text.push_back("");
            text[pos] += ' ';
            pos++;
        } else if (key.length() == 1 && key[0] >= 32 && key[0] < 127) {
            while (pos >= text.size())
                text.push_back("");
            text[pos] += key;
            pos++;
        }


        this_thread::sleep_for(chrono::milliseconds(1000/20));
    } while (key != "F1");

    return 0;
}