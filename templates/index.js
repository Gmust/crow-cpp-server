document.getElementById('todoForm').addEventListener('submit', function(event) {
    event.preventDefault();
    addTask();
});

function addTask() {
    var taskInput = document.getElementById('task');
    var taskList = document.getElementById('taskList');
    var errorElement = document.getElementById('error');

    errorElement.textContent = '';

    var data = {
        todo: taskInput.value,
        status: false
    };

    fetch('/add', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
        .then(response => {
            if (!response.ok) {
                errorElement.textContent = 'Error: ' + response.statusText;
                errorElement.scrollIntoView({ behavior: 'smooth', block: 'center' });
            }
        })
        .catch(error => {
            errorElement.textContent = 'Error: ' + error.message;
            errorElement.scrollIntoView({ behavior: 'smooth', block: 'center' });
        });

    taskInput.value = '';
}