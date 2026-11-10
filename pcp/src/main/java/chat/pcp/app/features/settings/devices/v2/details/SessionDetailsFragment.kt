/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.details

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isGone
import androidx.core.view.isVisible
import com.airbnb.mvrx.Success
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.platform.showOptimizedSnackbar
import chat.pcp.app.databinding.FragmentSessionDetailsBinding
import chat.pcp.app.features.settings.devices.v2.DeviceFullInfo
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Display the details info about a Session.
 */
@AndroidEntryPoint
class SessionDetailsFragment :
        PcpFragment<FragmentSessionDetailsBinding>() {

    @Inject lateinit var sessionDetailsController: SessionDetailsController

    private val viewModel: SessionDetailsViewModel by fragmentViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentSessionDetailsBinding {
        return FragmentSessionDetailsBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        initToolbar()
        initSessionDetails()
        observeViewEvents()
    }

    private fun initToolbar() {
        (activity as? AppCompatActivity)
                ?.supportActionBar
                ?.setTitle(CommonStrings.device_manager_session_details_title)
    }

    private fun initSessionDetails() {
        sessionDetailsController.callback = object : SessionDetailsController.Callback {
            override fun onItemLongClicked(content: String) {
                viewModel.handle(SessionDetailsAction.CopyToClipboard(content))
            }
        }
        views.sessionDetails.configureWith(sessionDetailsController)
    }

    private fun observeViewEvents() {
        viewModel.observeViewEvents { viewEvent ->
            when (viewEvent) {
                SessionDetailsViewEvent.ContentCopiedToClipboard -> view?.showOptimizedSnackbar(getString(CommonStrings.copied_to_clipboard))
            }
        }
    }

    override fun onDestroyView() {
        cleanUpSessionDetails()
        super.onDestroyView()
    }

    private fun cleanUpSessionDetails() {
        sessionDetailsController.callback = null
        views.sessionDetails.cleanup()
    }

    override fun invalidate() = withState(viewModel) { state ->
        if (state.deviceFullInfo is Success) {
            renderSessionDetails(state.deviceFullInfo.invoke())
        } else {
            hideSessionDetails()
        }
    }

    private fun renderSessionDetails(deviceFullInfo: DeviceFullInfo) {
        views.sessionDetails.isVisible = true
        sessionDetailsController.setData(deviceFullInfo)
    }

    private fun hideSessionDetails() {
        views.sessionDetails.isGone = true
    }
}
