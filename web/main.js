let configData = [];

function showBgLightOptions() {
    let optionBox = document.getElementById("bglightfield");
    let checkbox = document.getElementById("bglight");
    if (checkbox.checked) {
        optionBox.style.display = "block";
    } else {
        optionBox.style.display = "none";
    }
}
function showDayMonthOptions() {
    let optionBoxes = document.getElementsByClassName("daymonthconfig");
    let checkbox = document.getElementById("daymonth");
    if (checkbox.checked) {
        Array.from(optionBoxes).forEach((el) => { el.style.display = "block"; });
    } else {
        Array.from(optionBoxes).forEach((el) => { el.style.display = "none"; });
    }
}

function onChangedInput(saveData = false) {
    if (document.getElementById("configform").checkValidity()) {
        console.log(configData.saveData);
        configData.hostname = document.getElementById("hostname").value;
        configData.timeserver = document.getElementById("timeserver").value;
        configData.timezone = document.getElementById("timezone").value;

        configData.hourColor = document.getElementById("hourcolor").value;
        configData.minuteColor = document.getElementById("minutecolor").value;
        configData.secondColor = document.getElementById("secondcolor").value;

        configData.hourColorDimmed = document.getElementById("hourcolordimmed").value;
        configData.minuteColorDimmed = document.getElementById("minutecolordimmed").value;
        configData.secondColorDimmed = document.getElementById("secondcolordimmed").value;

        configData.hourDotColor = document.getElementById("hourdotcolor").value;
        configData.hourSegmentColor = document.getElementById("hoursegmentcolor").value;
        configData.hourQuarterColor = document.getElementById("hourquartercolor").value;

        configData.hourDotColorDimmed = document.getElementById("hourdotcolordimmed").value;
        configData.hourSegmentColorDimmed = document.getElementById("hoursegmentcolordimmed").value;
        configData.hourQuarterColorDimmed = document.getElementById("hourquartercolordimmed").value;

        configData.monthColor = document.getElementById("monthcolor").value;
        configData.dayColor = document.getElementById("daycolor").value;
        configData.weekdayColorDimmed = document.getElementById("weekdaycolor").value;
        configData.monthColorDimmed = document.getElementById("monthcolordimmed").value;
        configData.dayColorDimmed = document.getElementById("daycolordimmed").value;
        configData.weekdayColor = document.getElementById("weekdaycolordimmed").value;
        configData.dayOffset = parseInt(document.getElementById("dayoffset").value);
        configData.monthOffset = parseInt(document.getElementById("monthoffset").value);
        configData.weekdayOffset = parseInt(document.getElementById("weekdayoffset").value);

        configData.nightTimeBegins = document.getElementById("nighttimebegins").value;
        configData.nightTimeEnds = document.getElementById("nighttimeends").value;

        configData.hourLight = document.getElementById("hourlight").checked;
        configData.blendColors = document.getElementById("blendcolors").checked;
        configData.fluidMotion = document.getElementById("fluidmotion").checked;

        configData.ledPin = parseInt(document.getElementById("ledpin").value);
        configData.ledCount = parseInt(document.getElementById("ledcount").value);
        configData.ledRoot = parseInt(document.getElementById("ledroot").value);

        configData.alarmActive = document.getElementById("alarmactive").checked;
        configData.alarmTime = document.getElementById("alarmtime").value;

        configData.bgLight = document.getElementById("bglight").checked;
        configData.bgColor = document.getElementById("bgcolor").value;
        configData.bgColorDimmed = document.getElementById("bgcolordimmed").value;
        configData.bgLedPin = parseInt(document.getElementById("bgledpin").value);
        configData.bgLedCount = parseInt(document.getElementById("bgledcount").value);

        configData.saveData = saveData;

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
                console.log('Error:', error);
                alert("Failed to save settings!");
            });
    }
}


