
// init all elements
document.addEventListener("DOMContentLoaded", function (event) {
    // const pickr = Pickr.create({
    //     el: '.color-picker',
    //     theme: 'nano',
    //     comparison: false,
    //     components: {
    //         preview: true,
    //         hue: true,
    //         lockOpacity: true,
    //         interaction: {
    //             input: true,
    //             clear: false,
    //             save: false
    //         }
    //     }
    // });

    initTabs();
    initCheckboxAlarmEnable();
    document.getElementById("alarmactive").addEventListener('click', initCheckboxAlarmEnable, false);
    initCheckboxBacklightEnable();
    document.getElementById("bglight").addEventListener('click', initCheckboxBacklightEnable, false);

});