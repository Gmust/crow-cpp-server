#pragma once

#include "crow.h"
#include "todo_functions.h"

crow::response addTodoController(const crow::request &req);
crow::response getTodosController();
crow::response deleteTodoController(const crow::request &req);
crow::response changeTodoStatus(const crow::request &req);
crow::response changeTodoTask(const crow::request &req);
crow::response getTodoInfo(const string &id);