/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.verification.self

import android.app.Activity
import android.app.Dialog
import android.os.Bundle
import android.os.Parcelable
import android.view.KeyEvent
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.pcp.app.R
import chat.pcp.app.core.extensions.commitTransaction
import chat.pcp.app.core.extensions.registerStartForActivityResult
import chat.pcp.app.core.extensions.singletonEntryPoint
import chat.pcp.app.core.extensions.toMvRxBundle
import chat.pcp.app.core.platform.PcpBottomSheet
import chat.pcp.app.databinding.BottomSheetVerificationBinding
import chat.pcp.app.features.crypto.quads.SharedSecureStorageActivity
import chat.pcp.app.features.crypto.quads.SharedSecureStorageViewState
import chat.pcp.app.features.crypto.verification.VerificationAction
import chat.pcp.app.features.crypto.verification.VerificationBottomSheetViewEvents
import chat.pcp.app.features.settings.PcpSettingsActivity
import chat.pcp.lib.strings.CommonStrings
import kotlinx.parcelize.Parcelize
import org.matrix.android.sdk.api.session.crypto.crosssigning.KEYBACKUP_SECRET_SSSS_NAME
import org.matrix.android.sdk.api.session.crypto.crosssigning.MASTER_KEY_SSSS_NAME
import org.matrix.android.sdk.api.session.crypto.crosssigning.SELF_SIGNING_KEY_SSSS_NAME
import org.matrix.android.sdk.api.session.crypto.crosssigning.USER_SIGNING_KEY_SSSS_NAME
import kotlin.reflect.KClass

class SelfVerificationBottomSheet : PcpBottomSheet<BottomSheetVerificationBinding>() {

    override val showExpanded = true

    @Parcelize
    data class Args(
            // when verifying a new session from an existing safe one
            val targetDevice: String? = null,
            // when we started from an incoming request
            val transactionId: String? = null,
    ) : Parcelable

    private val viewModel by fragmentViewModel(SelfVerificationViewModel::class)

    init {
        // we manage dismiss/back manually as verification could be required
        isCancelable = false
    }

    override fun getBinding(
            inflater: LayoutInflater,
            container: ViewGroup?
    ) = BottomSheetVerificationBinding.inflate(inflater, container, false)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        showFragment(SelfVerificationFragment::class)
    }

    private val secretStartForActivityResult = registerStartForActivityResult { activityResult ->
        if (activityResult.resultCode == Activity.RESULT_OK) {
            val result = activityResult.data?.getStringExtra(SharedSecureStorageActivity.EXTRA_DATA_RESULT)
            val reset = activityResult.data?.getBooleanExtra(SharedSecureStorageActivity.EXTRA_DATA_RESET, false) ?: false
            if (result != null) {
                viewModel.handle(VerificationAction.GotResultFromSsss(result, SharedSecureStorageActivity.DEFAULT_RESULT_KEYSTORE_ALIAS))
            } else if (reset) {
                // all have been reset, so we are verified?
                viewModel.handle(VerificationAction.SecuredStorageHasBeenReset)
            }
        } else {
            viewModel.handle(VerificationAction.CancelledFromSsss)
        }
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        observeViewModelEvents()
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        return super.onCreateDialog(savedInstanceState).apply {
            setOnKeyListener { _, keyCode, keyEvent ->
                if (keyCode == KeyEvent.KEYCODE_BACK && keyEvent.action == KeyEvent.ACTION_UP) {
                    viewModel.queryCancel()
                    true
                } else {
                    false
                }
            }
        }
    }

    private fun observeViewModelEvents() {
        viewModel.observeViewEvents { event ->
            when (event) {
                VerificationBottomSheetViewEvents.AccessSecretStore -> {
                    secretStartForActivityResult.launch(
                            SharedSecureStorageActivity.newReadIntent(
                                    requireContext(),
                                    null, // use default key
                                    listOf(MASTER_KEY_SSSS_NAME, USER_SIGNING_KEY_SSSS_NAME, SELF_SIGNING_KEY_SSSS_NAME, KEYBACKUP_SECRET_SSSS_NAME),
                                    SharedSecureStorageActivity.DEFAULT_RESULT_KEYSTORE_ALIAS,
                            )
                    )
                }
                VerificationBottomSheetViewEvents.ResetAll -> {
                    secretStartForActivityResult.launch(
                            SharedSecureStorageActivity.newReadIntent(
                                    requireContext(),
                                    null, // use default key
                                    listOf(MASTER_KEY_SSSS_NAME, USER_SIGNING_KEY_SSSS_NAME, SELF_SIGNING_KEY_SSSS_NAME, KEYBACKUP_SECRET_SSSS_NAME),
                                    SharedSecureStorageActivity.DEFAULT_RESULT_KEYSTORE_ALIAS,
                                    SharedSecureStorageViewState.Step.ResetAll
                            )
                    )
                }
                VerificationBottomSheetViewEvents.Dismiss -> {
                    dismiss()
                }
                VerificationBottomSheetViewEvents.GoToSettings -> {
                    // nop for user verificaiton
                }
                is VerificationBottomSheetViewEvents.ModalError -> {
                    MaterialAlertDialogBuilder(requireContext())
                            .setTitle(getString(CommonStrings.dialog_title_error))
                            .setMessage(event.errorMessage)
                            .setCancelable(false)
                            .setPositiveButton(CommonStrings.ok, null)
                            .show()
                }
                VerificationBottomSheetViewEvents.DismissAndOpenDeviceSettings -> {
                    dismiss()
                    requireActivity().singletonEntryPoint().navigator().openSettings(
                            requireActivity(),
                            PcpSettingsActivity.EXTRA_DIRECT_ACCESS_SECURITY_PRIVACY_MANAGE_SESSIONS
                    )
                }
                is VerificationBottomSheetViewEvents.RequestNotFound -> {
                    dismiss()
                }
                is VerificationBottomSheetViewEvents.ConfirmCancel -> {
                    // TODO? applies to self?
                }
            }
        }
    }

    //        viewModel.observeViewEvents {
