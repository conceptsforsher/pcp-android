/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings

import android.os.Bundle
import androidx.preference.Preference
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.extensions.orEmpty
import chat.pcp.app.core.preference.PcpBasePreference
import chat.pcp.app.core.resources.BuildMeta
import chat.pcp.app.core.utils.FirstThrottler
import chat.pcp.app.core.utils.copyToClipboard
import chat.pcp.app.core.utils.openAppSettingsPage
import chat.pcp.app.core.utils.openUrlInChromeCustomTab
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.version.VersionProvider
import chat.pcp.lib.strings.CommonStrings
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.pcp.app.features.MainActivity
import chat.pcp.app.features.MainActivityArgs
import org.matrix.android.sdk.api.Matrix
import javax.inject.Inject

@AndroidEntryPoint
class PcpSettingsHelp :
        PcpSettingsBaseFragment() {

    @Inject lateinit var versionProvider: VersionProvider
    @Inject lateinit var buildMeta: BuildMeta

    override var titleRes = CommonStrings.preference_root_help_about
    override val preferenceXmlRes = R.xml.pcp_settings_help_about

    private val firstThrottler = FirstThrottler(1000)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsHelp
    }

    override fun bindPref() {
        // Help
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_HELP_PREFERENCE_KEY)!!
                .onPreferenceClickListener = Preference.OnPreferenceClickListener {
            if (firstThrottler.canHandle() is FirstThrottler.CanHandlerResult.Yes) {
                openUrlInChromeCustomTab(requireContext(), null, PcpSettingsUrls.HELP)
            }
            false
        }

        // preference to start the App info screen, to facilitate App permissions access
        findPreference<PcpBasePreference>(APP_INFO_LINK_PREFERENCE_KEY)!!
                .onPreferenceClickListener = Preference.OnPreferenceClickListener {
            activity?.let { openAppSettingsPage(it) }
            true
        }

        // application version
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_VERSION_PREFERENCE_KEY)!!.let {
            it.summary = buildString {
                append("PCP (development)")
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
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_NATIVE_CORE_VERSION_KEY)!!.let {
            it.summary = "libpcp_native.so — 95 C++ modules"
            it.setOnPreferenceClickListener { pref ->
                copyToClipboard(requireContext(), pref.summary.orEmpty())
                true
            }
        }

        // SDK version
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_SDK_VERSION_PREFERENCE_KEY)!!.let {
            it.summary = Matrix.getSdkVersion()

            it.setOnPreferenceClickListener { pref ->
                copyToClipboard(requireContext(), pref.summary.orEmpty())
                true
            }
        }

        // crypto version (libolm)
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_CRYPTO_VERSION_PREFERENCE_KEY)!!.let {
            val cryptoVersion = Matrix.getCryptoVersion(true)
            it.summary = "libolm ${cryptoVersion}"
            it.setOnPreferenceClickListener { pref ->
                copyToClipboard(requireContext(), it.summary.orEmpty())
                true
            }
        }

        // Legacy init sync
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_DO_LEGACY_INIT_SYNC)!!
                .onPreferenceClickListener = Preference.OnPreferenceClickListener {
            context?.let { ctx ->
                MaterialAlertDialogBuilder(ctx)
                    .setTitle("Legacy Init Sync")
                    .setMessage("This will clear the cache and perform a full re-sync from scratch. This may take a long time. Continue?")
                    .setPositiveButton(CommonStrings.ok) { _, _ ->
                        MainActivity.restartApp(requireActivity(), MainActivityArgs(clearCache = true))
                    }
                    .setNegativeButton(CommonStrings.later, null)
                    .show()
            }
            true
        }

        // Optimized init sync
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_DO_OPTIMIZED_INIT_SYNC)!!
                .onPreferenceClickListener = Preference.OnPreferenceClickListener {
            context?.let { ctx ->
                MaterialAlertDialogBuilder(ctx)
                    .setTitle("Optimized Init Sync")
                    .setMessage("This will perform a smart incremental sync, only downloading new data. Continue?")
                    .setPositiveButton(CommonStrings.ok) { _, _ ->
                        MainActivity.restartApp(requireActivity(), MainActivityArgs(clearCache = false))
                    }
                    .setNegativeButton(CommonStrings.later, null)
                    .show()
            }
            true
        }
    }

    companion object {
        private const val APP_INFO_LINK_PREFERENCE_KEY = "APP_INFO_LINK_PREFERENCE_KEY"
    }
}
