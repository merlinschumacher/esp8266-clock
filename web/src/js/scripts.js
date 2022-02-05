let config = {}
let time = 0;
let version = 0;
let toastVisible = false;
let app;
var resetRequired = false;
var configInterval = null;

const hourhandstyles = ["simple", "wide", "split"]

document.addEventListener("DOMContentLoaded", function () {

    initTabs();

    tinybind.formatters.sub = function (target, val) {
        return (target - val);
    };

    tinybind.formatters.power = function (ledCount, bgLedCount, bgOn) {
        if (!bgOn) {
            bgLedCount = 0;
        }
        let mAh = (ledCount + bgLedCount) * 50;
        return mAh;

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

    tinybind.formatters.time = {
        read: function (value) {
            let h = Math.floor(value / 60);
            h = String(h).padStart(2, '0');
            let m = value % 60;
            m = String(m).padStart(2, '0');
            return h + ":" + m;
        },
        publish: function (value) {
            const t = value.split(':');
            return (+t[0]) * 60 + (+t[1]);
        }
    }

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
        }
    };

    getData('time').then(function (t) {
        time = t;
        getData('version').then(function (d) {
            version = d.toString();
            getConfig().then(function (c) {
                config = c;
            }).then(function () {
                app = tinybind.bind(document.getElementById("app"), {
                    config, version, time, timezones, languages, toggleFirmwareModal, toggleResetModal, toggleWifiModal, loadLanguage, toastVisible, hourhandstyles
                });
            })
        })
    })

    initWatcher();
    let saveButton = document.getElementById("save-button");
    saveButton.addEventListener("click", function () {
        postConfig(true).then(c => { config = c });
    });

});

