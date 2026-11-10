# Client comparison: PCP vs Voyage vs NeoChat

All Matrix chat clients for Android with different native / JVM splits.
Measured 2026-09-30. LOC = lines via `find ... -exec cat {} + | wc -l` (excl. `.git`, excl. `po/`).

## Code size

|  | PCP (this repo) | Voyage | NeoChat |
|---|---|---|---|
| Repo | local (`pcp-android`) | `https://github.com/VoyageClient/Voyage` (`develop`) | `https://github.com/KDE/neochat` (`master`) |
| Base | Element Classic fork, Rust SDK removed, libolm C++ | Element Classic fork (v1.6.62 upstream) | Standalone Qt/QML + libQuotient |
| Native own | 892x `.cpp` 322,803 + 567x `.hpp/.h` 35,876 = ~358,679 | 0 C++ (GitHub languages: Kotlin/Java only) | 130x `.cpp` 29,914 + 140x `.h` 19,117 + 218x `.qml` 26,371 = 75,402 |
| JVM own | 4,196x `.kt` 345,829 + 35x `.java` 7,827 = ~353,656 | Kotlin 19,019,248 bytes + Java 137,807 bytes (GitHub API) | 0 `.java`/`.kt` (`android/` = 6 files: manifest + gradle + res) |
| Framework JVM (deps) | Android SDK / Jetpack (external) | Android SDK / Jetpack (external) | Qt6 `src/android/jar` 54x Java 11,214 + `src/android/java` stubs 49 + KUnifiedPush 3x Java 185 = ~11,448 |
| Key native dep | `pcp/src/main/cpp` + libolm | none (matrix-sdk-android Kotlin) | libQuotient 118x `.cpp` ~14,333 + 158x `.h` ~19,349 = ~33,682, 0 Java |
| UI | Native Android Views (Kotlin) + JNI C++ | Native Android Views (Kotlin) | QtQuick / Kirigami QML via `QtActivity` (`android/AndroidManifest.xml:12-13`) |
| Min Android | API 21 (target API 19, README.md:115-116) | API 14 (Android 4.0) | KDE nightly F-Droid (`https://cdn.kde.org/android/nightly/fdroid/repo`) |

## Deep dive

### Architecture / bridge

|  | PCP | Voyage | NeoChat |
|---|---|---|---|
| Language core | Hybrid: Kotlin UI + C++20 core (`CMAKE_CXX_STANDARD 20`, `pcp/src/main/cpp/CMakeLists.txt:5`), NDK 21.3+ / CMake 3.22+ | JVM-only: Kotlin 1.9.24, Java 17, no C++/Rust in-repo | Native-first: C++20 + QML, 0 custom JVM; JVM only from Qt framework |
| Bridge | `jni_bridge.cpp` 6,543 lines, ~840 JNI funcs (`JNI_FUNC` x831 + 17x `Java_...`), 65 file-static singletons, 18 wired in `PcpNative.kt` (`docs/cpp_jni_bridge.md:5,26,36`) | Standard Element Classic (Mavericks/Epoxy, Hilt, no JNI) | Qt for Android: `QtApplication/QtActivity` + `qtAndroidDir/src` (`android/build.gradle:58`), `libneochat` QML plugins |
| Build | `./gradlew assembleFdroidDebug`, F-Droid arm32 (`README.md:103-109`) | `./gradlew installDebug` / `installRelease` (R8-shrunk release runs on ICS, `vector-app/build.gradle`) | Craft + `ECMAddAndroidApk`, nightly F-Droid repo |

### Crypto / E2EE

