/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.debug.settings

import android.annotation.SuppressLint
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import chat.pcp.app.core.platform.PcpBaseFragment
import chat.pcp.app.features.home.room.list.home.release.ReleaseNotesActivity
import chat.pcp.application.databinding.FragmentDebugPrivateSettingsBinding

class DebugPrivateSettingsFragment : PcpBaseFragment<FragmentDebugPrivateSettingsBinding>() {

    private val viewModel: DebugPrivateSettingsViewModel by fragmentViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentDebugPrivateSettingsBinding {
        return FragmentDebugPrivateSettingsBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setViewListeners()
    }

    private fun setViewListeners() {
        views.forceDialPadTabDisplay.setOnCheckedChangeListener { _, isChecked ->
            viewModel.handle(DebugPrivateSettingsViewActions.SetDialPadVisibility(isChecked))
        }
        views.forceLoginFallback.setOnCheckedChangeListener { _, isChecked ->
            viewModel.handle(DebugPrivateSettingsViewActions.SetForceLoginFallbackEnabled(isChecked))
        }
        views.releaseNotesActivityHasBeenDisplayedReset.setOnClickListener {
            viewModel.handle(DebugPrivateSettingsViewActions.ResetReleaseNotesActivityHasBeenDisplayed)
        }
        views.showReleaseNotesActivity.setOnClickListener {
            startActivity(Intent(requireActivity(), ReleaseNotesActivity::class.java))
        }
    }

    override fun invalidate() = withState(viewModel) {
        views.forceDialPadTabDisplay.isChecked = it.dialPadVisible
        views.forceChangeDisplayNameCapability.bind(it.homeserverCapabilityOverrides.displayName) { option ->
            viewModel.handle(DebugPrivateSettingsViewActions.SetDisplayNameCapabilityOverride(option))
        }
        views.forceChangeAvatarCapability.bind(it.homeserverCapabilityOverrides.avatar) { option ->
            viewModel.handle(DebugPrivateSettingsViewActions.SetAvatarCapabilityOverride(option))
        }
        views.forceLoginFallback.isChecked = it.forceLoginFallback
        @SuppressLint("SetTextI18n")
        views.releaseNotesActivityHasBeenDisplayed.text = "ReleaseNotesActivity has been displayed: ${it.releaseNotesActivityHasBeenDisplayed}"
    }
}
