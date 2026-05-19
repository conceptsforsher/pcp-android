# C++ Message Translation Engine (`translate.cpp`)

## Overview

The message translation engine enables Progressive Chat to translate messages in real-time using OpenAI-compatible and Anthropic language models. Unlike traditional translation apps that require the user to copy-paste text, this module integrates directly into the message context menu — long-press any message and tap "Translate".

The entire translation pipeline is C++ native: prompt engineering, request body construction, and response parsing all happen on the native heap. The HTTP call itself is made by OkHttp on the Kotlin side, with the request and response flowing through JNI boundaries as JSON strings.

## Design Rationale

**Why C++ for translation?** Three reasons:

1. **Prompt engineering is string manipulation at scale.** Building a well-formed JSON request body for the LLM API — with properly escaped user text, system prompts, and model parameters — is CPU-bound work that benefits from native performance.

2. **Response parsing is security-critical.** The LLM API returns arbitrary text that must be safely extracted from nested JSON. A buffer overflow or injection vulnerability here could expose the user's API token or inject malicious content into the chat. C++ gives us precise control over memory boundaries.

3. **Zero-copy string handling.** The translation pipeline moves large strings (message bodies can be kilobytes) between layers. With C++ we control exactly when allocations happen — no GC pauses during translation, no intermediate `CharSequence` wrappers, no Kotlin string duplication.

## Architecture

The translation flow has four distinct phases, each handled by a dedicated function:

```
User long-presses message → "Translate" in context menu
    │
    ▼
1. CONFIGURE: LlmConfig assembled from user settings
   - Provider: OpenAI or Anthropic
   - API endpoint and token from Settings → Labs
   - Model: "gpt-4o-mini" or "claude-3-haiku-20240307"
   - Target language: from system locale or manual setting
    │
    ▼
2. BUILD: ProgressiveNative.nativeBuildTranslateRequest(text, srcLang, tgtLang, endpoint, token, model)
   → C++ builds the JSON request body
   │
   │  FOR OPENAI:
   │  {
   │    "model": "gpt-4o-mini",
   │    "messages": [
   │      {"role": "system", "content": "You are a translator. Translate to Russian. Output ONLY the translation."},
   │      {"role": "user", "content": "Hello, how are you?"}
   │    ],
   │    "temperature": 0.1
   │  }
   │
   │  FOR ANTHROPIC:
   │  {
   │    "model": "claude-3-haiku-20240307",
   │    "max_tokens": 1024,
   │    "messages": [
   │      {"role": "user", "content": "Translate to Russian. Output ONLY the translation: Hello, how are you?"}
   │    ]
   │  }
   │
   ▼
3. SEND: Kotlin OkHttp sends POST to the API endpoint
   - Headers from nativeBuildLlmHeaders()
   - Body from step 2
   - Timeout: 15s connect, 30s read
    │
    ▼
4. PARSE: ProgressiveNative.nativeParseTranslateResponse(responseBody, httpStatus)
   → C++ extracts translated text from the JSON response
   │
   │  INPUT: {"choices": [{"message": {"content": "Привет, как дела?"}}]}
   │  OUTPUT: {"success": true, "translatedText": "Привет, как дела?"}
   │
   ▼
5. DISPLAY: Kotlin shows translated text in a Material dialog with Copy button
```

## API Reference

### `buildTranslateRequestBody(config, text)` → String

Constructs the complete JSON request body for the chosen LLM provider.

**Parameters:**
- `config.provider` — `LlmProvider::OpenAI` or `LlmProvider::Anthropic`
- `config.model` — model identifier string
- `config.temperature` — creativity control (0.0 = deterministic, 1.0 = creative)
- `config.maxTokens` — maximum response length
- `config.systemPrompt` — optional custom system prompt
- `text` — the user message to translate