|  | PCP | Voyage | NeoChat |
|---|---|---|---|
| Backend | libolm C wrapper (`docs/cpp_libolm_wrapper.md`): `OlmAccount/Session/Megolm/SAS`, ~200KB, `PCP_HAS_OLM`; Rust `matrix-rust-sdk-crypto.aar` being removed (`README.md:5`, `docs/rust_crypto_integration.md`) | `libce` submodule replaces vodozemac (old-device builds); dehydrated devices MSC3814, encrypted state MSC4362, encrypted account data MSC4483, SAS/identity pinning (`CHANGES.md`) | libQuotient `Quotient/e2ee/` + `Quotient/crypto-sdk/` = Rust `matrix-sdk-crypto 0.16` via `cxx` staticlib (`Cargo.toml`) — i.e. C++/QML app, Rust crypto underneath |
| Status | Migration Rust->libolm in progress, coexistence planned | Full E2EE + key backup, share-history-on-invite MSC4268 | Gaps per upstream README: no VoIP, no threads, partial E2EE |

### Persistence / sync

|  | PCP | Voyage | NeoChat |
|---|---|---|---|
| DB | `eventdb.cpp` SQLite-native replaces Realm queries; `search_index.cpp` full-text with E2EE (`README.md:68-92`) | Realm replaced by SQLDelight + custom framework-SQLite driver, WAL, bulk queries replace N+1s (`CHANGES.md: Under the hood`) | libQuotient connection + SQLite (no Realm); `WITH_UNIFIEDPUSH ON` |
| Sync | Element Classic sync + `sync_analyzer.cpp` | MSC4525 paginated sync + MSC4186 simplified sliding sync, MSC4222 state-at-timeline-end, MSC3981 thread relations; single-alarm background sync + push retry queue | Quotient sync; no equivalent mobile background-sync work |

### Media / calling / push

|  | PCP | Voyage | NeoChat |
|---|---|---|---|
| Media | `audio_engine.cpp`, `chunked_upload.cpp` zero-copy, `markdown.cpp`, on-device `translate.cpp` (OpenAI-compat) | ExoPlayer 2.19.1 (API 16+, ICS keeps MediaPlayer) + pre-send image/video/audio editors, metadata stripping, JPEG XL, blurhash, MSC4274 galleries (Labs), MSC2246 early-send, NLLB-200 on-device translate ~1GB; `libopus` submodule R8-optimized | Qt Multimedia, KQuickImageEditor, blurhash (`src/app/blurhash.cpp`); `src/messagecontent/` 6/6/38, `src/timeline/` 13/14/16 |
| Calling | Kept: `jitsi-meet-sdk:11.5.1` (`pcp/build.gradle:319`) + 15x `ElementCall*` Kt + C++ `call_manager.cpp/call_signaling.cpp/webrtc_utils.cpp/lightweight_call.cpp` | Removed entirely (`jitsi|webrtc` 0 hits in `vector/build.gradle`): call events render inline | None: 1 hit `pushrulemodel.cpp:46` `TODO when VOIP added`; no `PlaceCall/CallHandler` |
| Push | FCM + UnifiedPush `android-connector:2.1.1` (`pushers/`: `UnifiedPushHelper.kt/PcpUnifiedPush.kt/PushersManager.kt`) + `proxy.cpp` (SOCKS5/Tor/I2P); C++ `push_controller.cpp/push_rules*/unified_push.cpp` | No FCM (`firebase-messaging` defined never implemented, no `google-services`); `unifiedpush.android:connector:3.0.10` only; retry-until-notified queue + push history + gateway picker; Conscrypt 2.5.3 + okhttp 3.12.13 (KitKat-compat) | KUnifiedPush C++-only (`KUnifiedPush::Connector` in `controller.cpp:171-186`, `listenForNotifications()` + 5s quit, D-Bus service non-Android); KF6 Notifications (`notificationsmanager.cpp:213 message/298 invite/146 decryptNotification`); `AndroidManifest.xml:47 background_running=false`, no ForegroundService/WorkManager — sync-poll only |
| Telemetry | PostHog 3.2.0 + Sentry 8.31.0 (`SentryAnalytics.kt`, `DefaultPcpAnalytics.kt`) | 0 (Sentry/analytics/bug-report/content-report/sunset all removed) | Sentry `sentry-android-core:8.43.0` (`android/build.gradle:30`, dsn `crash-reports.kde.org/18`); no PostHog |