document.addEventListener('DOMContentLoaded', function (event) {

    let main = document.getElementById("main");
    let currentTime = document.getElementById("currenttime");
    let versionSpan = document.getElementById("version");

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

            document.getElementById("hourdotcolor").value = configData.hourDotColor;
            document.getElementById("hourquartercolor").value = configData.hourQuarterColor;
            document.getElementById("hoursegmentcolor").value = configData.hourSegmentColor;

            document.getElementById("hourdotcolordimmed").value = configData.hourDotColorDimmed;
            document.getElementById("hourquartercolordimmed").value = configData.hourQuarterColorDimmed;
            document.getElementById("hoursegmentcolordimmed").value = configData.hourSegmentColorDimmed;

            document.getElementById("monthcolor").value = configData.monthColor;
            document.getElementById("daycolor").value = configData.hourDayColor;
            document.getElementById("weekdaycolor").value = configData.weekdayColor;
            document.getElementById("monthcolordimmed").value = configData.monthColorDimmed;
            document.getElementById("daycolordimmed").value = configData.hourDayColorDimmed;
            document.getElementById("weekdaycolordimmed").value = configData.weekdayColorDimmed;
            document.getElementById("monthoffset").value = configData.monthOffset;
            document.getElementById("dayoffset").value = configData.dayOffset;
            document.getElementById("weekdayoffset").value = configData.weekdayOffset;

            document.getElementById("nighttimebegins").value = configData.nightTimeBegins;
            document.getElementById("nighttimeends").value = configData.nightTimeEnds;

            document.getElementById("hourlight").checked = configData.hourLight;
            document.getElementById("blendcolors").checked = configData.blendColors;
            document.getElementById("fluidmotion").checked = configData.fluidMotion;

            document.getElementById("ledpin").value = configData.ledPin;
            document.getElementById("ledcount").value = configData.ledCount;
            document.getElementById("ledroot").value = configData.ledRoot;
            document.getElementById("dayoffset").value = configData.dayOffset;
            document.getElementById("monthoffset").value = configData.monthOffset;
            document.getElementById("weekdayoffset").value = configData.weekdayOffset;

            document.getElementById("alarmactive").checked = configData.alarmActive;
            document.getElementById("alarmtime").value = configData.alarmTime;

            document.getElementById("bglight").checked = configData.bgLight;
            document.getElementById("bgcolor").value = configData.bgColor;
            document.getElementById("bgcolordimmed").value = configData.bgColorDimmed;
            document.getElementById("bgledpin").value = configData.bgLedPin;
            document.getElementById("bgledcount").value = configData.bgLedCount;

            let configInputs = document.querySelectorAll("#configform input, #configform select");
            configInputs.forEach(element => {
                element.addEventListener("change", function () { onChangedInput(false); });
            });
            document.getElementById("submit").addEventListener("click", function () { onChangedInput(true); return false; });

            document.getElementById("reset").addEventListener("click", function () {

                fetch('reset')
                    .then(function (response) {
                        if (response.ok)
                            return response.text();
                        else
                            throw new Error('Failed to trigger reset!');
                    })
                    .then(function (res) {
                        location.reload();
                    })
                    .catch(function (err) {
                        alert('Resetting has failed! Try to manually reload the page.');
                        console.log(err);
                    });
            });
            document.getElementById("bglight").addEventListener("click", function () { showBgLightOptions(); return false; });
            document.getElementById("daymonth").addEventListener("click", function () { (showDayMonthOptions()); return false; });
            showBgLightOptions();
            showDayMonthOptions();
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

    fetch('time')
        .then(function (response) {
            if (response.ok)
                return response.text();
            else
                throw new Error('Failed to load current time!');
        })
        .then(function (time) {
            currentTime.innerText = time;
        })
        .catch(function (err) {
            console.log(err);
        });

    fetch('version')
        .then(function (response) {
            if (response.ok)
                return response.text();
            else
                throw new Error('Failed to load version!');
        })
        .then(function (version) {
            versionSpan.innerText = version;
        })
        .catch(function (err) {
            console.log(err);
        });
});
