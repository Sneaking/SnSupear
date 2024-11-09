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
            'Ctrl-Space': 'autocomplete',
            'Ctrl-S': () => saveFile(),
            'Cmd-S': () => saveFile(),
            'Ctrl-P': () => printFile(),
            'Cmd-P': () => printFile(),
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

    // File Operations
    document.getElementById('newFile').addEventListener('click', () => {
        if (confirm('Create new file? Unsaved changes will be lost.')) {
            editor.setValue('');
            updateStatus('New file created');
        }
    });

    document.getElementById('openFile').addEventListener('click', () => {
        const input = document.createElement('input');
        input.type = 'file';
        input.accept = '.js,.py,.html,.css,.txt';
        input.onchange = (e) => {
            const file = e.target.files[0];
            const reader = new FileReader();
            reader.onload = (e) => {
                editor.setValue(e.target.result);
                updateStatus(`Opened ${file.name}`);
                // Set mode based on file extension
                const extension = file.name.split('.').pop();
                const languageMap = {
                    'js': 'javascript',
                    'py': 'python',
                    'html': 'htmlmixed',
                    'css': 'css'
                };
                if (languageMap[extension]) {
                    languageSelect.value = languageMap[extension];
                    editor.setOption('mode', languageMap[extension]);
                }
            };
            reader.readAsText(file);
        };
        input.click();
    });

    function saveFile() {
        const content = editor.getValue();
        const blob = new Blob([content], { type: 'text/plain' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = `code.${getFileExtension()}`;
        a.click();
        URL.revokeObjectURL(url);
        updateStatus('File saved');
    }

    function printFile() {
        const printWindow = window.open('', '_blank');
        printWindow.document.write(`
            <html>
                <head>
                    <title>Print File</title>
                    <style>
                        body { font-family: monospace; white-space: pre; margin: 20px; }
                    </style>
                </head>
                <body>${editor.getValue().replace(/</g, '&lt;').replace(/>/g, '&gt;')}</body>
            </html>
        `);
        printWindow.document.close();
        printWindow.print();
        updateStatus('File sent to printer');
    }

    function getFileExtension() {
        const mode = editor.getOption('mode');
        const extensionMap = {
            'javascript': 'js',
            'python': 'py',
            'htmlmixed': 'html',
            'css': 'css'
        };
        return extensionMap[mode] || 'txt';
    }

    function updateStatus(message) {
        const statusElement = document.getElementById('status');
        statusElement.textContent = message;
        setTimeout(() => {
            statusElement.textContent = 'Ready';
        }, 3000);
    }
});
