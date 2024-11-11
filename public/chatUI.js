//public/chatUI.js
document.getElementById('chat-submit').addEventListener('click', async () => {
    const message = document.getElementById('chat-input').value;
    if (!message) {
        console.warn("Chat input is empty. Please enter a message.");
        return;
    }

    try {
        console.log("Sending message to GPT API:", message);
        const response = await fetch('/api/chat', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ message })
        });

        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        const data = await response.json();
        if (!data || !data.response) {
            throw new Error("Invalid response structure from GPT API.");
        }

        const chatOutput = document.getElementById('chat-output');
        chatOutput.innerHTML += `<div class="chat-message">${data.response}</div>`;
        console.log("Received chat response:", data.response);
    } catch (error) {
        console.error("Failed to fetch chat response:", error);
        alert("Error: Unable to get a response from the server. Please try again later.");
    }
});
document.getElementById('chat-submit').addEventListener('click', () => {
    const input = document.getElementById('chat-input').value;
    if (input.trim()) {
        // Clear input, show loading indicator, and disable the submit button
        document.getElementById('chat-input').value = '';
        document.getElementById('chat-submit').disabled = true;
        appendMessage('You', input);
        showLoadingIndicator();

        // Send to GPT API
        fetch('YOUR_GPT_API_ENDPOINT', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ prompt: input })
        })
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            return response.json();
        })
        .then(data => {
            if (data && data.answer) {
                appendMessage('GPT', data.answer);
            } else {
                appendMessage('GPT', 'Sorry, no response received.');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            appendMessage('GPT', 'Sorry, there was an error processing your request.');
        })
        .finally(() => {
            hideLoadingIndicator();
            document.getElementById('chat-submit').disabled = false;
        });
    } else {
        alert('Please enter a message.');
    }
});

function appendMessage(sender, message) {
    const chatOutput = document.getElementById('chat-output');
    const messageElement = document.createElement('div');
    messageElement.className = 'chat-message';
    messageElement.innerHTML = `<strong>${sender}:</strong> ${message}`;
    chatOutput.appendChild(messageElement);
    chatOutput.scrollTop = chatOutput.scrollHeight; // Scroll to bottom
}

function showLoadingIndicator() {
    // Implement your loading indicator display logic
}

function hideLoadingIndicator() {
    // Implement your loading indicator hide logic
}
