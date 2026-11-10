/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import android.annotation.SuppressLint
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.lifecycle.DefaultLifecycleObserver
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.lifecycleScope
import androidx.viewpager2.widget.ViewPager2
import com.google.android.material.tabs.TabLayoutMediator
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.incrementByOneAndWrap
import chat.pcp.app.core.extensions.setCurrentItem
import chat.pcp.app.core.resources.BuildMeta
import chat.pcp.app.databinding.FragmentFtueSplashCarouselBinding
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.onboarding.OnboardingAction
import chat.pcp.app.features.onboarding.OnboardingFlow
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.Job
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import javax.inject.Inject

private const val CAROUSEL_ROTATION_DELAY_MS = 5000L
private const val CAROUSEL_TRANSITION_TIME_MS = 500L

@AndroidEntryPoint
class FtueAuthSplashCarouselFragment :
        AbstractFtueAuthFragment<FragmentFtueSplashCarouselBinding>() {

    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var vectorFeatures: PcpFeatures
    @Inject lateinit var carouselController: SplashCarouselController
    @Inject lateinit var carouselStateFactory: SplashCarouselStateFactory
    @Inject lateinit var buildMeta: BuildMeta

    private var tabLayoutMediator: TabLayoutMediator? = null

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueSplashCarouselBinding {
        return FragmentFtueSplashCarouselBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupViews()
    }

    override fun onDestroyView() {
        tabLayoutMediator?.detach()
        tabLayoutMediator = null
        views.splashCarousel.adapter = null
        super.onDestroyView()
    }

    private fun setupViews() {
        val carouselAdapter = carouselController.adapter
        views.splashCarousel.adapter = carouselAdapter
        tabLayoutMediator = TabLayoutMediator(views.carouselIndicator, views.splashCarousel) { _, _ -> }
                .also { it.attach() }

        carouselController.setData(carouselStateFactory.create())

        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.apply {
            setText(if (isAlreadyHaveAccountEnabled) CommonStrings.login_splash_create_account else CommonStrings.login_splash_submit)
            debouncedClicks { splashSubmit(isAlreadyHaveAccountEnabled) }
        }
        views.loginSplashAlreadyHaveAccount.apply {
            isVisible = isAlreadyHaveAccountEnabled
            debouncedClicks { alreadyHaveAnAccount() }
        }

        if (buildMeta.isDebug || pcpPreferences.developerMode()) {
            views.loginSplashVersion.isVisible = true
            @SuppressLint("SetTextI18n")
            views.loginSplashVersion.text = "Version : ${buildMeta.versionName}\n" +
                    "Branch: ${buildMeta.gitBranchName} ${buildMeta.gitRevision}"
            views.loginSplashVersion.debouncedClicks { navigator.openDebug(requireContext()) }
        }
        views.splashCarousel.registerAutomaticUntilInteractionTransitions()
    }

    private fun ViewPager2.registerAutomaticUntilInteractionTransitions() {
        var scheduledTransition: Job? = null
        val pageChangingCallback = object : ViewPager2.OnPageChangeCallback() {
            private var hasUserManuallyInteractedWithCarousel: Boolean = false

            override fun onPageScrolled(position: Int, positionOffset: Float, positionOffsetPixels: Int) {
                hasUserManuallyInteractedWithCarousel = !isFakeDragging
            }

            override fun onPageSelected(position: Int) {
                scheduledTransition?.cancel()
                // only schedule automatic transitions whilst the user has not interacted with the carousel
                if (!hasUserManuallyInteractedWithCarousel) {
                    scheduledTransition = scheduleCarouselTransition()
                }
            }
        }
        viewLifecycleOwner.lifecycle.addObserver(object : DefaultLifecycleObserver {
            override fun onCreate(owner: LifecycleOwner) {
                registerOnPageChangeCallback(pageChangingCallback)
            }

            override fun onDestroy(owner: LifecycleOwner) {
                unregisterOnPageChangeCallback(pageChangingCallback)
            }
        })
    }

    private fun ViewPager2.scheduleCarouselTransition(): Job {
        val itemCount = adapter?.itemCount ?: throw IllegalStateException("An adapter must be set")
        return viewLifecycleOwner.lifecycleScope.launch {
            delay(CAROUSEL_ROTATION_DELAY_MS)
            setCurrentItem(currentItem.incrementByOneAndWrap(max = itemCount - 1), duration = CAROUSEL_TRANSITION_TIME_MS)
        }
    }

    private fun splashSubmit(isAlreadyHaveAccountEnabled: Boolean) {
        val getStartedFlow = if (isAlreadyHaveAccountEnabled) OnboardingFlow.SignUp else OnboardingFlow.SignInSignUp
        viewModel.handle(OnboardingAction.SplashAction.OnGetStarted(onboardingFlow = getStartedFlow))
    }

    private fun alreadyHaveAnAccount() {
        viewModel.handle(OnboardingAction.SplashAction.OnIAlreadyHaveAnAccount(onboardingFlow = OnboardingFlow.SignIn))
    }

    override fun resetViewModel() {
        // Nothing to do
    }
}
