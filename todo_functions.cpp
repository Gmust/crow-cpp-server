#include "todo_functions.h"
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

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

vector<Todo> readTodosFromFile(const string &username) {
    vector<Todo> todos;
    ifstream file("users/" + username + "/todos.txt");

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
    return {400, "Invalid JSON"};
}

crow::response todoNotFoundError(const string todoId) {
    setConsoleColor(FOREGROUND_RED);
    cout << "Error: Todo not found with that ID:" << todoId << endl;
    setConsoleColor(FOREGROUND_INTENSITY);
    return {404, "Not found"};
}

pair<string, string> separateString(const string &str, char delimiter) {
    size_t found = str.find(delimiter);
    return {str.substr(0, found), str.substr(found + 1)};
}

bool isUsernameAvailable(const std::string &username, const std::string &path) {
    for (const auto &entry: fs::directory_iterator(path)) {
        if (entry.is_directory() && entry.path().filename() == username) {
            return false;
        }
    }
    return true;
}

void createNewUser(const string &usersPath, string &username, string &encodedData, string &userId) {
    fs::create_directory(usersPath + "/" + username);

    ofstream infoFile(usersPath + "/" + username + "/info.txt");
    ofstream todosFile(usersPath + "/" + username + "/todos.txt");

    if (infoFile.is_open()) {
        infoFile << userId << endl;
        infoFile << encodedData << endl;
    } else {
        setConsoleColor(FOREGROUND_RED);
        cout << "Unable to open file!" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
    }

    infoFile.close();
}