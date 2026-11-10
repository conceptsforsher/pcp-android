/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.preview

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class SpaceTopSummaryItem : PcpEpoxyModel<SpaceTopSummaryItem.Holder>(R.layout.item_space_top_summary) {

    @EpoxyAttribute
    var topic: String? = null

    @EpoxyAttribute
    lateinit var formattedMemberCount: String

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.spaceTopicText.setTextOrHide(topic)
        holder.memberCountText.text = formattedMemberCount
    }

    class Holder : PcpEpoxyHolder() {
        val memberCountText by bind<TextView>(R.id.spaceSummaryMemberCountText)
        val spaceTopicText by bind<TextView>(R.id.spaceSummaryTopic)
    }
}
