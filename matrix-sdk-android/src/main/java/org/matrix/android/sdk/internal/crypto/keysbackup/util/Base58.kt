/*
 * Copyright 2011 Google Inc.
 * Copyright 2020 The Matrix.org Foundation C.I.C.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.matrix.android.sdk.internal.crypto.keysbackup.util

import chat.pcp.app.native.PcpNative

/**
 * Base58 encoding/decoding using C++ native implementation.
 * Ref: https://github.com/bitcoin-labs/bitcoin-mobile-android
 */
internal fun base58encode(input: ByteArray): String = PcpNative.nativeBase58Encode(input)
internal fun base58decode(input: String): ByteArray = PcpNative.nativeBase58Decode(input)
