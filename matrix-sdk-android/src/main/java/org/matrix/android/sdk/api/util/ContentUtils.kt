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
package org.matrix.android.sdk.api.util

import chat.pcp.app.native.PcpNative
import org.matrix.android.sdk.api.session.room.model.message.MessageFormat
import org.matrix.android.sdk.api.session.room.model.message.MessageTextContent

object ContentUtils {
    fun extractUsefulTextFromReply(repliedBody: String): String =
        PcpNative.nativeExtractUsefulTextFromReply(repliedBody)

    fun extractUsefulTextFromHtmlReply(repliedBody: String): String {
        if (repliedBody.startsWith(MX_REPLY_START_TAG)) {
            val closingTagIndex = repliedBody.lastIndexOf(MX_REPLY_END_TAG)
            if (closingTagIndex != -1) {
                return repliedBody.substring(closingTagIndex + MX_REPLY_END_TAG.length).trim()
            }
        }
        return repliedBody
    }

    fun ensureCorrectFormattedBodyInTextReply(messageTextContent: MessageTextContent, originalFormattedBody: String): MessageTextContent {
        return when {
            messageTextContent.formattedBody != null &&
                    !messageTextContent.formattedBody.contains(MX_REPLY_END_TAG) &&
                    originalFormattedBody.contains(MX_REPLY_END_TAG) -> {
                val newFormattedBody = originalFormattedBody.replaceAfterLast(MX_REPLY_END_TAG, messageTextContent.body)
                messageTextContent.copy(formattedBody = newFormattedBody, format = MessageFormat.FORMAT_MATRIX_HTML)
            }
            else -> messageTextContent
        }
    }

    fun formatSpoilerTextFromHtml(formattedBody: String): String =
        PcpNative.nativeFormatSpoilerTextFromHtml(formattedBody)

    private const val MX_REPLY_START_TAG = "<mx-reply>"
    private const val MX_REPLY_END_TAG = "</mx-reply>"
}
