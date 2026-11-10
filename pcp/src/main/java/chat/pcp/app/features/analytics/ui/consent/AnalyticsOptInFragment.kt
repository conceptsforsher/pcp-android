/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.analytics.ui.consent

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.activityViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.setTextWithColoredPart
import chat.pcp.app.core.platform.OnBackPressed
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.openUrlInChromeCustomTab
import chat.pcp.app.databinding.FragmentAnalyticsOptinBinding
import chat.pcp.app.features.analytics.AnalyticsConfig
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

@AndroidEntryPoint
class AnalyticsOptInFragment :
        PcpFragment<FragmentAnalyticsOptinBinding>(),
        OnBackPressed {

    @Inject lateinit var analyticsConfig: AnalyticsConfig

    // Share the view model with the Activity so that the Activity
    // can decide what to do when the data has been saved
    private val viewModel: AnalyticsConsentViewModel by activityViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentAnalyticsOptinBinding {
        return FragmentAnalyticsOptinBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupLink()
        setupListeners()
    }

    private fun setupListeners() {
        views.submit.debouncedClicks {
            viewModel.handle(AnalyticsConsentViewActions.SetUserConsent(userConsent = true))
        }
        views.later.debouncedClicks {
            viewModel.handle(AnalyticsConsentViewActions.SetUserConsent(userConsent = false))
        }
    }

    private fun setupLink() {
        views.subtitle.setTextWithColoredPart(
                fullTextRes = CommonStrings.analytics_opt_in_content,
                coloredTextRes = CommonStrings.analytics_opt_in_content_link,
                onClick = {
                    openUrlInChromeCustomTab(requireContext(), null, analyticsConfig.policyLink)
                }
        )
    }

    override fun onBackPressed(toolbarButton: Boolean): Boolean {
        // Consider user does not give consent
        viewModel.handle(AnalyticsConsentViewActions.SetUserConsent(userConsent = false))
        // And consume the event
        return true
    }
}
