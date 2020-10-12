function initResetModal() {
    let resetButtons = document.getElementsByClassName("modal-reset-toggle");
    let resetModal = document.getElementById("modalreset");
    for (let button of resetButtons) {
        button.addEventListener('click', function () {
            resetModal.classList.toggle('active');
            return false;
        }, false);
    }
};