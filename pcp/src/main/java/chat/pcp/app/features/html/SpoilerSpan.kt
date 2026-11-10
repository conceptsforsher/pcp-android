/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.html

import android.graphics.Color
import android.text.TextPaint
import android.text.style.ClickableSpan
import android.view.View
import chat.pcp.app.core.resources.ColorProvider

class SpoilerSpan(private val colorProvider: ColorProvider) : ClickableSpan() {

    override fun onClick(widget: View) {
        isHidden = !isHidden
        widget.invalidate()
    }

    private var isHidden = true

    override fun updateDrawState(tp: TextPaint) {
        if (isHidden) {
            tp.bgColor = colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_spoiler_background_color)
            tp.color = Color.TRANSPARENT
        } else {
            tp.bgColor = colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_markdown_block_background_color)
            tp.color = colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_primary)
        }
    }
}
