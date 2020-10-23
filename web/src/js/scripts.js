
let config = {}
let time = 0;
let version = 0;
let app;
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
        if (value != 0) {
            let date = new Date(value * 1000);
            return date.toLocaleString();
        } else {
            return "";
        }
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
    getTime().then(function (data) {
        time = data;
    })
    getVersion().then(function (data) {
        version = data;
    })

    loadConfig().then(function (data) {
        config = data;
    }).then(function () {
        app = tinybind.bind(document.getElementById("app"), {
            config: config, version: version, time: time, timezones, languages, toggleFirmwareModal, toggleResetModal, loadLanguage
        });
    })

    window.setInterval(function () {
        getTime().then(function (data) {
            app.models.time = data;
        });
    }, 1000);

});

