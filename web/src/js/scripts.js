document.addEventListener("DOMContentLoaded", function (event) {
    initTabs();
    initCheckboxAlarmEnable();
    document.getElementById("alarmactive").addEventListener('click', initCheckboxAlarmEnable, false);
    initCheckboxBacklightEnable();
    document.getElementById("bglight").addEventListener('click', initCheckboxBacklightEnable, false);
    initLedSliders();
    initResetModal();
    initFirmwareModal();
    tinybind.bind(document.getElementById('app'), { config: config });
});

