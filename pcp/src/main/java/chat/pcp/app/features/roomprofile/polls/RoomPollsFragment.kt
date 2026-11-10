/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.args
import com.airbnb.mvrx.fragmentViewModel
import com.google.android.material.tabs.TabLayoutMediator
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentRoomPollsBinding
import chat.pcp.app.features.roomprofile.RoomProfileArgs
import chat.pcp.lib.strings.CommonStrings

@AndroidEntryPoint
class RoomPollsFragment : PcpFragment<FragmentRoomPollsBinding>() {

    private val roomProfileArgs: RoomProfileArgs by args()

    private val viewModel: RoomPollsViewModel by fragmentViewModel()

    private var tabLayoutMediator: TabLayoutMediator? = null

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentRoomPollsBinding {
        return FragmentRoomPollsBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        setupToolbar()
        setupTabs()
    }

    override fun onDestroyView() {
        views.roomPollsViewPager.adapter = null
        tabLayoutMediator?.detach()
        tabLayoutMediator = null
        super.onDestroyView()
    }

    private fun setupToolbar() {
        setupToolbar(views.roomPollsToolbar)
                .allowBack()
    }

    private fun setupTabs() {
        views.roomPollsViewPager.adapter = RoomPollsPagerAdapter(this)

        tabLayoutMediator = TabLayoutMediator(views.roomPollsTabs, views.roomPollsViewPager) { tab, position ->
            when (position) {
                RoomPollsType.ACTIVE.ordinal -> tab.text = getString(CommonStrings.room_polls_active)
                RoomPollsType.ENDED.ordinal -> tab.text = getString(CommonStrings.room_polls_ended)
            }
        }.also { it.attach() }
    }
}
