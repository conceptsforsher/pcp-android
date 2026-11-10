/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.epoxy.SimpleEpoxyController
import com.airbnb.mvrx.activityViewModel
import chat.pcp.app.core.epoxy.bottomsheet.BottomSheetActionItem_
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpBottomSheet
import chat.pcp.app.databinding.BottomSheetGenericListBinding
import chat.pcp.app.features.call.audio.CallAudioManager
import chat.pcp.app.features.home.room.list.actions.RoomListQuickActionsBottomSheet

class CallSoundDeviceChooserBottomSheet : PcpBottomSheet<BottomSheetGenericListBinding>() {
    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): BottomSheetGenericListBinding {
        return BottomSheetGenericListBinding.inflate(inflater, container, false)
    }

    private val callViewModel: PcpCallViewModel by activityViewModel()
    private val controller = SimpleEpoxyController()

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        views.bottomSheetRecyclerView.configureWith(controller, hasFixedSize = false)
        callViewModel.observeViewEvents {
            when (it) {
                is PcpCallViewEvents.ShowSoundDeviceChooser -> {
                    render(it.available, it.current)
                }
                else -> {
                }
            }
        }
        callViewModel.handle(PcpCallViewActions.SwitchSoundDevice)
    }

    private fun render(available: Set<CallAudioManager.Device>, current: CallAudioManager.Device) {
        val models = available.map { device ->
            val title = when (device) {
                is CallAudioManager.Device.WirelessHeadset -> device.name ?: getString(device.titleRes)
                else -> getString(device.titleRes)
            }
            BottomSheetActionItem_().apply {
                id(device.titleRes)
                text(title)
                iconRes(device.drawableRes)
                selected(current == device)
                listener {
                    callViewModel.handle(PcpCallViewActions.ChangeAudioDevice(device))
                    dismiss()
                }
            }
        }
        controller.setModels(models)
    }

    companion object {
        fun newInstance(): RoomListQuickActionsBottomSheet {
            return RoomListQuickActionsBottomSheet()
        }
    }
}
