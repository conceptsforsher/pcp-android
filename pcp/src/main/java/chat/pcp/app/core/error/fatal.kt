/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.error

import timber.log.Timber

/**
 * throw in debug, only log in production. As this method does not always throw, next statement should be a return
 */
fun fatalError(message: String, failFast: Boolean) {
    if (failFast) {
        error(message)
    } else {
        Timber.e(message)
    }
}
