/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import com.airbnb.mvrx.Mavericks
import com.airbnb.mvrx.viewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.registerStartForActivityResult
import chat.pcp.app.core.extensions.replaceFragment
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivitySimpleBinding
import im.vector.app.features.analytics.plan.ViewRoom
import chat.pcp.app.features.matrixto.MatrixToBottomSheet
import chat.pcp.app.features.matrixto.OriginOfMatrixTo
import chat.pcp.app.features.navigation.Navigator
import chat.pcp.app.features.roomdirectory.createroom.CreateRoomActivity
import chat.pcp.app.features.spaces.explore.SpaceDirectoryArgs
import chat.pcp.app.features.spaces.explore.SpaceDirectoryFragment
import chat.pcp.app.features.spaces.explore.SpaceDirectoryViewAction
import chat.pcp.app.features.spaces.explore.SpaceDirectoryViewEvents
import chat.pcp.app.features.spaces.explore.SpaceDirectoryViewModel
import chat.pcp.lib.core.utils.compat.getParcelableExtraCompat
import chat.pcp.lib.strings.CommonStrings

@AndroidEntryPoint
class SpaceExploreActivity : PcpActivity<ActivitySimpleBinding>(), MatrixToBottomSheet.InteractionListener {

    override fun getBinding(): ActivitySimpleBinding = ActivitySimpleBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    override fun getTitleRes(): Int = CommonStrings.space_explore_activity_title

    val sharedViewModel: SpaceDirectoryViewModel by viewModel()

    private val createRoomResultLauncher = registerStartForActivityResult { activityResult ->
        if (activityResult.resultCode == Activity.RESULT_OK) {
            CreateRoomActivity.getCreatedRoomId(activityResult.data)?.let {
                // we want to refresh from API
                sharedViewModel.handle(SpaceDirectoryViewAction.RefreshUntilFound(it))
            }
        }
    }

    private val fragmentLifecycleCallbacks = object : FragmentManager.FragmentLifecycleCallbacks() {
        override fun onFragmentResumed(fm: FragmentManager, f: Fragment) {
            if (f is MatrixToBottomSheet) {
                f.interactionListener = this@SpaceExploreActivity
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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportFragmentManager.registerFragmentLifecycleCallbacks(fragmentLifecycleCallbacks, false)

        if (isFirstCreation()) {
            val args = intent?.getParcelableExtraCompat<SpaceDirectoryArgs>(Mavericks.KEY_ARG)
            replaceFragment(
                    views.simpleFragmentContainer,
                    SpaceDirectoryFragment::class.java,
                    args
            )
        }

        sharedViewModel.observeViewEvents {
            when (it) {
                SpaceDirectoryViewEvents.Dismiss -> {
                    finish()
                }
                is SpaceDirectoryViewEvents.NavigateToRoom -> {
                    navigator.openRoom(
                            context = this,
                            roomId = it.roomId,
                            trigger = ViewRoom.Trigger.SpaceHierarchy
                    )
                }
                is SpaceDirectoryViewEvents.NavigateToMxToBottomSheet -> {
                    MatrixToBottomSheet.withLink(it.link, OriginOfMatrixTo.SPACE_EXPLORE).show(supportFragmentManager, "ShowChild")
                }
                is SpaceDirectoryViewEvents.NavigateToCreateNewRoom -> {
                    createRoomResultLauncher.launch(
                            CreateRoomActivity.getIntent(
                                    this,
                                    openAfterCreate = false,
                                    currentSpaceId = it.currentSpaceId
                            )
                    )
                }
            }
        }
    }

    override fun onDestroy() {
        supportFragmentManager.unregisterFragmentLifecycleCallbacks(fragmentLifecycleCallbacks)
        super.onDestroy()
    }

    companion object {
        fun newIntent(context: Context, spaceId: String): Intent {
            return Intent(context, SpaceExploreActivity::class.java).apply {
                putExtra(Mavericks.KEY_ARG, SpaceDirectoryArgs(spaceId))
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
