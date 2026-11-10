/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy

import androidx.core.view.updateLayoutParams
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.features.home.room.detail.timeline.item.ItemWithEvents

@EpoxyModelClass
abstract class TimelineEmptyItem : PcpEpoxyModel<TimelineEmptyItem.Holder>(R.layout.item_timeline_empty), ItemWithEvents {

    @EpoxyAttribute lateinit var eventId: String
    @EpoxyAttribute var notBlank: Boolean = false

    override fun isVisible() = false

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.view.updateLayoutParams {
            // Force height to 1px so scrolling works correctly
            this.height = if (notBlank) 1 else 0
        }
    }

    override fun getEventIds(): List<String> {
        return listOf(eventId)
    }

    class Holder : PcpEpoxyHolder()
}
