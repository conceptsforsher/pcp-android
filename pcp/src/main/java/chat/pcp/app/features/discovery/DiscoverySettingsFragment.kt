/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.discovery

import android.app.Activity
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import com.airbnb.mvrx.args
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.extensions.observeEvent
import chat.pcp.app.core.extensions.registerStartForActivityResult
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.ensureProtocol
import chat.pcp.app.core.utils.openUrlInChromeCustomTab
import chat.pcp.app.core.utils.showIdentityServerConsentDialog
import chat.pcp.app.databinding.FragmentGenericRecyclerBinding
import chat.pcp.app.features.discovery.change.SetIdentityServerFragment
import chat.pcp.app.features.navigation.SettingsActivityPayload
import chat.pcp.app.features.settings.PcpSettingsActivity
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.identity.SharedState
import org.matrix.android.sdk.api.session.identity.ThreePid
import org.matrix.android.sdk.api.session.terms.TermsService
import javax.inject.Inject

@AndroidEntryPoint
class DiscoverySettingsFragment :
        PcpFragment<FragmentGenericRecyclerBinding>(),
        DiscoverySettingsController.Listener {

    @Inject lateinit var controller: DiscoverySettingsController

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentGenericRecyclerBinding {
        return FragmentGenericRecyclerBinding.inflate(inflater, container, false)
    }

    private val viewModel by fragmentViewModel(DiscoverySettingsViewModel::class)
    private val discoveryArgs: SettingsActivityPayload.DiscoverySettings by args()

    lateinit var sharedViewModel: DiscoverySharedViewModel

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        sharedViewModel = activityViewModelProvider.get(DiscoverySharedViewModel::class.java)

        controller.listener = this
        views.genericRecyclerView.configureWith(controller)

        sharedViewModel.navigateEvent.observeEvent(this) {
            when (it) {
                is DiscoverySharedViewModelAction.ChangeIdentityServer ->
                    viewModel.handle(DiscoverySettingsAction.ChangeIdentityServer(it.newUrl))
            }
        }

        viewModel.observeViewEvents {
            when (it) {
                is DiscoverySettingsViewEvents.Failure -> {
                    displayErrorDialog(it.throwable)
                }
            }
        }
        if (discoveryArgs.expandIdentityPolicies) {
            viewModel.handle(DiscoverySettingsAction.SetPoliciesExpandState(expanded = true))
        }
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
        (activity as? AppCompatActivity)?.supportActionBar?.setTitle(CommonStrings.settings_discovery_category)

        // If some 3pids are pending, we can try to check if they have been verified here
        viewModel.handle(DiscoverySettingsAction.Refresh)
    }

    private val termsActivityResultLauncher = registerStartForActivityResult {
        if (it.resultCode == Activity.RESULT_OK) {
            viewModel.handle(DiscoverySettingsAction.RetrieveBinding)
        } else {
            // add some error?
        }
    }

    override fun openIdentityServerTerms() = withState(viewModel) { state ->
        if (state.termsNotSigned) {
            navigator.openTerms(
                    requireContext(),
                    termsActivityResultLauncher,
                    TermsService.ServiceType.IdentityService,
                    state.identityServer()?.serverUrl?.ensureProtocol() ?: "",
                    null
            )
        }
    }

    override fun onTapRevoke(threePid: ThreePid) {
        viewModel.handle(DiscoverySettingsAction.RevokeThreePid(threePid))
    }

    override fun onTapShare(threePid: ThreePid) {
        viewModel.handle(DiscoverySettingsAction.ShareThreePid(threePid))
    }

    override fun checkEmailVerification(threePid: ThreePid.Email) {
        viewModel.handle(DiscoverySettingsAction.FinalizeBind3pid(threePid))
    }

    override fun sendMsisdnVerificationCode(threePid: ThreePid.Msisdn, code: String) {
        viewModel.handle(DiscoverySettingsAction.SubmitMsisdnToken(threePid, code))
    }

    override fun cancelBinding(threePid: ThreePid) {
        viewModel.handle(DiscoverySettingsAction.CancelBinding(threePid))
    }

    override fun onTapChangeIdentityServer() = withState(viewModel) { state ->
        // we should prompt if there are bound items with current is
        val pidList = state.emailList().orEmpty() + state.phoneNumbersList().orEmpty()
        val hasBoundIds = pidList.any { it.isShared() == SharedState.SHARED }

        if (hasBoundIds) {
            // we should prompt
            MaterialAlertDialogBuilder(requireActivity())
                    .setTitle(CommonStrings.change_identity_server)
                    .setMessage(getString(CommonStrings.settings_discovery_disconnect_with_bound_pid, state.identityServer(), state.identityServer()))
                    .setPositiveButton(CommonStrings._continue) { _, _ -> navigateToChangeIdentityServerFragment() }
                    .setNegativeButton(CommonStrings.action_cancel, null)
                    .show()
            Unit
        } else {
            navigateToChangeIdentityServerFragment()
        }
    }

    override fun onTapDisconnectIdentityServer() {
        // we should prompt if there are bound items with current is
        withState(viewModel) { state ->
            val pidList = state.emailList().orEmpty() + state.phoneNumbersList().orEmpty()
            val hasBoundIds = pidList.any { it.isShared() == SharedState.SHARED }

            val serverUrl = state.identityServer()?.serverUrl.orEmpty()
            val message = if (hasBoundIds) {
                getString(CommonStrings.settings_discovery_disconnect_with_bound_pid, serverUrl, serverUrl)
            } else {
                getString(CommonStrings.disconnect_identity_server_dialog_content, serverUrl)
            }

            MaterialAlertDialogBuilder(requireActivity())
                    .setTitle(CommonStrings.disconnect_identity_server)
                    .setMessage(message)
                    .setPositiveButton(CommonStrings.action_disconnect) { _, _ -> viewModel.handle(DiscoverySettingsAction.DisconnectIdentityServer) }
                    .setNegativeButton(CommonStrings.action_cancel, null)
                    .show()
        }
    }

    override fun onTapUpdateUserConsent(newValue: Boolean) {
        if (newValue) {
            withState(viewModel) { state ->
                requireContext().showIdentityServerConsentDialog(
                        state.identityServer.invoke(),
                        consentCallBack = { viewModel.handle(DiscoverySettingsAction.UpdateUserConsent(true)) }
                )
            }
        } else {
            viewModel.handle(DiscoverySettingsAction.UpdateUserConsent(false))
        }
    }

    override fun onTapRetryToRetrieveBindings() {
        viewModel.handle(DiscoverySettingsAction.RetrieveBinding)
    }

    override fun onPolicyUrlsExpandedStateToggled(newExpandedState: Boolean) {
        viewModel.handle(DiscoverySettingsAction.SetPoliciesExpandState(expanded = newExpandedState))
    }

    override fun onPolicyTapped(policy: ServerPolicy) {
        openUrlInChromeCustomTab(requireContext(), null, policy.url)
    }

    private fun navigateToChangeIdentityServerFragment() {
        (vectorBaseActivity as? PcpSettingsActivity)?.navigateTo(SetIdentityServerFragment::class.java)
    }
}
