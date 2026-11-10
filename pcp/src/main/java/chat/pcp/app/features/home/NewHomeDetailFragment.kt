/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.lifecycle.lifecycleScope
import com.airbnb.mvrx.activityViewModel
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import com.google.android.material.appbar.AppBarLayout
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.SpaceStateHandler
import chat.pcp.app.core.extensions.commitTransaction
import chat.pcp.app.core.platform.OnBackPressed
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.platform.PcpMenuProvider
import chat.pcp.app.core.resources.BuildMeta
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.core.ui.views.CurrentCallsView
import chat.pcp.app.core.ui.views.CurrentCallsViewPresenter
import chat.pcp.app.core.ui.views.KeysBackupBanner
import chat.pcp.app.core.ui.views.VerifyDeviceBanner
import chat.pcp.app.core.utils.openUrlInChromeCustomTab
import chat.pcp.app.databinding.FragmentNewHomeDetailBinding
import chat.pcp.app.features.call.SharedKnownCallsViewModel
import chat.pcp.app.features.call.PcpCallActivity
import chat.pcp.app.features.call.dialpad.PstnDialActivity
import chat.pcp.app.features.call.webrtc.WebRtcCallManager
import chat.pcp.app.features.crypto.verification.self.SelfVerificationBottomSheet
import chat.pcp.app.features.home.room.list.UnreadCounterBadgeView
import chat.pcp.app.features.home.room.list.actions.RoomListSharedAction
import chat.pcp.app.features.home.room.list.actions.RoomListSharedActionViewModel
import chat.pcp.app.features.home.room.list.home.HomeRoomListFragment
import chat.pcp.app.features.home.room.list.home.NewChatBottomSheet
import chat.pcp.app.features.popup.PopupAlertManager
import chat.pcp.app.features.popup.VerificationPcpAlert
import chat.pcp.app.features.qrcode.QrCodeScannerActivity
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.settings.PcpSettingsActivity.Companion.EXTRA_DIRECT_ACCESS_SECURITY_PRIVACY_MANAGE_SESSIONS
import chat.pcp.app.features.spaces.SpaceListBottomSheet
import chat.pcp.app.features.workers.signout.BannerState
import chat.pcp.app.features.workers.signout.ServerBackupStatusAction
import chat.pcp.app.features.workers.signout.ServerBackupStatusViewModel
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import org.matrix.android.sdk.api.session.crypto.model.DeviceInfo
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import javax.inject.Inject

