const pickr = Pickr.create({
    el: '.color-picker',
    theme: 'nano',
    comparison: false,
    components: {
        preview: true,
        hue: true,
        lockOpacity: true,
        interaction: {
            input: true,
            clear: false,
            save: false
        }
    }
});