## Even deeper

### PCP: compiled vs total, JNI wiring

* 891x `.cpp` total, 496 compiled (`CMakeLists.txt`: 496 `src/*.cpp`); rest ~395 `batch_*/big_*/mega_*/ngen_*/round3_*/sdkport_*/ktport_*/tz_*` generated/shim bulk. 566 headers.
* `CMakeLists.txt:562-580`: libolm via FetchContent `gitlab.matrix.org/matrix-org/olm.git` TAG 3.2.16, `OLM_TESTS OFF`, link `olm`, `PCP_HAS_OLM=1` (not vendored). `CMakeLists.txt:505-560`: SQLite amalgamation 3450200 zip downloaded at configure time, flags `-Os -flto --gc-sections -fvisibility=hidden`.
* `pcp/build.gradle:34`: NDK `21.3.6528147`, CMake 3.22.1 (`gradle.properties:47 cmake.buildParallelLevel=1`).
* JNI: `jni_bridge.cpp` 6,543 lines = 19x explicit `JNIEXPORT` (`:525-1078`) + 831x `JNI_FUNC` macro (`:320-321`) = ~850 entry points, `JNI_OnLoad :6330`, 65 file-static singletons. Kotlin: `pcp/.../PcpNative.kt:25-60` 18x `external fun` + `matrix-sdk-android/.../PcpNative.kt` 33x `external fun` (separate copy). No per-call fallback, only `UnsatisfiedLinkError` around `loadLibrary`. `jni_stubs_part1-6.cpp`: placeholders. `tests/test_pcp.cpp` 2,383 lines ~780 asserts. `API.md` 386 lines claims same; `docs/cpp_module_map.md` 76-row table.
* Still Realm: `matrix-sdk-android/build.gradle:5` `realm-android` + `realm-gradle-plugin:10.19.0` + `monarchy:0.7.1`; `io.realm` in 286x `.kt` (0 in `pcp/src/main/java`). No SQLDelight/Room. Rust crypto active: `crypto-android:26.1.28` (`:224`); C++ olm mirror 1,375 lines (`olm.cpp:318` + `olm_manager.cpp:434` + `olm_session.cpp:512` + `olm_account_manager.cpp:111`) + `megolm_*/sas_*/crypto_ops.cpp`.
* Toolchain: minSdk 21 / compile+target 35 / Java 21 / jvmTarget 21 / Kotlin 1.9.24 / coroutines 1.8.1; ABIs `armeabi-v7a/x86/arm64-v8a/x86_64` (CI `-PciAbi`).

### Voyage: 39 MSCs, SQLDelight, libce

* Linguist `Kotlin 19,019,248 + HTML 383,518 + Java 137,807` = no C++; caveat: native in submodules `.gitmodules`: `libce` (`VoyageClient/libce@fff6641`, `library/external/libce`) + `libopus` (xiph/opus).
* `CHANGES.md` MSCs (39): 1234,2246,2545,2723,2867,3015,3765,3787,3814,3827,3946,3952,3981,4095,4133,4175,4186,4221,4222,4230,4247,4262,4267,4268,4274,4277,4362,4380,4391,4426,4427,4429,4440,4441,4483,4522,4525,4529,4549. Crypto: 4362/3814/4268/4483 + identity banner pinning + hideable shields + PGP/OpenKeychain `/encrypt`. Sync: 4525 preferred + 4186 + legacy fallback (Labs off), 4222/3981/4429+4262/3946/4267/4277 stable.
* Persistence: `sqldelight:2.0.2` (`gradle-plugin/runtime/coroutines-extensions`) + custom framework-SQLite driver; `matrix-sdk-android/src/main/sqldelight`: `auth/contentscanner/crypto/eventindex/global/identity/redactedcontent/session` (8 DBs). Realm hits = comments only. Perf: WAL + reactive dedup + epoxy rework + gated space-hierarchy + bulk queries (N+1 fix) + memoized mapper.
* Crypto: `project(':library:external:libce')` in `vector` + `matrix-sdk-android`; 0x `vodozemac`; `olm` hits = `exclude module:'olm-jvm'` only.
* App: `applicationId im.voyage.app`, upstream `1.6.62` kept as SDK version, `versionCode=YYYYMMDD+20,000,000`, splits + `universalApk:true`, `multidex:true`. Debug unshrunk = KitKat+; release R8 shrink (obfuscate off, keep Signature) fits ICS 8MB LinearAlloc. API 14-18 needs rooted Dalvik-verifier disable (`/data/local.prop`). `desugar_jdk_libs:2.1.5`, Kotlin 1.9.24 / AGP 8.11.0 / KSP 1.9.24-1.0.20.
* Background sync: single alarm chain + bounded retry (not WorkManager; `work-runtime-ktx:2.8.1` kept for other jobs); foreground-return forces sync with progress.

