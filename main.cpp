#include "crow.h"

#include "todo_controllers.h"
#include "todo_functions.h"

using namespace std;

int main() {

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        auto page = crow::mustache::load_text("homepage.html");
        return page;
    });

    CROW_ROUTE(app, "/add")
            .methods("POST"_method)(addTodoController);

    CROW_ROUTE(app, "/get-todos")
            .methods("GET"_method)(getTodosController);

    CROW_ROUTE(app, "/delete-todo")
            .methods("DELETE"_method)(deleteTodoController);

    CROW_ROUTE(app, "/update-todo-status")
            .methods("PUT"_method)(changeTodoStatus);

    CROW_ROUTE(app, "/update-todo-task")
            .methods("PUT"_method)(changeTodoTask);

    CROW_ROUTE(app, "/get-todo-info/<string>")
            .methods("GET"_method)([](const string id) {
                return getTodoInfo(id);
            });

    setConsoleColor(FOREGROUND_GREEN);
    cout << "Server launched successfully!" << endl;
    setConsoleColor(FOREGROUND_INTENSITY);
    app.port(18080).multithreaded().run();


    return 0;
}