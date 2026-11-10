/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings

import android.os.Bundle
import androidx.preference.Preference
import androidx.preference.SeekBarPreference
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.core.preference.PcpBasePreference
import chat.pcp.app.core.preference.PcpPreferenceCategory
import chat.pcp.app.core.preference.PcpSwitchPreference
import chat.pcp.app.core.utils.copyToClipboard
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.home.NightlyProxy
import chat.pcp.app.features.rageshake.RageShake
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

@AndroidEntryPoint
class PcpSettingsAdvancedSettingsFragment :
        PcpSettingsBaseFragment() {

    override var titleRes = CommonStrings.settings_advanced_settings
    override val preferenceXmlRes = R.xml.pcp_settings_advanced_settings

    @Inject lateinit var nightlyProxy: NightlyProxy

    private var rageshake: RageShake? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsAdvanced
    }

    override fun onResume() {
        super.onResume()

        rageshake = (activity as? PcpActivity<*>)?.rageShake
        rageshake?.interceptor = {
            (activity as? PcpActivity<*>)?.showSnackbar(getString(CommonStrings.rageshake_detected))
        }
    }

    override fun onPause() {
        super.onPause()
        rageshake?.interceptor = null
        rageshake = null
    }

    override fun bindPref() {
        setupRageShakeSection()
        setupNightlySection()
        setupDevToolsSection()
    }

    private fun setupDevToolsSection() {
        findPreference<PcpBasePreference>("SETTINGS_ACCESS_TOKEN")?.setOnPreferenceClickListener {
            copyToClipboard(requireActivity(), session.sessionParams.credentials.accessToken)
            true
        }

        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_DEVELOPER_MODE_KEY_REQUEST_AUDIT_KEY)?.apply {
            isVisible = session.cryptoService().supportKeyRequestInspection()
        }
    }

    private fun setupRageShakeSection() {
        val isRageShakeAvailable = RageShake.isAvailable(requireContext())

        if (isRageShakeAvailable) {
            findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_USE_RAGE_SHAKE_KEY)!!
                    .onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->

                if (newValue as? Boolean == true) {
                    rageshake?.start()
                } else {
                    rageshake?.stop()
                }

                true
            }

            findPreference<SeekBarPreference>(PcpBasePreferences.SETTINGS_RAGE_SHAKE_DETECTION_THRESHOLD_KEY)!!
                    .onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                (activity as? PcpActivity<*>)?.let {
                    val newValueAsInt = newValue as? Int ?: return@OnPreferenceChangeListener true

                    rageshake?.setSensitivity(newValueAsInt)
                }

                true
            }
        } else {
            findPreference<PcpPreferenceCategory>("SETTINGS_RAGE_SHAKE_CATEGORY_KEY")!!.isVisible = false
        }
    }

    private fun setupNightlySection() {
        findPreference<PcpPreferenceCategory>("SETTINGS_NIGHTLY_BUILD_PREFERENCE_KEY")?.isVisible = nightlyProxy.isNightlyBuild()
        findPreference<PcpBasePreference>("SETTINGS_NIGHTLY_BUILD_UPDATE_PREFERENCE_KEY")?.setOnPreferenceClickListener {
            nightlyProxy.updateApplication()
            true
        }
    }
}
