let isEditMode = false;
const username = localStorage.getItem('username');

if (!username) {
    window.location.href = '/';
}

const toggleEditMod = () => {
    isEditMode = !isEditMode;
    const taskList = document.getElementById('taskList');
    const editTodo = document.getElementById('editTodo');

    if (isEditMode) {
        taskList.style.display = 'none';
        editTodo.style.display = 'block';
    } else {
        taskList.style.display = 'block';
        editTodo.style.display = 'none';
    }
}

const makeTaskEditable = async (id) => {
    const response = await fetch(`http://localhost:18080/get-todo-info/${id}-${username}`, {
        method: "GET",
    });
    const todo = await response.json();

    const editTaskInput = document.getElementById('editTask');
    editTaskInput.value = todo.task;
    document.getElementById('editTodoForm').setAttribute('data-id', id);

    toggleEditMod();
}

const updateTodoTask = async () => {
    const taskInput = document.getElementById('editTask');
    const errorElement = document.getElementById('error');
    errorElement.textContent = '';
    const id = document.getElementById('editTodoForm').getAttribute('data-id');

    const data = {
        id: id,
        task: taskInput.value,
        username: username
    }

    const response = await fetch('http://localhost:18080/update-todo-task', {
            method: "PUT",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(data)
        }
    )
    if (response.ok) {
        showSnackbar('Successfully updated todo task!')
        await fetchTodos();
    } else {
        errorElement.textContent = 'Error: ' + response.statusText;
        errorElement.scrollIntoView({behavior: 'smooth', block: 'center'});
    }
}

const updateTodoStatus = async (id) => {
    const errorElement = document.getElementById('error');
    errorElement.textContent = '';


    const checkbox = document.getElementById(`checkbox-${id}`);
    const status = checkbox.checked;

    const data = {
        id,
        status,
        username
    }

    const response = await fetch('http://localhost:18080/update-todo-status', {
        method: 'PUT',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
    if (response.ok) {
        console.log(response)
        showSnackbar("Status successfully updated!")
        await fetchTodos();
    } else {
        errorElement.textContent = 'Error: ' + response.statusText;
        errorElement.scrollIntoView({behavior: 'smooth', block: 'center'});
    }
}

const deleteTodo = async (id) => {
    const errorElement = document.getElementById('error');
    errorElement.textContent = '';

    const data = {
        id,
        username
    }

    const response = await fetch('http://localhost:18080/delete-todo', {
        method: 'DELETE',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
    if (response.ok) {
        showSnackbar("Task successfully deleted!");
        await fetchTodos();
    } else {
        errorElement.textContent = 'Error: ' + response.statusText;
        errorElement.scrollIntoView({behavior: 'smooth', block: 'center'});
    }
}

const fetchTodos = async () => {
    const tasksList = document.getElementById('taskList'); // Corrected ID
    const response = await fetch(`http://localhost:18080/get-todos?username=${username}`, {
        method: 'GET',
        headers: {
            'Content-Type': 'application/json'
        },
    });
    const todos = await response.json();
    tasksList.innerHTML = todos.map(todo =>
        `<li class="task-item">
                 <input
                  type="checkbox" ${todo.status ? 'checked' : ''}
                   id="checkbox-${todo.id}"
                   onChange="updateTodoStatus('${todo.id}')"
                 />
                 <p>${todo.task}</p>
                 <div class="icons-wrapper">
                    <i class="fa-solid fa-trash icon-style" id={todo.id} onClick="deleteTodo('${todo.id}')"></i>
                    <i class="fa-solid fa-pen icon-style" onclick="makeTaskEditable('${todo.id}')"></i>
                 </div>
            </li>`
    ).join('');
};

document.getElementById('todoForm').addEventListener('submit', async function (event) {
    event.preventDefault();
    await addTask();
});

document.getElementById('editTodoForm').addEventListener('submit', async function (event) {
    event.preventDefault();
    await updateTodoTask();
    toggleEditMod();
});
const addTask = async () => {
    const taskInput = document.getElementById('task');
    const errorElement = document.getElementById('error');
    errorElement.textContent = '';
    const data = {
        task: taskInput.value,
        status: false,
        username
    };
    try {
        const response = await fetch('http://localhost:18080/add', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(data)
        });

        if (!response.ok) {
            errorElement.textContent = 'Error: ' + response.statusText;
            errorElement.scrollIntoView({behavior: 'smooth', block: 'center'});
        }
        showSnackbar("New task successfully created!");
        await fetchTodos();
    } catch (error) {
        errorElement.textContent = 'Error: ' + error.message;
        errorElement.scrollIntoView({behavior: 'smooth', block: 'center'});
    }

    taskInput.value = '';
};


const showSnackbar = (message) => {
    const snackBar = document.getElementById('snackbar');
    snackBar.textContent = message;
    snackBar.className = "snackbar show";
    setTimeout(() => {
        snackBar.className = snackBar.className.replace("snackbar show", "snackbar");
    }, 3000);
}

fetchTodos();