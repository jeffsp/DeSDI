# Agent Delegation & MCP Routing Rules
Whenever I ask you to delegate a task, use the following strict mappings to format your MCP tool calls:

* **"coder_agent"**: You must route the request to the `network-ollama-coder` MCP server using the `qwen2.5-coder:7b` model.
* **"design_agent"**: You must route the request to the `network-ollama-main` MCP server using the `gemma4:31b-it-q8_0` model.

Do not ask for confirmation when using these aliases, just execute the tool call.
