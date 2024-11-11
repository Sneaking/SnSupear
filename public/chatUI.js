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
