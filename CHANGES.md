# Changes

## v0.1.5 — Nightly Build

> First nightly build since the C++ native engine was connected to the Matrix sync pipeline.

### Highlights
- **Native sync parser integrated** — C++ `parseSyncResponse()` runs alongside Moshi for validation (Labs-gated)
- **190+ JNI bridges** covering the full API surface: login→sync→timeline→rooms→profile→search→moderation→crypto→polls→spaces→calls→widgets
- **Native SQLite** (SqliteDB + EventDatabase) ready as Realm replacement
- **SQLite amalgamation** downloaded at build time — not stored in repo
- **Cleanup**: 10 duplicate declarations removed from ProgressiveNative.kt, section headers consolidated

### Known issues
- **Initial sync (account import) does not work** — the C++ native sync parser runs alongside Moshi for validation only. Full replacement in v0.2
- Exodus tracker scan may fail (pre-existing, informational only)
- Code quality checks (knit/ktlint/detekt) may fail (pre-existing, `continue-on-error`)
- OpenSSL not linked — AES-CTR and PBKDF2-HMAC-SHA512 unavailable via libolm

### Full changelog
See [git history](https://github.com/progressive-chat/progressive-android/commits/main) for all 26+ commits since v0.1.

---

## v0.2 (in development)
(see below for full v0.2 roadmap)

### Core Architecture
- **Pure C++ native engine** (`libprogressive_native.so`) — 194 modules replacing key SDK subsystems
- **Full login→sync→timeline pipeline** in native C++ with JNI bridges (50+ functions)
- **API surface**: 25+ Matrix endpoints callable from native code (login, sync, send, join/leave, profile, search, moderation, push rules, room messages, filters)

### Database
- **Native SQLite migration begun** — `SqliteDB` (WAL mode, 12-column events + 11-column rooms) and `EventDatabase` as eventual Realm replacements
- SQLite3 amalgamation downloaded at build time (not stored in repo)
- Controlled by Labs flag: `SETTINGS_LABS_NATIVE_DB_KEY`

### Sync & Parsing
- **Native `/sync` response parser** — `parseSyncResponse()`, `parseSyncRooms()`, `parseEvent()` — manual JSON parsing without Moshi
- C++ sync models mirroring all Kotlin data classes (SyncResponse, RoomSync, Timeline, DeviceList, etc.)
- Controlled by Labs flag: `SETTINGS_LABS_NATIVE_SYNC_PARSER_KEY`

### Authentication
- **OIDC/MAS port** from Element X: server discovery via `.well-known`, PKCE, OAuth URL builder, token exchange, callback parser, dynamic client registration
- Password login via native HTTP client

### Networking
- **Native HTTP/1.1 client** — URL parser, request builder, response parser, rate limit header parsing
- **JNI TLS bridge** → `javax.net.ssl.SSLSocket` for HTTPS
- Controlled by Labs flag: `SETTINGS_LABS_NATIVE_HTTP_KEY`

### Timeline
- **Native pagination engine** — `TimelineChunkManager` with display-index arithmetic, reply-map, edit chain detection, thread detection
- Sliding Sync client (MSC3575) ported from Element X
- Controlled by Labs flag: `SETTINGS_LABS_NATIVE_TIMELINE_KEY`

### Content & Relations
- Content builders for 20+ event types: text, image, file, location, polls, room state
- Relation builders: reply (m.in_reply_to), thread (m.thread), edit (m.replace), reaction (m.annotation), wrapper (`m.relates_to`)

### Crypto
- libolm integration: SHA-256, HMAC-SHA256, HKDF-SHA256, Base58 via `_olm_crypto_*` API
- Event encryption metadata parsing, session tracking, missed index detection
- Encryption algorithm detection (megolm, olm, AES-SHA2/256)

### Push Notifications
- UnifiedPush distributor discovery (ntfy, NextPush, FCM, etc.)
- Pusher JSON builder, endpoint registration/unregistration
- Push rule glob matching, notification dedup tracker
- Android notification builder with sound/priority mapping

### Utilities
- **KeyValueStore** — in-memory KVS with JSON serialization (put/get/remove/clear)
- **Logger** — singleton with Debug/Info/Warn/Error levels and optional callback
- **CSV builder** — row builder with proper quoting for commas/quotes
- **Timestamp utilities** — `currentTimeMillis()`, `formatIsoDate()`, `formatIsoDateTime()` (ISO 8601)
- **Feature flags registry** — 15 Labs flags with getters/setters
- **Session config** — timeout, PIN lock, biometric policies
- **String utilities** — SAS bit-math, waveform, HTML unescape, fingerprint formatting, HomeServerVersion, via params, edit validator, URL builder
- **Live draft** — auto-save messages while typing with configurable interval

### UI Changes
- Login splash: lock icon replaced with scales of justice (`ic_login_splash_scales.xml`)
- Registration text: removed Element X recommendation, stated MAS/OIDC coming soon
- Slash commands: `/llm`, `/llmp`, `/agent`, `/web`, `/hideemoji`
- Theme: 65+ SC theme attributes in `attrs_room_message_colors.xml`

### Build & CI
- `applicationId` changed to `chat.progressive.app`
- NDK 21.3.6528147 (libc++), CMake 3.22.1, C++20
- Android ARM32 F-Droid CI with `-PciAbi=armeabi-v7a`
- Gradle heap 3072m, `out-of-process` Kotlin, `--warn` logging
- All new features disabled by default via Labs settings

### Legal
- **AGPL-3.0-only forever** (LICENSE-COMMERCIAL removed)
- README updated with rocket-speed development, Rust removal, 1GB RAM minimum

---

## v0.1 (initial fork)

### Differences from Element Classic Android

| Area | Element Classic | Progressive Chat v0.1 |
|------|-----------------|----------------------|
| **Language** | Kotlin + some Rust (crypto) | Kotlin + C++20 native core |
| **Database** | Realm (encrypted per-session) | Realm + experimental native SQLite |
| **HTTP** | Retrofit + OkHttp | Retrofit + experimental C++ HTTP/1.1 client |
| **JSON parsing** | Moshi (compile-time adapters) | Moshi + experimental C++ manual parser |
| **Crypto** | Rust SDK (matrix-sdk-crypto) | libolm (C/C++ native) |
| **Package ID** | `im.vector.app` | `chat.progressive.app` |
| **License** | AGPL + Element Commercial | AGPL-3.0-only |
| **App name** | Element Classic | Progressive Chat |
| **Homeserver** | Any Matrix server | Any Matrix server |
| **OIDC/MAS** | Partial (MSC3824 awareness) | Ported from Element X |

### Removed from Element Classic
- Rust crypto SDK and all its Kotlin bindings
- Element Commercial license (LICENSE-COMMERCIAL deleted)
- Flipper debug tools
- Registration redirect to Element X
- Buildkite CI integration
- "Verify this device" banner (temporarily hidden)
- Posthog analytics (not configured)

### Kept from Element Classic
- Full Kotlin UI layer (all Activities, Fragments, Views, ViewModels)
- Realm database as primary storage (native SQLite is opt-in)
- Retrofit/OkHttp as primary HTTP stack (native client is opt-in)
- Moshi JSON parsing (native parser is opt-in)
- All Matrix SDK APIs and data classes
- E2EE key management and session handling
- Voice/video calls via Jitsi
- Push notifications via FCM/UnifiedPush
- All existing settings and preferences
