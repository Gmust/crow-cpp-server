const loginUser = async () => {

    const loginInput = document.getElementById('username');
    const passwordInput = document.getElementById('password');
    const errorElement = document.getElementById('error');
    const successElement = document.getElementById('success');

    if (!loginInput || !passwordInput) {
        errorElement.textContent = 'Please fill all fields';
    }

    const stringToEncode = loginInput.value + ':' + passwordInput.value;
    const encodedString = btoa(stringToEncode);

    const response = await fetch('http://localhost:18080/login', {
        headers: {
            'Authorization': `Basic ${encodedString}`,
        },
        method: 'POST'
    });
    if (response.ok) {
        const responseBody = await response.json();
        localStorage.setItem('username', responseBody.username);
        successElement.textContent = `${responseBody.message}. You will be redirected in 3 sec...`
        setTimeout(() => {
            window.location.href = '/home'
        }, 3000);
    } else {
        errorElement.textContent = 'Error: ' + response.statusText;
        errorElement.scrollIntoView({behavior: 'smooth', block: 'center'});
    }
}

document.addEventListener('submit', (event) => {
    event.preventDefault();
    loginUser();
})