@AndroidEntryPoint
class NewHomeDetailFragment :
        PcpFragment<FragmentNewHomeDetailBinding>(),
        KeysBackupBanner.Delegate,
        CurrentCallsView.Callback,
        OnBackPressed,
        PcpMenuProvider,
        VerifyDeviceBanner.Delegate {

    @Inject lateinit var avatarRenderer: AvatarRenderer
    @Inject lateinit var colorProvider: ColorProvider
    @Inject lateinit var alertManager: PopupAlertManager
    @Inject lateinit var callManager: WebRtcCallManager
    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var spaceStateHandler: SpaceStateHandler
    @Inject lateinit var buildMeta: BuildMeta

    private val viewModel: HomeDetailViewModel by fragmentViewModel()
    private val newHomeDetailViewModel: NewHomeDetailViewModel by fragmentViewModel()
    private val unknownDeviceDetectorSharedViewModel: UnknownDeviceDetectorSharedViewModel by activityViewModel()
    private val serverBackupStatusViewModel: ServerBackupStatusViewModel by activityViewModel()

    private lateinit var sharedActionViewModel: HomeSharedActionViewModel
    private lateinit var sharedRoomListActionViewModel: RoomListSharedActionViewModel
    private lateinit var sharedCallActionViewModel: SharedKnownCallsViewModel

    private val newChatBottomSheet = NewChatBottomSheet()
    private val spaceListBottomSheet = SpaceListBottomSheet()

    private var hasUnreadRooms = false
        set(value) {
            if (value != field) {
                field = value
                invalidateOptionsMenu()
            }
        }

    override fun getMenuRes() = R.menu.room_list

    override fun handleMenuItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.menu_home_mark_all_as_read -> {
                viewModel.handle(HomeDetailAction.MarkAllRoomsRead)
                true
            }
            R.id.menu_home_dialpad -> {
                startActivity(Intent(requireContext(), PstnDialActivity::class.java))
                true
            }
            else -> false
        }
    }

    override fun handlePrepareMenu(menu: Menu) {
        withState(viewModel) { state ->
            val isRoomList = state.currentTab is HomeTab.RoomList
            menu.findItem(R.id.menu_home_mark_all_as_read).isVisible = isRoomList && hasUnreadRooms
            menu.findItem(R.id.menu_home_dialpad).isVisible = state.showDialPadTab
        }
    }

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentNewHomeDetailBinding {
        return FragmentNewHomeDetailBinding.inflate(inflater, container, false)
    }

    private val currentCallsViewPresenter = CurrentCallsViewPresenter()

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        sharedActionViewModel = activityViewModelProvider.get(HomeSharedActionViewModel::class.java)
        sharedCallActionViewModel = activityViewModelProvider.get(SharedKnownCallsViewModel::class.java)
        setupToolbar()
        setupKeysBackupBanner()
        setupVerificationBanner()
        setupActiveCallView()
        setupDebugButton()
        setupFabs()
        setupObservers()

        childFragmentManager.commitTransaction {
            add(R.id.roomListContainer, HomeRoomListFragment::class.java, null, HOME_ROOM_LIST_FRAGMENT_TAG)
        }

        viewModel.onEach(HomeDetailViewState::selectedSpace) { selectedSpace ->
            onSpaceChange(selectedSpace)
        }

        viewModel.observeViewEvents { viewEvent ->
            when (viewEvent) {
                HomeDetailViewEvents.CallStarted -> Unit
                is HomeDetailViewEvents.FailToCall -> Unit
                HomeDetailViewEvents.Loading -> showLoadingDialog()
            }
        }

        unknownDeviceDetectorSharedViewModel.onEach { state ->
            state.unknownSessions.invoke()?.let { unknownDevices ->
                val uid = PopupAlertManager.REVIEW_LOGIN_UID
                if (unknownDevices.firstOrNull()?.currentSessionTrust == true) {
                    alertManager.cancelAlert(uid)
                    val olderUnverified = unknownDevices.filter { !it.isNew }
                    val newest = unknownDevices.firstOrNull { it.isNew }?.deviceInfo
                    if (newest != null) {
                        promptForNewUnknownDevices(uid, state, newest)
                    } else if (olderUnverified.isNotEmpty()) {
                        // In this case we prompt to go to settings to review logins
                        promptToReviewChanges(uid, state, olderUnverified.map { it.deviceInfo })
                    }
                } else {
                    // cancel as there are not anymore untrusted devices
                    alertManager.cancelAlert(uid)
                }
            }
        }

        sharedCallActionViewModel
                .liveKnownCalls
                .observe(viewLifecycleOwner) {
                    currentCallsViewPresenter.updateCall(callManager.getCurrentCall(), callManager.getCalls())
                    invalidateOptionsMenu()
                }

        newHomeDetailViewModel.onEach { viewState ->
            refreshUnreadCounterBadge(viewState.spacesNotificationCounterBadgeState)
        }
    }

    private fun setupObservers() {
        sharedRoomListActionViewModel = activityViewModelProvider[RoomListSharedActionViewModel::class.java]

        sharedRoomListActionViewModel
                .stream()
                .onEach(::handleSharedAction)
                .launchIn(viewLifecycleOwner.lifecycleScope)
    }

    private fun handleSharedAction(action: RoomListSharedAction) {
        when (action) {
            RoomListSharedAction.CloseBottomSheet -> spaceListBottomSheet.dismiss()
        }
    }

    private fun setupFabs() {
        showFABs()

        views.newLayoutCreateChatButton.debouncedClicks {
            newChatBottomSheet.takeIf { !it.isAdded }?.show(requireActivity().supportFragmentManager, NewChatBottomSheet.TAG)
        }

        views.newLayoutOpenSpacesButton.debouncedClicks {
            spaceListBottomSheet.takeIf { !it.isAdded }?.show(requireActivity().supportFragmentManager, SpaceListBottomSheet.TAG)
        }
    }

    private fun showFABs() {
        views.newLayoutCreateChatButton.show()
        views.newLayoutOpenSpacesButton.show()
    }

    private fun setCurrentSpace(spaceId: String?) {
        spaceStateHandler.setCurrentSpace(spaceId, isForwardNavigation = false)
        sharedActionViewModel.post(HomeActivitySharedAction.OnCloseSpace)
    }

    override fun onDestroyView() {
        currentCallsViewPresenter.unBind()
        super.onDestroyView()
    }

    override fun onResume() {
        super.onResume()
        callManager.checkForProtocolsSupportIfNeeded()
        refreshSpaceState()
    }

    private fun refreshSpaceState() {
        spaceStateHandler.getCurrentSpace()?.let {
            onSpaceChange(it)
        }
    }

    private fun promptForNewUnknownDevices(uid: String, state: UnknownDevicesState, newest: DeviceInfo) {
        val user = state.myMatrixItem
        alertManager.postPcpAlert(
                VerificationPcpAlert(
                        uid = uid,
                        title = getString(CommonStrings.new_session),
                        description = getString(CommonStrings.verify_this_session, newest.displayName ?: newest.deviceId ?: ""),
                        iconId = R.drawable.ic_shield_warning
                ).apply {
                    viewBinder = VerificationPcpAlert.ViewBinder(user, avatarRenderer)
                    colorInt = colorProvider.getColorFromAttribute(com.google.android.material.R.attr.colorPrimary)
                    contentAction = Runnable {
                        (weakCurrentActivity?.get() as? PcpActivity<*>)?.let { vectorBaseActivity ->
                            vectorBaseActivity.navigator
                                    .requestSessionVerification(vectorBaseActivity, newest.deviceId ?: "")
                        }
                        unknownDeviceDetectorSharedViewModel.handle(
                                UnknownDeviceDetectorSharedViewModel.Action.IgnoreNewLogin(newest.deviceId?.let { listOf(it) }.orEmpty())
                        )
                    }
                    dismissedAction = Runnable {
                        unknownDeviceDetectorSharedViewModel.handle(
                                UnknownDeviceDetectorSharedViewModel.Action.IgnoreNewLogin(newest.deviceId?.let { listOf(it) }.orEmpty())
                        )
                    }
                }
        )
    }

    private fun promptToReviewChanges(uid: String, state: UnknownDevicesState, oldUnverified: List<DeviceInfo>) {
        val user = state.myMatrixItem
        alertManager.postPcpAlert(
                VerificationPcpAlert(
                        uid = uid,
                        title = getString(CommonStrings.review_unverified_sessions_title),
                        description = getString(CommonStrings.review_unverified_sessions_description),
                        iconId = R.drawable.ic_shield_warning,
                        shouldBeDisplayedIn = { activity ->
                            // do not show when there is an ongoing verification flow
                            if (activity is PcpActivity<*>) {
                                activity.supportFragmentManager.findFragmentByTag(SelfVerificationBottomSheet.TAG) == null &&
                                        activity !is QrCodeScannerActivity
                            } else true
                        }
                ).apply {
                    viewBinder = VerificationPcpAlert.ViewBinder(user, avatarRenderer)
                    colorInt = colorProvider.getColorFromAttribute(com.google.android.material.R.attr.colorPrimary)
                    contentAction = Runnable {
                        (weakCurrentActivity?.get() as? PcpActivity<*>)?.let { activity ->
                            // mark as ignored to avoid showing it again
                            unknownDeviceDetectorSharedViewModel.handle(
                                    UnknownDeviceDetectorSharedViewModel.Action.IgnoreDevice(oldUnverified.mapNotNull { it.deviceId })
                            )
                            activity.navigator.openSettings(activity, EXTRA_DIRECT_ACCESS_SECURITY_PRIVACY_MANAGE_SESSIONS)
                        }
                    }
                    dismissedAction = Runnable {
                        unknownDeviceDetectorSharedViewModel.handle(
                                UnknownDeviceDetectorSharedViewModel.Action.IgnoreDevice(oldUnverified.mapNotNull { it.deviceId })
                        )
                    }
                }
        )
    }

    private fun onSpaceChange(spaceSummary: RoomSummary?) {
        views.collapsingToolbar.title = (spaceSummary?.displayName ?: getString(CommonStrings.all_chats))
    }

    private fun setupKeysBackupBanner() {
        serverBackupStatusViewModel.handle(ServerBackupStatusAction.OnBannerDisplayed)
        serverBackupStatusViewModel
                .onEach {
                    when (val banState = it.bannerState.invoke()) {
                        is BannerState.Setup,
                        BannerState.BackingUp,
                        BannerState.Hidden -> views.homeKeysBackupBanner.render(banState, false)
                        null -> views.homeKeysBackupBanner.render(BannerState.Hidden, false)
                        else -> Unit /* No op? */
                    }
                }
        views.homeKeysBackupBanner.delegate = this
    }

    private fun setupVerificationBanner() {
        views.homeVerifyDevice.delegate = this
    }

    private fun setupActiveCallView() {
        currentCallsViewPresenter.bind(views.currentCallsView, this)
    }

    private fun setupToolbar() {
        setupToolbar(views.toolbar)

        views.collapsingToolbar.debouncedClicks(::openSpaceSettings)
        views.toolbar.debouncedClicks(::openSpaceSettings)

        views.avatar.debouncedClicks {
            navigator.openSettings(requireContext())
        }
    }

    private fun openSpaceSettings() = withState(viewModel) { viewState ->
        viewState.selectedSpace?.let {
            sharedActionViewModel.post(HomeActivitySharedAction.ShowSpaceSettings(it.roomId))
        }
    }

    private fun setupDebugButton() {
        views.debugButton.debouncedClicks {
            sharedActionViewModel.post(HomeActivitySharedAction.CloseDrawer)
            navigator.openDebug(requireActivity())
        }

        views.appBarLayout.addOnOffsetChangedListener(AppBarLayout.OnOffsetChangedListener { _, verticalOffset ->
            views.debugButton.isVisible = verticalOffset == 0 && buildMeta.isDebug && pcpPreferences.developerMode()
        })
    }

    /* ==========================================================================================
     * KeysBackupBanner Listener
     * ========================================================================================== */

    override fun onCloseClicked() {
        serverBackupStatusViewModel.handle(ServerBackupStatusAction.OnBannerClosed)
    }

    override fun setupKeysBackup() {
        navigator.openKeysBackupSetup(requireActivity(), false)
    }

    override fun recoverKeysBackup() {
        navigator.openKeysBackupManager(requireActivity())
    }

    override fun invalidate() = withState(viewModel) {
        views.syncStateView.render(
                it.syncState,
                it.incrementalSyncRequestState,
                it.pushCounter,
                pcpPreferences.developerShowDebugInfo()
        )
        views.homeVerifyDevice.isVisible = it.showVerifyDeviceBanner
        refreshAvatar()
        hasUnreadRooms = it.hasUnreadMessages
    }

    private fun refreshAvatar() = withState(viewModel) { state ->
        state.myMatrixItem?.let { user ->
            avatarRenderer.render(user, views.avatar)
        }
    }

    private fun refreshUnreadCounterBadge(badgeState: UnreadCounterBadgeView.State) {
        views.spacesUnreadCounterBadge.render(badgeState)
    }

    override fun onTapToReturnToCall() {
        callManager.getCurrentCall()?.let { call ->
            PcpCallActivity.newIntent(
                    context = requireContext(),
                    callId = call.callId,
                    signalingRoomId = call.signalingRoomId,
                    otherUserId = call.mxCall.opponentUserId,
                    isIncomingCall = !call.mxCall.isOutgoing,
                    isVideoCall = call.mxCall.isVideoCall,
                    mode = null
            ).let {
                startActivity(it)
            }
        }
    }

    override fun onBackPressed(toolbarButton: Boolean) = if (spaceStateHandler.isRoot()) {
        false
    } else {
        val lastSpace = spaceStateHandler.popSpaceBackstack()
        spaceStateHandler.setCurrentSpace(lastSpace, isForwardNavigation = false)
        true
    }

    private fun SpaceStateHandler.isRoot() = getSpaceBackstack().isEmpty()

    override fun onVerificationLearnMoreClicked() {
        openUrlInChromeCustomTab(
                context = requireContext(),
                session = null,
                url = "https://docs.element.io/latest/element-support/device-verification/how-to-verify-devices",
        )
    }

    override fun onVerifySession() {
        (activity as? PcpActivity<*>)?.let { vectorBaseActivity ->
            vectorBaseActivity.navigator
                    .requestSelfSessionVerification(vectorBaseActivity)
        }
    }

    companion object {
        private const val HOME_ROOM_LIST_FRAGMENT_TAG = "TAG_HOME_ROOM_LIST"
    }
}
