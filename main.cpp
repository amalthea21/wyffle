#include <chrono>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>
#include <fstream>

#include "include/Formatting.h"
#include "include/printFunctions.h"
#include "include/fileFunctions.h"

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

termios initializeTerminal() {
    termios term = {0};

    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &term) < 0)
        perror("tcsetattr ICANON");

    return term;
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
                text.erase(text.begin() + pos);
            }
        }
    }
    else if (key == "Delete") {
        if (pos < text.size()) {
            text.erase(text.begin() + pos);
        }
    }
    else if (key == "Enter") {
        text.insert(text.begin() + pos, "\n");
        pos++;
    }
    else if (key == "Space") {
        while (pos >= text.size())
            text.push_back("");
        text[pos] = " ";
        pos++;
    }
    else if (key.length() == 1 && key[0] >= 32 && key[0] < 127) {
        while (pos >= text.size())
            text.push_back("");
        text[pos] = key;
        pos++;
    }
}

bool applyFormatting(string key, string activeSubmenu, vector<string> &text, int &pos) {
    if (activeSubmenu.empty()) return false;

    string formatting = "";

    if (activeSubmenu == "F2") {
        if (key == "1") formatting = ansi::BRIGHT_RED;
        else if (key == "2") formatting = ansi::BRIGHT_GREEN;
        else if (key == "3") formatting = ansi::BRIGHT_WHITE;
        else if (key == "4") formatting = ansi::BRIGHT_YELLOW;
        else if (key == "5") formatting = ansi::BRIGHT_BLACK;
        else if (key == "6") formatting = ansi::BRIGHT_BLUE;
        else if (key == "7") formatting = ansi::BRIGHT_CYAN;
        else if (key == "8") formatting = ansi::BRIGHT_MAGENTA;
    }
    else if (activeSubmenu == "F3") {
        if (key == "1") formatting = ansi::BG_BRIGHT_RED;
        else if (key == "2") formatting = ansi::BG_BRIGHT_GREEN;
        else if (key == "3") formatting = ansi::BG_BRIGHT_WHITE;
        else if (key == "4") formatting = ansi::BG_BRIGHT_YELLOW;
        else if (key == "5") formatting = ansi::BG_BRIGHT_BLACK;
        else if (key == "6") formatting = ansi::BG_BRIGHT_BLUE;
        else if (key == "7") formatting = ansi::BG_BRIGHT_CYAN;
        else if (key == "8") formatting = ansi::BG_BRIGHT_MAGENTA;
    }
    else if (activeSubmenu == "F4") {
        if (key == "1") formatting = ansi::BOLD;
        else if (key == "2") formatting = ansi::DIM;
        else if (key == "3") formatting = ansi::ITALIC;
        else if (key == "4") formatting = ansi::UNDERLINE;
        else if (key == "5") formatting = ansi::BLINK;
        else if (key == "6") formatting = ansi::REVERSE;
        else if (key == "7") formatting = ansi::HIDDEN;
        else if (key == "8") formatting = ansi::STRIKETHROUGH;
    }
    else if (activeSubmenu == "F5") {
        if (key == "1") { // Save
            string path = getFilePathInSubmenu("SAVE");
            if (!path.empty()) {
                ofstream file(path);
                if (file.is_open()) {
                    for (const string& element : text) {
                        file << element;
                    }
                    file.close();
                }
            }
            return true;
        }
        else if (key == "2") { // Load
            string path = getFilePathInSubmenu("LOAD");
            if (!path.empty()) {
                vector<string> loadedText = readFromFile(path);
                if (!loadedText.empty()) {
                    text = loadedText;
                    pos = 0;
                }
            }
            return true;
        }
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
    string key, activeSubmenu = "";
    vector<string> text;
    int pos = 0;

    cout << ansi::HIDE_CURSOR;
    fflush(stdout);

    do {
        cout << ansi::CLEAR_SCREEN;
        printMenu();
        printSubMenu(activeSubmenu);
        cout << ansi::moveTo(4, 1);
        printText(text, pos);
        cout.flush();

        key = readKey();

        if (key == "F2" || key == "F3" || key == "F4" || key == "F5") {
            activeSubmenu = (activeSubmenu == key) ? "" : key;
        }

        bool formattingApplied = applyFormatting(key, activeSubmenu, text, pos);

        if (!formattingApplied) {
            editText(key, text, pos);
        }

        this_thread::sleep_for(chrono::milliseconds(1000/60));
    } while (key != "F1");

    cout << ansi::SHOW_CURSOR;

    return 0;
}