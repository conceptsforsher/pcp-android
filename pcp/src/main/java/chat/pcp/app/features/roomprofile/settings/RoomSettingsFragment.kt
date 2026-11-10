/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.settings

import android.net.Uri
import android.os.Bundle
import android.view.LayoutInflater
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.lifecycle.lifecycleScope
import com.airbnb.mvrx.args
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.dialogs.GalleryOrCameraDialogHelper
import chat.pcp.app.core.dialogs.GalleryOrCameraDialogHelperFactory
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.intent.getFilenameFromUri
import chat.pcp.app.core.platform.OnBackPressed
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.platform.PcpMenuProvider
import chat.pcp.app.core.utils.toast
import chat.pcp.app.databinding.FragmentRoomSettingGenericBinding
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.roomprofile.RoomProfileArgs
import chat.pcp.app.features.roomprofile.RoomProfileSharedActionViewModel
import chat.pcp.app.features.roomprofile.settings.historyvisibility.RoomHistoryVisibilityBottomSheet
import chat.pcp.app.features.roomprofile.settings.historyvisibility.RoomHistoryVisibilitySharedActionViewModel
import chat.pcp.app.features.roomprofile.settings.joinrule.RoomJoinRuleActivity
import chat.pcp.app.features.roomprofile.settings.joinrule.RoomJoinRuleSharedActionViewModel
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import org.matrix.android.sdk.api.session.room.model.GuestAccess
import org.matrix.android.sdk.api.util.toMatrixItem
import java.util.UUID
import javax.inject.Inject

