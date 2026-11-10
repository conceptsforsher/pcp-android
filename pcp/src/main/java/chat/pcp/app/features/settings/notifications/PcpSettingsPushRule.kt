/*
 * Copyright 2023, 2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.notifications

import android.os.Bundle
import android.view.View
import androidx.preference.Preference
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import chat.pcp.app.core.preference.PcpCheckboxPreference
import chat.pcp.app.features.settings.PcpSettingsBaseFragment
import chat.pcp.app.features.themes.ThemeUtils
import chat.pcp.lib.strings.CommonStrings

abstract class PcpSettingsPushRule :
        PcpSettingsBaseFragment() {

    protected val viewModel: PcpSettingsPushRuleVM by fragmentViewModel()

    abstract val prefKeyToPushRuleId: Map<String, String>

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        observeViewEvents()
        viewModel.onEach(PcpSettingsPushRuleState::allRules) { refreshPreferences() }
        viewModel.onEach(PcpSettingsPushRuleState::isLoading) { updateLoadingView(it) }
        viewModel.onEach(PcpSettingsPushRuleState::rulesOnError) { refreshErrors(it) }
    }

    private fun observeViewEvents() {
        viewModel.observeViewEvents {
            when (it) {
                is PcpSettingsPushRuleEvent.Failure -> onFailure(it.ruleId)
                is PcpSettingsPushRuleEvent.PushRuleUpdated -> {
                    updatePreference(it.ruleId, it.checked)
                    if (it.failure != null) {
                        onFailure(it.ruleId)
                    }
                }
            }
        }
    }

    override fun bindPref() {
        prefKeyToPushRuleId.forEach { (preferenceKey, ruleId) ->
            findPreference<PcpCheckboxPreference>(preferenceKey)?.apply {
                isIconSpaceReserved = false
                onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                    viewModel.handle(PcpSettingsPushRuleAction.UpdatePushRule(ruleId, newValue as Boolean))
                    false
                }
            }
        }
    }

    private fun updateLoadingView(isLoading: Boolean) {
        if (isLoading) {
            displayLoadingView()
        } else {
            hideLoadingView()
        }
    }

    private fun refreshPreferences() {
        prefKeyToPushRuleId.values.forEach { ruleId -> updatePreference(ruleId, viewModel.isPushRuleChecked(ruleId)) }
    }

    private fun refreshErrors(rulesWithError: Set<String>) {
        if (withState(viewModel, PcpSettingsPushRuleState::isLoading)) return
        prefKeyToPushRuleId.forEach { (preferenceKey, ruleId) ->
            findPreference<PcpCheckboxPreference>(preferenceKey)?.apply {
                if (ruleId in rulesWithError) {
                    summaryTextColor = ThemeUtils.getColor(context, com.google.android.material.R.attr.colorError)
                    setSummary(CommonStrings.settings_notification_error_on_update)
                } else {
                    summaryTextColor = null
                    summary = null
                }
            }
        }
    }

    protected fun refreshDisplay() {
        listView?.adapter?.notifyDataSetChanged()
    }

    private fun updatePreference(ruleId: String, checked: Boolean) {
        val preferenceKey = prefKeyToPushRuleId.entries.find { it.value == ruleId }?.key ?: return
        val preference = findPreference<PcpCheckboxPreference>(preferenceKey) ?: return
        val ruleIds = withState(viewModel) { state -> state.allRules.map { it.ruleId } }
        preference.isVisible = ruleId in ruleIds
        preference.isChecked = checked
    }

    protected open fun onFailure(ruleId: String) {
        refreshDisplay()
    }
}
