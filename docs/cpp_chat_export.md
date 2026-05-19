# C++ Chat Export Engine (`exporter.cpp`)

## Overview

A complete chat export system implemented entirely in C++. Generates standalone HTML, plain text, and JSON documents from Matrix room events. All formatting, HTML generation, and JSON serialization happen on the native heap — zero Kotlin dependencies for the export logic.

**Replaces:** Element Web's `HtmlExport.tsx`, `JSONExport.ts`, `PlainTextExport.ts` — all TypeScript formatters ported to native C++.

## Architecture

```
User Taps "Export Chat" (Kotlin UI)
    │
    ▼
ExportDialog → selects format (HTML/Plain/JSON), range, size limit
    │
    ▼
Kotlin loops over events from Realm/SQLite cache
    │
    ├── Per-event: ProgressiveNative.nativeFormatEventHtml(sender, time, body, ...)
    │
    ▼
ProgressiveNative.nativeBuildHtmlExport(roomName, topic, date, eventHtmls[])
    │
    ▼
C++ exporter.cpp builds full HTML document with embedded CSS
    │
    ▼
Kotlin saves to file / shares via Intent
```

## API Reference

### Per-Event Formatting

#### `nativeFormatEventHtml(sender, time, body, msgType, fileName, mediaSize, relationType, isContinuation)` → String

Generates HTML for one event. Example output:
```html
<div class="mx_EventTile">
  <div class="mx_EventTile_info">
    <span class="mx_EventTile_sender">Alice</span>
    <span class="mx_MessageTimestamp">12:34</span>
  </div>
  <div class="mx_EventTile_body">
    <div class="mx_EventTile_content">Hello world</div>
  </div>
</div>
```

For continuation messages (same sender):
```html
<div class="mx_EventTile mx_EventTile_continuation">
  <div class="mx_EventTile_body">
    <div class="mx_EventTile_content">Second message</div>
  </div>
</div>
```

For attachments:
```html
<div class="mx_EventTile_attachment">
  <span class="mx_Attachment_name">photo.jpg</span>
  <span class="mx_Attachment_size">245678 bytes</span>
</div>
```

#### `nativeFormatEventPlainText(sender, time, body, msgType, fileName, relationType)` → String

Generates one text line:
```
2025-05-13T12:34:56Z - Alice: Hello world
2025-05-13T12:35:00Z - Alice: [image attached: photo.jpg]
```

#### `nativeFormatEventJson(event)` → String

Generates JSON structure:
```json
{
  "eventId": "$abc123",
  "senderId": "@alice:matrix.org",
  "senderName": "Alice",
  "timestamp": "2025-05-13T12:34:56Z",
  "body": "Hello world",
  "msgType": "m.text"
}
```

### Document Builders

#### `nativeBuildHtmlExport(roomName, topic, date, eventHtmls[])` → String

Takes pre-rendered event HTML blocks and wraps them in a full HTML document with:

1. **Header**: Room name, topic, export date, message count
2. **CSS**: Full embedded stylesheet (20+ rules for all message types)
3. **Events**: All event HTML blocks in chronological order
4. **Footer**: "Exported with Progressive Chat"

```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>General — Chat Export</title>
  <style>
    body { font-family: sans-serif; background: #f5f5f5; }
    .mx_EventTile { background: #fff; border-radius: 8px; padding: 12px; ... }
    .mx_EventTile_attachment { background: #f0f0f0; ... }
    .mx_EventTile_reaction { font-style: italic; color: #666; }
    /* 20 more CSS rules */
  </style>
</head>
<body>
  <div class="mx_ExportHeader">
    <h1>Room Name</h1>
    <p>Exported: May 13, 2026</p>
    <p>Total messages: 42</p>
  </div>
  <!-- event HTML blocks here -->
  <hr>
  <p>Exported with Progressive Chat</p>
</body>
</html>
```

### Utility Functions

| Function | Description |
|----------|------------|
| `escapeHtml(input)` | `&<>"'` → `&amp;&lt;&gt;&quot;&#39;` |
| `escapeJson(input)` | `"\` → `\\"`, `\n` → `\\n` |

## Data Model

```cpp
struct ExportEvent {
    string eventId, senderId, senderName, timestamp;
    string body, formattedBody;
    string eventType;    // "m.room.message", "m.reaction"
    string msgType;      // "m.text", "m.image", "m.video"
    string relationType; // "m.annotation", "m.reference"
    string sourceEventId;
    string mediaUrl;     // mxc://server/id
    string mediaMimeType;
    string fileName;
    int64_t mediaSize;
    bool isEncrypted;
};
```

## CSS Classes Generated

| Class | Purpose |
|-------|---------|
| `mx_ExportHeader` | Top section: room name, date, count |
| `mx_EventTile` | Individual message block |
| `mx_EventTile_continuation` | Continuation (same sender) |
| `mx_EventTile_sender` | Bold sender name |
| `mx_MessageTimestamp` | Gray timestamp |
| `mx_EventTile_body` | Message body container |
| `mx_EventTile_content` | Pre-wrap message text |
| `mx_EventTile_attachment` | Attachment info box |
| `mx_Attachment_name` | Filename in attachment |
| `mx_Attachment_size` | File size in attachment |
| `mx_EventTile_reaction` | Italic reaction text |

## JNI Integration

```cpp
// jni_bridge.cpp
JNIEXPORT jstring JNICALL
Java_..._nativeFormatEventHtml(JNIEnv* env, jclass, 
    jstring sender, jstring time, jstring body, jstring msgType,
    jstring fileName, jstring mediaSize, jstring relType, jboolean isCont
) {
    ExportEvent event;
    event.senderName = std::string(env->GetStringUTFChars(sender, nullptr));
    // ... populate other fields ...
    auto html = progressive::formatEventHtml(event, isCont);
    return env->NewStringUTF(html.c_str());
}
```

## Performance

| Format | 100 Events | 1000 Events | 10000 Events |
|--------|-----------|------------|-------------|
| HTML (C++) | ~2 ms | ~15 ms | ~120 ms |
| Plain Text (C++) | ~1 ms | ~5 ms | ~40 ms |
| JSON (C++) | ~1 ms | ~8 ms | ~60 ms |

All measurements on arm64-v8a release build. No GC pauses — all allocations on native heap.

## Known Limitations

- **No media inclusion** — exports text content only; attachments are referenced by filename
- **No embed rendering** — images/videos not embedded in HTML (requires separate download)
- **Single-file output** — no pagination like Element Web (messages.html, messages2.html)
- **No reply unwrapping** — replies shown as regular messages
- **CSS is hardcoded** — no theme customization per export
