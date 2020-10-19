
const toggleResetModal = function () {
    let modal = document.getElementById("modalreset");
    modal.classList.toggle('active');
    return false;
}
const toggleFirmwareModal = function () {
    let modal = document.getElementById("modalfirmware");
    modal.classList.toggle('active');
    return false;
}

document.addEventListener("DOMContentLoaded", function (event) {
    initTabs();
    tinybind.formatters.sub = function (target, val) {
        return (target - val);
    };
    tinybind.binders.barwidth = function (el, value) {
        let width = value / config.ledCount * 100;
        el.style.width = width + "%";
    }
    tinybind.binders.barpos = function (el, value) {
        let margin = (value - 1) / config.ledCount * 100;
        el.style.marginLeft = margin + "%";
    };
    tinybind.formatters.addEmoji = function (value, emoji) {
        return emoji + value;
    };
    tinybind.formatters.formatDate = function (value) {
        let date = new Date(value * 1000);
        return date.toLocaleString();
    };
    tinybind.binders.baroverflow = function (el) {
        if (el.scrollHeight > el.clientHeight || el.scrollWidth > el.clientWidth) {
            console.log("overflow");
            // el.style.boxShadow = "-8px 0 4px -3px red inset";
        } else {

            console.log("no overflow");
            // el.style.boxShadow = "";
        };
    };
    tinybind.bind(document.getElementById('app'), { config, timezones, date, toggleFirmwareModal, toggleResetModal });
});
