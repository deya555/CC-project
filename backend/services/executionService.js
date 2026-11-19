
const { spawn } = require("child_process");
const fs = require("fs/promises");
const os = require("os");
const path = require("path");
const { randomUUID } = require("crypto");

const TIMEOUT_MS = 5000;
const MAX_CODE_SIZE = 50 * 1024;
const MAX_INPUT_SIZE = 10 * 1024;
const MAX_OUTPUT_SIZE = 64 * 1024;

// Runs a command safely without using a shell.
function runCommand(command, args, options = {}) {
  return new Promise((resolve, reject) => {
    const child = spawn(command, args, {
      cwd: options.cwd,
      shell: false,
      windowsHide: true,
      stdio: ["pipe", "pipe", "pipe"],
    });

    let stdout = "";
    let stderr = "";
    let outputSize = 0;
    let settled = false;

    const finish = (error, result) => {
      if (settled) return;
      settled = true;
      clearTimeout(timer);

      if (error) reject(error);
      else resolve(result);
    };

    const timer = setTimeout(() => {
      child.kill();
      finish(new Error("Execution timed out after 5 seconds."));
    }, TIMEOUT_MS);

    function collect(target, chunk) {
      outputSize += chunk.length;

      if (outputSize > MAX_OUTPUT_SIZE) {
        child.kill();
        finish(new Error("Output exceeded the 64 KB limit."));
        return;
      }

      if (target === "stdout") stdout += chunk.toString();
      else stderr += chunk.toString();
    }

    child.stdout.on("data", (chunk) => collect("stdout", chunk));
    child.stderr.on("data", (chunk) => collect("stderr", chunk));

    child.on("error", (error) => {
      finish(new Error(`Could not start ${command}: ${error.message}`));
    });

    child.on("close", (exitCode) => {
      finish(null, { exitCode, stdout, stderr });
    });

    child.stdin.on("error", () => {});
    child.stdin.end(options.input || "");
  });
}

// Runs our custom C compiler pipeline.
async function analyzeC(code) {
  const projectRoot = path.resolve(__dirname, "../..");
  const parserExecutable = path.join(projectRoot, "c_parser.exe");

  await fs.access(parserExecutable);

  const result = await runCommand(
    parserExecutable,
    [],
    {
      cwd: projectRoot,
      input: code,
    }
  );

  const output = [result.stdout, result.stderr]
    .filter(Boolean)
    .join("\n");

  const syntaxFailed =
    output.includes("Syntax Analysis Failed!") ||
    output.includes("Syntax Error:");

  const semanticFailed =
    output.includes("Semantic Error:") ||
    output.includes("Type Error:");

  return {
    success:
      result.exitCode === 0 &&
      !syntaxFailed &&
      !semanticFailed,
    output: output || "C compiler finished without output.",
    exitCode: result.exitCode,
    stage: syntaxFailed
      ? "compile"
      : semanticFailed
        ? "semantic"
        : "analysis",
    analysisReady: true,
  };
}

// Compiles and executes C, C++ and Java programs.
async function executeCode(language, code, input = "") {
  if (typeof code !== "string" || !code.trim()) {
    throw new Error("Please enter some source code.");
  }

  if (Buffer.byteLength(code, "utf8") > MAX_CODE_SIZE) {
    throw new Error("Source code exceeds the 50 KB limit.");
  }

  if (typeof input !== "string") {
    throw new Error("Input must be text.");
  }

  if (Buffer.byteLength(input, "utf8") > MAX_INPUT_SIZE) {
    throw new Error("Input exceeds the 10 KB limit.");
  }

  if (!["c", "cpp", "java"].includes(language)) {
    throw new Error("Unsupported language.");
  }

  const workDir = path.join(
    os.tmpdir(),
    `multicompiler-${randomUUID()}`
  );

  await fs.mkdir(workDir, { recursive: false });

  try {
    let executable;
    let runArgs = [];

    if (language === "c") {
      const source = path.join(workDir, "main.c");
      executable = path.join(workDir, "main.exe");

      await fs.writeFile(source, code, "utf8");

      const result = await runCommand(
        "gcc",
        [source, "-o", executable],
        { cwd: workDir }
      );

      if (result.exitCode !== 0) {
        return {
          success: false,
          output: result.stderr || "C compilation failed.",
          stage: "compile",
        };
      }
    } else if (language === "cpp") {
      const source = path.join(workDir, "main.cpp");
      executable = path.join(workDir, "main.exe");

      await fs.writeFile(source, code, "utf8");

      const result = await runCommand(
        "g++",
        [source, "-o", executable],
        { cwd: workDir }
      );

      if (result.exitCode !== 0) {
        return {
          success: false,
          output: result.stderr || "C++ compilation failed.",
          stage: "compile",
        };
      }
    } else {
      const source = path.join(workDir, "Main.java");

      await fs.writeFile(source, code, "utf8");

      const result = await runCommand(
        "javac",
        [source],
        { cwd: workDir }
      );

      if (result.exitCode !== 0) {
        return {
          success: false,
          output: result.stderr || "Java compilation failed.",
          stage: "compile",
        };
      }

      executable = "java";
      runArgs = ["-cp", workDir, "Main"];
    }

    const result = await runCommand(
      executable,
      runArgs,
      {
        cwd: workDir,
        input,
      }
    );

    return {
      success: result.exitCode === 0,
      output: [result.stdout, result.stderr]
        .filter(Boolean)
        .join("\n") || "Program finished without output.",
      exitCode: result.exitCode,
      stage: "run",
      executionReady: true,
    };
  } finally {
    await fs.rm(workDir, {
      recursive: true,
      force: true,
    });
  }
}

module.exports = {
  executeCode,
  analyzeC,
};
