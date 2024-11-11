//public/chatUI.js
// Event listener for the 'Send' button
document.getElementById('chat-submit').addEventListener('click', async () => {
    const input = document.getElementById('chat-input').value.trim();

    if (!input) {
        alert("Please enter a message.");
        return;
    }

    // Clear the input field and disable the button to prevent multiple requests
    document.getElementById('chat-input').value = '';
    document.getElementById('chat-submit').disabled = true;
    appendMessage('You', input);
    showLoadingIndicator();

    try {
        console.log("Sending message to GPT API:", input);

        // Send the message to the GPT API
        const response = await fetch('/api/chat', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ message: input })
        });

        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        const data = await response.json();
        if (!data || !data.response) {
            throw new Error("Invalid response structure from GPT API.");
        }

        appendMessage('GPT', data.response);
        console.log("Received chat response:", data.response);
    } catch (error) {
        console.error("Failed to fetch chat response:", error);
        appendMessage('GPT', "Sorry, there was an error processing your request.");
        alert("Error: Unable to get a response from the server. Please try again later.");
    } finally {
        // Re-enable the button and hide the loading indicator
        document.getElementById('chat-submit').disabled = false;
        hideLoadingIndicator();
    }
});

// Function to append a chat message to the output
function appendMessage(sender, message) {
    const chatOutput = document.getElementById('chat-output');
    const messageElement = document.createElement('div');
    messageElement.className = 'chat-message';
    messageElement.innerHTML = `<strong>${sender}:</strong> ${message}`;
    chatOutput.appendChild(messageElement);
    chatOutput.scrollTop = chatOutput.scrollHeight; // Scroll to the bottom
}

// Function to show a loading indicator
function showLoadingIndicator() {
    // Implement your loading indicator display logic
    console.log("Loading indicator shown (implement your UI logic here).");
}

// Function to hide the loading indicator
function hideLoadingIndicator() {
    // Implement your loading indicator hide logic
    console.log("Loading indicator hidden (implement your UI logic here).");
}
