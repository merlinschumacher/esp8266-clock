function switchTab(el) {
    const target = el.getAttribute("href").substring(1);
    const contentBlocks = document.querySelectorAll(".contenttab");
    const contentTabs = document.querySelectorAll("li.tab-item");
    contentTabs.forEach(tab => {
        if (tab.id.includes(target)) {
            tab.classList.add("active");
        } else {
            tab.classList.remove("active");
        }
    });

    contentBlocks.forEach(block => {
        if (block.id == target) {
            block.style.display = 'unset';
        } else {
            block.style.display = 'none';
        }
    });
}

function initTabs() {
    document.getElementById("colorconfig").style.display = "unset";
    const tabButtons = document.querySelectorAll(".tab-item > a");
    tabButtons.forEach(button => {
        button.addEventListener('click', function (b) {
            switchTab(b.target); return false;
        }, false);
    });
}
