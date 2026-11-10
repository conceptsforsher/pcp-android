/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.utils.openUrlInChromeCustomTab
import chat.pcp.app.databinding.FragmentLoginServerSelectionBinding
import chat.pcp.app.features.login.EMS_LINK
import chat.pcp.app.features.login.ServerType
import chat.pcp.app.features.login.SignMode
import chat.pcp.app.features.onboarding.OnboardingAction
import chat.pcp.app.features.onboarding.OnboardingViewState
import chat.pcp.lib.strings.CommonStrings
import me.gujun.android.span.span

/**
 * In this screen, the user will choose between matrix.org, modular or other type of homeserver.
 */
@AndroidEntryPoint
class FtueAuthServerSelectionFragment :
        AbstractFtueAuthFragment<FragmentLoginServerSelectionBinding>() {

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentLoginServerSelectionBinding {
        return FragmentLoginServerSelectionBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        initViews()
        initTextViews()
    }

    private fun initViews() {
        views.loginServerChoiceEmsLearnMore.setOnClickListener { learnMore() }
        views.loginServerChoiceMatrixOrg.setOnClickListener { selectMatrixOrg() }
        views.loginServerChoiceEms.setOnClickListener { selectEMS() }
        views.loginServerChoiceOther.setOnClickListener { selectOther() }
        views.loginServerIKnowMyIdSubmit.setOnClickListener { loginWithMatrixId() }
    }

    private fun updateSelectedChoice(state: OnboardingViewState) {
        views.loginServerChoiceMatrixOrg.isChecked = state.serverType == ServerType.MatrixOrg
    }

    private fun initTextViews() {
        views.loginServerChoiceEmsLearnMore.text = span {
            text = getString(CommonStrings.login_server_modular_learn_more)
            textDecorationLine = "underline"
        }
    }

    private fun learnMore() {
        openUrlInChromeCustomTab(requireActivity(), null, EMS_LINK)
    }

    private fun selectMatrixOrg() {
        viewModel.handle(OnboardingAction.UpdateServerType(ServerType.MatrixOrg))
    }

    private fun selectEMS() {
        viewModel.handle(OnboardingAction.UpdateServerType(ServerType.EMS))
    }

    private fun selectOther() {
        viewModel.handle(OnboardingAction.UpdateServerType(ServerType.Other))
    }

    private fun loginWithMatrixId() {
        viewModel.handle(OnboardingAction.UpdateSignMode(SignMode.SignInWithMatrixId))
    }

    override fun resetViewModel() {
        viewModel.handle(OnboardingAction.ResetHomeServerType)
    }

    override fun updateWithState(state: OnboardingViewState) {
        updateSelectedChoice(state)
    }
}
