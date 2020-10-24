function showToast(typeClass, messageIndex, fadeOut = false) {
    app.models.toastVisible = true;
    toast = document.getElementById('header-toast');
    document.getElementById('header-toast').style.opacity = '1'
    document.getElementById('header-toast').style.display = '';
    const classes = toast.className.split(" ").filter(c => !c.startsWith("toast-"));
    toast.className = classes.join(" ").trim();
    spans = toast.children;
    for (let i = 0; i < spans.length; i++) {
        if (i == messageIndex) {
            spans[i].classList.remove('d-none');
            toast.classList.add(typeClass);
        } else {
            spans[i].classList.add('d-none');
        }
    };
    if (fadeOut) {
        setTimeout(function () {
            document.getElementById('header-toast').style.opacity = '0';
        }, 3000);
        setTimeout(function () {
            document.getElementById('header-toast').style.display = 'none';
        }, 4000);
    };

}

