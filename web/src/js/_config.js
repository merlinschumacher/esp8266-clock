
function saveConfig(saveData = false) {
    if (document.getElementById("configform").checkValidity()) {
        conf = tinybind.config;
        conf.saveData = saveData;

        fetch('data.json', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(conf),
        })
            .then(response => response.json())
            .then(data => {
                console.log('Success:', data);
            })
            .catch((error) => {
                console.error('Error:', error);
                alert("Failed to save settings!");
            });
    }
}

function loadConfig() {
    fetch('/data.json')
        .then(function (response) {
            if (response.ok)
                return response.json();
            else
                throw new Error('Failed to load settings!');
        }).then(function (json) {
            return json;
        });
};

function getVersion() {
    fetch('version')
        .then(function (response) {
            if (response.ok)
                return response.text();
            else
                throw new Error('Failed to load version!');
        }).then(function (text) {
            return text;
        });
}

function getTime() {
    fetch('time')
        .then(function (response) {
            if (response.ok)
                return response.text();
            else
                throw new Error('Failed to load current time!');
        }).then(function (text) {
            return text;
        });
}
