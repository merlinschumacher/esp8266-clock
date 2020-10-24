function initWatcher() {
    let configInputs = document.querySelectorAll("#configform input, #configform select");
    configInputs.forEach(element => {
        element.addEventListener("change", function () {
            let saveButton = document.getElementById("save-button");
            saveButton.classList.add('badge');
            postConfig(false);
        });
    });
}