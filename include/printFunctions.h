#pragma once

#include <string>
#include <vector>

std::string getBorder();
void printMenu();
void printSubMenu(std::string key);
void printText(std::vector<std::string> text, int pos);
std::string getFilePathInSubmenu(std::string mode);