#include "todo_controllers.h"


crow::response addTodoController(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        jsonError();
    }

    Todo todo;

    todo.id = generateId(10);
    todo.task = json["task"].s();
    todo.status = json["status"].b();

    ofstream file("todos.txt", ios::app);
    if (file.is_open()) {
        file << quoted(todo.id) << " " << quoted(todo.task) << " " << todo.status << endl;
        file.close();
        setConsoleColor(FOREGROUND_GREEN);
        cout << "Todo added successfully: " << todo.task << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return {200, "Todo added successfully"};
    } else {
        setConsoleColor(FOREGROUND_RED);
        cout << "Error: Failed to create todo file" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return {500, "Failed to create todo file"};
    }
}

crow::response getTodosController() {
    std::vector<Todo> todos = readTodosFromFile();

    crow::json::wvalue::list todosResponse;
    for (auto &todo: todos) {
        crow::json::wvalue todoJson;
        todoJson["id"] = todo.id;
        todoJson["task"] = todo.task;
        todoJson["status"] = todo.status;
        todosResponse.push_back(todoJson);
    }

    crow::json::wvalue jsonResponse = crow::json::wvalue(todosResponse);
    return {jsonResponse};
}

crow::response deleteTodoController(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        jsonError();
    }

    string todoId = json["id"].s();
    vector<Todo> todos = readTodosFromFile();

    auto todoToRemove = remove_if(todos.begin(), todos.end(), [todoId](Todo &todo) {
        return todo.id == todoId;
    });

    if (todoToRemove != todos.end()) {
        todos.erase(todoToRemove, todos.end());
        ofstream file("todos.txt");
        if (file.is_open()) {
            for (auto &todo: todos) {
                file << quoted(todo.id) << " " << quoted(todo.task) << " " << todo.status << endl;
            }
            file.close();
            setConsoleColor(FOREGROUND_GREEN);
            cout << "TODO successfully deleted" << todoId << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
            return {200, "Todo deleted successfully"};
        } else {
            setConsoleColor(FOREGROUND_RED);
            cout << "Error: Failed to open todo file" << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
            return {500, "Failed to delete todo"};
        }
    } else {
        setConsoleColor(FOREGROUND_RED);
        cout << "Error: Todo not found with that ID:" << todoId << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return {404, "Not found"};
    }
}