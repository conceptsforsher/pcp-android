/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.epoxy.bottomsheet

import android.view.View
import android.widget.TextView
import androidx.annotation.DrawableRes
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

/**
 * A send state for bottom sheet.
 */
@EpoxyModelClass
abstract class BottomSheetSendStateItem : PcpEpoxyModel<BottomSheetSendStateItem.Holder>(R.layout.item_bottom_sheet_message_status) {

    @EpoxyAttribute
    var showProgress: Boolean = false

    @EpoxyAttribute
    lateinit var text: String

    @EpoxyAttribute
    @DrawableRes
    var drawableStart: Int = 0

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.progress.isVisible = showProgress
        holder.text.setCompoundDrawablesWithIntrinsicBounds(drawableStart, 0, 0, 0)
        holder.text.text = text
    }

    class Holder : PcpEpoxyHolder() {
        val progress by bind<View>(R.id.messageStatusProgress)
        val text by bind<TextView>(R.id.messageStatusText)
    }
}
