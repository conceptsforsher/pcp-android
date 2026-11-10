/*
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

package org.matrix.android.sdk.internal.util

import chat.pcp.app.native.PcpNative
import timber.log.Timber
import java.util.Locale

internal fun convertToUTF8(s: String): String {
    return try {
        String(s.toByteArray(Charsets.UTF_8))
    } catch (e: Exception) {
        Timber.e(e, "## convertToUTF8() failed")
        s
    }
}

internal fun convertFromUTF8(s: String): String {
    return try {
        String(s.toByteArray(), Charsets.UTF_8)
    } catch (e: Exception) {
        Timber.e(e, "## convertFromUTF8() failed")
        s
    }
}

internal fun String.caseInsensitiveFind(subString: String): Boolean {
    if (subString.isEmpty() || isEmpty()) return false
    return try {
        Regex("(\\W|^)" + Regex.escape(subString) + "(\\W|$)", RegexOption.IGNORE_CASE).containsMatchIn(this)
    } catch (e: Exception) {
        Timber.e(e, "## caseInsensitiveFind() failed")
        false
    }
}

internal val spaceChars = "[\\u00A0\\u2000-\\u200B\\u2800\\u3000]".toRegex()

internal fun String.replaceSpaceChars(replacement: String = "") =
    PcpNative.nativeReplaceSpaceChars(this, replacement)

internal fun String.safeCapitalize(): String {
    return replaceFirstChar { char ->
        if (char.isLowerCase()) char.titlecase(Locale.getDefault()) else char.toString()
    }
}

internal fun String.removeInvalidRoomNameChars() =
    PcpNative.nativeRemoveInvalidRoomNameChars(this)
