#pragma once

#include "crow.h"
#include "todo_functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

crow::response addTodoController(const crow::request &req);
crow::response getTodosController();