/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.header

import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.features.home.room.list.UnreadCounterBadgeView

@EpoxyModelClass
abstract class InviteCounterItem : PcpEpoxyModel<InviteCounterItem.Holder>(R.layout.item_invites_count) {

    @EpoxyAttribute var invitesCount: Int = 0
    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash) var listener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.view.setOnClickListener(listener)
        holder.unreadCounterBadgeView.render(UnreadCounterBadgeView.State.Count(invitesCount, true))
    }

    class Holder : PcpEpoxyHolder() {
        val unreadCounterBadgeView by bind<UnreadCounterBadgeView>(R.id.invites_count_badge)
    }
}
