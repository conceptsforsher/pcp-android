/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy

import android.widget.ProgressBar
import android.widget.TextView
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class LoadingItem : PcpEpoxyModel<LoadingItem.Holder>(R.layout.item_loading) {

    @EpoxyAttribute var loadingText: String? = null
    @EpoxyAttribute var showLoader: Boolean = true

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.progressBar.isVisible = showLoader
        holder.textView.setTextOrHide(loadingText)
    }

    class Holder : PcpEpoxyHolder() {
        val textView by bind<TextView>(R.id.loadingText)
        val progressBar by bind<ProgressBar>(R.id.loadingProgress)
    }
}
