#pragma once

#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> readFromFile(std::string path);
void printToFile(std::vector<std::string> text);