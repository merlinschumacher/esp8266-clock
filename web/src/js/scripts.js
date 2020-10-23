
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
    tinybind.formatters.replaceUnderscore = function (value) {
        return value.replace(/_/g, ' ');
    };
    tinybind.formatters.formatDate = function (value) {
        console.log(value);
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

    let config = {};
    let time = 0;
    let version = "";
    let app = tinybind.bind(document.getElementById("app"), {
        config: config, version: version, time: time, timezones, languages, toggleFirmwareModal, toggleResetModal, loadLanguage
    });
    app.models.config = loadConfig();
    app.models.time = getTime();
    app.models.version = getVersion();

    window.setInterval(function () {
        app.models.time = getTime();
    }, 1000);

});
