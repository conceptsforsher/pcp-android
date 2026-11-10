/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail

import android.content.Context
import android.content.Intent
import android.graphics.Color
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.core.view.GravityCompat
import androidx.drawerlayout.widget.DrawerLayout
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.lifecycle.lifecycleScope
import com.airbnb.mvrx.Mavericks
import com.airbnb.mvrx.viewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.endKeepScreenOn
import chat.pcp.app.core.extensions.hideKeyboard
import chat.pcp.app.core.extensions.keepScreenOn
import chat.pcp.app.core.extensions.replaceFragment
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivityRoomDetailBinding
import chat.pcp.app.features.MainActivity
import im.vector.app.features.analytics.plan.MobileScreen
import im.vector.app.features.analytics.plan.ViewRoom
import chat.pcp.app.features.home.room.breadcrumbs.BreadcrumbsFragment
import chat.pcp.app.features.home.room.detail.arguments.TimelineArgs
import chat.pcp.app.features.home.room.detail.timeline.helper.AudioMessagePlaybackTracker
import chat.pcp.app.features.matrixto.MatrixToBottomSheet
import chat.pcp.app.features.navigation.Navigator
import chat.pcp.app.features.room.RequireActiveMembershipAction
import chat.pcp.app.features.room.RequireActiveMembershipViewEvents
import chat.pcp.app.features.room.RequireActiveMembershipViewModel
import chat.pcp.lib.core.utils.compat.getParcelableCompat
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import javax.inject.Inject

