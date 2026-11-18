
const express = require("express");
const cors = require("cors");
const { executeCode } = require("./services/executionService");

const app = express();
const PORT = 5000;

app.use(cors({
  origin: [
    "http://localhost:5173",
    "http://localhost:5174"
  ]
}));

app.use(express.json({ limit: "100kb" }));

// Check whether the backend is running
app.get("/", (req, res) => {
  res.json({
    message: "CodeLab backend is running",
    status: "success"
  });
});

// Execute endpoint
app.post("/api/execute", async (req, res) => {
  try {
    const { language, code, input = "" } = req.body;

    if (!language || typeof code !== "string") {
      return res.status(400).json({
        success: false,
        error: "Language and source code are required.",
      });
    }

    const result = await executeCode(language, code, input);

    return res.json({
      ...result,
      executionReady: true,
    });
  } catch (error) {
    return res.status(400).json({
      success: false,
      error: error.message || "Code execution failed.",
    });
  }
});

app.listen(PORT, () => {
  console.log(`CodeLab backend running at http://localhost:${PORT}`);
});
