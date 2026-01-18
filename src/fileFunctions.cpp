#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "../include/fileFunctions.h"

using namespace std;

vector<string> readFromFile(string path) {
    vector<string> text;
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << path << "'" << endl;
        return text; // Return empty vector
    }

    char ch;
    string currentChar;

    while (file.get(ch)) {
        if (ch == '\033') {
            currentChar = ch;
            while (file.get(ch) && ch != 'm') {
                currentChar += ch;
            }
            currentChar += ch;
            text.push_back(currentChar);
            currentChar = "";
        }
        else if (ch == '\n') {
            text.push_back("\n");
        }
        else {
            text.push_back(string(1, ch));
        }
    }

    file.close();
    return text;
}

void printToFile(vector<string> text) {
    string filename;
    cout << "Enter filename to save: ";
    getline(cin, filename);

    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not create file '" << filename << "'" << endl;
        return;
    }

    for (const string& element : text) {
        file << element;
    }

    file.close();
    cout << "File saved successfully to '" << filename << "'" << endl;
}