//            when (it) {
//                is VerificationBottomSheetViewEvents.Dismiss -> dismiss()
//                is VerificationBottomSheetViewEvents.AccessSecretStore -> {
//                    secretStartForActivityResult.launch(
//                            SharedSecureStorageActivity.newReadIntent(
//                                    requireContext(),
//                                    null, // use default key
//                                    listOf(MASTER_KEY_SSSS_NAME, USER_SIGNING_KEY_SSSS_NAME, SELF_SIGNING_KEY_SSSS_NAME, KEYBACKUP_SECRET_SSSS_NAME),
//                                    SharedSecureStorageActivity.DEFAULT_RESULT_KEYSTORE_ALIAS
//                            )
//                    )
//                }
//                is VerificationBottomSheetViewEvents.ModalError -> {
//                    MaterialAlertDialogBuilder(requireContext())
//                            .setTitle(getString(CommonStrings.dialog_title_error))
//                            .setMessage(it.errorMessage)
//                            .setCancelable(false)
//                            .setPositiveButton(CommonStrings.ok, null)
//                            .show()
//                    Unit
//                }
//                VerificationBottomSheetViewEvents.GoToSettings -> {
//                    dismiss()
//                    (activity as? PcpActivity<*>)?.let { activity ->
//                        activity.navigator.openSettings(activity, PcpSettingsActivity.EXTRA_DIRECT_ACCESS_SECURITY_PRIVACY)
//                    }
//                }
//            }
//        }
//    }

    override fun invalidate() = withState(viewModel) { state ->
//        avatarRenderer.render(state.otherUserMxItem, views.otherUserAvatarImageView)
        views.otherUserShield.isVisible = false
        if (state.isThisSessionVerified) {
            views.otherUserAvatarImageView.setImageResource(
                    R.drawable.ic_shield_trusted
            )
            views.otherUserNameText.text = getString(CommonStrings.verification_profile_verify)
        } else {
            views.otherUserAvatarImageView.setImageResource(
                    R.drawable.ic_shield_black
            )
            views.otherUserNameText.text = getString(CommonStrings.crosssigning_verify_this_session)
        }

        super.invalidate()
    }

    private fun showFragment(fragmentClass: KClass<out Fragment>, argsParcelable: Parcelable? = null) {
        if (childFragmentManager.findFragmentByTag(fragmentClass.simpleName) == null) {
            childFragmentManager.commitTransaction {
                replace(
                        R.id.bottomSheetFragmentContainer,
                        fragmentClass.java,
                        argsParcelable?.toMvRxBundle(),
                        fragmentClass.simpleName
                )
            }
        }
    }

    companion object {

        const val TAG: String = "VERIF"

        fun verifyOwnUntrustedDevice(): SelfVerificationBottomSheet {
            return SelfVerificationBottomSheet().apply {
                setArguments(
                        Args()
                )
            }
        }

        fun forTransaction(transactionId: String): SelfVerificationBottomSheet {
            return SelfVerificationBottomSheet().apply {
                setArguments(
                        Args(transactionId = transactionId)
                )
            }
        }
    }
}
