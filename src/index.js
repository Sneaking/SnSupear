import express from 'express';
import cors from 'cors';
import compression from 'compression';
import rateLimit from 'express-rate-limit';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';
import { Configuration, OpenAIApi } from "openai";

const __dirname = dirname(fileURLToPath(import.meta.url));
const app = express();
const port = process.env.PORT || 3000;

// Get API key from environment variable
const apiKey = process.env.OPENAI_API_KEY;
if (!apiKey) {
  console.error("Error: OPENAI_API_KEY environment variable not set.");
  process.exit(1);
}

const configuration = new Configuration({
  apiKey: apiKey,
});
const openai = new OpenAIApi(configuration);

// Middleware
app.use(express.json());
app.use(cors());
app.use(compression());

// Rate limiting
const limiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 minutes
  max: 100 // limit each IP to 100 requests per windowMs
});
app.use('/api', limiter);

// Serve static files
app.use(express.static(join(__dirname, '../public')));

// API Routes
app.get('/api/status', (req, res) => {
  res.json({ status: 'online', timestamp: new Date().toISOString() });
});

// Code Completion API
app.post('/completions', async (req, res) => {
  const code = req.body.code;
  const line = req.query.line;
  const column = req.query.column;

  try {
    const response = await openai.createCompletion({
      model: "text-davinci-003",
      prompt: `Complete this code:\n${code}`, // Removed line and column from prompt
      max_tokens: 50,
      temperature: 0.7,
      stop: ["###"],
    });

    const suggestions = response.data.choices.map(choice => choice.text.trim());
    res.json({ suggestions });
  } catch (error) {
    console.error("Error with OpenAI API:", error);
    res.status(500).json({ error: "Failed to get code completions" });
  }
});

// GPT Chat API
app.post('/chat', async (req, res) => {
  const message = req.body.message;

  try {
    const response = await openai.createChatCompletion({
      model: "gpt-3.5-turbo",
      messages: [{ role: "user", content: message }],
    });

    const chatResponse = response.data.choices[0].message.content;
    res.json({ response: chatResponse });
  } catch (error) {
    console.error("Error with OpenAI API:", error);
    res.status(500).json({ error: "Failed to get chat response" });
  }
});

// Serve index.html for all other routes
app.get('*', (req, res) => {
  res.sendFile(join(__dirname, '../public/index.html'));
});

// Error handling middleware
app.use((err, req, res, next) => {
  console.error(err.stack);
  res.status(500).json({ error: 'Something went wrong!' });
});

app.listen(port, '0.0.0.0', () => {
  console.log(`Server running on http://localhost:${port}`);
  console.log(`Access from other devices using your computer's IP address`);
});
