# PCP — Authors

PCP is developed and maintained by **Anton Maurer**.

The project is a fork of [Element Android Classic](https://github.com/element-hq/element-android).
Most of the Kotlin/Java codebase originates from Element and its contributors.
You can read about the original Element Android authors in their
[AUTHORS.md](https://github.com/element-hq/element-android/blob/develop/AUTHORS.md).

## C++ Native Core

The `libpcp_native.so` C++ library (165+ modules in `vector/src/main/cpp/`)
is original work by Anton Maurer, gradually replacing Kotlin/Java components
with high-performance native code. All C++ modules carry the pcp
copyright alongside the original Element SPDX license.

## License

All original C++ code is licensed under AGPL-3.0-only, matching the inherited
license from Element Android.
