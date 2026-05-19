/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.settings

import android.os.Bundle
import androidx.preference.Preference
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
import im.vector.app.core.extensions.orEmpty
import im.vector.app.core.preference.VectorPreference
import im.vector.app.core.resources.BuildMeta
import im.vector.app.core.utils.FirstThrottler
import im.vector.app.core.utils.copyToClipboard
import im.vector.app.core.utils.openAppSettingsPage
import im.vector.app.core.utils.openUrlInChromeCustomTab
import im.vector.app.features.analytics.plan.MobileScreen
import im.vector.app.features.version.VersionProvider
import im.vector.lib.strings.CommonStrings
import org.matrix.android.sdk.api.Matrix
import javax.inject.Inject

@AndroidEntryPoint
class VectorSettingsHelpAboutFragment :
        VectorSettingsBaseFragment() {

    @Inject lateinit var versionProvider: VersionProvider
    @Inject lateinit var buildMeta: BuildMeta

    override var titleRes = CommonStrings.preference_root_help_about
    override val preferenceXmlRes = R.xml.vector_settings_help_about

    private val firstThrottler = FirstThrottler(1000)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsHelp
    }

    override fun bindPref() {
        // Help
        findPreference<VectorPreference>(VectorPreferences.SETTINGS_HELP_PREFERENCE_KEY)!!
                .onPreferenceClickListener = Preference.OnPreferenceClickListener {
            if (firstThrottler.canHandle() is FirstThrottler.CanHandlerResult.Yes) {
                openUrlInChromeCustomTab(requireContext(), null, VectorSettingsUrls.HELP)
            }
            false
        }

        // preference to start the App info screen, to facilitate App permissions access
        findPreference<VectorPreference>(APP_INFO_LINK_PREFERENCE_KEY)!!
                .onPreferenceClickListener = Preference.OnPreferenceClickListener {
            activity?.let { openAppSettingsPage(it) }
            true
        }

        // application version
        findPreference<VectorPreference>(VectorPreferences.SETTINGS_VERSION_PREFERENCE_KEY)!!.let {
            it.summary = buildString {
                append("Progressive Chat (development)")
                if (buildMeta.isDebug) {
                    append("\n")
                    append(buildMeta.gitBranchName)
                    append(" @ ")
                    append(buildMeta.gitRevision)
                }
            }

            it.setOnPreferenceClickListener { pref ->
                copyToClipboard(requireContext(), pref.summary.orEmpty())
                true
            }
        }

        // Native core version
        findPreference<VectorPreference>(VectorPreferences.SETTINGS_NATIVE_CORE_VERSION_KEY)!!.let {
            it.summary = "libprogressive_native.so — 95 C++ modules"
            it.setOnPreferenceClickListener { pref ->
                copyToClipboard(requireContext(), pref.summary.orEmpty())
                true
            }
        }

        // SDK version
        findPreference<VectorPreference>(VectorPreferences.SETTINGS_SDK_VERSION_PREFERENCE_KEY)!!.let {
            it.summary = Matrix.getSdkVersion()

            it.setOnPreferenceClickListener { pref ->
                copyToClipboard(requireContext(), pref.summary.orEmpty())
                true
            }
        }

        // crypto version (libolm)
        findPreference<VectorPreference>(VectorPreferences.SETTINGS_CRYPTO_VERSION_PREFERENCE_KEY)!!.let {
            val cryptoVersion = Matrix.getCryptoVersion(true)
            it.summary = "libolm ${cryptoVersion}"
            it.setOnPreferenceClickListener { pref ->
                copyToClipboard(requireContext(), it.summary.orEmpty())
                true
            }
        }

        // Legacy init sync
        findPreference<VectorPreference>(VectorPreferences.SETTINGS_DO_LEGACY_INIT_SYNC)!!
                .onPreferenceClickListener = Preference.OnPreferenceClickListener {
            // vectorPreferences.didAskLegacyInitSync() — FIXME: restore when accessible
            true
        }

        // Optimized init sync
        findPreference<VectorPreference>(VectorPreferences.SETTINGS_DO_OPTIMIZED_INIT_SYNC)!!
                .onPreferenceClickListener = Preference.OnPreferenceClickListener {
            // vectorPreferences.didAskOptimizedInitSync() — FIXME: restore when accessible
            true
        }
    }

    companion object {
        private const val APP_INFO_LINK_PREFERENCE_KEY = "APP_INFO_LINK_PREFERENCE_KEY"
    }
}
