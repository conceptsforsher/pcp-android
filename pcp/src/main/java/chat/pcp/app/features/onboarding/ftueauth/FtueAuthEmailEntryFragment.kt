/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import android.os.Bundle
import android.os.Parcelable
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.args
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.associateContentStateWith
import chat.pcp.app.core.extensions.autofillEmail
import chat.pcp.app.core.extensions.clearErrorOnChange
import chat.pcp.app.core.extensions.content
import chat.pcp.app.core.extensions.editText
import chat.pcp.app.core.extensions.hasContent
import chat.pcp.app.core.extensions.setOnImeDoneListener
import chat.pcp.app.core.extensions.toReducedUrl
import chat.pcp.app.databinding.FragmentFtueEmailInputBinding
import chat.pcp.app.features.onboarding.OnboardingAction
import chat.pcp.app.features.onboarding.OnboardingViewState
import chat.pcp.app.features.onboarding.RegisterAction
import chat.pcp.lib.strings.CommonStrings
import kotlinx.parcelize.Parcelize
import org.matrix.android.sdk.api.auth.registration.RegisterThreePid
import org.matrix.android.sdk.api.extensions.isEmail

@Parcelize
data class FtueAuthEmailEntryFragmentArgument(
        val mandatory: Boolean,
) : Parcelable

@AndroidEntryPoint
class FtueAuthEmailEntryFragment : AbstractFtueAuthFragment<FragmentFtueEmailInputBinding>() {

    private val params: FtueAuthEmailEntryFragmentArgument by args()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueEmailInputBinding {
        return FragmentFtueEmailInputBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupViews()
    }

    private fun setupViews() {
        views.emailEntryInput.hint = getString(if (params.mandatory) CommonStrings.ftue_auth_email_entry_title else CommonStrings.login_set_email_optional_hint)
        views.emailEntryInput.associateContentStateWith(
                button = views.emailEntrySubmit,
                enabledPredicate = { it.isEmail() || it.isEmptyAndOptional() },
        )
        views.emailEntryInput.setOnImeDoneListener { updateEmail() }
        views.emailEntryInput.clearErrorOnChange(viewLifecycleOwner)
        views.emailEntrySubmit.debouncedClicks { updateEmail() }
        views.emailEntryInput.autofillEmail()
    }

    private fun updateEmail() {
        val email = views.emailEntryInput.content()
        when {
            email.isEmptyAndOptional() -> viewModel.handle(OnboardingAction.PostRegisterAction(RegisterAction.RegisterDummy))
            else -> viewModel.handle(OnboardingAction.PostRegisterAction(RegisterAction.AddThreePid(RegisterThreePid.Email(email))))
        }
    }

    private fun String.isEmptyAndOptional() = isEmpty() && !params.mandatory

    override fun updateWithState(state: OnboardingViewState) {
        views.emailEntryHeaderSubtitle.text = getString(CommonStrings.ftue_auth_email_subtitle, state.selectedHomeserver.userFacingUrl.toReducedUrl())

        if (!views.emailEntryInput.hasContent()) {
            views.emailEntryInput.editText().setText(state.registrationState.email)
        }
    }

    override fun onError(throwable: Throwable) {
        views.emailEntryInput.error = errorFormatter.toHumanReadable(throwable)
    }

    override fun resetViewModel() {
        viewModel.handle(OnboardingAction.ResetAuthenticationAttempt)
    }
}
