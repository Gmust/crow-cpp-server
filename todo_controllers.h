#pragma once

#include "crow.h"
#include "todo_functions.h"
#include <fstream>
#include <sstream>


crow::response addTodoController(const crow::request &req);

crow::response getTodosController(const crow::request &req);

crow::response deleteTodoController(const crow::request &req);

crow::response changeTodoStatus(const crow::request &req);

crow::response changeTodoTask(const crow::request &req);

crow::response getTodoInfo(string id, const crow::request &req);

crow::response loginUser(crow::request req);