// /public/app.js

import CodeMirror from 'codemirror';
import 'codemirror/mode/javascript/javascript.js';
import 'codemirror/mode/python/python.js';
import 'codemirror/mode/xml/xml.js';
import 'codemirror/mode/css/css.js';

/**
 * @file /public/app.js
 * Initializes the CodeMirror editor and handles file operations, language selection, and GPT chat features.
 */

// Initialize CodeMirror editor with SnSupear custom theme
const editor = CodeMirror.fromTextArea(document.getElementById('editor'), {
    mode: 'javascript', // Default mode
    theme: 'monokai', // Use theme
    lineNumbers: true,
    autoCloseBrackets: true,
    matchBrackets: true,
    indentUnit: 4,
    tabSize: 4,
    lineWrapping: true,
    extraKeys: {
        'Ctrl-S': function (cm) { saveFile(); },
        'Cmd-S': function (cm) { saveFile(); }
    }
});

// Language selection event
const languageSelect = document.getElementById('language-select');
languageSelect.addEventListener('change', (e) => {
    const mode = e.target.value;
    editor.setOption('mode', mode);
    updateStatus(`Language mode changed to ${mode}`);
});

/**
 * Handles creation of a new file, with a prompt to confirm.
 */
document.getElementById('newFile').addEventListener('click', () => {
    if (confirm('Create new file? Unsaved changes will be lost.')) {
        editor.setValue('');
        updateStatus('New file created');
    }
});

/**
 * Opens a file and loads its content into the editor.
 * Automatically sets the language mode based on file extension.
 */
document.getElementById('openFile').addEventListener('click', () => {
    const input = document.createElement('input');
    input.type = 'file';
    input.accept = '.js,.py,.html,.css,.txt';
    input.onchange = (e) => {
        const file = e.target.files[0];
        const reader = new FileReader();
        reader.onload = (e) => {
            editor.setValue(e.target.result);
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

/**
 * Saves the content of the editor to a file.
 */
document.getElementById('saveFile').addEventListener('click', saveFile);

/**
 * @function saveFile
 * Saves the current editor content to a file, using the appropriate file extension.
 */
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

/**
 * @function getFileExtension
 * Determines the appropriate file extension based on the current mode.
 * @returns {string} - The file extension
 */
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

/**
 * @function updateStatus
 * Updates the status message in the status bar.
 * @param {string} message - The message to display
 */
function updateStatus(message) {
    const statusElement = document.getElementById('status');
    statusElement.textContent = message;
    setTimeout(() => {
        statusElement.textContent = 'Ready';
    }, 3000);
}

// Update cursor position in the status bar
editor.on('cursorActivity', () => {
    const pos = editor.getCursor();
    document.getElementById('status').textContent = 
        `Line: ${pos.line + 1}, Column: ${pos.ch + 1}`;
});

// Set initial content with a welcome message
editor.setValue(`// Welcome to SnSupear Web Editor\n// Start coding here...`);
