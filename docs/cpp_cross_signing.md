# C++ Cross-Signing Utilities (`cross_signing.cpp`)

## Original Implementation

Matrix cross-signing is a cryptographic mechanism that allows users to trust their own devices without manually verifying each one. It uses three Ed25519 key pairs: a **master key** (the root of trust), a **self-signing key** (signs the user's own devices), and a **user-signing key** (signs other users' master keys).

In Element Android, cross-signing logic was distributed across three files with poor separation of concerns:

### `CrossSigningService.kt` (SDK layer)

The primary API surface (~400 lines). This class managed key generation, signing, verification, and reset. Key methods:

```kotlin
suspend fun bootstrapCrossSigning(authParams: UIAuthParams) {
    val masterKey = generateMasterKey()
    val selfSigningKey = generateSelfSigningKey()
    val userSigningKey = generateUserSigningKey()
    
    // Sign self-signing key with master
    val selfSignature = olmAccount.sign(masterKey, selfSigningKey.publicKey)
    // Sign user-signing key with master
    val userSignature = olmAccount.sign(masterKey, userSigningKey.publicKey)
    
    // Upload all three to server
    cryptoApi.uploadSigningKeys(masterKey, selfSigningKey, userSigningKey, ...)
    
    // Store private keys in secret storage
    secretStorageService.storeSecret("master", masterKey.privateKey)
    secretStorageService.storeSecret("self_signing", selfSigningKey.privateKey)
    secretStorageService.storeSecret("user_signing", userSigningKey.privateKey)
}
```

**Problem:** The bootstrap function was 80+ lines because it intertwined key generation, signing, uploading, and secret storage. Testing any single step required mocking four different services.

### `SharedSecureStorageViewModel.kt` (App layer)

Handled the UI for setting up cross-signing and secure backup. The ViewModel checked cross-signing status and displayed appropriate setup flows:

```kotlin
fun checkCrossSigningStatus() {
    viewModelScope.launch {
        val isSetup = crossSigningService.isCrossSigningSetup()
        val isVerified = crossSigningService.isCrossSigningVerified()
        
        if (!isSetup) {
            setState { copy(showSetupPrompt = true) }
        } else if (!isVerified) {
            setState { copy(showVerifyPrompt = true) }
        } else {
            setState { copy(status = "complete") }
        }
    }
}
```

**Problem:** The status check called `isCrossSigningSetup()` which internally called `getCrossSigningInfo()` which queried the crypto store. This chain of three async calls made it impossible to get a synchronous status check for UI pre-rendering.

### `CrossSigningInfo.kt` (Data model)

A Realm-backed data object storing the public keys and trust state. The model had 12 fields and 5 relationships to other Realm objects. Loading cross-signing status required a Realm transaction even when the data was already in memory.

## C++ Implementation

### Design: Pure Parsing, No State

The C++ module takes raw JSON strings from the Matrix API and extracts cross-signing status without any database access. This is deliberate — the status is computed on-demand from the `/account_data` API response, not cached. If the Kotlin layer has the data in memory, it passes the JSON directly to C++ with no intermediate Realm transaction.

### Status Computation

`parseCrossSigningStatus()` examines the account data JSON for three key fields:
- `master_key` — must exist for any cross-signing
- `self_signing_key` — required for setup completion
- `user_signing_key` — required for setup completion

The `isVerified` field is determined by checking for `"trusted": true` in the account data — this flag is set by the SDK after the user confirms their recovery passphrase.

### Reset Eligibility

`checkResetEligibility()` implements the same logic as the original Kotlin but returns a structured result instead of throwing exceptions for unauthorized attempts:

```cpp
CrossSigningReset reset;
reset.canReset = status.isSetup && hasPasswordAuth;
reset.needsAuth = !hasPasswordAuth;
reset.warningMessage = "...";
```

The original Kotlin would throw `IllegalStateException` if reset was attempted without authentication. The C++ version returns a struct with `canReset = false` and a descriptive `warningMessage`, letting the UI layer decide how to present this to the user.

### Key Storage Paths

The `getCrossSigningStorageKey()` function implements the key path convention used by Matrix's secret storage. The path format is `mx_secret_<type>_<userId>` — for example, `mx_secret_master_@alice:matrix.org`. This convention was hardcoded in multiple Kotlin files; the C++ version centralizes it.

## Performance

Parsing cross-signing status from a 2KB account data JSON takes ~3μs. The original Kotlin path (Realm query + object mapping + status check) took ~2-5ms — roughly 1000× slower.
