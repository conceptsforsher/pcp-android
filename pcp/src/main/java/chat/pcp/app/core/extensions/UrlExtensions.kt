/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.extensions

/**
 * Ex: "https://matrix.org/" -> "matrix.org".
 */
fun String?.toReducedUrl(keepSchema: Boolean = false): String {
    return (this ?: "")
            .run { if (keepSchema) this else substringAfter("://") }
            .trim { it == '/' }
}
