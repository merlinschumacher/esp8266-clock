let jsonData = [];

document.addEventListener('DOMContentLoaded', function (event) {
    console.log(event);
    let xmlhttp = new XMLHttpRequest();
    let url = "/data.json";
    xmlhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            jsonData = JSON.parse(this.responseText);
        }
    };

    xmlhttp.open("GET", url, true);
    xmlhttp.send();

    console.log(jsonData);

    let main = this.getElementById("main");
    main.innerHTML = '';
    document.title = "ESP UHR";
    let title = this.createElement("h1");
    title.textContent = "ESP UHR";
    main.appendChild(title);
    let pre = this.createElement("pre");
    pre.textContent = jsonData;
    let code = this.createElement("code");
    code.appendChild(pre);
    main.appendChild(code);

});