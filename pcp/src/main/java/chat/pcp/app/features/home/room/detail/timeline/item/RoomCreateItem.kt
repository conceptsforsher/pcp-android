/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.item

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.lib.core.utils.epoxy.charsequence.EpoxyCharSequence
import me.saket.bettermovementmethod.BetterLinkMovementMethod

@EpoxyModelClass
abstract class RoomCreateItem : PcpEpoxyModel<RoomCreateItem.Holder>(R.layout.item_timeline_event_create) {

    @EpoxyAttribute lateinit var text: EpoxyCharSequence

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.description.movementMethod = BetterLinkMovementMethod.getInstance()
        holder.description.text = text.charSequence
    }

    class Holder : PcpEpoxyHolder() {
        val description by bind<TextView>(R.id.roomCreateItemDescription)
    }
}