@AndroidEntryPoint
class RoomDetailActivity :
        PcpActivity<ActivityRoomDetailBinding>(),
        MatrixToBottomSheet.InteractionListener {

    override fun getBinding() = ActivityRoomDetailBinding.inflate(layoutInflater)

    private val fragmentLifecycleCallbacks = object : FragmentManager.FragmentLifecycleCallbacks() {

        override fun onFragmentResumed(fm: FragmentManager, f: Fragment) {
            if (f is MatrixToBottomSheet) {
                f.interactionListener = this@RoomDetailActivity
            }
            super.onFragmentResumed(fm, f)
        }

        override fun onFragmentPaused(fm: FragmentManager, f: Fragment) {
            if (f is MatrixToBottomSheet) {
                f.interactionListener = null
            }
            super.onFragmentPaused(fm, f)
        }
    }

    private var lastKnownPlayingOrRecordingState: Boolean? = null
    private val playbackActivityListener = AudioMessagePlaybackTracker.ActivityListener { isPlayingOrRecording ->
        if (lastKnownPlayingOrRecordingState == isPlayingOrRecording) return@ActivityListener
        when (isPlayingOrRecording) {
            true -> keepScreenOn()
            false -> endKeepScreenOn()
        }
        lastKnownPlayingOrRecordingState = isPlayingOrRecording
    }

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    @Inject lateinit var playbackTracker: AudioMessagePlaybackTracker
    private lateinit var sharedActionViewModel: RoomDetailSharedActionViewModel
    private val requireActiveMembershipViewModel: RequireActiveMembershipViewModel by viewModel()

    // Simple filter
    var currentRoomId: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // For dealing with insets and status bar background color
        @Suppress("DEPRECATION")
        window.statusBarColor = Color.TRANSPARENT

        supportFragmentManager.registerFragmentLifecycleCallbacks(fragmentLifecycleCallbacks, false)
        waitingView = views.waitingView.waitingView
        val timelineArgs: TimelineArgs = intent?.extras?.getParcelableCompat(EXTRA_ROOM_DETAIL_ARGS) ?: return
        intent.putExtra(Mavericks.KEY_ARG, timelineArgs)
        currentRoomId = timelineArgs.roomId

        if (isFirstCreation()) {
            replaceFragment(views.roomDetailContainer, TimelineFragment::class.java, timelineArgs)
            replaceFragment(views.roomDetailDrawerContainer, BreadcrumbsFragment::class.java)
        }

        sharedActionViewModel = viewModelProvider.get(RoomDetailSharedActionViewModel::class.java)

        sharedActionViewModel
                .stream()
                .onEach { sharedAction ->
                    when (sharedAction) {
                        is RoomDetailSharedAction.SwitchToRoom -> switchToRoom(sharedAction)
                    }
                }
                .launchIn(lifecycleScope)

        requireActiveMembershipViewModel.observeViewEvents {
            when (it) {
                is RequireActiveMembershipViewEvents.RoomLeft -> handleRoomLeft(it)
            }
        }
        views.drawerLayout.addDrawerListener(drawerListener)

        playbackTracker.trackActivity(playbackActivityListener)
    }

    private fun handleRoomLeft(roomLeft: RequireActiveMembershipViewEvents.RoomLeft) {
        if (roomLeft.leftMessage != null) {
            Toast.makeText(this, roomLeft.leftMessage, Toast.LENGTH_LONG).show()
        }
        finish()
    }

    private fun switchToRoom(switchToRoom: RoomDetailSharedAction.SwitchToRoom) {
        views.drawerLayout.closeDrawer(GravityCompat.START)
        // Do not replace the Fragment if it's the same roomId
        if (currentRoomId != switchToRoom.roomId) {
            currentRoomId = switchToRoom.roomId
            requireActiveMembershipViewModel.handle(RequireActiveMembershipAction.ChangeRoom(switchToRoom.roomId))
            replaceFragment(views.roomDetailContainer, TimelineFragment::class.java, TimelineArgs(switchToRoom.roomId))
        }
    }

    override fun onDestroy() {
        supportFragmentManager.unregisterFragmentLifecycleCallbacks(fragmentLifecycleCallbacks)
        views.drawerLayout.removeDrawerListener(drawerListener)
        playbackTracker.untrackActivity(playbackActivityListener)
        super.onDestroy()
    }

    private val drawerListener = object : DrawerLayout.SimpleDrawerListener() {
        override fun onDrawerOpened(drawerView: View) {
            analyticsTracker.screen(MobileScreen(screenName = MobileScreen.ScreenName.Breadcrumbs))
        }

        override fun onDrawerStateChanged(newState: Int) {
            hideKeyboard()

            if (!views.drawerLayout.isDrawerOpen(GravityCompat.START) && newState == DrawerLayout.STATE_DRAGGING) {
                // User is starting to open the drawer, scroll the list to top
                scrollBreadcrumbsToTop()
            }
        }
    }

    private fun scrollBreadcrumbsToTop() {
        supportFragmentManager.fragments.filterIsInstance<BreadcrumbsFragment>()
                .forEach { it.scrollToTop() }
    }

    @Suppress("OVERRIDE_DEPRECATION")
    override fun onBackPressed() {
        if (views.drawerLayout.isDrawerOpen(GravityCompat.START)) {
            views.drawerLayout.closeDrawer(GravityCompat.START)
        } else {
            super.onBackPressed()
        }
    }

    companion object {
        const val EXTRA_ROOM_DETAIL_ARGS = "EXTRA_ROOM_DETAIL_ARGS"

        fun newIntent(context: Context, timelineArgs: TimelineArgs, firstStartMainActivity: Boolean): Intent {
            val intent = Intent(context, RoomDetailActivity::class.java).apply {
                putExtra(EXTRA_ROOM_DETAIL_ARGS, timelineArgs)
            }
            return if (firstStartMainActivity) {
                MainActivity.getIntentWithNextIntent(context, intent)
            } else {
                intent
            }
        }
    }

    override fun mxToBottomSheetNavigateToRoom(roomId: String, trigger: ViewRoom.Trigger?) {
        navigator.openRoom(this, roomId, trigger = trigger)
    }

    override fun mxToBottomSheetSwitchToSpace(spaceId: String) {
        navigator.switchToSpace(this, spaceId, Navigator.PostSwitchSpaceAction.None)
    }
}
