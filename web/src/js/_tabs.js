function switchTab(element) {
    const target = element.getAttribute("href").substring(1);
    const contentBlocks = document.querySelectorAll(".contenttab");
    const contentTabs = document.querySelectorAll("li.tab-item");
    contentTabs.forEach(element => {
        if (element.id.includes(target)) {
            element.classList.add("active");
        } else {
            element.classList.remove("active");
        }
    });

    contentBlocks.forEach(element => {
        if (element.id == target) {
            element.style.display = 'unset';
        } else {
            element.style.display = 'none';
        }
    });
};

function initTabs() {
    document.getElementById("timeconfig").style.display = "unset";
    const tabButtons = document.querySelectorAll(".tab-item > a");
    tabButtons.forEach(element => {
        element.addEventListener('click', function (element) { switchTab(element.target) }, false);
    });
};