@AndroidEntryPoint
class RoomSettingsFragment :
        PcpFragment<FragmentRoomSettingGenericBinding>(),
        RoomSettingsController.Callback,
        OnBackPressed,
        GalleryOrCameraDialogHelper.Listener,
        PcpMenuProvider {

    @Inject lateinit var controller: RoomSettingsController
    @Inject lateinit var avatarRenderer: AvatarRenderer
    @Inject lateinit var galleryOrCameraDialogHelperFactory: GalleryOrCameraDialogHelperFactory

    private val viewModel: RoomSettingsViewModel by fragmentViewModel()
    private lateinit var roomProfileSharedActionViewModel: RoomProfileSharedActionViewModel
    private lateinit var roomHistoryVisibilitySharedActionViewModel: RoomHistoryVisibilitySharedActionViewModel
    private lateinit var roomJoinRuleSharedActionViewModel: RoomJoinRuleSharedActionViewModel

    private val roomProfileArgs: RoomProfileArgs by args()
    private lateinit var galleryOrCameraDialogHelper: GalleryOrCameraDialogHelper

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentRoomSettingGenericBinding {
        return FragmentRoomSettingGenericBinding.inflate(inflater, container, false)
    }

    override fun getMenuRes() = R.menu.pcp_room_settings

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.RoomSettings
        galleryOrCameraDialogHelper = galleryOrCameraDialogHelperFactory.create(this)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        roomProfileSharedActionViewModel = activityViewModelProvider.get(RoomProfileSharedActionViewModel::class.java)
        setupRoomHistoryVisibilitySharedActionViewModel()
        setupRoomJoinRuleSharedActionViewModel()
        controller.callback = this
        setupToolbar(views.roomSettingsToolbar)
                .allowBack()
        views.roomSettingsRecyclerView.configureWith(controller, hasFixedSize = true)
        views.waitingView.waitingStatusText.setText(CommonStrings.please_wait)
        views.waitingView.waitingStatusText.isVisible = true

        viewModel.observeViewEvents {
            when (it) {
                is RoomSettingsViewEvents.Failure -> showFailure(it.throwable)
                RoomSettingsViewEvents.Success -> showSuccess()
                RoomSettingsViewEvents.GoBack -> {
                    ignoreChanges = true
                    @Suppress("DEPRECATION")
                    vectorBaseActivity.onBackPressed()
                }
            }
        }
    }

    private fun setupRoomJoinRuleSharedActionViewModel() {
        roomJoinRuleSharedActionViewModel = activityViewModelProvider.get(RoomJoinRuleSharedActionViewModel::class.java)
        roomJoinRuleSharedActionViewModel
                .stream()
                .onEach { action ->
                    viewModel.handle(RoomSettingsAction.SetRoomJoinRule(action.roomJoinRule))
                }
                .launchIn(viewLifecycleOwner.lifecycleScope)
    }

    private fun setupRoomHistoryVisibilitySharedActionViewModel() {
        roomHistoryVisibilitySharedActionViewModel = activityViewModelProvider.get(RoomHistoryVisibilitySharedActionViewModel::class.java)
        roomHistoryVisibilitySharedActionViewModel
                .stream()
                .onEach { action ->
                    viewModel.handle(RoomSettingsAction.SetRoomHistoryVisibility(action.roomHistoryVisibility))
                }
                .launchIn(viewLifecycleOwner.lifecycleScope)
    }

    private fun showSuccess() {
        activity?.toast(CommonStrings.room_settings_save_success)
    }

    override fun onDestroyView() {
        controller.callback = null
        views.roomSettingsRecyclerView.cleanup()
        super.onDestroyView()
    }

    override fun handlePrepareMenu(menu: Menu) {
        withState(viewModel) { state ->
            menu.findItem(R.id.roomSettingsSaveAction).isVisible = state.showSaveAction
        }
    }

    override fun handleMenuItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.roomSettingsSaveAction -> {
                viewModel.handle(RoomSettingsAction.Save)
                true
            }
            else -> false
        }
    }

    override fun invalidate() = withState(viewModel) { viewState ->
        controller.setData(viewState)
        renderRoomSummary(viewState)
    }

    private fun renderRoomSummary(state: RoomSettingsViewState) {
        views.waitingView.root.isVisible = state.isLoading

        state.roomSummary()?.let {
            views.roomSettingsToolbarTitleView.text = it.displayName
            avatarRenderer.render(it.toMatrixItem(), views.roomSettingsToolbarAvatarImageView)
            views.roomSettingsDecorationToolbarAvatarImageView.render(it.roomEncryptionTrustLevel)
        }

        invalidateOptionsMenu()
    }

    override fun onNameChanged(name: String) {
        viewModel.handle(RoomSettingsAction.SetRoomName(name))
    }

    override fun onTopicChanged(topic: String) {
        viewModel.handle(RoomSettingsAction.SetRoomTopic(topic))
    }

    override fun onHistoryVisibilityClicked() = withState(viewModel) { state ->
        val currentHistoryVisibility = state.newHistoryVisibility ?: state.currentHistoryVisibility
        RoomHistoryVisibilityBottomSheet.newInstance(currentHistoryVisibility)
                .show(childFragmentManager, "RoomHistoryVisibilityBottomSheet")
    }

    override fun onJoinRuleClicked() {
        startActivity(RoomJoinRuleActivity.newIntent(requireContext(), roomProfileArgs.roomId))
    }

    override fun onToggleGuestAccess() = withState(viewModel) { state ->
        val currentGuestAccess = state.newRoomJoinRules.newGuestAccess ?: state.currentGuestAccess
        val toggled = if (currentGuestAccess == GuestAccess.Forbidden) GuestAccess.CanJoin else GuestAccess.Forbidden
        viewModel.handle(RoomSettingsAction.SetRoomGuestAccess(toggled))
    }

    override fun onImageReady(uri: Uri?) {
        uri ?: return
        viewModel.handle(
                RoomSettingsAction.SetAvatarAction(
                        RoomSettingsViewState.AvatarAction.UpdateAvatar(
                                newAvatarUri = uri,
                                newAvatarFileName = getFilenameFromUri(requireContext(), uri) ?: UUID.randomUUID().toString()
                        )
                )
        )
    }

    override fun onAvatarDelete() {
        withState(viewModel) {
            when (it.avatarAction) {
                RoomSettingsViewState.AvatarAction.None -> {
                    viewModel.handle(RoomSettingsAction.SetAvatarAction(RoomSettingsViewState.AvatarAction.DeleteAvatar))
                }
                RoomSettingsViewState.AvatarAction.DeleteAvatar -> {
                    /* Should not happen */
                }
                is RoomSettingsViewState.AvatarAction.UpdateAvatar -> {
                    // Cancel the update of the avatar
                    viewModel.handle(RoomSettingsAction.SetAvatarAction(RoomSettingsViewState.AvatarAction.None))
                }
            }
        }
    }

    override fun onAvatarChange() {
        galleryOrCameraDialogHelper.show()
    }

    private var ignoreChanges = false

    override fun onBackPressed(toolbarButton: Boolean): Boolean {
        if (ignoreChanges) return false

        return withState(viewModel) {
            return@withState if (it.showSaveAction) {
                MaterialAlertDialogBuilder(requireContext())
                        .setTitle(CommonStrings.dialog_title_warning)
                        .setMessage(CommonStrings.warning_unsaved_change)
                        .setPositiveButton(CommonStrings.warning_unsaved_change_discard) { _, _ ->
                            viewModel.handle(RoomSettingsAction.Cancel)
                        }
                        .setNegativeButton(CommonStrings.action_cancel, null)
                        .show()
                true
            } else {
                false
            }
        }
    }
}
