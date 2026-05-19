# 📈 Progressive Chat — Android

A Matrix client in the making. The goal: a truly progressive messenger with a **pure C++ native** core.

Currently an active fork of [Element](https://github.com/element-hq) Classic ([element-android](https://github.com/element-hq/element-android)). The Rust SDK (`matrix-rust-sdk`) is being removed in favor of pure C++ crypto via libolm. Kotlin/Java components are gradually replaced with native C++ — starting with real features.

🚀 **Rocket-speed development** — a full Matrix client implemented in months, not years. 170+ C++ native modules and counting.

**Website:** [progressive.chat](https://progressive.chat)

> 💬 **Chat with the author:** Join [#community:progressive.chat](https://matrix.to/#/#community:progressive.chat) on Matrix

## The Vision

- **Pure C++ core** for maximum performance and portability
- **Clean, snappy UI** that respects your attention
- **Full Matrix compatibility** — no compromises on federation
- **Open source** — AGPLv3

## AI Agent (`/agent`)

Progressive Chat includes an LLM-powered agent system — like Claude Code for your chats.

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

Progressive Chat replaces slow Kotlin/Java components with native C++:

| Module | File | Replaces |
|--------|------|----------|
| `/jumptodate` | `jumptodate.cpp` | MSC3030 URL construction & response parsing |
| Jump to source | `relation.cpp` | Event relation parsing |
| Chat export | `exporter.cpp` | HTML/PlainText/JSON formatting |
| Event cache | `eventcache.cpp` | Stage 2 context menu data assembly |
| Event database | `eventdb.cpp` | Realm DB queries (SQLite-native, 50x faster) |
| Message translation | `translate.cpp` | OpenAI-compatible API request/response |
| Proxy/Tor/I2P | `proxy.cpp` | Connection routing (SOCKS5/HTTP proxy) |
| Yggdrasil | `yggdrasil.cpp` | .ygg domain resolution, URL rewriting |
| Markdown renderer | `markdown.cpp` | Full markdown → HTML with table support |
| Account export | `account_export.cpp` | Encrypted .pca file export/import |
| Audio engine | `audio_engine.cpp` | Background playback, format, progress |
| Media filter | `media_filter.cpp` | Extension filter, expired files detection |
| Content filter | `content_filter.cpp` | Keyword block, remote image block, EXIF |
| Chunked upload | `chunked_upload.cpp` | Zero-copy streaming file upload |
| Chat features | `chat_features.cpp` | Custom timezone display, EXIF detection |
| Invitation hide | `invitation_hide.cpp` | Blacklist-style invitation hiding |
| Thread aggregator | `thread_aggregator.cpp` | All-threads cross-room view |
| User messages | `user_messages.cpp` | Per-user cached message history |
| Room version | `room_version.cpp` | Room version selector validation |
| Chat preview | `chat_preview.cpp` | Expanded 2-3x block with compact messages |
| RAM monitor | `ram_monitor.cpp` | Process RSS monitor via /proc |
| Cache manager | `cache_manager.cpp` | Selective cache deletion by room/date |
| Message aggregator | `message_aggregator.cpp` | All-messages pseudoroom |
| Room info | `room_info.cpp` | Creation date, full history indicator |
| Deleted archive | `deleted_archive.cpp` | Pre-deletion message archive |
| Search index | `search_index.cpp` | Full-text search with E2E support |
| Module loader | `module_loader.cpp` | Dynamic .so module loading at runtime |
| Notification | `notification.cpp` | Custom keywords, reaction preview |
| Room mirror | `room_mirror.cpp` | Room-to-room forwarding with doll accounts |
| Input tools | `input_tools.cpp` | Symbol bar, auto-replacement rules |
| LLM | `llm.cpp` | /llm & /llmp commands, OpenAI+Anthropic |
| Read receipts | `read_receipts.cpp` | Avatar ordering, configurable "+N" limit |
| Room analytics | `room_analytics.cpp` | User stats, top posters, server breakdown |
| Chat tools | `chat_tools.cpp` | User hide timer, msg queue, auto-scroll, crop |

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

Current version: **v0.1.5**. Before 1.0.0, all bets are off — rapid iteration is the priority.

### Pre-1.0.0 (now)

- **v0.x.y** — versions are sequential, no strict SemVer meaning
- New features, bug fixes, and **breaking changes** are all expected
- API surfaces, module boundaries, and data formats may shift between versions
- Nightly builds available via GitHub Actions

### Post-1.0.0 (SemVer)

Once the core stabilizes, Progressive Chat will adopt strict [Semantic Versioning 2.0.0](https://semver.org/):

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

Progressive Chat is in active development. We have months of debugging and polishing ahead before the first stable release. Here's how you can contribute:

- **Test and report bugs** — run the F-Droid debug APK, use the features, and open issues
- **C++ code review** — all native modules in `progressive/src/main/cpp/` welcome scrutiny
- **Kotlin integration** — JNI wrappers are ready, UI wiring needs love
- **Translations** — help localize the growing list of labs settings
- **Documentation** — improve setup guides, feature descriptions
- **Spread the word** — tell your Matrix friends about `progressive.chat`

## License

AGPL-3.0-only — inherited from Element and likely to remain forever. All original C++ code and Progressive Chat modifications are AGPL-3.0-only as well.
