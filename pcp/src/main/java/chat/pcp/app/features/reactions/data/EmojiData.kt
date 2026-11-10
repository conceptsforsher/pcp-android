/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.reactions.data

import com.squareup.moshi.Json
import com.squareup.moshi.JsonClass

@JsonClass(generateAdapter = true)
data class EmojiData(
        @Json(name = "categories") val categories: List<EmojiCategory>,
        @Json(name = "emojis") val emojis: Map<String, EmojiItem>,
        @Json(name = "aliases") val aliases: Map<String, String>
)
