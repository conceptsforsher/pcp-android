/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.ui.list

import android.widget.TextView
import androidx.annotation.ColorInt
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.extensions.setTextOrHide
import chat.pcp.app.features.themes.ThemeUtils

/**
 * A generic list item header left aligned with notice color.
 */
@EpoxyModelClass
abstract class GenericHeaderItem : PcpEpoxyModel<GenericHeaderItem.Holder>(R.layout.item_generic_header) {

    @EpoxyAttribute
    var text: String? = null

    @EpoxyAttribute
    @ColorInt
    var textColor: Int? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.text.setTextOrHide(text)
        if (textColor != null) {
            holder.text.setTextColor(textColor!!)
        } else {
            holder.text.setTextColor(ThemeUtils.getColor(holder.view.context, chat.pcp.lib.ui.styles.R.attr.vctr_notice_text_color))
        }
    }

    class Holder : PcpEpoxyHolder() {
        val text by bind<TextView>(R.id.itemGenericHeaderText)
    }
}
