#include "crow.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Todo {
    string task;
    bool status;
};


void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

vector<Todo> readTodosFromFile() {
    std::vector<Todo> todos;
    std::ifstream file("todos.txt");
    if (!file.is_open()) {
        setConsoleColor(FOREGROUND_RED);
        cout << "Unable to open file: todos.txt" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string task, statusStr;

        if (!(iss >> quoted(task))) {
            setConsoleColor(FOREGROUND_RED);
            cout << "Error parsing task in the file" << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
        }

        if (!(iss >> statusStr)) {
            setConsoleColor(FOREGROUND_RED);
            cout << "Error parsing status in the file" << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
        }

        Todo todo;

        todo.task = task;
        todo.status = (statusStr == "0") ? "Incomplete" : "Complete";
        todos.push_back(todo);
    }

    return todos;
}


int main() {

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        auto page = crow::mustache::load_text("homepage.html");
        return page;
    });

    CROW_ROUTE(app, "/add")
            .methods("POST"_method)
                    ([](const crow::request &req) {
                        auto json = crow::json::load(req.body);
                        if (!json) {
                            setConsoleColor(FOREGROUND_RED);
                            std::cout << "Error: Invalid JSON" << std::endl;
                            setConsoleColor(FOREGROUND_INTENSITY);
                            return crow::response(400, "Invalid JSON");
                        }

                        Todo todo;

                        todo.task = json["task"].s();
                        todo.status = json["status"].b();

                        ofstream file("todos.txt", ios::app);
                        if (file.is_open()) {
                            file << quoted(todo.task) << " " << todo.status << endl;
                            file.close();
                            cout << "Todo added successfully: " << todo.task << endl;
                            return crow::response(200, "Todo added successfully");
                        } else {
                            setConsoleColor(FOREGROUND_RED);
                            cout << "Error: Failed to create todo file" << endl;
                            setConsoleColor(FOREGROUND_INTENSITY);
                            return crow::response(500, "Failed to create todo file");
                        }
                    });

    CROW_ROUTE(app, "/get-todos")
            .methods("GET"_method)
                    ([]() {
                        std::vector<Todo> todos = readTodosFromFile();

                        crow::json::wvalue::list todosResponse;
                        for (const auto &todo : todos) {
                            crow::json::wvalue todoJson;
                            todoJson["task"] = todo.task;
                            todoJson["status"] = todo.status;
                            todosResponse.push_back(todoJson);
                        }

                        crow::json::wvalue jsonResponse = crow::json::wvalue(todosResponse);
                        return crow::response(jsonResponse);
                    });
    app.port(18080).multithreaded().run();
    return 0;
}