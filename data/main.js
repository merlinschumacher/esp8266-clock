let configData = [];

function onChangedInput() {
    configData.hostname = document.getElementById("hostname").value;;
    configData.timeserver = document.getElementById("timeserver").value;
    configData.timezone = document.getElementById("timezone").value;
    configData.hourColor = document.getElementById("hourcolor").value;
    configData.minuteColor = document.getElementById("minutecolor").value;
    configData.secondColor = document.getElementById("secondcolor").value;
    configData.hourColorDimmed = document.getElementById("hourcolordimmed").value;
    configData.minuteColorDimmed = document.getElementById("minutecolordimmed").value;
    configData.secondColorDimmed = document.getElementById("secondcolordimmed").value;
    configData.ledPin = document.getElementById("ledpin").value;
    configData.ledCount = document.getElementById("ledcount").value;
    configData.ledRoot = document.getElementById("ledroot").value;
    console.log(configData);

    fetch('data.json', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(configData),
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


document.addEventListener('DOMContentLoaded', function (event) {

    let main = document.getElementById("main");
    document.getElementById("jswarn").remove();
    main.style.display = "block";
    fetch('data.json')
        .then(function (response) {
            if (response.ok)
                return response.json();
            else
                throw new Error('Failed to load settings!');
        })
        .then(function (json) {
            configData = json;
            console.log(json);
            getTimezones();
            document.title = "⏰" + configData.hostname;
            document.getElementById("title").innerText = "⏰" + configData.hostname;
            document.getElementById("hostname").value = configData.hostname;
            document.getElementById("timeserver").value = configData.timeserver;
            document.getElementById("hourcolor").value = configData.hourColor;
            document.getElementById("minutecolor").value = configData.minuteColor;
            document.getElementById("secondcolor").value = configData.secondColor;
            document.getElementById("hourcolordimmed").value = configData.hourColorDimmed;
            document.getElementById("minutecolordimmed").value = configData.minuteColorDimmed;
            document.getElementById("secondcolordimmed").value = configData.secondColorDimmed;
            document.getElementById("ledpin").value = configData.ledPin;
            document.getElementById("ledcount").value = configData.ledPin;
            document.getElementById("ledroot").value = configData.ledRoot;


            let configInputs = document.querySelectorAll("#configform input, #configform select");
            configInputs.forEach(element => {
                element.addEventListener("change", onChangedInput);
            });

        })
        .catch(function (err) {
            alert("Failed to load settings!");
            console.log(err);
        });

    function getTimezones() {
        fetch('timezones.json')
            .then(function (response) {
                if (response.ok)
                    return response.json();
                else
                    throw new Error('Failed to load timezones!');
            })
            .then(function (json) {

                let select = document.getElementById("timezone");
                for (var i = 0; i < json.length; i++) {
                    let selected = false;
                    if (json[i] == configData.timezone) {
                        console.log("selected");
                        console.log(json[i]);
                        console.log(configData.timezone);
                        selected = true;
                    }
                    select.options[i] = new Option(json[i], json[i], false, selected);
                }

            })
            .catch(function (err) {
                alert("Failed to load timezones!");
                console.log(err);
            });
    }


});