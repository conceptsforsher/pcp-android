# C++ Authentication Utilities (`auth_utils.cpp`)

## Original Implementation

The Kotlin authentication code was spread across three files in the Matrix SDK:

**`AuthenticationService.kt`** — The main entry point. A Dagger-injected service that orchestrated login, registration, and token refresh. Used `suspend fun` with Retrofit for all API calls. The authentication flow logic was mixed with UI state management (sealed classes for `LoginState`, `RegistrationState`) making it hard to unit test independently of the Android framework.

**`DefaultLoginWizard.kt`** — Handled the multi-stage login flow. When a server required additional authentication (captcha, terms of service, email verification), this wizard managed the state machine. Each stage produced a `LoginWizardState` sealed class that the UI layer rendered. The wizard itself was ~200 lines of `when()` branches checking for `M_LIMIT_EXCEEDED`, `M_UNAUTHORIZED`, and `M_TERMS_NOT_SIGNED` error codes.

**`DefaultRegistrationWizard.kt`** — Similar to login but with additional stages for username validation, email verification, and CAPTCHA. The username availability check (`isUsernameAvailable()`) made a separate API call that wasn't part of the wizard state machine, leading to race conditions where the UI showed "available" but the registration failed.

### Why Port to C++

1. **Error code parsing was string-heavy.** Every HTTP 401/403 response required parsing JSON to extract `errcode` and `error` fields. In Kotlin, this created `org.json.JSONObject` instances that were immediately discarded — pure GC pressure for a simple string extraction operation.

2. **Rate limiting was reactive.** The `M_LIMIT_EXCEEDED` error was handled by Kotlin coroutines with `delay(retryAfterMs)`. This tied the retry logic to the coroutine scope lifecycle — if the user navigated away during a delay, the coroutine was cancelled and the retry never happened.

3. **Auth flow parsing was fragile.** The `flows` array in the server response contains multiple authentication pathways. The Kotlin code only used the first flow. Our C++ parser extracts ALL flows and the Kotlin layer can choose intelligently.

## C++ Implementation

### AuthFlow Parsing

The `parseAuthFlow()` function takes a raw HTTP response and extracts:
- `session` — the authentication session ID
- `flows` — array of available authentication methods
- `stages` — the specific stages required (e.g., `["m.login.password", "m.login.terms"]`)

The parser handles both `401` (authentication required) and `403` (authorization required) responses, which the Matrix spec unfortunately uses inconsistently.

### Rate Limit Handling

`parseRateLimit()` extracts the `retry_after_ms` field from 429 responses. The C++ function is stateless — it doesn't manage the retry timer. Instead, it returns a `RateLimit` struct that the Kotlin layer can use with whatever timer mechanism it prefers (WorkManager for background, Handler for foreground).

### CAPTCHA Integration

`parseCaptchaInfo()` extracts the reCAPTCHA public key from the server's registration parameters. The `buildCaptchaResponse()` function constructs the authentication stage body with the user's captcha token. The separation of parsing from request building means the same code works for both login and registration captcha challenges.

### Key Design Decision: Stateless Functions

All functions in this module are pure computation — they take inputs and return outputs with no side effects and no stored state. This is a deliberate departure from the Kotlin originals which were stateful singletons. By keeping the C++ layer stateless:

1. **Thread safety is free.** No mutexes, no synchronization.
2. **Testing is trivial.** Every function is deterministic: same input → same output.
3. **Kotlin retains control.** The UI layer decides when to call these functions, not the other way around.
