/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home

import android.app.Dialog
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.platform.PcpBottomSheet
import chat.pcp.app.databinding.FragmentNewChatBottomSheetBinding
import chat.pcp.app.features.navigation.Navigator
import javax.inject.Inject

@AndroidEntryPoint
class NewChatBottomSheet : PcpBottomSheet<FragmentNewChatBottomSheetBinding>() {

    @Inject lateinit var navigator: Navigator

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentNewChatBottomSheetBinding {
        return FragmentNewChatBottomSheetBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        initFABs()
    }

    private fun initFABs() {
        views.startChat.debouncedClicks {
            dismiss()
            navigator.openCreateDirectRoom(requireActivity())
        }

        views.createRoom.debouncedClicks {
            dismiss()
            navigator.openCreateRoom(requireActivity())
        }

        views.exploreRooms.debouncedClicks {
            dismiss()
            navigator.openRoomDirectory(requireContext())
        }
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        return super.onCreateDialog(savedInstanceState).apply {
            setPeekHeightAsScreenPercentage(0.5f)
        }
    }

    companion object {
        const val TAG = "NewChatBottomSheet"
    }
}
