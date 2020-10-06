function initFirmwareModal() {
    let resetButtons = document.getElementsByClassName("modal-firmware-toggle");
    let resetModal = document.getElementById("modalfirmware");
    for (let button of resetButtons) {
        button.addEventListener('click', function () {
            resetModal.classList.toggle('active');
            return false;
        }, false);
    }
}