
function initCheckboxBacklightEnable() {
    const alarmActive = document.getElementById("bglight");
    const alarmTime = document.getElementsByClassName("bglightsettings");
    if (alarmActive.checked) {
        Array.from(alarmTime).forEach((element) => {
            element.style.display = "unset";
        });
    } else {
        Array.from(alarmTime).forEach((element) => {
            element.style.display = "none";
        });
    }
}