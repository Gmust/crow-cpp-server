#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <windows.h>
#include <minwindef.h>

using namespace std;

struct Todo {
    string task;
    bool status;
    string id;
};

string generateId(int id_length);

void setConsoleColor(WORD color);

vector<Todo> readTodosFromFile();