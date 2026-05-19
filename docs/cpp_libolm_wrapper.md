# C++ libolm E2EE Crypto Wrapper (`olm.cpp`)

## Overview

A complete C++ wrapper around the libolm C library — the reference implementation of Matrix's Double Ratchet and Megolm encryption algorithms. This module provides the same cryptographic primitives that Element Android used before migrating to the Rust crypto SDK (PR #8901).

**Strategic importance:** Bringing libolm back into Progressive Chat as a C++ native module eliminates the Rust crypto dependency entirely. Every E2EE operation — key generation, session establishment, message encryption/decryption, device verification — runs through our own C++ code calling directly into the C libolm library.

## Why libolm

libolm is **already C**. Unlike the Rust crypto SDK (`matrix-rust-sdk`) which brings in an entire Rust toolchain, libolm compiles with any C99 compiler. By wrapping it in C++ classes, we get:

1. **Zero-Rust build.** No `cargo`, no `uniffi`, no Rust NDK integration.
2. **Smaller binary.** libolm is ~200KB; Rust crypto SDK is ~4MB per ABI.
3. **Auditable codebase.** libolm's C API is 24 functions. Rust crypto SDK's API is 200+ methods.
4. **Historical compatibility.** Element Android's realm database still has migration paths from olm to rust — we can reverse them.

## Module Structure

```
include/progressive/olm.hpp          — C++ class declarations
src/olm.cpp                           — Implementation (wraps libolm C API)
```

To enable, add libolm source and uncomment in CMakeLists.txt:
```cmake
add_subdirectory(libolm)
target_link_libraries(progressive_native olm)
target_compile_definitions(progressive_native PRIVATE PROGRESSIVE_HAS_OLM=1)
```

## Cryptographic Primitives

### OlmAccount — Identity Keypair

Manages a user's permanent identity: one Curve25519 key for encryption and one Ed25519 key for signing.

```
create()              → generates random identity keypair
identityKeys()        → returns {"curve25519":"...", "ed25519":"..."}
generateOneTimeKeys(n)→ creates n one-time pre-keys for session initiation
sign(message)         → Ed25519 signature
pickle(key)/unpickle()→ serialize/deserialize for persistent storage
```

### OlmSession — Double Ratchet (1:1)

Implements the Signal Protocol's Double Ratchet algorithm for pairwise encrypted communication.

**Outbound session (Alice initiates):**
```
OlmSession::createOutbound(account, bobIdentityKey, bobOneTimeKey)
    → generates root key + chain key from ECDH shared secret
    → outputs pre-key message to send to Bob
```

**Inbound session (Bob receives):**
```
OlmSession::createInbound(account, preKeyMessage)
    → derives shared secret from received pre-key
    → establishes matching ratchet state
```

**Message exchange:**
```
encrypt("hello")  → advances sender chain, outputs ciphertext
decrypt(ciphertext) → advances receiver chain, outputs plaintext
```

Each encrypt/decrypt call **advances the ratchet forward**. This provides forward secrecy — compromising one message key does not reveal past messages.

### Megolm — Group Ratchet

Used for room encryption. A single outbound session encrypts for all room members; each member has an inbound session derived from the shared session key.

**Outbound (sender):**
```
OlmOutboundGroupSession::create()     → new megolm session
encrypt("room message")                → advances ratchet, outputs ciphertext
sessionKey()                           → key to share with new members
```

**Inbound (receiver):**
```
OlmInboundGroupSession::create(sessionKey) → derive from shared key
decrypt(ciphertext)                        → decrypt with message index check
isVerified()                               → Ed25519 signature verified
```

### OlmSAS — Short Authentication String

Implements the SAS device verification protocol:
1. Alice and Bob exchange public keys
2. Both compute a 6-byte SAS using the ECDH shared secret
3. The SAS is displayed as 7 emoji or decimal numbers
4. Users compare visually or via QR code

## JNI Integration Plan

When libolm is linked, the following JNI functions will be added:

```kotlin
// Account management
ProgressiveNative.nativeOlmCreateAccount(): String         // → identity keys JSON
ProgressiveNative.nativeOlmGenerateOneTimeKeys(count: Int) 

// Session establishment
ProgressiveNative.nativeOlmCreateOutboundSession(theirIdKey, theirOtKey): String
ProgressiveNative.nativeOlmCreateInboundSession(preKeyMsg): String

// Encryption
ProgressiveNative.nativeOlmEncrypt(sessionId, plaintext): String
ProgressiveNative.nativeOlmDecrypt(sessionId, ciphertext): String

// Megolm
ProgressiveNative.nativeMegolmCreateOutbound(): String     // → session key
ProgressiveNative.nativeMegolmEncrypt(sessionId, plaintext): String
ProgressiveNative.nativeMegolmDecrypt(sessionKey, ciphertext): String
```

## Migration from Rust Crypto

The Rust crypto SDK stores sessions in a dedicated SQLite database. To migrate:

1. **Read Rust sessions** via the existing `RustCryptoService` API (temporary, during transition)
2. **Export** each session as its raw key material (session key + message index)
3. **Import** into libolm via `OlmInboundGroupSession::create(sessionKey)`
4. **Pickle** the libolm session for persistent storage
5. **Switch** the send path to use `OlmOutboundGroupSession::encrypt()`

This can be done incrementally — both crypto backends can coexist during migration.

## Security Properties

| Property | Provided by |
|----------|------------|
| Confidentiality | AES-256-CBC (Megolm payload encryption) |
| Integrity | HMAC-SHA256 (Megolm message authentication) |
| Forward secrecy | Double Ratchet (each message advances the chain) |
| Post-compromise security | Double Ratchet (new ECDH on each round trip) |
| Identity verification | Ed25519 signatures + SAS protocol |
| Deniability | No digital signatures on messages (MAC only) |
