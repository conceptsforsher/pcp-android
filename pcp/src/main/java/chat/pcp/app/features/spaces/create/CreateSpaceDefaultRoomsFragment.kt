/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.create

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.activityViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.extensions.hideKeyboard
import chat.pcp.app.core.platform.OnBackPressed
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentSpaceCreateGenericEpoxyFormBinding
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

@AndroidEntryPoint
class CreateSpaceDefaultRoomsFragment :
        PcpFragment<FragmentSpaceCreateGenericEpoxyFormBinding>(),
        SpaceDefaultRoomEpoxyController.Listener,
        OnBackPressed {

    @Inject lateinit var epoxyController: SpaceDefaultRoomEpoxyController

    private val sharedViewModel: CreateSpaceViewModel by activityViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?) =
            FragmentSpaceCreateGenericEpoxyFormBinding.inflate(layoutInflater, container, false)

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        views.recyclerView.configureWith(epoxyController)
        epoxyController.listener = this

        sharedViewModel.onEach {
            epoxyController.setData(it)
        }

        views.nextButton.setText(CommonStrings.create_space)
        views.nextButton.debouncedClicks {
            view.hideKeyboard()
            sharedViewModel.handle(CreateSpaceAction.NextFromDefaultRooms)
        }
    }

    override fun onNameChange(index: Int, newName: String) {
        sharedViewModel.handle(CreateSpaceAction.DefaultRoomNameChanged(index, newName))
    }

    override fun onBackPressed(toolbarButton: Boolean): Boolean {
        sharedViewModel.handle(CreateSpaceAction.OnBackPressed)
        return true
    }
}
