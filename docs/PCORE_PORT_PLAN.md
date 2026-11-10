# Port to pcp-core (pcore)

Goal: replace the vendored `matrix-sdk-android` Kotlin SDK with the org's
Qt-free C++ core (`pcp-core`), keeping every user-facing feature.

## Why a bridge
pcore is a native C++ library (E2EE + incremental sync + CS API). Android
consumes it through a thin JNI seam: one Kotlin object per pcore subsystem,
each exposing suspend functions backed by native calls on a dedicated
dispatcher thread owned by the core.

## Phases
| # | Phase | Exit criteria |
|---|---|---|
| 0 | Inventory of matrix-sdk-android usage surface | list of every public API call site, grouped by subsystem |
| 1 | NDK skeleton: libpcore_jni.so loads, JNI_OnLoad, version ping | `PcoreBridge.ping()=="ok"` in debug screen |
| 2 | Session/auth: login, token store, restore | login works against test HS; token survives restart |
| 3 | Sync pipeline: incremental /sync → event stream → RoomStore | room list + timeline render offline from cache |
| 4 | Send path: text, reply, edit, redact, reactions | parity with current send flows |
| 5 | E2EE: olm/megolm via core, key backup | decrypt+encrypt in E2EE rooms |
| 6 | Media: upload/download/attach | parity |
| 7 | Cleanup: delete matrix-sdk-android dependency, shrink APK | no sdk import remains |

## Rules
- No behavior change lands without its phase exit criteria.
- The old SDK stays compile-able until Phase 7 (feature-flagged source set).
- Every bridge function documents the pcore header it wraps.
