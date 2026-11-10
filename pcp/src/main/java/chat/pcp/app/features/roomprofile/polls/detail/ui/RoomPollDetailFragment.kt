/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.detail.ui

import android.os.Bundle
import android.os.Parcelable
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.args
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentRoomPollDetailBinding
import chat.pcp.lib.strings.CommonStrings
import kotlinx.parcelize.Parcelize
import javax.inject.Inject

@Parcelize
data class RoomPollDetailArgs(
        val pollId: String,
        val roomId: String,
        val isEnded: Boolean,
) : Parcelable

@AndroidEntryPoint
class RoomPollDetailFragment :
        PcpFragment<FragmentRoomPollDetailBinding>(),
        RoomPollDetailController.Callback {

    @Inject lateinit var viewNavigator: RoomPollDetailNavigator
    @Inject lateinit var roomPollDetailController: RoomPollDetailController

    private val viewModel: RoomPollDetailViewModel by fragmentViewModel()
    private val roomPollDetailArgs: RoomPollDetailArgs by args()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentRoomPollDetailBinding {
        return FragmentRoomPollDetailBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupToolbar(isEnded = roomPollDetailArgs.isEnded)
        setupDetailView()
    }

    override fun onDestroyView() {
        roomPollDetailController.callback = null
        views.pollDetailRecyclerView.cleanup()
        super.onDestroyView()
    }

    private fun setupDetailView() {
        roomPollDetailController.callback = this
        views.pollDetailRecyclerView.configureWith(
                roomPollDetailController,
                hasFixedSize = true,
        )
    }

    private fun setupToolbar(isEnded: Boolean) {
        val title = when (isEnded) {
            true -> getString(CommonStrings.room_polls_ended)
            false -> getString(CommonStrings.room_polls_active)
        }

        setupToolbar(views.roomPollDetailToolbar)
                .setTitle(title)
                .allowBack(useCross = true)
    }

    override fun invalidate() = withState(viewModel) { state ->
        roomPollDetailController.setData(state)
    }

    override fun vote(pollEventId: String, optionId: String) {
        viewModel.handle(RoomPollDetailAction.Vote(pollEventId = pollEventId, optionId = optionId))
    }

    override fun goToTimelineEvent(eventId: String) = withState(viewModel) { state ->
        viewNavigator.goToTimelineEvent(
                context = requireContext(),
                roomId = state.roomId,
                eventId = eventId,
        )
    }
}
