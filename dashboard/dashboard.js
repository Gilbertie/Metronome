// const fetch = require("node-fetch");
document.getElementById('refresh').addEventListener('click', refresh);
document.getElementById('set_bpm').addEventListener('click', set_bpm);
document.getElementById('reset_min').addEventListener('click', reset_min);
document.getElementById('reset_max').addEventListener('click', reset_max);

function refresh() {
    console.log('refreshing...');
    refresh_bpm();
    refresh_bpm_min();
    refresh_bpm_max();
}

function refresh_bpm() {
    fetch('http://192.168.0.77:8080/bpm/')
    .then(response => response.json())
    .then(data => document.getElementById('bpm').innerHTML = data)
    .catch((error) => {
        console.log('error-bpm');
    });
}

function refresh_bpm_min() {
    fetch('http://192.168.0.77:8080/bpm/min')
    .then(response => response.json())
    .then(data => document.getElementById('bpm_min').innerHTML = data)
    .catch((error) => {
        console.log('error-min');
    });
}

function refresh_bpm_max() {
    fetch('http://192.168.0.77:8080/bpm/max')
    .then(response => response.json())
    .then(data => document.getElementById('bpm_max').innerHTML = data)
    .catch((error) => {
        console.log('error-max');
    });
}

function set_bpm() {
    var text = document.getElementById('new_bpm').value;
    console.log(text);
    fetch('http://192.168.0.77:8080/bpm/',
    {
        method: 'PUT',
        body: text
    })
    .then(response => {
        if (response.ok) {
            console.log('new bpm set')
        } else {
            console.log('new bpm not set');
    }})
    .catch((error) => {
        console.log('error-set-bpm');
    });
}

function reset_min() {
    fetch('http://192.168.0.77:8080/bpm/min',
    {
        method: 'DELETE',
    })
    .catch((error) => {
        console.log('error-reset-min');
    });
}

function reset_max() {
    fetch('http://192.168.0.77:8080/bpm/max',
    {
        method: 'DELETE',
    })
    .catch((error) => {
        console.log('error-reset-max');
    });
}