// web/app.js

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
            // AI assistant integration shortcuts
            'Ctrl-I': () => getAISuggestions(),
            'Cmd-I': () => getAISuggestions()
        }
    });

    const languageSelect = document.getElementById('language-select');
    languageSelect.addEventListener('change', (e) => {
        const mode = e.target.value;
        editor.setOption('mode', mode);
    });

    // Set initial size of editor
    editor.setSize('100%', '100%');

    // Handle window resize
    window.addEventListener('resize', () => {
        editor.refresh();
    });

    // AI Assistant Integration
    async function getAISuggestions() {
        const selectedText = editor.getSelection() || editor.getValue();
        if (!selectedText) {
            updateStatus('No code selected for AI suggestions');
            return;
        }

        updateStatus('Fetching AI suggestions...');
        try {
            const response = await fetch('/api/ai/suggestions', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    code: selectedText,
                    language: editor.getOption('mode')
                })
            });

            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);

            const data = await response.json();
            if (data.suggestion) {
                if (editor.somethingSelected()) {
                    editor.replaceSelection(data.suggestion);
                } else {
                    const cursor = editor.getCursor();
                    editor.replaceRange(data.suggestion, cursor);
                }
                updateStatus('AI suggestion applied');
            } else {
                updateStatus('No suggestion received from AI');
            }
        } catch (error) {
            console.error('AI suggestion error:', error);
            updateStatus('Error getting AI suggestions');
        }
    }

    // Add AI suggestion button to the toolbar
    const aiButton = document.createElement('button');
    aiButton.id = 'aiSuggest';
    aiButton.textContent = 'AI Suggest';
    aiButton.className = 'toolbar-button';
    aiButton.onclick = getAISuggestions;
    document.querySelector('.toolbar').appendChild(aiButton);

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
                // Automatically set the language mode based on the file extension
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
        updateStatus('File saved successfully');
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
