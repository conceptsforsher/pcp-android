/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.ui.bottomsheet

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.extensions.setTextOrHide

/**
 * A title for bottom sheet, with an optional subtitle. It does not include the bottom separator.
 */
@EpoxyModelClass
abstract class BottomSheetTitleItem : PcpEpoxyModel<BottomSheetTitleItem.Holder>(R.layout.item_bottom_sheet_title) {

    @EpoxyAttribute
    lateinit var title: String

    @EpoxyAttribute
    var subTitle: String? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.title.text = title
        holder.subtitle.setTextOrHide(subTitle)
    }

    class Holder : PcpEpoxyHolder() {
        val title by bind<TextView>(R.id.itemBottomSheetTitleTitle)
        val subtitle by bind<TextView>(R.id.itemBottomSheetTitleSubtitle)
    }
}
