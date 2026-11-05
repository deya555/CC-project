import { useState } from "react";
import CodeEditor from "./components/CodeEditor";
import { executeCode } from "./services/compilerService";
import "./App.css";

const starterCode = {
  c: `#include <stdio.h>

int main() {
    printf("Hello from C!\\n");
    return 0;
}`,
  cpp: `#include <iostream>
using namespace std;

int main() {
    cout << "Hello from C++!" << endl;
    return 0;
}`,
  java: `public class Main {
    public static void main(String[] args) {
        System.out.println("Hello from Java!");
    }
}`,
  mini: `int x;
x = 10 + 20;
print(x);`
};

function App() {
  const [language, setLanguage] = useState("c");
  const [code, setCode] = useState(starterCode.c);
  const [activeTab, setActiveTab] = useState("output");
  const [input, setInput] = useState("");
  const [output, setOutput] = useState(
    "Your program output will appear here."
  );
  const [isRunning, setIsRunning] = useState(false);

  // Added execution status state
  const [executionStatus, setExecutionStatus] = useState("idle");
  const [statusMessage, setStatusMessage] = useState("Ready");

  const languageNames = {
    c: "C",
    cpp: "C++",
    java: "Java",
    mini: "Mini Language"
  };

  const fileNames = {
    c: "main.c",
    cpp: "main.cpp",
    java: "Main.java",
    mini: "program.mini"
  };

  function handleLanguageChange(event) {
    const selected = event.target.value;
    setLanguage(selected);
    setCode(starterCode[selected]);
    setOutput("Ready to run. The execution backend is not connected yet.");
  }

  async function handleRun() {
    setActiveTab("output");
    setIsRunning(true);
    setExecutionStatus("running");
    setStatusMessage("Compiling and running...");
    setOutput("Please wait...");

    try {
      const result = await executeCode(language, code, input);

      setOutput(result.output || "Program finished without output.");

      if (result.success) {
        setExecutionStatus("success");
        setStatusMessage("Execution completed successfully");
      } else {
        setExecutionStatus("error");
        setStatusMessage(
          result.stage === "compile"
            ? "Compilation failed"
            : "Execution failed"
        );
      }
    } catch (error) {
      setExecutionStatus("error");
      setStatusMessage(error.message);
      setOutput(error.message);
    } finally {
      setIsRunning(false);
    }
  }

  return (
    <div className="ide">
      <header className="ide-header">
        <div className="brand">
          <div className="brand-logo">&lt;/&gt;</div>
          <div>
            <h1>Code<span>Lab</span></h1>
            <p>ONLINE COMPILER STUDIO</p>
          </div>
        </div>

        <div className="header-center">
          <span className="live-dot"></span>
          Compiler Workspace
        </div>

        <div className="header-actions">
          <span className="system-status">
            <span className="status-dot"></span>
            Editor Ready
          </span>
          <button
            className="run-button"
            onClick={handleRun}
            disabled={isRunning || language === "mini"}
            title={language === "mini" ? "Mini Language execution is coming soon" : "Run your code"}
          >
            <span>{isRunning ? "…" : "▶"}</span>
            {isRunning ? "Sending..." : "Run Code"}
          </button>
        </div>
      </header>

      <main className="ide-main">
        <section className="page-heading">
          <div>
            <p className="eyebrow">YOUR DEVELOPMENT SPACE</p>
            <h2>Code. Compile. Explore.</h2>
            <p className="subtitle">
              Write programs, test ideas, and explore compiler analysis
              in one workspace.
            </p>
          </div>

          <div className="language-badge">
            <span className="language-symbol">
              {language === "java" ? "J" :
               language === "cpp" ? "C+" :
               language === "mini" ? "M" : "C"}
            </span>
            <div>
              <strong>{languageNames[language]}</strong>
              <small>Selected language</small>
            </div>
          </div>
        </section>

        <section className="workspace">
          <div className="editor-panel">
            <div className="panel-heading">
              <div className="panel-title">
                <span className="panel-icon">&lt;/&gt;</span>
                <div>
                  <h3>Code Editor</h3>
                  <p>Write your source code</p>
                </div>
              </div>

              <div className="language-control">
                <label htmlFor="language">LANGUAGE</label>
                <select
                  id="language"
                  value={language}
                  onChange={handleLanguageChange}
                >
                  <option value="c">C</option>
                  <option value="cpp">C++</option>
                  <option value="java">Java</option>
                  <option value="mini">Mini Language</option>
                </select>
              </div>
            </div>

            <div className="file-tab">
              <span className="file-dot"></span>
              {fileNames[language]}
              <span className="file-state">●</span>
            </div>

            <CodeEditor
              language={language}
              code={code}
              onChange={setCode}
            />

            <div className="editor-status">
              <span>● Ready</span>
              <span>UTF-8</span>
              <span>Spaces: 4</span>
              <span>{languageNames[language]}</span>
            </div>
          </div>

          <aside className="side-panel">
            <div className="side-heading">
              <div>
                <h3>Program Console</h3>
                <p>Input, output &amp; analysis</p>
              </div>
              <span className="console-icon">▤</span>
            </div>

            <div className="console-tabs">
              <button
                className={activeTab === "output" ? "tab active" : "tab"}
                onClick={() => setActiveTab("output")}
              >
                <span>▸</span> Output
              </button>
              <button
                className={activeTab === "input" ? "tab active" : "tab"}
                onClick={() => setActiveTab("input")}
              >
                <span>⌨</span> Input
              </button>
              <button
                className={activeTab === "analysis" ? "tab active" : "tab"}
                onClick={() => setActiveTab("analysis")}
              >
                <span>⌘</span> Analysis
              </button>
            </div>

            <div className="console-content">
              {activeTab === "output" && (
                <div className="output-view">
                  <div className={`execution-status ${executionStatus}`}>
                    <span className="status-indicator"></span>
                    <span>{statusMessage}</span>
                  </div>
                  <div className="console-label">
                    <span className="status-dot"></span>
                    CONSOLE OUTPUT
                  </div>
                  <pre>{output}</pre>
                  {output === "Your program output will appear here." && (
                    <>
                      <div className="console-empty-icon">›_</div>
                      <p className="console-hint">
                        Run your program to view its output here.
                      </p>
                    </>
                  )}
                </div>
              )}

              {activeTab === "input" && (
                <div className="input-view">
                  <div className="console-label">STANDARD INPUT (STDIN)</div>
                  <p className="input-description">
                    Enter the input your program should receive when it runs.
                  </p>
                  <textarea
                    value={input}
                    onChange={(event) => setInput(event.target.value)}
                    placeholder={"Enter input here...\nExample:\n10"}
                  />
                  <p className="small-note">
                    Input will be sent to the program when you run your code.
                  </p>
                </div>
              )}

              {activeTab === "analysis" && (
                <div className="analysis-view">
                  <div className="console-label">COMPILER ANALYSIS</div>
                  <div className="analysis-item">
                    <span>01</span>
                    <div>
                      <strong>Lexical Analysis</strong>
                      <p>Token generation with Flex</p>
                    </div>
                    <span className="pending">Pending</span>
                  </div>
                  <div className="analysis-item">
                    <span>02</span>
                    <div>
                      <strong>Syntax Analysis</strong>
                      <p>Parsing and AST with Bison</p>
                    </div>
                    <span className="pending">Pending</span>
                  </div>
                  <div className="analysis-item">
                    <span>03</span>
                    <div>
                      <strong>Semantic Analysis</strong>
                      <p>Symbol table and type checking</p>
                    </div>
                    <span className="pending">Pending</span>
                  </div>
                  <div className="analysis-item">
                    <span>04</span>
                    <div>
                      <strong>Three-Address Code</strong>
                      <p>Intermediate code generation</p>
                    </div>
                    <span className="pending">Pending</span>
                  </div>
                  <p className="small-note">
                    These analysis stages will process the Mini Language,
                    not ordinary C, C++, or Java source code.
                  </p>
                </div>
              )}
            </div>

            <div className="console-footer">
              <span><span className="status-dot"></span> Workspace active</span>
              <span>CodeLab v1.0</span>
            </div>
          </aside>
        </section>

        <section className="feature-strip">
          <div className="feature">
            <span className="feature-icon blue">⌘</span>
            <div>
              <strong>Multiple Languages</strong>
              <p>C · C++ · Java</p>
            </div>
          </div>
          <div className="feature">
            <span className="feature-icon purple">⌘</span>
            <div>
              <strong>Compiler Design</strong>
              <p>Flex · Bison · AST</p>
            </div>
          </div>
          <div className="feature">
            <span className="feature-icon green">↗</span>
            <div>
              <strong>Intermediate Code</strong>
              <p>Semantic checks · TAC</p>
            </div>
          </div>
        </section>
      </main>

      <footer className="ide-footer">
        <span>CodeLab <span className="footer-separator">/</span> Compiler Design Project</span>
        <span>Built with React &amp; Monaco Editor</span>
      </footer>
    </div>
  );
}

export default App;