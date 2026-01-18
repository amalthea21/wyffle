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
    cout << " " << ansi::BLACK << ansi::BG_BRIGHT_RED << " QUIT: [F1] " << ansi::RESET << " ";
    cout << " " << ansi::BLACK << ansi::BG_CYAN << " FG COLORS: [F2] " << ansi::RESET << " ";
    cout << " " << ansi::BLACK << ansi::BG_BRIGHT_GREEN << " BG COLORS: [F3] " << ansi::RESET << " ";
    cout << " " << ansi::BLACK << ansi::BG_BRIGHT_YELLOW << " FORMATTING: [F4] " << ansi::RESET << " ";
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

    cout.flush();
}

void printText(vector<string> text, int pos) {
    for (int i = 0; i < text.size(); i++) {
        if (i == pos) {
            if (text.at(i).empty()) {
                cout << ansi::BG_BRIGHT_WHITE << " " << ansi::RESET;
            } else {
                cout << ansi::BG_BRIGHT_WHITE << ansi::BLACK << text.at(i) << ansi::RESET;
            }
        } else {
            cout << text.at(i);
        }
    }

    if (pos >= text.size()) {
        cout << ansi::BG_BRIGHT_WHITE << " " << ansi::RESET;
    }
}

void editText(string key, vector<string> &text, int &pos) {
    if (key == "Left") {
        if (pos > 0)
            pos--;
    }
    else if (key == "Right") {
        if (pos < text.size())
            pos++;
    }
    else if (key == "Up") {
        for (int i = pos - 1; i >= 0; i--) {
            if (text.at(i) == "\n") {
                pos = i;
                break;
            }
        }
    }
    else if (key == "Down") {
        for (int i = pos + 1; i < text.size(); i++) {
            if (text.at(i) == "\n") {
                pos = i + 1;
                break;
            }
        }
    }
    else if (key == "Home") {
        pos = 0;
    }
    else if (key == "End") {
        pos = text.size();
    }
    else if (key == "Backspace") {
        if (pos > 0) {
            pos--;
            if (pos < text.size()) {
                text[pos] = "";
            }
        }
    }
    else if (key == "Delete") {
        if (pos < text.size()) {
            text[pos+1] = "";
        }
    }
    else if (key == "Enter") {
        text.insert(text.begin() + pos, "\n");
        pos++;
    }
    else if (key == "Space") {
        while (pos >= text.size())
            text.push_back("");
        text[pos] += ' ';
        pos++;
    }
    else if (key.length() == 1 && key[0] >= 32 && key[0] < 127) { // Normal character
        while (pos >= text.size())
            text.push_back("");
        text[pos] += key;
        pos++;
    }
}

bool applyFormatting(string key, string activeSubmenu, vector<string> &text, int &pos) {
    if (activeSubmenu.empty()) return false;

    string formatting = "";

    if (activeSubmenu == "F2") { // Foreground colors
        if (key == "1") formatting = ansi::BRIGHT_RED;
        else if (key == "2") formatting = ansi::BRIGHT_GREEN;
        else if (key == "3") formatting = ansi::BRIGHT_WHITE;
        else if (key == "4") formatting = ansi::BRIGHT_YELLOW;
        else if (key == "5") formatting = ansi::BRIGHT_BLACK;
        else if (key == "6") formatting = ansi::BRIGHT_BLUE;
        else if (key == "7") formatting = ansi::BRIGHT_CYAN;
        else if (key == "8") formatting = ansi::BRIGHT_MAGENTA;
    }
    else if (activeSubmenu == "F3") { // Background colors
        if (key == "1") formatting = ansi::BG_BRIGHT_RED;
        else if (key == "2") formatting = ansi::BG_BRIGHT_GREEN;
        else if (key == "3") formatting = ansi::BG_BRIGHT_WHITE;
        else if (key == "4") formatting = ansi::BG_BRIGHT_YELLOW;
        else if (key == "5") formatting = ansi::BG_BRIGHT_BLACK;
        else if (key == "6") formatting = ansi::BG_BRIGHT_BLUE;
        else if (key == "7") formatting = ansi::BG_BRIGHT_CYAN;
        else if (key == "8") formatting = ansi::BG_BRIGHT_MAGENTA;
    }
    else if (activeSubmenu == "F4") { // Text formatting
        if (key == "1") formatting = ansi::BOLD;
        else if (key == "2") formatting = ansi::DIM;
        else if (key == "3") formatting = ansi::ITALIC;
        else if (key == "4") formatting = ansi::UNDERLINE;
        else if (key == "5") formatting = ansi::BLINK;
        else if (key == "6") formatting = ansi::REVERSE;
        else if (key == "7") formatting = ansi::HIDDEN;
        else if (key == "8") formatting = ansi::STRIKETHROUGH;
    }

    if (!formatting.empty()) {
        while (pos >= text.size())
            text.push_back("");
        text.insert(text.begin() + pos, formatting);
        pos++;
        return true;
    }

    return false;
}

int main(int argc, char* argv[]) {
    cout << ansi::HIDE_CURSOR;
    fflush(stdout);
    string key;
    string activeSubmenu = "";
    vector<string> text;
    int pos = 0;

    do {
        cout << ansi::CLEAR_SCREEN;
        printMenu();
        printSubMenu(activeSubmenu);
        cout << ansi::moveTo(4, 1);
        printText(text, pos);
        cout.flush();

        key = readKey();

        if (key == "F2" || key == "F3" || key == "F4") {
            activeSubmenu = (activeSubmenu == key) ? "" : key;
        }

        bool formattingApplied = applyFormatting(key, activeSubmenu, text, pos);

        if (!formattingApplied) {
            editText(key, text, pos);
        }

        this_thread::sleep_for(chrono::milliseconds(1000/20));
    } while (key != "F1");

    return 0;
}