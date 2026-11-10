/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.settings.legals

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.extensions.addFragmentToBackstack
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.FirstThrottler
import chat.pcp.app.core.utils.displayInWebView
import chat.pcp.app.core.utils.openUrlInChromeCustomTab
import chat.pcp.app.databinding.FragmentGenericRecyclerBinding
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.discovery.ServerPolicy
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

@AndroidEntryPoint
class LegalsFragment :
        PcpFragment<FragmentGenericRecyclerBinding>(),
        LegalsController.Listener {

    @Inject lateinit var controller: LegalsController
    @Inject lateinit var flavorLegals: FlavorLegals

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentGenericRecyclerBinding {
        return FragmentGenericRecyclerBinding.inflate(inflater, container, false)
    }

    private val viewModel by fragmentViewModel(LegalsViewModel::class)
    private val firstThrottler = FirstThrottler(1000)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsLegals
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        controller.listener = this
        views.genericRecyclerView.configureWith(controller)
    }

    override fun onDestroyView() {
        views.genericRecyclerView.cleanup()
        controller.listener = null
        super.onDestroyView()
    }

    override fun invalidate() = withState(viewModel) { state ->
        controller.setData(state)
    }

    override fun onResume() {
        super.onResume()
        (activity as? AppCompatActivity)?.supportActionBar?.setTitle(CommonStrings.preference_root_legals)
        viewModel.handle(LegalsAction.Refresh)
    }

    override fun onTapRetry() {
        viewModel.handle(LegalsAction.Refresh)
    }

    override fun openPolicy(policy: ServerPolicy) {
        openUrl(policy.url)
    }

    override fun openThirdPartyNotice() {
        addFragmentToBackstack(R.id.pcp_settings_page, ThirdPartyLicensesFragment::class.java)
    }

    private fun openUrl(url: String) {
        if (firstThrottler.canHandle() is FirstThrottler.CanHandlerResult.Yes) {
            if (url.startsWith("file://")) {
                activity?.displayInWebView(url)
            } else {
                openUrlInChromeCustomTab(requireContext(), null, url)
            }
        }
    }

    override fun openThirdPartyNoticeGplay() {
        if (firstThrottler.canHandle() is FirstThrottler.CanHandlerResult.Yes) {
            flavorLegals.navigateToThirdPartyNotices(requireContext())
        }
    }
}
