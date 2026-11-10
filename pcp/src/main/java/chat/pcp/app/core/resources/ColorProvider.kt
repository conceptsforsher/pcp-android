/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.resources

import android.content.Context
import androidx.annotation.AttrRes
import androidx.annotation.ColorInt
import androidx.annotation.ColorRes
import androidx.core.content.ContextCompat
import chat.pcp.app.features.themes.ThemeUtils
import javax.inject.Inject

class ColorProvider @Inject constructor(private val context: Context) {

    @ColorInt
    fun getColor(@ColorRes colorRes: Int): Int {
        return ContextCompat.getColor(context, colorRes)
    }

    /**
     * Translates color attributes to colors.
     *
     * @param colorAttribute Color Attribute
     * @return Requested Color
     */
    @ColorInt
    fun getColorFromAttribute(@AttrRes colorAttribute: Int): Int {
        return ThemeUtils.getColor(context, colorAttribute)
    }
}
