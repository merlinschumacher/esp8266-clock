function initWatcher() {
    let configInputs = document.querySelectorAll("#configform input, #configform select");
    configInputs.forEach(element => {
        element.addEventListener("change", function () {
            if (this.classList.contains("resetneeded")) {
                resetRequired = true;
            };
            let saveButton = document.getElementById("save-button");
            saveButton.classList.add('badge');
            clearInterval(configInterval);
            configInterval = window.setInterval(function () {
                postConfig(false);
                clearInterval(configInterval);
            }, 500);
        });
    });
}