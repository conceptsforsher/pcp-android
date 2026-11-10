/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.threads

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import androidx.fragment.app.FragmentTransaction
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.addFragmentToBackstack
import chat.pcp.app.core.extensions.replaceFragment
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivityThreadsBinding
import chat.pcp.app.features.MainActivity
import chat.pcp.app.features.analytics.extensions.toAnalyticsInteraction
import im.vector.app.features.analytics.plan.Interaction
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.home.room.detail.TimelineFragment
import chat.pcp.app.features.home.room.detail.arguments.TimelineArgs
import chat.pcp.app.features.home.room.threads.arguments.ThreadListArgs
import chat.pcp.app.features.home.room.threads.arguments.ThreadTimelineArgs
import chat.pcp.app.features.home.room.threads.list.views.ThreadListFragment
import chat.pcp.lib.core.utils.compat.getParcelableCompat
import javax.inject.Inject

@AndroidEntryPoint
class ThreadsActivity : PcpActivity<ActivityThreadsBinding>() {

    @Inject lateinit var avatarRenderer: AvatarRenderer

//    private val roomThreadDetailFragment: RoomThreadDetailFragment?
//        get() {
//            return supportFragmentManager.findFragmentByTag(FRAGMENT_TAG) as? RoomThreadDetailFragment
//        }

    override fun getBinding() = ActivityThreadsBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        initFragment()
    }

    private fun initFragment() {
        if (isFirstCreation()) {
            when (val fragment = fragmentToNavigate()) {
                is DisplayFragment.ThreadList -> {
                    initThreadListFragment(fragment.threadListArgs)
                }
                is DisplayFragment.ThreadTimeLine -> {
                    initThreadTimelineFragment(fragment.threadTimelineArgs)
                }
                is DisplayFragment.ErrorFragment -> {
                    finish()
                }
            }
        }
    }

    private fun initThreadListFragment(threadListArgs: ThreadListArgs) {
        replaceFragment(
                views.threadsActivityFragmentContainer,
                ThreadListFragment::class.java,
                threadListArgs
        )
    }

    private fun initThreadTimelineFragment(threadTimelineArgs: ThreadTimelineArgs) =
            replaceFragment(
                    views.threadsActivityFragmentContainer,
                    TimelineFragment::class.java,
                    TimelineArgs(
                            roomId = threadTimelineArgs.roomId,
                            eventId = getEventIdToNavigate(),
                            threadTimelineArgs = threadTimelineArgs
                    )
            )

    /**
     * This function is used to navigate to the selected thread timeline.
     * One usage of that is from the Threads Activity
     */
    fun navigateToThreadTimeline(threadTimelineArgs: ThreadTimelineArgs) {
        analyticsTracker.capture(Interaction.Name.MobileThreadListThreadItem.toAnalyticsInteraction())
        val commonOption: (FragmentTransaction) -> Unit = {
            it.setCustomAnimations(
                    chat.pcp.lib.ui.styles.R.anim.animation_slide_in_right,
                    chat.pcp.lib.ui.styles.R.anim.animation_slide_out_left,
                    chat.pcp.lib.ui.styles.R.anim.animation_slide_in_left,
                    chat.pcp.lib.ui.styles.R.anim.animation_slide_out_right
            )
        }
        addFragmentToBackstack(
                container = views.threadsActivityFragmentContainer,
                fragmentClass = TimelineFragment::class.java,
                params = TimelineArgs(
                        roomId = threadTimelineArgs.roomId,
                        threadTimelineArgs = threadTimelineArgs
                ),
                option = commonOption
        )
    }

    /**
     * Determine in witch fragment we should navigate.
     */
    private fun fragmentToNavigate(): DisplayFragment {
        getThreadTimelineArgs()?.let {
            return DisplayFragment.ThreadTimeLine(it)
        }
        getThreadListArgs()?.let {
            return DisplayFragment.ThreadList(it)
        }
        return DisplayFragment.ErrorFragment
    }

    private fun getThreadTimelineArgs(): ThreadTimelineArgs? = intent?.extras?.getParcelableCompat(THREAD_TIMELINE_ARGS)
    private fun getThreadListArgs(): ThreadListArgs? = intent?.extras?.getParcelableCompat(THREAD_LIST_ARGS)
    private fun getEventIdToNavigate(): String? = intent?.extras?.getString(THREAD_EVENT_ID_TO_NAVIGATE)

    companion object {
        //        private val FRAGMENT_TAG = RoomThreadDetailFragment::class.simpleName
        const val THREAD_TIMELINE_ARGS = "THREAD_TIMELINE_ARGS"
        const val THREAD_EVENT_ID_TO_NAVIGATE = "THREAD_EVENT_ID_TO_NAVIGATE"
        const val THREAD_LIST_ARGS = "THREAD_LIST_ARGS"

        fun newIntent(
                context: Context,
                threadTimelineArgs: ThreadTimelineArgs?,
                threadListArgs: ThreadListArgs?,
                eventIdToNavigate: String? = null,
                firstStartMainActivity: Boolean = false
        ): Intent {
            val intent = Intent(context, ThreadsActivity::class.java).apply {
                putExtra(THREAD_TIMELINE_ARGS, threadTimelineArgs)
                putExtra(THREAD_EVENT_ID_TO_NAVIGATE, eventIdToNavigate)
                putExtra(THREAD_LIST_ARGS, threadListArgs)
            }

            return if (firstStartMainActivity) {
                MainActivity.getIntentWithNextIntent(context, intent)
            } else {
                intent
            }
        }
    }

    sealed class DisplayFragment {
        data class ThreadList(val threadListArgs: ThreadListArgs) : DisplayFragment()
        data class ThreadTimeLine(val threadTimelineArgs: ThreadTimelineArgs) : DisplayFragment()
        object ErrorFragment : DisplayFragment()
    }
}
