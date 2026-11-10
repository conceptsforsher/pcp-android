/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.themes

import android.content.Context
import javax.inject.Inject

/**
 * Injectable class to encapsulate ThemeUtils call...
 */
class ThemeProvider @Inject constructor(
        private val context: Context
) {
    fun isLightTheme() = ThemeUtils.isLightTheme(context)
}
