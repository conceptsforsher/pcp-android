/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.banned

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.widget.SearchView
import androidx.core.view.isVisible
import com.airbnb.mvrx.args
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.toast
import chat.pcp.app.databinding.FragmentRoomSettingGenericBinding
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.roomprofile.RoomProfileArgs
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.room.model.RoomMemberSummary
import org.matrix.android.sdk.api.util.toMatrixItem
import javax.inject.Inject

@AndroidEntryPoint
class RoomBannedMemberListFragment :
        PcpFragment<FragmentRoomSettingGenericBinding>(),
        RoomBannedMemberListController.Callback {

    @Inject lateinit var roomMemberListController: RoomBannedMemberListController
    @Inject lateinit var avatarRenderer: AvatarRenderer

    private val viewModel: RoomBannedMemberListViewModel by fragmentViewModel()
    private val roomProfileArgs: RoomProfileArgs by args()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentRoomSettingGenericBinding {
        return FragmentRoomSettingGenericBinding.inflate(inflater, container, false)
    }

    override fun onUnbanClicked(roomMember: RoomMemberSummary) {
        viewModel.handle(RoomBannedMemberListAction.QueryInfo(roomMember))
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        roomMemberListController.callback = this
        setupToolbar(views.roomSettingsToolbar)
                .allowBack()
        setupSearchView()
        views.roomSettingsRecyclerView.configureWith(roomMemberListController, hasFixedSize = true)

        viewModel.observeViewEvents {
            when (it) {
                is RoomBannedMemberListViewEvents.ShowBannedInfo -> {
                    val canBan = withState(viewModel) { state -> state.canUserBan }
                    MaterialAlertDialogBuilder(requireActivity())
                            .setTitle(getString(CommonStrings.member_banned_by, it.bannedByUserId))
                            .setMessage(getString(CommonStrings.reason_colon, it.banReason))
                            .setPositiveButton(CommonStrings.ok, null)
                            .apply {
                                if (canBan) {
                                    setNegativeButton(CommonStrings.room_participants_action_unban) { _, _ ->
                                        viewModel.handle(RoomBannedMemberListAction.UnBanUser(it.roomMemberSummary))
                                    }
                                }
                            }
                            .show()
                }
                is RoomBannedMemberListViewEvents.ToastError -> {
                    requireActivity().toast(it.info)
                }
            }
        }
    }

    override fun onDestroyView() {
        views.roomSettingsRecyclerView.cleanup()
        super.onDestroyView()
    }

    private fun setupSearchView() {
        views.searchViewAppBarLayout.isVisible = true
        views.searchView.queryHint = getString(CommonStrings.search_banned_user_hint)
        views.searchView.setOnQueryTextListener(object : SearchView.OnQueryTextListener {
            override fun onQueryTextSubmit(query: String): Boolean {
                return true
            }

            override fun onQueryTextChange(newText: String): Boolean {
                viewModel.handle(RoomBannedMemberListAction.Filter(newText))
                return true
            }
        })
    }

    override fun invalidate() = withState(viewModel) { viewState ->
        roomMemberListController.setData(viewState)
        renderRoomSummary(viewState)
    }

    private fun renderRoomSummary(state: RoomBannedMemberListViewState) {
        state.roomSummary()?.let {
            views.roomSettingsToolbarTitleView.text = it.displayName
            avatarRenderer.render(it.toMatrixItem(), views.roomSettingsToolbarAvatarImageView)
            views.roomSettingsDecorationToolbarAvatarImageView.render(it.roomEncryptionTrustLevel)
        }
    }
}
