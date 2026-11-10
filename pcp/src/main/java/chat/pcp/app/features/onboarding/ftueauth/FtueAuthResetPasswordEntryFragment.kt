/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.associateContentStateWith
import chat.pcp.app.core.extensions.clearErrorOnChange
import chat.pcp.app.core.extensions.content
import chat.pcp.app.core.extensions.editText
import chat.pcp.app.core.extensions.hidePassword
import chat.pcp.app.core.extensions.setOnImeDoneListener
import chat.pcp.app.databinding.FragmentFtueResetPasswordInputBinding
import chat.pcp.app.features.onboarding.OnboardingAction
import chat.pcp.app.features.onboarding.OnboardingViewState
import org.matrix.android.sdk.api.failure.isMissingEmailVerification

@AndroidEntryPoint
class FtueAuthResetPasswordEntryFragment :
        AbstractFtueAuthFragment<FragmentFtueResetPasswordInputBinding>() {

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueResetPasswordInputBinding {
        return FragmentFtueResetPasswordInputBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupViews()
    }

    private fun setupViews() {
        views.newPasswordInput.associateContentStateWith(button = views.newPasswordSubmit)
        views.newPasswordInput.setOnImeDoneListener { resetPassword() }
        views.newPasswordInput.clearErrorOnChange(viewLifecycleOwner)
        views.newPasswordSubmit.debouncedClicks { resetPassword() }
    }

    private fun resetPassword() {
        viewModel.handle(
                OnboardingAction.ConfirmNewPassword(
                        newPassword = views.newPasswordInput.content(),
                        signOutAllDevices = views.entrySignOutAll.isChecked
                )
        )
    }

    override fun onError(throwable: Throwable) {
        when {
            throwable.isMissingEmailVerification() -> super.onError(throwable)
            else -> {
                views.newPasswordInput.error = errorFormatter.toHumanReadable(throwable)
            }
        }
    }

    override fun updateWithState(state: OnboardingViewState) {
        views.signedOutAllGroup.isVisible = state.resetState.supportsLogoutAllDevices

        if (state.isLoading) {
            views.newPasswordInput.editText().hidePassword()
        }
    }

    override fun resetViewModel() {
        viewModel.handle(OnboardingAction.ResetResetPassword)
    }
}
