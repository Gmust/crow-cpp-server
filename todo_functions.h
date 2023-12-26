#pragma once

#include "crow.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <minwindef.h>
#include <windows.h>

using namespace std;

struct Todo {
    string task;
    bool status;
    string id;
};

string generateId(int id_length);

void setConsoleColor(WORD color);

vector<Todo> readTodosFromFile(const string &username);

crow::response jsonError();

crow::response todoNotFoundError(string todoId);

pair<string, string> separateString(const string &str, char delimiter);

bool isUsernameAvailable(const string &username, const string &path);

void createNewUser(const string &usersPath, string &username, string &encodedData, string &userId);