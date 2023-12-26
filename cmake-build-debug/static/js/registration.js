const validateUsername = (username) => {
    const nameRegex = /^[a-zA-Z0-9]+$/;
    if (!username.match(nameRegex)) {
        const error = document.getElementById("username-error");
        error.textContent = "Your username is not valid. Only characters A-Z, a-z and '-' are  acceptable.";
        error.focus();
        return false;
    }
    return true;
}


const validatePassword = (password) => {
    const error = document.getElementById("password-error");
    const passwordRegex = /(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])(?=.{8,})/;
    if (!password.match(passwordRegex)) {
        error.textContent = " Password must be a minimum of 8 characters including number, Upper, Lower And one special character";
        error.focus();
        return false;
    }
    error.textContent = "";
    return true;
}

const matchPasswords = (firstPassword, secondPassword) => {
    const error = document.getElementById("repeat-password-error");
    if (firstPassword.toString() === secondPassword.toString()) {
        error.textContent = "";
        return true
    } else {
        error.textContent = "Passwords don`t match!";
        error.focus();
        return false;
    }
}

const registerUser = async () => {
    const usernameInput = document.getElementById('username');
    const passwordInput = document.getElementById('password');
    const repeatedPasswordInput = document.getElementById('repeat-password');
    if (!validateUsername(usernameInput.value)) {
        return
    }
    if (!validatePassword(passwordInput.value)) {
        return
    }
    if (!matchPasswords(passwordInput.value, repeatedPasswordInput.value)) {
        return
    }

    const data = {
        username: usernameInput.value,
        password: passwordInput.value
    }

    const response = await fetch('http://localhost:18080/signup', {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
    })

    if (response.ok) {
        document.getElementById('success-message').textContent = "User successfully created!";
    } else {
        let errorElement = document.getElementById('error');
        errorElement.textContent = 'Error: ' + response.statusText;
        errorElement.scrollIntoView({behavior: 'smooth', block: 'center'});
    }
}

document.getElementById('registration-form').addEventListener('submit', (event) => {
    event.preventDefault();
    registerUser();
});