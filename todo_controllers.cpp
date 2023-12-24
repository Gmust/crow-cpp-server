#include "todo_controllers.h"


crow::response addTodoController(const crow::request &req){
    auto json = crow::json::load(req.body);
    if (!json) {
        setConsoleColor(FOREGROUND_RED);
        std::cout << "Error: Invalid JSON" << std::endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return crow::response(400, "Invalid JSON");
    }

    Todo todo;

    todo.id = generateId(10);
    todo.task = json["task"].s();
    todo.status = json["status"].b();

    ofstream file("todos.txt", ios::app);
    if (file.is_open()) {
        file << quoted(todo.id) << " " << quoted(todo.task) << " " << todo.status << endl;
        file.close();
        cout << "Todo added successfully: " << todo.task << endl;
        return crow::response(200, "Todo added successfully");
    } else {
        setConsoleColor(FOREGROUND_RED);
        cout << "Error: Failed to create todo file" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return crow::response(500, "Failed to create todo file");
    }
}

crow ::response getTodosController(){
    std::vector<Todo> todos = readTodosFromFile();

    crow::json::wvalue::list todosResponse;
    for (const auto &todo: todos) {
        crow::json::wvalue todoJson;
        todoJson["id"] = todo.id;
        todoJson["task"] = todo.task;
        todoJson["status"] = todo.status;
        todosResponse.push_back(todoJson);
    }

    crow::json::wvalue jsonResponse = crow::json::wvalue(todosResponse);
    return crow::response(jsonResponse);
}