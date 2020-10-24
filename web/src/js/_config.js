
async function postConfig(saveData = false) {
    if (document.getElementById("configform").checkValidity()) {
        conf = app.models.config;
        conf.saveData = saveData;
    } else {
        return false;
    }
    document.getElementById('save-button').classList.add("loading");
    return await fetch('data.json', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(conf),
    })
        .then(function (response) {
            document.getElementById('save-button').classList.remove("loading");
            return response.json();
        })
        .then(data => {
            app.models.config = data;
            showToast('toast-success', 2, true)
        })
        .catch((error) => {
            console.error('Error:', error);
            showToast('toast-error', 1)
            throw new Error('Failed to save settings!');
        });
};

async function getConfig() {
    return await fetch('/data.json')
        .then(function (response) {
            if (response.ok) {
                return response.json();
            }
            else {
                throw new Error('Failed to load settings!');
            }
        }).then(function (json) {
            return json;
        });
};

async function getData(url) {
    return await fetch(url)
        .then(function (response) {
            if (response.ok) {
                return response.text();
            }
            else {
                throw new Error('Failed to load ' + url + ' !');
            }
        }).then(function (text) {
            return text;
        });
};
