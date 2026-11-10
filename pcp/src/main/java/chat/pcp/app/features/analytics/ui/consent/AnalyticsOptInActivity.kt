/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.analytics.ui.consent

import android.view.View
import com.airbnb.mvrx.viewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.addFragment
import chat.pcp.app.core.platform.ScreenOrientationLocker
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivitySimpleBinding
import javax.inject.Inject

/**
 * Simple container for AnalyticsOptInFragment.
 */
@AndroidEntryPoint
class AnalyticsOptInActivity : PcpActivity<ActivitySimpleBinding>() {

    @Inject lateinit var orientationLocker: ScreenOrientationLocker

    private val viewModel: AnalyticsConsentViewModel by viewModel()

    override fun getBinding() = ActivitySimpleBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    override fun initUiAndData() {
        orientationLocker.lockPhonesToPortrait(this)
        if (isFirstCreation()) {
            addFragment(views.simpleFragmentContainer, AnalyticsOptInFragment::class.java)
        }

        viewModel.observeViewEvents {
            when (it) {
                AnalyticsOptInViewEvents.OnDataSaved -> finish()
            }
        }
    }
}
