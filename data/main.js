let jsonData = [];

document.addEventListener('DOMContentLoaded', function (event) {

    let main = document.getElementById("main");
    document.getElementById("jswarn").remove();
    main.style.display = "block";
    fetch('data.json')
        .then(function (response) {
            if (response.ok)
                return response.json();
            else
                throw new Error('Failed to load settings!');
        })
        .then(function (json) {
            let pre = document.createElement("pre");
            pre.textContent = JSON.stringify(json);
            let code = document.createElement("code");

            code.appendChild(pre);
            main.appendChild(code);
        })
        .catch(function (err) {
            alert("Failed to load settings!");
            console.log(err);
        });

    main.innerHTML = '';
    document.title = "ESP8266 LED Clock";

    let title = document.createElement("h1");
    title.textContent = "ESP8266 LED Clock";
    main.appendChild(title);


});