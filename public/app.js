// Initialize CodeMirror editor
let editor = CodeMirror.fromTextArea(document.getElementById('editor'), {
    mode: 'javascript',
    theme: 'monokai',
    lineNumbers: true,
    autoCloseBrackets: true,
    matchBrackets: true,
    indentUnit: 4,
    tabSize: 4,
    lineWrapping: true,
    extraKeys: {
        'Ctrl-S': function(cm) {
            saveFile();
        },
        'Cmd-S': function(cm) {
            saveFile();
        }
    }
});

// Language selector
const languageSelect = document.getElementById('language');
languageSelect.addEventListener('change', (e) => {
    editor.setOption('mode', e.target.value);
    updateStatus(`Language changed to ${e.target.value}`);
});

// File operations
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
            // Set language based on file extension
            const extension = file.name.split('.').pop();
            const languageMap = {
                'js': 'javascript',
                'py': 'python',
                'html': 'xml',
                'css': 'css'
            };
            if (languageMap[extension]) {
                languageSelect.value = languageMap[extension];
                editor.setOption('mode', languageMap[extension]);
            }
            updateStatus(`Opened ${file.name}`);
        };
        reader.readAsText(file);
    };
    input.click();
});

document.getElementById('saveFile').addEventListener('click', saveFile);

// Helper functions
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

function getFileExtension() {
    const mode = editor.getOption('mode');
    const extensionMap = {
        'javascript': 'js',
        'python': 'py',
        'xml': 'html',
        'css': 'css'
    };
    return extensionMap[mode] || 'txt';
}

function updateStatus(message) {
    document.getElementById('status').textContent = message;
    setTimeout(() => {
        document.getElementById('status').textContent = 'Ready';
    }, 3000);
}

// Update cursor position
editor.on('cursorActivity', () => {
    const pos = editor.getCursor();
    document.getElementById('cursor-position').textContent = 
        `Line: ${pos.line + 1}, Column: ${pos.ch + 1}`;
});

// Set initial content
editor.setValue(`// Welcome to SnSupear Editor
// Start coding here...`);