/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.homeserver

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentGenericRecyclerBinding
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Display some information about the homeserver.
 */
@AndroidEntryPoint
class HomeserverSettingsFragment :
        PcpFragment<FragmentGenericRecyclerBinding>(),
        HomeserverSettingsController.Callback {

    @Inject lateinit var homeserverSettingsController: HomeserverSettingsController

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentGenericRecyclerBinding {
        return FragmentGenericRecyclerBinding.inflate(inflater, container, false)
    }

    private val viewModel: HomeserverSettingsViewModel by fragmentViewModel()

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        homeserverSettingsController.callback = this
        views.genericRecyclerView.configureWith(homeserverSettingsController)
    }

    override fun onDestroyView() {
        homeserverSettingsController.callback = null
        views.genericRecyclerView.cleanup()
        super.onDestroyView()
    }

    override fun onResume() {
        super.onResume()
        (activity as? AppCompatActivity)?.supportActionBar?.setTitle(CommonStrings.settings_home_server)
    }

    override fun retry() {
        viewModel.handle(HomeserverSettingsAction.Refresh)
    }

    override fun invalidate() = withState(viewModel) { state ->
        homeserverSettingsController.setData(state)
    }
}
