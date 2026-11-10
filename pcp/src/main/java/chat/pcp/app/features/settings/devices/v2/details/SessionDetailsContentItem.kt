/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.details

import android.view.View
import android.widget.TextView
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

@EpoxyModelClass
abstract class SessionDetailsContentItem : PcpEpoxyModel<SessionDetailsContentItem.Holder>(R.layout.item_session_details_content) {

    @EpoxyAttribute
    var title: String? = null

    @EpoxyAttribute
    var description: String? = null

    @EpoxyAttribute
    var hasDivider: Boolean = true

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var onLongClickListener: View.OnLongClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.sessionDetailsContentTitle.text = title
        holder.sessionDetailsContentDescription.text = description
        holder.view.isClickable = onLongClickListener != null
        holder.view.setOnLongClickListener(onLongClickListener)
        holder.sessionDetailsContentDivider.isVisible = hasDivider
    }

    class Holder : PcpEpoxyHolder() {
        val sessionDetailsContentTitle by bind<TextView>(R.id.sessionDetailsContentTitle)
        val sessionDetailsContentDescription by bind<TextView>(R.id.sessionDetailsContentDescription)
        val sessionDetailsContentDivider by bind<View>(R.id.sessionDetailsContentDivider)
    }
}