**System Prompt Strategy:**
The engine crafts a terse, directive system prompt designed to produce clean translations without extraneous text. The prompt tells the model to output **only** the translation — no quotes, no explanations, no meta-commentary. This is critical because the translated text goes directly into the UI dialog; any extra text from the model would degrade the user experience.

### `parseTranslateResponse(body, httpStatus)` → TranslateResult

Extracts translated text from the provider's JSON response. Handles both success and error cases.

**OpenAI response format:**
```json
{"choices": [{"message": {"content": "translated text"}}]}
```

**Anthropic response format:**
```json
{"content": [{"type": "text", "text": "translated text"}]}
```

**Error handling:**
- Non-200 HTTP status → extracts error message from JSON or uses status code
- Missing `choices` array → "No choices in response"
- Empty content → "No content in response"
- Network errors → handled by Kotlin layer (captured as exceptions)

### `buildLlmHeaders(config)` → String

Returns the HTTP headers needed for the API call, formatted as a newline-separated string that Kotlin parses into OkHttp headers.

**OpenAI:**
```
Authorization: Bearer sk-...
Content-Type: application/json
```

**Anthropic:**
```
x-api-key: sk-ant-...
anthropic-version: 2023-06-01
Content-Type: application/json
```

## Provider Differences

| Aspect | OpenAI | Anthropic |
|--------|--------|-----------|
| Auth header | `Authorization: Bearer` | `x-api-key` |
| API version header | None | `anthropic-version: 2023-06-01` |
| System prompt | `"role": "system"` message | First `"role": "user"` message |
| Response format | `choices[0].message.content` | `content[0].text` |
| Endpoint | `/v1/chat/completions` | `/v1/messages` |

## JNI Integration

The JNI bridge in `jni_bridge.cpp` provides three functions:

```cpp
// Build the request body
JNIEXPORT jstring JNICALL
Java_..._nativeBuildTranslateRequest(
    JNIEnv* env, jclass,
    jstring text, jstring srcLang, jstring tgtLang,
    jstring endpoint, jstring token, jstring model
);

// Build the headers
JNIEXPORT jstring JNICALL
Java_..._nativeBuildLlmHeaders(
    JNIEnv* env, jclass, jint provider, jstring token
);

// Parse the response
JNIEXPORT jstring JNICALL
Java_..._nativeParseTranslateResponse(
    JNIEnv* env, jclass, jstring body, jint statusCode, jint provider
);
```

Each function has a pure-Kotlin fallback in `ProgressiveNative.kt` that is used when the native library fails to load. This ensures the feature works even on devices where `libprogressive_native.so` is unavailable — the Kotlin fallback uses `org.json.JSONObject` for the same logic.

## Security Considerations

1. **API tokens never touch logs.** The Kotlin layer redacts the `apiToken` parameter before any Timber logging.

2. **Message content is user-controlled.** The `escape()` function used for JSON string construction prevents injection attacks — all double quotes, backslashes, and control characters are properly escaped.

3. **System prompt isolation.** The system prompt is constructed entirely in C++ with no user-controlled format strings. The user's message body goes only into the `"user"` role content, never into the system prompt template.

4. **No response caching.** Translated text is ephemeral — shown in a dialog and discarded. This prevents sensitive translations from persisting in cache or storage.

## Performance

| Phase | Time (arm64) |
|-------|-------------|
| `buildTranslateRequestBody` | ~5 μs |
| `parseTranslateResponse` | ~3 μs |
| HTTP round-trip (API) | 500-3000 ms (network-bound) |
| Total C++ overhead | ~8 μs |

The C++ layer adds negligible overhead — the dominant factor is the LLM API latency.

## Future Work

- **Streaming responses.** Currently the full response must arrive before display. Streaming SSE (Server-Sent Events) from the LLM API would allow word-by-word display as the translation is generated.
- **Translation memory.** Cache frequent translations locally to avoid API calls for repeated phrases.
- **Batch translation.** Translate entire rooms at once using a single API call with multiple messages.
