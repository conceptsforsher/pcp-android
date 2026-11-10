/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.ui.list

import android.widget.ProgressBar
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

/**
 * A generic progress bar item.
 */
@EpoxyModelClass
abstract class GenericProgressBarItem : PcpEpoxyModel<GenericProgressBarItem.Holder>(R.layout.item_generic_progress) {

    @EpoxyAttribute
    var progress: Int = 0

    @EpoxyAttribute
    var total: Int = 100

    @EpoxyAttribute
    var indeterminate: Boolean = false

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.progressbar.progress = progress
        holder.progressbar.max = total
        holder.progressbar.isIndeterminate = indeterminate
    }

    class Holder : PcpEpoxyHolder() {
        val progressbar by bind<ProgressBar>(R.id.genericProgressBar)
    }
}
