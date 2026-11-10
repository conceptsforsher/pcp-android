/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding

import android.content.Context
import android.content.Intent
import android.net.Uri
import android.view.View
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.lazyViewModel
import chat.pcp.app.core.extensions.validateBackPressed
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.core.platform.lifecycleAwareLazy
import chat.pcp.app.databinding.ActivityLoginBinding
import chat.pcp.app.features.login.LoginConfig
import chat.pcp.app.features.pin.UnlockedActivity
import javax.inject.Inject

@AndroidEntryPoint
class OnboardingActivity : PcpActivity<ActivityLoginBinding>(), UnlockedActivity {

    private val onboardingVariant by lifecycleAwareLazy {
        onboardingVariantFactory.create(this, views = views, onboardingViewModel = lazyViewModel())
    }

    @Inject lateinit var onboardingVariantFactory: OnboardingVariantFactory

    override fun getBinding() = ActivityLoginBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)
        onboardingVariant.onNewIntent(intent)
    }

    @Suppress("OVERRIDE_DEPRECATION")
    override fun onBackPressed() {
        validateBackPressed {
            super.onBackPressed()
        }
    }

    override fun initUiAndData() {
        onboardingVariant.initUiAndData(isFirstCreation())
    }

    // Hack for AccountCreatedFragment
    fun setIsLoading(isLoading: Boolean) {
        onboardingVariant.setIsLoading(isLoading)
    }

    companion object {
        const val EXTRA_CONFIG = "EXTRA_CONFIG"

        fun newIntent(context: Context, loginConfig: LoginConfig?): Intent {
            return Intent(context, OnboardingActivity::class.java).apply {
                putExtra(EXTRA_CONFIG, loginConfig)
            }
        }

        fun redirectIntent(context: Context, data: Uri?): Intent {
            return Intent(context, OnboardingActivity::class.java).apply {
                setData(data)
            }
        }
    }
}
