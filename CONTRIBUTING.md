# Contributing to Progressive Chat

Progressive Chat is a Matrix client with a **pure C++ native core**.
We're steadily replacing Kotlin/Java with compiled C++ modules.

## Core Philosophy

- **Minimal toolchain** — the goal is to build with `cmake` + `make` alone.
  Android Studio and Gradle are convenience tools, not requirements.
- **Systematic porting** — Kotlin → C++ ports must read the original file first,
  preserve logic line-by-line, and include inline comments with the original code.
- **No regressions** — every C++ module has a Kotlin fallback for when the native
  library isn't loaded.

## Quick Start

```bash
# Build just the C++ native library (no Android SDK needed)
cd progressive/src/main/cpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
# Output: libprogressive_native.so (95+ modules)
```

```bash
# Full Android APK build (needs Android SDK + NDK 21)
./gradlew assembleFdroidRelease
```

## Project Structure

| Path | Purpose |
|------|---------|
| `progressive/src/main/cpp/` | **C++ native modules** (main focus) |
| `progressive/src/main/cpp/include/progressive/` | Headers (1 per module) |
| `progressive/src/main/cpp/src/` | Implementations + `jni_bridge.cpp` |
| `progressive/src/main/cpp/libolm/` | libolm E2EE crypto (git submodule) |
| `vector/src/main/java/` | Kotlin/JNI bridge (being replaced) |
| `matrix-sdk-android/` | Original Matrix SDK (legacy) |
| `docs/` | Documentation, module maps, design notes |

## Porting a Kotlin Module to C++

1. **Read the original** — find the Kotlin file in `matrix-sdk-android/src/` or `vector/src/`
2. **Create header** — `include/progressive/your_module.hpp` with structs + function declarations
3. **Create implementation** — `src/your_module.cpp` with inline `// Original Kotlin:` comments
4. **Add to CMakeLists.txt** — add `src/your_module.cpp` to sources
5. **Add JNI bridge** — add `extern "C"` functions in `src/jni_bridge.cpp`
6. **Add Kotlin fallback** — add `@JvmStatic external fun` declarations in `ProgressiveNative.kt`
7. **Document** — add entry to `docs/cpp_module_map.md`

See existing modules in `progressive/src/main/cpp/include/progressive/` for patterns.

## Code Style

- C++20, 4-space indent, 160 char line limit
- Manual brace-counting JSON parsers (no external JSON library)
- No structured bindings (NDK 21 compatibility)
- Raw strings: `R"(text)"` syntax
- Use `constexpr const char*` for string constants
- Functions return by value, containers use RAII — no manual memory management

## Submitting Changes

1. Fork the repository
2. Create a feature branch
3. Make your changes (read→port→document cycle)
4. Verify C++ compilation: `cmake --build build`
5. Open a pull request against `main`

## What NOT to do

- Don't add dependencies that require package managers (conan, vcpkg, etc.)
- Don't use Android-specific APIs in C++ code
- Don't remove Kotlin fallbacks — every JNI function needs a pure-Kotlin backup
- Don't change the `libolm` submodule version without testing the API compatibility

## Need Help?

Open an issue on GitHub or contact the project author:
- **Anton Maurer** — [GitHub: progressive-chat](https://github.com/progressive-chat)

---

*This project began as a fork of Element Android Classic. See their
[CONTRIBUTING.md](https://github.com/element-hq/element-android/blob/develop/CONTRIBUTING.md)
for the original contributing guidelines.*
