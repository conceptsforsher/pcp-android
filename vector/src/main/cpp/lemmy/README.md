# lemmy-client — C++20 Lemmy API Client

A lightweight C++20 client library for the [Lemmy](https://join-lemmy.org/) API (v3).
Designed for embedding into [pcp-android](https://github.com/...), uses the
existing `pcp::HttpClient` (POSIX sockets, no external HTTP deps).

## Structure

```
lemmy/
├── CMakeLists.txt               # Standalone build (static lib)
├── README.md
└── ../src/main/cpp/
    ├── include/pcp/
    │   ├── lemmy_types.hpp       # Data structs (LemmyAuth, LemmyError, ...)
    │   └── lemmy_api.hpp        # API surface (~50 functions)
    └── src/
        └── lemmy_api.cpp        # Implementation (uses pcp/http_client)
```

## API Coverage

| Group | Endpoints |
|-------|----------|
| Site | getSite, createSite, blockInstance |
| Auth | login, register, getCaptcha |
| Community | list, get, follow, block, hide |
| Post | list, get, create, edit, delete, remove, like, save, report, lock, feature, markRead, getSiteMetadata |
| Comment | list, get, create, edit, delete, remove, like, save, report, distinguish, markRead |
| User | get, block, mentions, replies, markAllAsRead, unreadCount, saveSettings, deleteAccount, changePassword |
| PrivateMessage | list, send, edit, delete, markRead |
| Search | search, resolveObject |
| Moderation | getModlog |
| Admin | addAdmin, listRegistrationApplications, approveRegistration |

## Usage

```cpp
#include "pcp/lemmy_api.hpp"

using namespace pcp;

// Set target instance
setLemmyInstance("https://lemmy.ml");

// Login
std::string loginResp = lemmyLogin("username", "password");
std::string jwt = parseJsonStringValue(loginResp, "jwt");

// Browse posts
std::string posts = lemmyListPosts(jwt, "Subscribed", "Active", 1, 20);

// Vote
lemmyLikePost(jwt, postId, 1);   // upvote
lemmyLikePost(jwt, postId, -1);  // downvote

// Comment
lemmyCreateComment(jwt, postId, "Nice post!");

// Get unread counts
std::string counts = lemmyGetUnreadCount(jwt);
```

Return type for all functions is `std::string` (raw JSON). Parse with
`pcp::parseJsonStringValue()` or pass through JNI to Kotlin.

## Build

### As part of pcp-android (Gradle + CMake)

The library is compiled automatically through `externalNativeBuild` in
`vector/build.gradle`. It's part of the `pcp_native` shared library.

```bash
./gradlew :vector:externalNativeBuildDebug
```

### Standalone (CMake)

```cmake
add_subdirectory(lemmy)
target_link_libraries(your_app lemmy_client)
```

Set `PCP_NATIVE_TARGET` if the `pcp_native` target is named
differently in your build.

## Auth Model

Auth tokens (JWT) are passed per-call — the library is stateless. For GET
requests, the token is sent as an `auth` query parameter. For POST/PUT, it's
sent as `Authorization: Bearer <jwt>`.

## License

AGPL-3.0-only — same as pcp-android.
