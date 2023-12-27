#pragma once

#include "crow.h"
#include "todo_functions.h"
#include <fstream>
#include <sstream>
#include <filesystem>


crow::response addTodoController(const crow::request &req);

crow::response getTodosController(const crow::request &req);

crow::response deleteTodoController(const crow::request &req);

crow::response changeTodoStatus(const crow::request &req);

crow::response changeTodoTask(const crow::request &req);

crow::response getTodoInfo(string path, const crow::request &req);

crow::response loginUser(const crow::request &req);

crow::response registerUser(const crow::request &req);