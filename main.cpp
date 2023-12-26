#include "crow.h"

#include "todo_controllers.h"
#include "todo_functions.h"

using namespace std;

int main() {

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        auto page = crow::mustache::load_text("login.html");
        return page;
    });

    CROW_ROUTE(app, "/signup")
            .methods("POST"_method)(registerUser);

    CROW_ROUTE(app, "/login")
            .methods("POST"_method)(loginUser);

    CROW_ROUTE(app, "/registration")([]() {
        auto page = crow::mustache::load_text("registration.html");
        return page;
    });


    CROW_ROUTE(app, "/error")([]() {
        auto page = crow::mustache::load_text("error.html");
        return page;
    });

    CROW_ROUTE(app, "/home")([]() {
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
            ([](const crow::request &req, crow::response &res, const std::string &id) {
                getTodoInfo(id, req);
            });

    setConsoleColor(FOREGROUND_GREEN);
    cout << "Server launched successfully!" << endl;
    setConsoleColor(FOREGROUND_INTENSITY);
    app.port(18080).multithreaded().run();


    return 0;
}