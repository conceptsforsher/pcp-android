/*
 * Copyright (c) 2022 The Matrix.org Foundation C.I.C.
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

package org.matrix.android.sdk.internal.crypto

import org.matrix.android.sdk.api.session.crypto.MXCryptoError
import org.matrix.android.sdk.api.session.crypto.model.MXEventDecryptionResult
import org.matrix.android.sdk.api.session.events.model.Event
import javax.inject.Inject

internal class EventDecryptor @Inject constructor(val decryptRoomEventUseCase: DecryptRoomEventUseCase) {

    /**
     * Optional native C++ Megolm decryptor for Progressive Chat.
     * Set by the app layer when SETTINGS_LABS_NATIVE_CRYPTO is enabled.
     * Returns null if native decryption is unavailable/fails, falling through to Rust SDK.
     */
    companion object {
        @JvmStatic
        var nativeDecryptAttempt: ((Event) -> MXEventDecryptionResult?)? = null
    }

    @Throws(MXCryptoError::class)
    @Suppress("UNUSED_PARAMETER")
    suspend fun decryptEvent(event: Event, timeline: String): MXEventDecryptionResult {
        // Progressive Chat: attempt native C++ MegolmDecryptor first (Labs-gated)
        val nativeDecryptor = nativeDecryptAttempt
        if (nativeDecryptor != null) {
            val result = nativeDecryptor(event)
            if (result != null) return result
        }
        return decryptRoomEventUseCase.invoke(event)
    }

    @Suppress("UNUSED_PARAMETER")
    suspend fun decryptEventAndSaveResult(event: Event, timeline: String) {
        return decryptRoomEventUseCase.decryptAndSaveResult(event)
    }
}
