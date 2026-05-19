# C++ JNI Bridge Architecture (`jni_bridge.cpp`)

## Overview

The JNI bridge is the single translation layer between Kotlin and all 80+ C++ native modules in Progressive Chat. It is the largest file in the project (~3400 lines) and follows a strict naming convention mandated by the Java Native Interface specification.

## How JNI Naming Works

Every native function must follow the exact naming pattern:

```
Java_<package>_<class>_<method>
```

Where dots in the package name are replaced with underscores. For Progressive Chat:

```
Kotlin:  im.vector.app.features.jumptodate.ProgressiveNative.nativeParseUrl()
JNI:     Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseUrl
```

The Kotlin class `ProgressiveNative` at package `im.vector.app.features.jumptodate` produces JNI function names with prefix `Java_im_vector_app_features_jumptodate_ProgressiveNative_`.

## Singleton Architecture

All C++ modules are accessed through **global singleton instances** — one static variable per module, declared at the top of `jni_bridge.cpp`:

```cpp
static progressive::EventCache        g_eventCache;
static progressive::KeywordFilter     g_keywordFilter;
static progressive::NetworkStatsCollector g_netStats;
static progressive::UserMaskRegistry  g_userMasks;
static progressive::DesyncDetector    g_desyncDetector;
static progressive::LatencyTracker    g_latencyTracker;
// ... 20+ more singletons
```

**Why singletons?** JNI functions are C-style free functions with no `this` pointer. They cannot access instance state directly. By holding state in static globals, each JNI call can access the same module instance that previous calls modified.

**Thread safety:** All JNI calls originate from the Kotlin main thread (Android UI thread). Since JNI serializes through the single JVM thread, no mutex is needed.

## Data Flow: Kotlin → C++ → Kotlin

```
1. Kotlin calls ProgressiveNative.nativeValidateAndBuild(roomId, date, server, token, enabled)
       │
       ▼ jstring parameters
2. JNI receives: JNIEnv* env, jclass, jstring jRoomId, jstring jDateString, ...
       │  GetStringUTFChars() converts jstring → const char*
       │  constructs progressive::JumpToDateRequest
       ▼
3. C++ processes: progressive::validateAndComputeTimestamp(request)
                  progressive::buildMsc3030Url(request)
       │  Returns JSON string with url + accessToken
       ▼
4. JNI returns: env->NewStringUTF(json.c_str())
       │
       ▼ String result
5. Kotlin receives: val json = JSONObject(result)
```

## Memory Management Rules

**Critical rule:** Every `GetStringUTFChars()` must be paired with `ReleaseStringUTFChars()`. Failure to release causes JNI memory leaks.

```cpp
// CORRECT:
const char* chars = env->GetStringUTFChars(jParam, nullptr);
std::string cppString(chars);
env->ReleaseStringUTFChars(jParam, chars);

// INCORRECT — never released:
std::string cppString(env->GetStringUTFChars(jParam, nullptr));
```

**NewStringUTF()** allocates a Java string on the JVM heap. The C++ side can discard its copy immediately after — the JVM manages the returned string's lifetime.

## Namespace Usage

All C++ functions live in the `progressive` namespace. To avoid qualifying every type with `progressive::`, the bridge file uses:

```cpp
using namespace progressive;
```

This is placed once at the top of `jni_bridge.cpp` after all `#include` directives. It applies only to this translation unit — other `.cpp` files must qualify or use their own `using` directive.

## Kotlin Fallback Pattern

Every JNI function has a **pure-Kotlin fallback** in `ProgressiveNative.kt` for when `libprogressive_native.so` fails to load:

```kotlin
fun parseResponseFallback(body: String?, httpStatus: Int): JSONObject {
    try {
        val json = JSONObject(body ?: return result.put("error", "Empty response"))
        // ... same logic as C++ but in Kotlin/Java
    } catch (e: Exception) {
        return result.put("error", "Failed to parse")
    }
}
```

The fallback is triggered by catching `UnsatisfiedLinkError`:

```kotlin
val result = try {
    JSONObject(ProgressiveNative.nativeParseResponse(body, code))
} catch (e: UnsatisfiedLinkError) {
    ProgressiveNative.parseResponseFallback(body, code)
}
```

## Adding a New JNI Function

1. **Create C++ module:** `include/progressive/new_module.hpp` + `src/new_module.cpp`
2. **Add to CMakeLists.txt:** `src/new_module.cpp`
3. **Add singleton to jni_bridge.cpp:** `static progressive::NewModule g_newModule;`
4. **Add JNI function:** Follow the naming convention
5. **Add Kotlin declaration in ProgressiveNative.kt:**
   ```kotlin
   @JvmStatic external fun nativeNewFunction(param: String): String
   ```
6. **Optional: Add Kotlin fallback** in `ProgressiveNative.kt`

## Performance Characteristics

| Operation | Time | Notes |
|-----------|------|-------|
| JNI call overhead (empty) | ~50 ns | Function call boundary |
| GetStringUTFChars (1KB) | ~200 ns | String copy to native heap |
| NewStringUTF (1KB) | ~300 ns | String copy to JVM heap |
| jint/jboolean pass | ~5 ns | Direct register pass |
| jlong/jdouble pass | ~10 ns | Direct register pass |

Total JNI overhead per call: ~500ns for string parameters, ~50ns for primitive parameters. This is negligible compared to the C++ processing time (typically microseconds to milliseconds).
