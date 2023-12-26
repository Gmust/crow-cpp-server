#include "todo_controllers.h"


crow::response todoNotFoundError();

crow::response addTodoController(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        jsonError();
    }

    Todo todo;

    todo.id = generateId(10);
    todo.task = json["task"].s();
    todo.status = json["status"].b();
    string username = json["username"].s();

    ofstream file("users/" + username + "/todos.txt", ios::app);
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

crow::response getTodosController(const crow::request &req) {

    auto json = crow::json::load(req.body);
    std::vector<Todo> todos = readTodosFromFile(json["username"].s());

    crow::json::wvalue::list todosResponse;
    for (auto &todo: todos) {
        crow::json::wvalue todoJson;
        todoJson["id"] = todo.id;
        todoJson["task"] = todo.task;
        todoJson["status"] = todo.status;
        todosResponse.push_back(todoJson);
    }

    crow::json::wvalue jsonResponse = crow::json::wvalue(todosResponse);
    return {200, jsonResponse};
}

crow::response deleteTodoController(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        jsonError();
    }

    string todoId = json["id"].s();
    vector<Todo> todos = readTodosFromFile(json["username"].s());

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

crow::response changeTodoStatus(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return jsonError();
    }

    string todoId = json["id"].s();
    bool todoStatus = json["status"].b();

    vector<Todo> todos = readTodosFromFile(json["username"].s());

    auto existingTodo = find_if(todos.begin(), todos.end(), [todoId](const Todo &todo) {
        return todo.id == todoId;
    });

    if (existingTodo != todos.end()) {
        string existingTask = existingTodo->task;

        existingTodo->status = todoStatus;

        ofstream file("todos.txt", ios::trunc);
        if (file.is_open()) {
            for (const auto &todo: todos) {
                file << quoted(todo.id) << " " << quoted(todo.task) << " " << todo.status << endl;
            }
            file.close();
            setConsoleColor(FOREGROUND_GREEN);
            cout << "TODO status updated successfully: " << todoId << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
            return {200, "TODO status updated successfully"};
        } else {
            setConsoleColor(FOREGROUND_RED);
            cout << "Error: Failed to open todo file" << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
            return {500, "Failed to update todo status"};
        }
    } else {
        setConsoleColor(FOREGROUND_RED);
        cout << "Error: Todo not found with that ID:" << todoId << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return {404, "Not found"};
    }
}


crow::response changeTodoTask(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return jsonError();
    }

    string todoId = json["id"].s();
    string newTodoTask = json["task"].s();

    vector<Todo> todos = readTodosFromFile(json["username"].s());

    auto existingTodo = find_if(todos.begin(), todos.end(), [todoId](const Todo &todo) {
        return todo.id == todoId;
    });

    if (existingTodo != todos.end()) {
        existingTodo->task = newTodoTask;

        existingTodo->status = existingTodo->status;

        ofstream file("todos.txt", ios::trunc);
        if (file.is_open()) {
            for (const auto &todo: todos) {
                file << quoted(todo.id) << " " << quoted(todo.task) << " " << todo.status << endl;
            }
            file.close();
            setConsoleColor(FOREGROUND_GREEN);
            cout << "TODO  updated successfully: " << todoId << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
            return {200, "TODO  updated successfully"};
        } else {
            setConsoleColor(FOREGROUND_RED);
            cout << "Error: Failed to open todo file" << endl;
            setConsoleColor(FOREGROUND_INTENSITY);
            return {500, "Failed to update  status"};
        }
    } else {
        return todoNotFoundError(todoId);
    }
}

crow::response getTodoInfo(string id, const crow::request &req) {
    auto json = crow::json::load(req.body);

    vector<Todo> todos = readTodosFromFile(json["username"].s());
    auto todo = find_if(todos.begin(), todos.end(), [id](const Todo &todo) {
        return todo.id == id;
    });

    if (todo != todos.end()) {
        crow::json::wvalue todoJson;
        todoJson["id"] = todo->id;
        todoJson["task"] = todo->task;
        todoJson["status"] = todo->status;
        crow::json::wvalue jsonResponse = crow::json::wvalue(todoJson);
        return {200, jsonResponse};
    } else {
        return todoNotFoundError(id);
    }
}

crow::response loginUser(const crow::request &req) {
    string authHeader = req.get_header_value("Authorization");
    string userCredits = authHeader.substr(6);
    string decodedUserCredits = crow::utility::base64decode(userCredits, userCredits.size());

    auto [username, password] = separateString(decodedUserCredits, ':');

    string userFolder = "users/" + username;
    ifstream userInfoFile(userFolder + "/info.txt");

    if (!userInfoFile) {
        setConsoleColor(FOREGROUND_RED);
        cout << "Error: Unable to open user file" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);
        return {500, "Internal error"};
    }

    string userId, userStoredEncodedInfo;
    getline(userInfoFile, userId);
    getline(userInfoFile, userStoredEncodedInfo);

    string userStoredDecodedInfo = crow::utility::base64decode(userStoredEncodedInfo);
    auto [storedUsername, storedPassword] = separateString(userStoredDecodedInfo, ':');

    if (username == storedUsername && password == storedPassword) {
        crow::json::wvalue userJson;
        userJson["username"] = storedUsername;
        userJson["message"] = "Authentificated";

        return crow::response(200, userJson);
    } else {
        return crow::response(401, "Invalid credentials");
    }
}

crow::response registerUser(const crow::request &req) {
    auto json = crow::json::load(req.body);

    if (!json) {
        jsonError();
    }

    string username = json["username"].s();
    string password = json["password"].s();
    string userId = generateId(10);

    string userPath = "users";

    if (username.size() < 0 || password.size() < 0) {
        setConsoleColor(FOREGROUND_RED);
        cout << "Provide username and password!" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);

        return {400, "Provide username and password!"};
    }

    if (!isUsernameAvailable(username, userPath)) {

        setConsoleColor(FOREGROUND_RED);
        cout << "Username is not available" << endl;
        setConsoleColor(FOREGROUND_INTENSITY);

        return {400, "Username is not available"};
    }

    string strToEncode = username + ':' + password;
    string encodedInfo = crow::utility::base64encode(strToEncode, strToEncode.size());

    createNewUser(userPath, username, encodedInfo, userId);

    return {200, "New user successfully registered"};
}

