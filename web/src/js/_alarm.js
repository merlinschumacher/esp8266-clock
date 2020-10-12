
function initCheckboxAlarmEnable() {
    const alarmActive = document.getElementById("alarmactive");
    const alarmTime = document.getElementsByClassName("alarmtime");
    if (alarmActive.checked) {
        Array.from(alarmTime).forEach((element) => {
            element.style.display = "unset";
        });
    } else {
        Array.from(alarmTime).forEach((element) => {
            element.style.display = "none";
        });
    };
};