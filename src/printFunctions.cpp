#include <string>
#include <unistd.h>
#include <vector>
#include <sys/ioctl.h>
#include <iostream>
#include <termios.h>

#include "../include/Formatting.h"
#include "../include/printFunctions.h"

using namespace std;

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
    cout << " " << ansi::BLACK << ansi::BG_BRIGHT_RED << " QUIT: [F1] " << ansi::RESET << " ";
    cout << " " << ansi::BLACK << ansi::BG_CYAN << " FG COLORS: [F2] " << ansi::RESET << " ";
    cout << " " << ansi::BLACK << ansi::BG_BRIGHT_GREEN << " BG COLORS: [F3] " << ansi::RESET << " ";
    cout << " " << ansi::BLACK << ansi::BG_BRIGHT_YELLOW << " FORMATTING: [F4] " << ansi::RESET << " ";
    cout << " " << ansi::BLACK << ansi::BG_BRIGHT_MAGENTA << " FILE: [F5] " << ansi::RESET << " ";
    cout << endl;

    cout << endl; // Leave line for Submenu
    cout << getBorder() << endl;
    cout.flush();
}

void printSubMenu(string key) {
    cout << ansi::moveTo(2, 1);
    cout << ansi::CLEAR_LINE;

    if (key == "F2") { // Foreground
        cout << " " << ansi::BG_BLACK << ansi::BRIGHT_RED << " RED: [1] " << ansi::RESET << " ";
        cout << " " << ansi::BG_BLACK << ansi::BRIGHT_GREEN << " GREEN: [2] " << ansi::RESET << " ";
        cout << " " << ansi::BG_BLACK << ansi::BRIGHT_WHITE << " WHITE: [3] " << ansi::RESET << " ";
        cout << " " << ansi::BG_BLACK << ansi::BRIGHT_YELLOW << " YELLOW: [4] " << ansi::RESET << " ";
        cout << " " << ansi::BG_BLACK << ansi::BRIGHT_BLACK << " BLACK: [5] " << ansi::RESET << " ";
        cout << " " << ansi::BG_BLACK << ansi::BRIGHT_BLUE << " BLUE: [6] " << ansi::RESET << " ";
        cout << " " << ansi::BG_BLACK << ansi::BRIGHT_CYAN << " CYAN: [7] " << ansi::RESET << " ";
        cout << " " << ansi::BG_BLACK << ansi::BRIGHT_MAGENTA << " MAGENTA: [8] " << ansi::RESET << " ";
    }
    else if (key == "F3") { // Background
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_RED << " RED: [1] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_GREEN << " GREEN: [2] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << " WHITE: [3] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_YELLOW << " YELLOW: [4] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_BLACK << " BLACK: [5] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_BLUE << " BLUE: [6] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_CYAN << " CYAN: [7] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_MAGENTA << " MAGENTA: [8] " << ansi::RESET << " ";
    }
    else if (key == "F4") { // Formatting
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << ansi::BOLD << " BOLD: [1] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << ansi::DIM << " DIM: [2] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << ansi::ITALIC << " ITALIC: [3] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << ansi::UNDERLINE << " UNDERLINE: [4] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << ansi::BLINK << " BLINK: [5] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << ansi::REVERSE << " REVERSE: [6] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << ansi::HIDDEN << " HIDDEN: [7] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_WHITE << ansi::STRIKETHROUGH << " STRIKETHROUGH: [8] " << ansi::RESET << " ";
    }
    else if (key == "F5") { // File operations
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_MAGENTA << " SAVE: [1] " << ansi::RESET << " ";
        cout << " " << ansi::BLACK << ansi::BG_BRIGHT_MAGENTA << " LOAD: [2] " << ansi::RESET << " ";
    }
    else if (key == "SAVE" || key == "LOAD") { // File path input
        cout << " File: ";
    }

    cout.flush();
}

void printText(vector<string> text, int pos) {
    int lineIndex = 1;

    cout << ansi::DIM << lineIndex << ansi::RESET << " ";

    for (int i = 0; i < text.size(); i++) {
        if (text[i] == "\n") {
            cout << endl;
            lineIndex++;
            cout << ansi::DIM << lineIndex << ansi::RESET << " ";
        }
        else if (i == pos) {
            if (text.at(i).empty()) {
                cout << ansi::BG_BRIGHT_WHITE << " " << ansi::RESET;
            } else {
                cout << ansi::BG_BRIGHT_WHITE << ansi::BLACK << text.at(i) << ansi::RESET;
            }
        }
        else {
            cout << text.at(i);
        }
    }

    if (pos >= text.size()) {
        cout << ansi::BG_BRIGHT_WHITE << " " << ansi::RESET;
    }
}

string getFilePathInSubmenu(string mode) {
    string path;

    printSubMenu(mode);
    cout << ansi::SHOW_CURSOR;
    cout.flush();

    termios old = {0};
    tcgetattr(STDIN_FILENO, &old);

    termios normal = old;
    normal.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &normal);

    getline(cin, path);

    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    cout << ansi::HIDE_CURSOR;

    return path;
}