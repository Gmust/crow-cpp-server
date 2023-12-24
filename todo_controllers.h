#pragma once

#include "crow.h"
#include "todo_functions.h"

crow::response addTodoController(const crow::request &req);
crow::response getTodosController();
crow::response deleteTodoController(const crow::request &req);