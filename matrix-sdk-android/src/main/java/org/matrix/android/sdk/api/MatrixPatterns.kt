/*
 * Copyright 2026 The Matrix.org Foundation C.I.C.
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

package org.matrix.android.sdk.api

import chat.pcp.app.native.PcpNative
import org.matrix.android.sdk.BuildConfig
import timber.log.Timber

/**
 * Matrix identifier validation using C++ native implementation.
 * Ref: https://matrix.org/docs/spec/appendices#identifier-grammar
 */
object MatrixPatterns {

    // Legacy regex patterns (kept for backward compat — delegates to C++)
    val PATTERN_CONTAIN_MATRIX_USER_IDENTIFIER by lazy {
        "@[A-Z0-9\\x21-\\x39\\x3B-\\x7F]+:[A-Z0-9.-]+(:[0-9]{2,5})?".toRegex(RegexOption.IGNORE_CASE)
    }

    val MATRIX_PATTERNS by lazy { listOf(PATTERN_CONTAIN_MATRIX_USER_IDENTIFIER) }
    val ORDER_STRING_REGEX by lazy { "[ -~]+".toRegex() }
    const val SEP_REGEX = "/"

    fun isUserId(str: String?): Boolean = str != null && PcpNative.nativeIsUserId(str)
    fun isRoomId(str: String?): Boolean = str != null && PcpNative.nativeIsRoomId(str)
    fun isRoomAlias(str: String?): Boolean = str != null && PcpNative.nativeIsRoomAlias(str)
    fun isEventId(str: String?): Boolean = str != null && PcpNative.nativeIsEventId(str)
    fun isGroupId(str: String?): Boolean = str != null && PcpNative.nativeIsGroupId(str)

    fun isPermalink(str: String?): Boolean {
        if (str == null) return false
        val r1 = Regex("https://matrix\\.to/#/", RegexOption.IGNORE_CASE)
        val r2 = Regex("https://[A-Z0-9.-]+\\.[A-Z]{2,}/#/(room|user)/", RegexOption.IGNORE_CASE)
        return r1.containsMatchIn(str) || r2.containsMatchIn(str)
    }

    fun extractServerNameFromId(matrixId: String?): String? {
        return PcpNative.nativeExtractServerNameFromId(matrixId ?: "").takeIf { it.isNotEmpty() }
    }

    fun extractUserNameFromId(matrixId: String): String? {
        return if (isUserId(matrixId)) PcpNative.nativeExtractUserNameFromId(matrixId).takeIf { it.isNotEmpty() } else null
    }

    fun isValidOrderString(order: String?): Boolean {
        return order != null && order.length < 50 && PcpNative.nativeIsValidOrderString(order)
    }

    fun candidateAliasFromRoomName(roomName: String, domain: String): String {
        return PcpNative.nativeCandidateAliasFromRoomName(roomName, domain)
    }

    fun String.getServerName(): String {
        if (BuildConfig.DEBUG && !isUserId(this)) {
            Timber.w("Not a valid user ID: $this")
        }
        return substringAfter(":")
    }
}
