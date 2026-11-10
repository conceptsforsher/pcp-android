/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy

import android.widget.Button
import android.widget.TextView
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R

@EpoxyModelClass
abstract class ErrorWithRetryItem : PcpEpoxyModel<ErrorWithRetryItem.Holder>(R.layout.item_error_retry) {

    @EpoxyAttribute
    var text: String? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var listener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.textView.text = text
        holder.buttonView.isVisible = listener != null
        holder.buttonView.onClick(listener)
    }

    class Holder : PcpEpoxyHolder() {
        val textView by bind<TextView>(R.id.itemErrorRetryText)
        val buttonView by bind<Button>(R.id.itemErrorRetryButton)
    }
}
