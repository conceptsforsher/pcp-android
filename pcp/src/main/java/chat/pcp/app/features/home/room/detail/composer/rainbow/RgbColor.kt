/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.composer.rainbow

data class RgbColor(
        val r: Int,
        val g: Int,
        val b: Int
)

fun RgbColor.toDashColor(): String {
    return listOf(r, g, b)
            .joinToString(separator = "", prefix = "#") {
                it.toString(16).padStart(2, '0')
            }
}
