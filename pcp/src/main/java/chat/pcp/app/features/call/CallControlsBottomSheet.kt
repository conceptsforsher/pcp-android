/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.content.ContextCompat
import androidx.core.view.isVisible
import com.airbnb.mvrx.activityViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.platform.PcpBottomSheet
import chat.pcp.app.databinding.BottomSheetCallControlsBinding
import chat.pcp.app.features.PcpFeatures
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

@AndroidEntryPoint
class CallControlsBottomSheet : PcpBottomSheet<BottomSheetCallControlsBinding>() {
    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): BottomSheetCallControlsBinding {
        return BottomSheetCallControlsBinding.inflate(inflater, container, false)
    }

    @Inject lateinit var vectorFeatures: PcpFeatures

    private val callViewModel: PcpCallViewModel by activityViewModel()

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        callViewModel.onEach {
            renderState(it)
        }

        views.callControlsSwitchCamera.views.bottomSheetActionClickableZone.debouncedClicks {
            callViewModel.handle(PcpCallViewActions.ToggleCamera)
            dismiss()
        }

        views.callControlsToggleSDHD.views.bottomSheetActionClickableZone.debouncedClicks {
            callViewModel.handle(PcpCallViewActions.ToggleHDSD)
            dismiss()
        }

        views.callControlsToggleHoldResume.views.bottomSheetActionClickableZone.debouncedClicks {
            callViewModel.handle(PcpCallViewActions.ToggleHoldResume)
            dismiss()
        }

        views.callControlsOpenDialPad.views.bottomSheetActionClickableZone.debouncedClicks {
            callViewModel.handle(PcpCallViewActions.OpenDialPad)
        }

        views.callControlsTransfer.views.bottomSheetActionClickableZone.debouncedClicks {
            callViewModel.handle(PcpCallViewActions.InitiateCallTransfer)
            dismiss()
        }

        views.callControlsShareScreen.isVisible = vectorFeatures.isScreenSharingEnabled()
        views.callControlsShareScreen.views.bottomSheetActionClickableZone.debouncedClicks {
            callViewModel.handle(PcpCallViewActions.ToggleScreenSharing)
            dismiss()
        }
    }

    private fun renderState(state: PcpCallViewState) {
        views.callControlsSwitchCamera.isVisible = state.isVideoCall && state.canSwitchCamera
        views.callControlsSwitchCamera.subTitle = getString(if (state.isFrontCamera) CommonStrings.call_camera_front else CommonStrings.call_camera_back)
        if (state.isVideoCall) {
            views.callControlsToggleSDHD.isVisible = true
            if (state.isHD) {
                views.callControlsToggleSDHD.title = getString(CommonStrings.call_format_turn_hd_off)
                views.callControlsToggleSDHD.subTitle = null
                views.callControlsToggleSDHD.leftIcon = ContextCompat.getDrawable(requireContext(), R.drawable.ic_hd_disabled)
            } else {
                views.callControlsToggleSDHD.title = getString(CommonStrings.call_format_turn_hd_on)
                views.callControlsToggleSDHD.subTitle = null
                views.callControlsToggleSDHD.leftIcon = ContextCompat.getDrawable(requireContext(), R.drawable.ic_hd)
            }
        } else {
            views.callControlsToggleSDHD.isVisible = false
        }
        if (state.isRemoteOnHold) {
            views.callControlsToggleHoldResume.title = getString(CommonStrings.call_resume_action)
            views.callControlsToggleHoldResume.subTitle = null
            views.callControlsToggleHoldResume.leftIcon = ContextCompat.getDrawable(requireContext(), R.drawable.ic_call_resume_action)
        } else {
            views.callControlsToggleHoldResume.title = getString(CommonStrings.call_hold_action)
            views.callControlsToggleHoldResume.subTitle = null
            views.callControlsToggleHoldResume.leftIcon = ContextCompat.getDrawable(requireContext(), R.drawable.ic_call_hold_action)
        }
        views.callControlsTransfer.isVisible = state.canOpponentBeTransferred
        views.callControlsShareScreen.title = getString(
                if (state.isSharingScreen) CommonStrings.call_stop_screen_sharing else CommonStrings.call_start_screen_sharing
        )
    }
}
