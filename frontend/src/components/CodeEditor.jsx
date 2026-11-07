import Editor from "@monaco-editor/react";

function CodeEditor({ language, code, onChange }) {
  const editorLanguage = {
    c: "c",
    cpp: "cpp",
    java: "java",
    mini: "c"
  };

  // Custom Monaco theme to match CSS colors
  const handleEditorWillMount = (monaco) => {
    monaco.editor.defineTheme("custom-dark", {
      base: "vs-dark",
      inherit: true,
      rules: [],
      colors: {
        "editor.background": "#0b1220",
        "editor.lineHighlightBackground": "#131f33",
        "editorLineNumber.foreground": "#4a5a73",
        "editorLineNumber.activeForeground": "#8cb7ff",
        "editor.selectionBackground": "#2a3d5c",
        "editorIndentGuide.background": "#1e293b",
      },
    });
  };

  return (
    <div className="editor-container">
      <Editor
        height="100%"
        language={editorLanguage[language] || "c"}
        value={code}
        onChange={(value) => onChange(value || "")}
        theme="custom-dark"
        beforeMount={handleEditorWillMount}
        options={{
          fontSize: 14,
          fontFamily: "'Fira Code', Consolas, 'Courier New', monospace",
          fontLigatures: true,
          minimap: { enabled: false },
          automaticLayout: true,
          scrollBeyondLastLine: false,
          tabSize: 4,
          padding: { top: 20, bottom: 20 },
          smoothScrolling: true,
          cursorBlinking: "smooth",
          cursorSmoothCaretAnimation: "on",
        }}
      />
    </div>
  );
}

export default CodeEditor;