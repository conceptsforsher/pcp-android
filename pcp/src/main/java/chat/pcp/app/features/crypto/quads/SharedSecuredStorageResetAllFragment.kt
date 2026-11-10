/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.quads

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.activityViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.setTextOrHide
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentSsssResetAllBinding
import chat.pcp.app.features.roommemberprofile.devices.DeviceListBottomSheet
import chat.pcp.lib.strings.CommonPlurals

@AndroidEntryPoint
class SharedSecuredStorageResetAllFragment :
        PcpFragment<FragmentSsssResetAllBinding>() {

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentSsssResetAllBinding {
        return FragmentSsssResetAllBinding.inflate(inflater, container, false)
    }

    val sharedViewModel: SharedSecureStorageViewModel by activityViewModel()

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        views.ssssResetButtonReset.debouncedClicks {
            sharedViewModel.handle(SharedSecureStorageAction.DoResetAll)
        }

        views.ssssResetButtonCancel.debouncedClicks {
            sharedViewModel.handle(SharedSecureStorageAction.Back)
        }

        views.ssssResetOtherDevices.debouncedClicks {
            withState(sharedViewModel) {
                DeviceListBottomSheet.newInstance(it.userId).show(childFragmentManager, "DEV_LIST")
            }
        }

        sharedViewModel.onEach { state ->
            views.ssssResetOtherDevices.setTextOrHide(
                    state.activeDeviceCount
                            .takeIf { it > 0 }
                            ?.let { resources.getQuantityString(CommonPlurals.secure_backup_reset_devices_you_can_verify, it, it) }
            )
        }
    }
}
