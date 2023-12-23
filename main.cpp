#include "crow.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Todo {
    string task;
    bool status;
};


void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
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
                            // Print error message in red
                            setConsoleColor(FOREGROUND_RED);
                            std::cout << "Error: Invalid JSON" << std::endl;
                            setConsoleColor(FOREGROUND_INTENSITY); // Reset color to default
                            return crow::response(400, "Invalid JSON");
                        }

                        Todo todo;

                        todo.task = json["task"].s();
                        todo.task = json["task"].s();

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

    app.port(18080).multithreaded().run();
    return 0;
}