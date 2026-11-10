/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home

import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.platform.StateView

@EpoxyModelClass
abstract class RoomListEmptyItem : PcpEpoxyModel<RoomListEmptyItem.Holder>(R.layout.item_state_view) {

    @EpoxyAttribute
    lateinit var emptyData: StateView.State.Empty

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.stateView.state = emptyData
    }

    class Holder : PcpEpoxyHolder() {
        val stateView by bind<StateView>(R.id.stateView)
    }
}
