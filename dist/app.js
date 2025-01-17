// Initialize CodeMirror
const editor = CodeMirror.fromTextArea(document.getElementById('editor'), {
  mode: 'python',
  theme: 'marina',
  lineNumbers: true,
  autoCloseBrackets: true,
  matchBrackets: true,
  indentUnit: 4,
  tabSize: 4,
  indentWithTabs: false,
  lineWrapping: true,
  extraKeys: {"Ctrl-Space": "autocomplete"},
  styleActiveLine: true,
  viewportMargin: Infinity,
  gutters: ["CodeMirror-linenumbers"],
  highlightSelectionMatches: {showToken: /\w/, annotateScrollbar: true}
});

// Set initial content
editor.setValue(`def example():
    print("Welcome to SnSupear Editor!")
    
# This is a comment
numbers = [1, 2, 3, 4, 5]
for num in numbers:
    if num % 2 == 0:
        print(f"{num} is even")
    else:
        print(f"{num} is odd")`);

// Language selector
const languageSelect = document.getElementById('language-select');
languageSelect.addEventListener('change', (e) => {
  const mode = e.target.value;
  editor.setOption('mode', mode);
});

// Button handlers
document.getElementById('new-file').addEventListener('click', () => {
  editor.setValue('');
});

document.getElementById('save-file').addEventListener('click', () => {
  const content = editor.getValue();
  const blob = new Blob([content], { type: 'text/plain' });
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = 'code.py';
  a.click();
  URL.revokeObjectURL(url);
});

document.getElementById('open-file').addEventListener('click', () => {
  const input = document.createElement('input');
  input.type = 'file';
  input.accept = '.py,.js,.cpp';
  input.onchange = (e) => {
    const file = e.target.files[0];
    const reader = new FileReader();
    reader.onload = (e) => {
      editor.setValue(e.target.result);
      // Auto-detect language mode
      if (file.name.endsWith('.py')) {
        editor.setOption('mode', 'python');
        languageSelect.value = 'python';
      } else if (file.name.endsWith('.js')) {
        editor.setOption('mode', 'javascript');
        languageSelect.value = 'javascript';
      } else if (file.name.endsWith('.cpp')) {
        editor.setOption('mode', 'text/x-c++src');
        languageSelect.value = 'cpp';
      }
    };
    reader.readAsText(file);
  };
  input.click();
});

// Set initial size
editor.setSize('100%', '100%');

// Autocomplete function
function autocomplete() {
  // Get the current cursor position and token
  const cursor = editor.getCursor();
  const token = editor.getTokenAt(cursor);

  // Get the current line and column
  const currentLine = editor.getCursor().line;
  const currentCh = editor.getCursor().ch;

  // Extract the code context
  const codeContext = editor.getValue(); 

  // Construct the API request URL with line and column information
  const apiUrl = `http://localhost:3000/completions?line=${currentLine}&column=${currentCh}`; 

  // Send the API request
  fetch(apiUrl, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      code: codeContext,
    }),
  })
    .then((response) => response.json())
    .then((data) => {
      // Extract suggestions from the API response
      const suggestions = data.suggestions || []; 

      // Show the suggestions in a completer
      if (suggestions.length > 0) {
        showCompleter(suggestions);
      }
    })
    .catch((error) => {
      console.error("Error fetching completions:", error);
      // Handle the error, e.g., show an error message to the user
    });
}

// Function to show the completer (example implementation)
function showCompleter(suggestions) {
  const completer = new CodeMirror.hint.auto(editor, () => ({
    list: suggestions,
    from: editor.getCursor(),
    to: editor.getCursor(),
  }));
  completer();
}
