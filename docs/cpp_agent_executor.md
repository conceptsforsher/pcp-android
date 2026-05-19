# AI Agent (`/agent`) — Design & Architecture

## Overview

`/agent` is an LLM-powered assistant that can read, search, send, edit, and react
to messages in Matrix rooms. Inspired by Claude Code and Cursor's agent patterns.

```
User: /agent find messages from Alice about the future
Agent: [reads last 50 messages] → [searches for "future"] →
       "Found 3 messages from Alice about the future:
       1. 'I think AI will change everything' (Jan 15)
       2. 'The future of Matrix is federation' (Feb 3)
       3. 'We need better encryption' (Mar 10)"
```

## Architecture

```
┌─────────────┐     ┌──────────────────┐     ┌─────────────┐
│  Kotlin UI  │────▶│  C++ agent_exec  │────▶│  LLM API    │
│  /agent msg │     │  buildPrompt()   │     │  OpenAI/    │
│             │◀────│  parseToolCalls()│◀────│  Anthropic  │
│  executes   │     │  extractAnswer() │     │             │
│  tool calls │     └──────────────────┘     └─────────────┘
└─────────────┘
```

### Flow

1. User types `/agent <task>` in any room
2. `slash_command.cpp` parses the command → `ParseResultType::AgentTask`
3. Kotlin builds agent state with room context (recent messages, members)
4. C++ `agent_executor` constructs system prompt with tools + rules
5. Kotlin sends prompt to LLM API (OpenAI/Anthropic)
6. C++ `parseToolCalls()` extracts tool requests from LLM response
7. If tool calls: Kotlin executes them (read messages, search, etc.)
8. Results fed back to LLM → repeat until task complete
9. Final answer displayed in room

## Available Tools (9)

| Tool | Parameters | Description |
|------|-----------|-------------|
| `read_messages` | room_id, limit, before_event_id | Read recent messages |
| `send_message` | room_id, text | Send a text message |
| `edit_message` | room_id, event_id, new_text | Edit a message |
| `search_messages` | room_id, query | Full-text search |
| `list_users` | room_id | List room members |
| `get_user_info` | user_id | Get user profile |
| `react_to_message` | room_id, event_id, emoji | Add reaction |
| `get_room_info` | room_id | Room details |
| `send_direct_message` | user_id, text | DM a user |

## C++ Module: agent_executor

### Header (`agent_executor.hpp`)

```cpp
struct AgentConfig {
    std::string systemPrompt;      // role + rules for LLM
    std::string toolsDescription;  // JSON Schema of available tools
    int maxIterations = 10;        // safety limit
    int maxContextMessages = 50;   // recent messages to include
};

struct AgentState {
    std::string task;              // user's request
    int iteration = 0;             // current step
    std::string finalAnswer;       // result for user
    std::vector<AgentToolCall> pendingToolCalls;
    bool isComplete = false;
};
```

### Key Functions

- `buildAgentSystemPrompt()` — constructs the system message defining agent role, rules, tools
- `buildAgentUserPrompt()` — includes room context + task for each iteration
- `parseToolCalls()` — extracts `{"tool_calls": [...]}` from LLM JSON response
- `extractTextAnswer()` — separates tool calls from final text
- `processAgentIteration()` — state machine updating agent state
- `formatMessagesForAgent()` — formats chat history as readable context
- `getAgentToolsSchema()` — returns the 9-tool schema injected into system prompt

## Safety

- **Max iterations**: 10 by default — prevents infinite loops
- **No destructive actions without confirmation**: send/edit/react tools only
- **Room-scoped**: agent operates within a single room context
- **Read-only by default**: tools that modify (send, edit) are explicit

## Configuration

Set LLM API credentials in Labs → LLM Settings:
- Provider: OpenAI or Anthropic
- API Endpoint + Token
- Model: gpt-4o-mini / claude-3-haiku

## Performance

| Operation | Time |
|-----------|------|
| System prompt build | < 1ms |
| Tool call parsing | < 1ms |
| LLM API round-trip | 500-3000ms |
| Full task (2 iterations) | ~3-6s |

## Related Modules

- `slash_command.cpp` — parses `/agent <task>` command
- `llm.cpp` — builds API request bodies for OpenAI/Anthropic
- `event_classifier.cpp` — determines event types for message reading
- `search_index.cpp` — full-text message search
