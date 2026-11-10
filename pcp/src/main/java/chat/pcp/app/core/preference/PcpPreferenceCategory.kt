/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.preference

import android.content.Context
import android.graphics.Typeface
import android.util.AttributeSet
import android.view.ViewGroup
import android.widget.TextView
import androidx.preference.PreferenceCategory
import androidx.preference.PreferenceViewHolder
import chat.pcp.app.features.themes.ThemeUtils

/**
 * Customize PreferenceCategory class to redefine some attributes.
 */
class PcpPreferenceCategory : PreferenceCategory {

    constructor(context: Context) : super(context)

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs)

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int) : super(context, attrs, defStyleAttr)

    init {
        // Set to false to remove the space when there is no icon
        isIconSpaceReserved = true
    }

    override fun onBindViewHolder(holder: PreferenceViewHolder) {
        super.onBindViewHolder(holder)

        val titleTextView = holder.findViewById(android.R.id.title) as? TextView

        titleTextView?.setTypeface(null, Typeface.BOLD)
        titleTextView?.setTextColor(ThemeUtils.getColor(context, chat.pcp.lib.ui.styles.R.attr.vctr_content_primary))

        // "isIconSpaceReserved = false" does not work for preference category, so remove the padding
        if (!isIconSpaceReserved) {
            (titleTextView?.parent as? ViewGroup)?.setPadding(0, 0, 0, 0)
        }
    }
}
