#include "todo_functions.h"
#include <iomanip>

using namespace std;

string generateId(int id_length) {
    static char signs[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    string randomId;
    randomId.reserve(id_length);

    auto timeNow = chrono::system_clock::now();
    auto currentTimestamp = chrono::duration_cast<chrono::milliseconds>(timeNow.time_since_epoch()).count();
    randomId += to_string(currentTimestamp);

    for (int i = randomId.length(); i < id_length; i++) {
        randomId += signs[rand() % (sizeof(signs) - 1)];
    }

    return randomId;
}

void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

vector <Todo> readTodosFromFile() {
    vector <Todo> todos;
    ifstream file("todos.txt");
    if (!file.is_open()) {
        setConsoleColor(FOREGROUND_RED);
        cout << "Unable to open file: todos.txt" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string id, task, statusStr;

        if (!(iss >> quoted(id) >> quoted(task) >> statusStr)) {
            setConsoleColor(FOREGROUND_RED);
            cout << "Error parsing values in the file" << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
        }

        Todo todo;

        todo.id = id;
        todo.task = task;
        todo.status = statusStr == "0" ? false : true;
        todos.push_back(todo);
    }

    return todos;
}

crow::response jsonError() {
    setConsoleColor(FOREGROUND_RED);
    std::cout << "Error: Invalid JSON" << std::endl;
    setConsoleColor(FOREGROUND_INTENSITY);
    return crow::response(400, "Invalid JSON");
}

crow::response updateTodosFile(vector <Todo> todos) {
    ofstream file("todos.txt", ios::trunc);
    if (file.is_open()) {
        for (const auto &todo: todos) {
            file << quoted(todo.id) << " " << quoted(todo.task) << " " << todo.status << endl;
        }
        file.close();
        setConsoleColor(FOREGROUND_GREEN);
        cout << "TODO  updated successfully! " << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return {200, "TODO updated successfully"};
    } else {
        setConsoleColor(FOREGROUND_RED);
        cout << "Error: Failed to open todo file" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return {500, "Failed to update todo"};
    }
}