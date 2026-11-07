const API_URL = "http://localhost:5000";

export async function executeCode(language, code, input = "") {
  let response;

  try {
    response = await fetch(`${API_URL}/api/execute`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        language,
        code,
        input,
      }),
    });
  } catch (error) {
    throw new Error(
      "Cannot connect to the compiler server. Please make sure the backend is running."
    );
  }

  let data;

  try {
    data = await response.json();
  } catch {
    throw new Error("The compiler server returned an invalid response.");
  }

  if (!response.ok) {
    throw new Error(
      data.error || `Request failed with status ${response.status}.`
    );
  }

  return data;
}