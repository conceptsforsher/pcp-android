/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R

@EpoxyModelClass
abstract class FontScaleSectionItem : PcpEpoxyModel<FontScaleSectionItem.Holder>(R.layout.item_font_scale_section) {

    @EpoxyAttribute var sectionName: String = ""

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.textView.text = sectionName
    }

    class Holder : PcpEpoxyHolder() {
        val textView by bind<TextView>(R.id.font_scale_section_name)
    }
}
