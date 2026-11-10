/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.recover

fun String.formatRecoveryKey(): String {
    return this.replace(" ", "")
            .chunked(16)
            .joinToString("\n") {
                it
                        .chunked(4)
                        .joinToString(" ")
            }
}
