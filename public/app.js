<!-- /public/index.html -->
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SnSupear Web Editor</title>
    <!-- Load local and external stylesheets -->
    <link rel="stylesheet" href="/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.65.2/codemirror.min.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.65.2/theme/dracula.min.css"> <!-- Optional: Add a theme -->
</head>
<body>
    <div class="container">
        <header>
            <h1>SnSupear Web Editor</h1>
            <div class="toolbar">
                <button id="newFile">New File</button>
                <button id="openFile">Open File</button>
                <button id="saveFile">Save File</button>
                <select id="language-select">
                    <option value="javascript">JavaScript</option>
                    <option value="python">Python</option>
                    <option value="htmlmixed">HTML/XML</option>
                    <option value="css">CSS</option>
                </select>
            </div>
        </header>
        <main>
            <div id="editor-container">
                <textarea id="editor"></textarea>
            </div>
            <div class="status-bar">
                <span id="status">Ready</span>
                <span id="cursor-position">Line: 1, Column: 1</span>
            </div>
            <div id="chat-container">
                <div id="chat-output"></div>
                <input type="text" id="chat-input" placeholder="Ask GPT...">
                <button id="chat-submit">Send</button>
            </div>
        </main>
    </div>
    <!-- Load CodeMirror and initialize the editor -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.65.2/codemirror.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.65.2/mode/javascript/javascript.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.65.2/mode/python/python.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.65.2/mode/xml/xml.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.65.2/mode/css/css.min.js"></script>
    <script src="app.js"></script> <!-- Load your app script -->
</body>
</html>
