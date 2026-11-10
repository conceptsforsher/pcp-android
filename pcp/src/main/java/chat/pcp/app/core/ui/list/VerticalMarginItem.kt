/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.ui.list

import android.view.View
import androidx.core.view.updateLayoutParams
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

/**
 * A generic item with empty space.
 */
@EpoxyModelClass
abstract class VerticalMarginItem : PcpEpoxyModel<VerticalMarginItem.Holder>(R.layout.item_vertical_margin) {

    @EpoxyAttribute
    var heightInPx: Int = 0

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.space.updateLayoutParams {
            height = heightInPx
        }
    }

    class Holder : PcpEpoxyHolder() {
        val space by bind<View>(R.id.item_vertical_margin_space)
    }
}
