// /public/app.js
// Initialize CodeMirror editor with enhanced error handling
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
        },
        'Ctrl-Space': function(cm) {
            toggleGPTChat();
        }
    }
});

// Language selector with error handling
const languageSelect = document.getElementById('language');
languageSelect.addEventListener('change', (e) => {
    editor.setOption('mode', e.target.value);
    updateStatus(`Language changed to ${e.target.value}`);
});

// GPT Chat Integration
const gptChat = document.getElementById('gpt-chat');
const chatLog = document.getElementById('chat-log');
const chatInput = document.getElementById('chat-input');
const sendMessageButton = document.getElementById('send-message');

// Toggle GPT Chat
document.getElementById('gptChatToggle').addEventListener('click', toggleGPTChat);

function toggleGPTChat() {
    gptChat.classList.toggle('hidden');
}

// Send message to GPT
sendMessageButton.addEventListener('click', () => {
    const message = chatInput.value.trim();
    if (message) {
        addChatMessage('You', message);
        chatInput.value = '';
        // Call the GPT API
        callGPTAPI(message);
    }
});

function addChatMessage(sender, message) {
    const messageElement = document.createElement('div');
    messageElement.classList.add('chat-message');
    messageElement.innerHTML = `<strong>${sender}:</strong> ${message}`;
    chatLog.appendChild(messageElement);
    chatLog.scrollTop = chatLog.scrollHeight;
}

// Call GPT API
async function callGPTAPI(message) {
    try {
        const response = await fetch('/gpt-api', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ prompt: message })
        });
        const data = await response.json();
        if (data && data.reply) {
            addChatMessage('GPT', data.reply);
        } else {
            addChatMessage('Error', 'No reply from GPT.');
        }
    } catch (error) {
        console.error('Error calling GPT API:', error);
        addChatMessage('Error', 'Failed to communicate with GPT.');
    }
}

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

editor.on('cursorActivity', () => {
    const pos = editor.getCursor();
    document.getElementById('cursor-position').textContent = 
        `Line: ${pos.line + 1}, Column: ${pos.ch + 1}`;
});

editor.setValue(`// Welcome to SnSupear Editor - Test03
// Start coding here...`);
