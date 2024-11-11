//public/chatUI.js
document.getElementById('chat-submit').addEventListener('click', async () => {
    const message = document.getElementById('chat-input').value;
    if (!message) return;

    try {
        const response = await fetch('/api/chat', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ message })
        });

        const data = await response.json();
        const chatOutput = document.getElementById('chat-output');
        chatOutput.innerHTML += `<div class="chat-message">${data.response}</div>`;
    } catch (error) {
        console.error("Failed to fetch chat response:", error);
    }
});
