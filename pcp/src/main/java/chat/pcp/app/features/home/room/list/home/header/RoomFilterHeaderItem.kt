/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.header

import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import com.google.android.material.tabs.TabLayout
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.features.analytics.AnalyticsTracker
import im.vector.app.features.analytics.plan.Interaction

@EpoxyModelClass
abstract class RoomFilterHeaderItem : PcpEpoxyModel<RoomFilterHeaderItem.Holder>(R.layout.item_home_filter_tabs) {

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var onFilterChangedListener: ((HomeRoomFilter) -> Unit)? = null

    @EpoxyAttribute
    var filtersData: List<HomeRoomFilter>? = null

    @EpoxyAttribute
    var selectedFilter: HomeRoomFilter? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var analyticsTracker: AnalyticsTracker? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        with(holder.tabLayout) {
            removeAllTabs()
            clearOnTabSelectedListeners()

            filtersData?.forEach { filter ->
                addTab(
                        newTab().setText(filter.titleRes).setTag(filter),
                        filter == (selectedFilter ?: HomeRoomFilter.ALL)
                )
            }

            addOnTabSelectedListener(object : TabLayout.OnTabSelectedListener {
                override fun onTabSelected(tab: TabLayout.Tab?) {
                    (tab?.tag as? HomeRoomFilter)?.let { filter ->
                        trackFilterChangeEvent(filter)
                        onFilterChangedListener?.invoke(filter)
                    }
                }

                override fun onTabUnselected(tab: TabLayout.Tab?) = Unit
                override fun onTabReselected(tab: TabLayout.Tab?) = Unit
            })
        }
    }

    private fun trackFilterChangeEvent(filter: HomeRoomFilter) {
        val interactionName = when (filter) {
            HomeRoomFilter.ALL -> Interaction.Name.MobileAllChatsFilterAll
            HomeRoomFilter.UNREADS -> Interaction.Name.MobileAllChatsFilterUnreads
            HomeRoomFilter.FAVOURITES -> Interaction.Name.MobileAllChatsFilterFavourites
            HomeRoomFilter.PEOPlE -> Interaction.Name.MobileAllChatsFilterPeople
        }

        analyticsTracker?.capture(
                Interaction(
                        index = null,
                        interactionType = null,
                        name = interactionName
                )
        )
    }

    override fun unbind(holder: Holder) {
        holder.tabLayout.clearOnTabSelectedListeners()
        super.unbind(holder)
    }

    class Holder : PcpEpoxyHolder() {
        val tabLayout by bind<TabLayout>(R.id.home_filter_tabs_tabs)
    }
}
