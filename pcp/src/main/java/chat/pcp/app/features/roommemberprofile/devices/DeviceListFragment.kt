/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.roommemberprofile.devices

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.parentFragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.DimensionConverter
import chat.pcp.app.databinding.BottomSheetGenericListBinding
import org.matrix.android.sdk.api.session.crypto.model.CryptoDeviceInfo
import javax.inject.Inject

@AndroidEntryPoint
class DeviceListFragment :
        PcpFragment<BottomSheetGenericListBinding>(),
        DeviceListEpoxyController.InteractionListener {

    @Inject lateinit var dimensionConverter: DimensionConverter
    @Inject lateinit var epoxyController: DeviceListEpoxyController

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): BottomSheetGenericListBinding {
        return BottomSheetGenericListBinding.inflate(inflater, container, false)
    }

    private val viewModel: DeviceListBottomSheetViewModel by parentFragmentViewModel(DeviceListBottomSheetViewModel::class)

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        views.bottomSheetRecyclerView.setPadding(0, dimensionConverter.dpToPx(16), 0, dimensionConverter.dpToPx(16))
        views.bottomSheetRecyclerView.configureWith(
                epoxyController,
                hasFixedSize = false
        )
        epoxyController.interactionListener = this
    }

    override fun onDestroyView() {
        views.bottomSheetRecyclerView.cleanup()
        super.onDestroyView()
    }

    override fun invalidate() = withState(viewModel) {
        epoxyController.setData(it)
    }

    override fun onDeviceSelected(device: CryptoDeviceInfo) {
        viewModel.handle(DeviceListAction.SelectDevice(device))
    }
}
