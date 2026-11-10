# 📈 PCP — Android

A Matrix client in the making. The goal: a truly modern messenger with a **pure C++ native** core.

Currently an active fork of [Element](https://github.com/element-hq) Classic ([element-android](https://github.com/element-hq/element-android)). The Rust SDK (`matrix-rust-sdk`) is being removed in favor of pure C++ crypto via libolm. Kotlin/Java components are gradually replaced with native C++ — starting with real features.

🚀 **Rocket-speed development** — a full Matrix client implemented in months, not years. 200+ C++ native modules and counting.

I worked on the project for one month, then took a month off to rest. Right now a new month of intensive development is underway — or however long my energy for it lasts. P.S. check [pcp-desktop](https://github.com/MaurerAnton/pcp-desktop)

**Website:** [pcp.chat](https://pcp.chat)

> ⚠️ **Server note:** the Matrix homeserver (`pcp.chat`) is currently **IPv6-only** — clients on IPv4-only networks (many mobile carriers, some WiFis) cannot connect yet. IPv4 support is planned.

> 💬 **Chat with the author:** Join [#community:pcp.chat](https://matrix.to/#/#community:pcp.chat) (!lU43N9MCwKQgSAn5UvoDp15jA_JftwZJZv2mZ8avw0E) on Matrix (last update - 05.08.2026)

## Development Status

The client is approximately **halfway complete**. When ready, it will be published in the [Matrix.org clients directory](https://matrix.org/clients/).

> 🧪 **Last tested commit:** `3b038a30` (`3b038a305622e641a155a4438cacb9136225d072`, 2026-08-06 — "README: update date; note that homeserver is IPv6-only")

## The Vision

- **Pure C++ core** for maximum performance and portability
- **Clean, snappy UI** that respects your attention
- **Full Matrix compatibility** — no compromises on federation
- **Open source** — AGPLv3

## AI Agent (`/agent`)

PCP includes an LLM-powered agent system — like Claude Code for your chats.

```
/agent find messages from Alice about the future
/agent summarize what happened since yesterday
/agent search for any mentions of "meeting" and tell me who said what
```

The agent can:
- **Read** your chat history
- **Search** messages by keyword
- **Send** and **edit** messages
- **List** room members and their profiles
- **React** to messages with emoji
- **Navigate** between rooms and DMs

**How it works:**
1. You write `/agent <task>` in any room
2. The C++ `agent_executor` module builds a system prompt with room context + available tools
3. It sends the request to your configured LLM (OpenAI / Anthropic API)
4. The LLM can call "tools" (read messages, search, etc.) to gather information
5. The agent loop continues until the task is complete or max iterations reached
6. Results are displayed in the room

**Configuration:** Set your LLM API endpoint in Labs → LLM Settings.

**C++ modules used:** `agent_executor` (orchestrator), `llm` (API calls), `slash_command` (parsing)

## C++ Native Modules

PCP replaces slow Kotlin/Java components with native C++:

| # | Module | File | Replaces |
|---|--------|------|----------|
| 1 | `/jumptodate` | [`jumptodate.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/jumptodate.cpp) | MSC3030 URL construction & response parsing |
| 2 | Jump to source | [`relation.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/relation.cpp) | Event relation parsing |
| 3 | Chat export | [`exporter.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/exporter.cpp) | HTML/PlainText/JSON formatting |
| 4 | Event cache | [`eventcache.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/eventcache.cpp) | Stage 2 context menu data assembly |
| 5 | Event database | [`eventdb.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/eventdb.cpp) | Realm DB queries (SQLite-native, 50x faster) |
| 6 | Message translation | [`translate.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/translate.cpp) | OpenAI-compatible API request/response |
| 7 | Proxy/Tor/I2P | [`proxy.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/proxy.cpp) | Connection routing (SOCKS5/HTTP proxy) |
| 8 | Yggdrasil | [`yggdrasil.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/yggdrasil.cpp) | .ygg domain resolution, URL rewriting |
| 9 | Markdown renderer | [`markdown.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/markdown.cpp) | Full markdown → HTML with table support |
| 10 | Account export | [`account_export.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/account_export.cpp) | Encrypted .pca file export/import |
| 11 | Audio engine | [`audio_engine.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/audio_engine.cpp) | Background playback, format, progress |
| 12 | Media filter | [`media_filter.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/media_filter.cpp) | Extension filter, expired files detection |
| 13 | Content filter | [`content_filter.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/content_filter.cpp) | Keyword block, remote image block, EXIF |
| 14 | Chunked upload | [`chunked_upload.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/chunked_upload.cpp) | Zero-copy streaming file upload |
| 15 | Chat features | [`chat_features.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/chat_features.cpp) | Custom timezone display, EXIF detection |
| 16 | Invitation hide | [`invitation_hide.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/invitation_hide.cpp) | Blacklist-style invitation hiding |
| 17 | Thread aggregator | [`thread_aggregator.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/thread_aggregator.cpp) | All-threads cross-room view |
| 18 | User messages | [`user_messages.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/user_messages.cpp) | Per-user cached message history |
| 19 | Room version | [`room_version.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/room_version.cpp) | Room version selector validation |
| 20 | Chat preview | [`chat_preview.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/chat_preview.cpp) | Expanded 2-3x block with compact messages |
| 21 | RAM monitor | [`ram_monitor.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/ram_monitor.cpp) | Process RSS monitor via /proc |
| 22 | Cache manager | [`cache_manager.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/cache_manager.cpp) | Selective cache deletion by room/date |
| 23 | Message aggregator | [`message_aggregator.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/message_aggregator.cpp) | All-messages pseudoroom |
| 24 | Room info | [`room_info.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/room_info.cpp) | Creation date, full history indicator |
| 25 | Deleted archive | [`deleted_archive.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/deleted_archive.cpp) | Pre-deletion message archive |
| 26 | Search index | [`search_index.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/search_index.cpp) | Full-text search with E2E support |
| 27 | Module loader | [`module_loader.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/module_loader.cpp) | Dynamic .so module loading at runtime |
| 28 | Notification | [`notification.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/notification.cpp) | Custom keywords, reaction preview |
| 29 | Room mirror | [`room_mirror.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/room_mirror.cpp) | Room-to-room forwarding with doll accounts |
| 30 | Input tools | [`input_tools.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/input_tools.cpp) | Symbol bar, auto-replacement rules |
| 31 | LLM | [`llm.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/llm.cpp) | /llm & /llmp commands, OpenAI+Anthropic |
| 32 | Read receipts | [`read_receipts.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/read_receipts.cpp) | Avatar ordering, configurable "+N" limit |
| 33 | Room analytics | [`room_analytics.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/room_analytics.cpp) | User stats, top posters, server breakdown |
| 34 | Chat tools | [`chat_tools.cpp`](https://github.com/pcp-chat/pcp-android/blob/main/pcp/src/main/cpp/src/chat_tools.cpp) | User hide timer, msg queue, auto-scroll, crop |

## Building

Standard Android build with NDK/CMake for native C++:

```bash
./gradlew assembleFdroidDebug
```

CI builds F-Droid arm32 (armeabi-v7a, works on all devices). Requires Android SDK, NDK 21.3+, CMake 3.22+.

## System Requirements

- **Minimum:** Android smartphone with **1 GB RAM**
- Exact memory requirements will be precisely determined as the C++ native core matures — current figures are preliminary and may be revised
- Android 5.0 (API 21) or newer (current Kotlin-based ceiling)
- **Roadmap:** as Kotlin/Java components are replaced with C++ native code, the minimum Android version will drop. Target is **Android 4.4 (API 19)**, possibly as low as **Android 2.3 (API 9)** depending on technical obstacles, resource constraints, and priorities encountered during the port

## Versioning

Current version: **v0.5.0**. Before 1.0.0, all bets are off — rapid iteration is the priority.

### Pre-1.0.0 (now)

- **v0.x.y** — versions are sequential, no strict SemVer meaning
- New features, bug fixes, and **breaking changes** are all expected
- API surfaces, module boundaries, and data formats may shift between versions
- Nightly builds available via GitHub Actions

### Post-1.0.0 (SemVer)

Once the core stabilizes, PCP will adopt strict [Semantic Versioning 2.0.0](https://semver.org/):

| Segment | Meaning | Cadence |
|---|---|---|
| **x** (MAJOR) | Architecture changes, API breaks, redesigns | Every few months |
| **y** (MINOR) | New features, new modules, new Labs flags | ~weekly |
| **z** (PATCH) | Bug fixes, performance improvements, typos | ~daily |

- `1.0.z` — bug fixes
- `1.y.0` — new features
- `x.0.0` — arch changes

Nightly builds will continue alongside stable releases.

## How You Can Help

PCP is in active development. We have months of debugging and polishing ahead before the first stable release. Here's how you can contribute:

- **Test and report bugs** — run the F-Droid debug APK, use the features, and open issues
- **C++ code review** — all native modules in `pcp/src/main/cpp/` welcome scrutiny
- **Kotlin integration** — JNI wrappers are ready, UI wiring needs love
- **Translations** — help localize the growing list of labs settings
- **Documentation** — improve setup guides, feature descriptions
- **Spread the word** — tell your Matrix friends about `pcp.chat`

## License

AGPL-3.0-only — inherited from Element and likely to remain forever. All original C++ code and PCP modifications are AGPL-3.0-only as well.