### NeoChat: src map, Rust-underneath, Qt packaging

* `src/` 12 subdirs: `app/ 23/23/52 (main.cpp/controller.cpp:401L/models/qml)`, `libneochat/ 55/65/9 (connection/room/eventhandler/models/jobs/events)`, `messagecontent/ 6/6/38`, `settings/ 11/11/38 (7 models: devices/pushrule/permissions)`, `timeline/ 13/14/16 (messagemodel/Bubble/ThreadBody)`, `chatbar/ 6/6/21`, `rooms/ 6/6/10`, `login/ 2/2/14`, `spaces/ 3/3/4`, `roominfo/ 1/1/9`, `devtools/ 3/3/7`, `purpose/ 1/0/0`.
* Startup `main.cpp:176-177`: `setEncryptionDefault(true)` + direct-chat encryption default; `:293` loads `org.kde.neochat/Main`; `:193-194,210-229` `HAVE_KUNIFIEDPUSH` + `--dbus-activated` push-only mode + `FakeRunner` guard.
* E2EE delegated to libQuotient (`connection.h:139,143,153-158,757-777,939-973`): `exportMegolmSessions` (`neochatconnection.cpp:580-599`), `unlockSSSS*` (`:615-649`), `ManageKeyStorageDialog` (backup/SSSS), `KeyVerificationDialog` states + `EmojiSas`. `grep src/`: `olm=15 verif=271 backup=26 cross-sign=6`. Threads present (`thread.h/cpp`, `neochatroom.h:628,634`, `ThreadBody*.qml`) — earlier "no threads" gap is stale.
* libQuotient v0.10.0: `e2ee/` 6 files, `csapi/` 125 files (62h+63cpp) + `definitions/` 19, `crypto-sdk/Cargo.toml`: `matrix-sdk-crypto/common/sqlite 0.16 (crypto-store)`, `cxx=1.0.138`, `tokio full`; `src/cryptomachine.rs 2,138L/lib.rs 531L/request.rs 334L`; `CMakeLists:102 Corrosion 0.6.1 REQUIRED` + `corrosion_import_crate` + link `matrix_rust_sdk_crypto_cpp`. Rust mandatory.
* Requires: `KF 6.26` + `Qt 6.10` (`Core/Quick/Gui/QuickControls2/Multimedia/Svg/TextToSpeech/WebView` + desktop `Widgets`), `KirigamiAddons 1.12.0`, `QuotientQt6 0.9.5`, `QCoro6 0.4`, `KQuickImageEditor`, `cmark`, `OpenSSL` (Android REQUIRED). Tests need `Test/HttpServer/QuickTest`.
* Android: 3 perms only (`INTERNET/ACCESS_NETWORK_STATE/POST_NOTIFICATIONS`), `Gradle 8.6.0`, `applicationId org.kde.neochat`, `qtMinSdkVersion` not in repo — from Qt (`qtbase/.../build.gradle:81-82`, default `minSdk 28`).

## UI navigation: Activities vs QML pages

