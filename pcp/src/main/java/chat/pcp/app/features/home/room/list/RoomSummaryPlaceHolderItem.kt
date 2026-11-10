/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

@EpoxyModelClass
abstract class RoomSummaryPlaceHolderItem : PcpEpoxyModel<RoomSummaryPlaceHolderItem.Holder>(R.layout.item_room_placeholder) {

    @EpoxyAttribute
    var useSingleLineForLastEvent: Boolean = false

    override fun bind(holder: Holder) {
        super.bind(holder)
        if (useSingleLineForLastEvent) {
            holder.subtitleView.setLines(1)
        }
    }

    class Holder : PcpEpoxyHolder() {
        val subtitleView by bind<TextView>(R.id.subtitleView)
    }
}
