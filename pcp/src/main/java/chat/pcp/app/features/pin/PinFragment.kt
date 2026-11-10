/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.pin

import android.app.Activity
import android.os.Bundle
import android.os.Parcelable
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import com.airbnb.mvrx.args
import com.airbnb.mvrx.asMavericksArgs
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.extensions.replaceFragment
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.toast
import chat.pcp.app.databinding.FragmentPinBinding
import chat.pcp.app.features.MainActivity
import chat.pcp.app.features.MainActivityArgs
import chat.pcp.app.features.pin.lockscreen.biometrics.BiometricAuthError
import chat.pcp.app.features.pin.lockscreen.configuration.LockScreenConfiguration
import chat.pcp.app.features.pin.lockscreen.configuration.LockScreenMode
import chat.pcp.app.features.pin.lockscreen.ui.AuthMethod
import chat.pcp.app.features.pin.lockscreen.ui.LockScreenFragment
import chat.pcp.app.features.pin.lockscreen.ui.LockScreenListener
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.strings.CommonPlurals
import chat.pcp.lib.strings.CommonStrings
import kotlinx.parcelize.Parcelize
import timber.log.Timber
import javax.inject.Inject

@Parcelize
data class PinArgs(
        val pinMode: PinMode
) : Parcelable

@AndroidEntryPoint
class PinFragment :
        PcpFragment<FragmentPinBinding>() {

    @Inject lateinit var pinCodeStore: PinCodeStore
    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var defaultConfiguration: LockScreenConfiguration

    private val fragmentArgs: PinArgs by args()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentPinBinding {
        return FragmentPinBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        when (fragmentArgs.pinMode) {
            PinMode.CREATE -> showCreateFragment()
            PinMode.AUTH -> showAuthFragment()
            PinMode.MODIFY -> showCreateFragment() // No need to create another function for now because texts are generic
        }
    }

    private fun showCreateFragment() {
        val createFragment = LockScreenFragment()
        createFragment.lockScreenListener = object : LockScreenListener {
            override fun onNewCodeValidationFailed() {
                Toast.makeText(requireContext(), getString(CommonStrings.create_pin_confirm_failure), Toast.LENGTH_SHORT).show()
            }

            override fun onPinCodeCreated() {
                vectorBaseActivity.setResult(Activity.RESULT_OK)
                vectorBaseActivity.finish()
            }
        }
        createFragment.arguments = defaultConfiguration.copy(
                mode = LockScreenMode.CREATE,
                title = getString(CommonStrings.create_pin_title),
                needsNewCodeValidation = true,
                newCodeConfirmationTitle = getString(CommonStrings.create_pin_confirm_title),
        ).asMavericksArgs()
        replaceFragment(R.id.pinFragmentContainer, createFragment)
    }

    private fun showAuthFragment() {
        val authFragment = LockScreenFragment()
        authFragment.onLeftButtonClickedListener = View.OnClickListener { displayForgotPinWarningDialog() }
        authFragment.lockScreenListener = object : LockScreenListener {
            override fun onAuthenticationFailure(authMethod: AuthMethod) {
                when (authMethod) {
                    AuthMethod.PIN_CODE -> onWrongPin()
                    AuthMethod.BIOMETRICS -> Unit
                }
            }

            override fun onAuthenticationSuccess(authMethod: AuthMethod) {
                pinCodeStore.resetCounter()
                vectorBaseActivity.setResult(Activity.RESULT_OK)
                vectorBaseActivity.finish()
            }

            override fun onAuthenticationError(authMethod: AuthMethod, throwable: Throwable) {
                if (throwable is BiometricAuthError) {
                    // System disabled biometric auth, no need to do it ourselves
                    if (throwable.isAuthPermanentlyDisabledError) {
                        pcpPreferences.setUseBiometricToUnlock(false)
                    }
                    Toast.makeText(requireContext(), throwable.localizedMessage, Toast.LENGTH_SHORT).show()
                } else {
                    Timber.e(throwable)
                }
            }

            override fun onBiometricKeyInvalidated() {
                // Disable biometric auth in settings and remove system key
                pcpPreferences.setUseBiometricToUnlock(false)

                MaterialAlertDialogBuilder(requireContext())
                        .setMessage(CommonStrings.auth_biometric_key_invalidated_message)
                        .setPositiveButton(CommonStrings.ok, null)
                        .show()
            }
        }
        authFragment.arguments = defaultConfiguration.copy(
                mode = LockScreenMode.VERIFY,
                title = getString(CommonStrings.auth_pin_title),
                leftButtonTitle = getString(CommonStrings.auth_pin_forgot),
                clearCodeOnError = true,
        ).asMavericksArgs()
        replaceFragment(R.id.pinFragmentContainer, authFragment)
    }

    private fun onWrongPin() {
        val remainingAttempts = pinCodeStore.onWrongPin()
        when {
            remainingAttempts > 1 ->
                requireActivity().toast(resources.getQuantityString(CommonPlurals.wrong_pin_message_remaining_attempts, remainingAttempts, remainingAttempts))
            remainingAttempts == 1 ->
                requireActivity().toast(CommonStrings.wrong_pin_message_last_remaining_attempt)
            else -> {
                requireActivity().toast(CommonStrings.too_many_pin_failures)
                // Logout
                launchResetPinFlow()
            }
        }
    }

    private fun displayForgotPinWarningDialog() {
        MaterialAlertDialogBuilder(requireContext())
                .setTitle(getString(CommonStrings.auth_pin_reset_title))
                .setMessage(getString(CommonStrings.auth_pin_reset_content))
                .setPositiveButton(getString(CommonStrings.auth_pin_new_pin_action)) { _, _ ->
                    launchResetPinFlow()
                }
                .setNegativeButton(CommonStrings.action_cancel, null)
                .show()
    }

    private fun launchResetPinFlow() {
        MainActivity.restartApp(
                activity = requireActivity(),
                args = MainActivityArgs(
                        clearCredentials = true,
                        ignoreLogoutServerError = true,
                )
        )
    }
}
