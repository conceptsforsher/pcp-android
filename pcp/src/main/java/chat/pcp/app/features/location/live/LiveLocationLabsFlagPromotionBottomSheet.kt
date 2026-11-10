/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location.live

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.setFragmentResult
import chat.pcp.app.core.platform.PcpBottomSheet
import chat.pcp.app.databinding.BottomSheetLiveLocationLabsFlagPromotionBinding

/**
 * Bottom sheet to warn users that feature is still in active development. Users are able to enable labs flag by using the switch in this bottom sheet.
 * This should not be shown if the user already enabled the labs flag.
 */
class LiveLocationLabsFlagPromotionBottomSheet :
        PcpBottomSheet<BottomSheetLiveLocationLabsFlagPromotionBinding>() {

    override val showExpanded = true

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): BottomSheetLiveLocationLabsFlagPromotionBinding {
        return BottomSheetLiveLocationLabsFlagPromotionBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        initOkButton()
    }

    private fun initOkButton() {
        views.promoteLiveLocationFlagOkButton.debouncedClicks {
            val enableLabsFlag = views.promoteLiveLocationFlagSwitch.isChecked
            setFragmentResult(REQUEST_KEY, Bundle().apply {
                putBoolean(BUNDLE_KEY_LABS_APPROVAL, enableLabsFlag)
            })
            dismiss()
        }
    }

    companion object {

        const val REQUEST_KEY = "LiveLocationLabsFlagPromotionBottomSheetRequest"
        const val BUNDLE_KEY_LABS_APPROVAL = "BUNDLE_KEY_LABS_APPROVAL"

        fun newInstance() = LiveLocationLabsFlagPromotionBottomSheet()
    }
}
