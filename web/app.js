document.addEventListener('DOMContentLoaded', () => {
    const editor = CodeMirror.fromTextArea(document.getElementById('editor'), {
        mode: 'javascript',
        theme: 'marina',
        lineNumbers: true,
        autoCloseBrackets: true,
        matchBrackets: true,
        indentUnit: 4,
        tabSize: 4,
        indentWithTabs: false,
        extraKeys: {
            'Ctrl-Space': 'autocomplete'
        }
    });

    const languageSelect = document.getElementById('language-select');
    languageSelect.addEventListener('change', (e) => {
        const mode = e.target.value;
        editor.setOption('mode', mode);
    });

    // Set initial size
    editor.setSize('100%', '100%');

    // Handle window resize
    window.addEventListener('resize', () => {
        editor.refresh();
    });
});