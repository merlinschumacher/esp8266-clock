let config = {}
let time = 0;
let version = 0;
let toastVisible = false;
let app;
document.addEventListener("DOMContentLoaded", function () {

    initTabs();

    tinybind.formatters.sub = function (target, val) {
        return (target - val);
    };

    tinybind.binders.barwidth = function (el, value) {
        let width = value / config.ledCount * 100;
        el.style.width = width + "%";
    }

    tinybind.binders.height = function (el, value) {
        if (value) {
            el.style.height = "2.666rem";
        } else {
            el.style.height = "0.666rem";
        }
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
    tinybind.formatters.int = {
        read: function (value) {
            return value
        },
        publish: function (value) {
            return parseInt(value)
        }
    };
    tinybind.binders.baroverflow = function (el) {
        let indicator = document.getElementById("overflow-indicator");
        if (el.scrollWidth > el.clientWidth) {
            indicator.style.boxShadow = "-8px 0 4px -3px red inset";
        } else {
            indicator.style.boxShadow = "";
        };
    };

    getData('time').then(function (data) {
        time = data;
    })
    getData('version').then(function (data) {
        console.log(version);
        version = data.toString();
    })

    getConfig().then(function (data) {
        config = data;
    }).then(function () {
        app = tinybind.bind(document.getElementById("app"), {
            config, version, time, timezones, languages, toggleFirmwareModal, toggleResetModal, loadLanguage, toastVisible
        });
    })

    window.setInterval(function () {
        getData('time').then(function (data) {
            app.models.time = data;
        });
    }, 1000);
    initWatcher();
});

