/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.breadcrumbs

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.updatePadding
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentBreadcrumbsBinding
import chat.pcp.app.features.home.room.detail.RoomDetailSharedAction
import chat.pcp.app.features.home.room.detail.RoomDetailSharedActionViewModel
import javax.inject.Inject

@AndroidEntryPoint
class BreadcrumbsFragment :
        PcpFragment<FragmentBreadcrumbsBinding>(),
        BreadcrumbsController.Listener {

    @Inject lateinit var breadcrumbsController: BreadcrumbsController

    private lateinit var sharedActionViewModel: RoomDetailSharedActionViewModel
    private val breadcrumbsViewModel: BreadcrumbsViewModel by fragmentViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentBreadcrumbsBinding {
        return FragmentBreadcrumbsBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        ViewCompat.setOnApplyWindowInsetsListener(views.breadcrumbsRecyclerView) { v, insets ->
            val systemBars = insets.getInsets(
                    WindowInsetsCompat.Type.systemBars() or
                            WindowInsetsCompat.Type.displayCutout()
            )
            v.updatePadding(
                    systemBars.left,
                    systemBars.top,
                    systemBars.right,
                    systemBars.bottom,
            )
            WindowInsetsCompat.CONSUMED
        }
        setupRecyclerView()
        sharedActionViewModel = activityViewModelProvider.get(RoomDetailSharedActionViewModel::class.java)
    }

    override fun onDestroyView() {
        views.breadcrumbsRecyclerView.cleanup()
        breadcrumbsController.listener = null
        super.onDestroyView()
    }

    private fun setupRecyclerView() {
        views.breadcrumbsRecyclerView.configureWith(breadcrumbsController, BreadcrumbsAnimator(), hasFixedSize = false)
        breadcrumbsController.listener = this
    }

    override fun invalidate() = withState(breadcrumbsViewModel) { state ->
        breadcrumbsController.update(state)
    }

    // BreadcrumbsController.Listener **************************************************************

    override fun onBreadcrumbClicked(roomId: String) {
        sharedActionViewModel.post(RoomDetailSharedAction.SwitchToRoom(roomId))
    }

    fun scrollToTop() {
        views.breadcrumbsRecyclerView.scrollToPosition(0)
    }
}