|  | PCP / Voyage | NeoChat |
|---|---|---|
| Android Activity | Dozens (`VectorApp`, `MainActivity`, `RoomDetailActivity`, `LoginActivity`, ... in `vector-app/src`) | 1: `org.qtproject.qt.android.bindings.QtActivity` (`android/AndroidManifest.xml:13-18`, `lib_name=neochat-app`) |
| Navigation | Jetpack Fragments + Mavericks/Epoxy controllers per screen | `Kirigami.ApplicationWindow.pageStack` (`src/app/qml/Main.qml:14,75-82`): states `noConnection/loading/noRoom/room` (`:41-73`) swap `items: [welcomePage]/[loadingPage]/[roomListPage]/[roomListPage,roomPage]`; `ColumnView DynamicColumns` if `wideScreen (>65 gridUnit :39)` else `SingleColumn`; dialogs via `pushDialogLayer(Qt.createComponent('org.kde.neochat',...))` |
| Screen unit | Fragment (`HomeFragment`, `RoomDetailFragment`) | `Kirigami.Page` e.g. `RoomPage.qml:16` (`currentRoom/timelineModel/messageFilterModel/mediaMessageFilterModel` props), `TimelineView.qml`, `SpaceHomePage.qml`; 201x `import org.kde.kirigami`, 72x `formcard`, 24x `delegates` |
| Backend binding | ViewModel -> Matrix SDK | `Controller.activeConnection` (`Main.qml:17`), `RoomManager.currentRoom/timelineModel`, `NeoChatConnection/Room` C++ (`Q_PROPERTY`) directly in QML |

### How to design NeoChat screens (no Android Studio)

Hand-write QML, not visual Activities. Toolchain:

* `Qt Creator` Design mode / Qt Quick Designer — works for plain `QtQuick.Controls`, breaks on custom `Kirigami.*`/`org.kde.neochat` types; KDE devs keep it off, use Code mode + `qmlscene` live reload.
* `Qt Design Studio` — designer+developer workflow (`doc.qt.io/qtdesignstudio`): designers mock in `.ui.qml`, devs bind to C++ models; usable for isolated delegates, not full `Main.qml` (needs live `Controller/RoomManager`).
* `Kirigami Gallery` (`kirigami-gallery` app) — component catalog to copy `Page/ScrollablePage/FormCard/OverlaySheet/ColumnView` patterns; NeoChat `RoomPage header: ColumnLayout`, `pushDialogLayer` follow it.
* Fast loop: `kde-builder --run neochat` (Linux) or Craft Android deploy; edit `src/app/qml/*.qml` + `qmlcachegen` rebuild; `QT_QML_GENERATE_QMLLS_INI ON` (`CMakeLists.txt:28`) gives QML language-server completion.
* Add screen: new `FooPage.qml` (`Kirigami.Page`), register in `src/<mod>/CMakeLists.txt` via `ECMQmlModule`, push with `pageStack.pushDialogLayer(Qt.createComponent('org.kde.neochat','FooPage'),{...})` — same pattern as `RoomSearchPage/MessageSourceSheet` in `RoomPage.qml:140,411`.

Summary: PCP = hybrid (largest both sides, Kotlin UI + growing C++ core). Voyage = JVM-only Element modernization, max compat. NeoChat = native-first (100% C++/QML own code, JVM only from Qt framework).

## Appendix: install QML tooling (Arch, verified 2026-09-30)

Already present: `kirigami 6.29`, `kirigami-addons 1.13.1`, `qt6-declarative 6.11.2`, `qt6-tools 6.11.2` (`qmlls6`, `qmlformat`, `qmlimportscanner`), `kate`, `qmlscene`.

```bash
sudo pacman -S qtcreator kirigami-gallery
# kde-builder not in repos:
pip install --user kde-builder
# or: git clone https://invent.kde.org/sdk/kde-builder.git ~/.local/share/kde-builder
```

Optional: `Qt Design Studio` not in Arch repos — Qt online installer from `qt.io` only; skip for Kirigami (visual designer breaks on `org.kde.kirigami`/`org.kde.neochat` types, use Code mode + `qmlscene` reload + `kirigami-gallery` patterns).
