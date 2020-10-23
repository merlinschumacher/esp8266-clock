const languages = [
    {
        code: "en",
        name: "English",
        icon: "🇬🇧"
    },
    {
        code: "de",
        name: "Deutsch",
        icon: "🇩🇪"
    }
]

const loadLanguage = function () {
    saveConfig();
    location.reload();
}
