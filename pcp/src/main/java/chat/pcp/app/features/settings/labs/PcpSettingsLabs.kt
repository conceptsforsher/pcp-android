/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.labs

import android.os.Build
import android.os.Bundle
import android.text.method.LinkMovementMethod
import android.widget.TextView
import androidx.preference.Preference
import androidx.preference.Preference.OnPreferenceChangeListener
import androidx.preference.SwitchPreference
import com.airbnb.mvrx.fragmentViewModel
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.preference.PcpSwitchPreference
import chat.pcp.app.features.MainActivity
import chat.pcp.app.features.MainActivityArgs
import chat.pcp.app.features.PcpFeatures
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.home.room.threads.ThreadsManager
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.settings.PcpSettingsBaseFragment
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.settings.LightweightSettingsStorage
import javax.inject.Inject

@AndroidEntryPoint
class PcpSettingsLabs :
        PcpSettingsBaseFragment() {

    private val viewModel: PcpSettingsLabsVM by fragmentViewModel()

    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var lightweightSettingsStorage: LightweightSettingsStorage
    @Inject lateinit var threadsManager: ThreadsManager
    @Inject lateinit var vectorFeatures: PcpFeatures

    override var titleRes = CommonStrings.room_settings_labs_pref_title
    override val preferenceXmlRes = R.xml.pcp_settings_labs

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsLabs
    }

    override fun bindPref() {
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_AUTO_REPORT_UISI)?.let { pref ->
            // ensure correct default
            pref.isChecked = pcpPreferences.labsAutoReportUISI()
        }

        // clear cache
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_ENABLE_THREAD_MESSAGES)?.let { vectorPref ->
            vectorPref.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                onThreadsPreferenceClickedInterceptor(vectorPref)
                false
            }
        }

        findPreference<SwitchPreference>(PcpBasePreferences.SETTINGS_LABS_MSC3061_SHARE_KEYS_HISTORY)?.let { pref ->
            if (session.cryptoService().supportsShareKeysOnInvite()) {
                // ensure correct default
                pref.isChecked = session.cryptoService().isShareKeysOnInviteEnabled()

                pref.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                    session.cryptoService().enableShareKeyOnInvite(pref.isChecked)
                    MainActivity.restartApp(requireActivity(), MainActivityArgs(clearCache = true))
                    true
                }
            } else {
                pref.isEnabled = false
                pref.isChecked = false
            }
        }

        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_NEW_APP_LAYOUT_KEY)?.let { pref ->
            pref.isVisible = vectorFeatures.isNewAppLayoutFeatureEnabled()

            pref.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                onNewLayoutPreferenceClicked()
                true
            }
        }

        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_VOICE_BROADCAST_KEY)?.let { pref ->
            // Voice Broadcast recording is not available on Android < 10
            pref.isVisible = Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q && vectorFeatures.isVoiceBroadcastEnabled()
        }

        configureUnreadNotificationsAsTabPreference()
        configureEnableClientInfoRecordingPreference()
        configurePcpLabsPreferences()
    }

    /**
     * Wire up pcp-specific labs preferences to native C++ handlers.
     */
    private fun configurePcpLabsPreferences() {
        // Transparent Overlay
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_TRANSPARENT_OVERLAY_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setTransparentOverlayEnabled(enabled)
                true
            }

        // Text Undo
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_TEXT_UNDO_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setTextUndoEnabled(enabled)
                true
            }

        // LLM Slash Commands
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_LLM_SLASH_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setLlmSlashEnabled(enabled)
                true
            }

        // Web Search
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_WEB_SEARCH_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setWebSearchEnabled(enabled)
                true
            }

        // Native DB backend
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_NATIVE_DB_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setNativeDbEnabled(enabled)
                true
            }

        // Yggdrasil network
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_YGGDRASIL_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setYggdrasilEnabled(enabled)
                true
            }

        // Tor routing
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_TOR_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setTorEnabled(enabled)
                true
            }

        // I2P integration
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_I2P_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setI2pEnabled(enabled)
                true
            }

        // Chat Pushdown
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_CHAT_PUSHDOWN_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setChatPushdownEnabled(enabled)
                true
            }

        // Desync Detector
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_DESYNC_DETECTOR_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setDesyncDetectorEnabled(enabled)
                true
            }

        // Profile Swiper
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_PROFILE_SWIPER_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setProfileSwiperEnabled(enabled)
                true
            }

        // SVG Render
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_SVG_DRAW_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                pcpPreferences.setSvgDrawEnabled(enabled)
                true
            }
    }

    private fun configureUnreadNotificationsAsTabPreference() {
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_UNREAD_NOTIFICATIONS_AS_TAB)?.let { pref ->
            pref.isVisible = !vectorFeatures.isNewAppLayoutFeatureEnabled()
            pref.isEnabled = !pcpPreferences.isNewAppLayoutEnabled()
        }
    }

    /**
     * Intercept the click to display a user friendly dialog when their homeserver do not support threads.
     */
    private fun onThreadsPreferenceClickedInterceptor(vectorSwitchPreference: PcpSwitchPreference) {
        val userEnabledThreads = pcpPreferences.areThreadMessagesEnabled()
        if (!session.homeServerCapabilitiesService().getHomeServerCapabilities().canUseThreading && userEnabledThreads) {
            activity?.let {
                MaterialAlertDialogBuilder(it)
                        .setTitle(CommonStrings.threads_labs_enable_notice_title)
                        .setMessage(threadsManager.getLabsEnableThreadsMessage())
                        .setCancelable(true)
                        .setNegativeButton(CommonStrings.action_not_now) { _, _ ->
                            vectorSwitchPreference.isChecked = false
                        }
                        .setPositiveButton(CommonStrings.action_try_it_out) { _, _ ->
                            onThreadsPreferenceClicked()
                        }
                        .show()
                        ?.findViewById<TextView>(android.R.id.message)
                        ?.apply {
                            linksClickable = true
                            movementMethod = LinkMovementMethod.getInstance()
                        }
            }
        } else {
            onThreadsPreferenceClicked()
        }
    }

    /**
     * Action when threads preference switch is actually clicked.
     */
    private fun onThreadsPreferenceClicked() {
        // We should migrate threads only if threads are disabled
        pcpPreferences.setThreadFlagChangedManually()
        pcpPreferences.setShouldMigrateThreads(!pcpPreferences.areThreadMessagesEnabled())
        lightweightSettingsStorage.setThreadMessagesEnabled(pcpPreferences.areThreadMessagesEnabled())
        displayLoadingView()
        MainActivity.restartApp(requireActivity(), MainActivityArgs(clearCache = true))
    }

    /**
     * Action when new layout preference switch is actually clicked.
     */
    private fun onNewLayoutPreferenceClicked() {
        configureUnreadNotificationsAsTabPreference()
    }

    private fun configureEnableClientInfoRecordingPreference() {
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_LABS_CLIENT_INFO_RECORDING_KEY)?.onPreferenceChangeListener =
                OnPreferenceChangeListener { _, newValue ->
                    when (newValue as? Boolean) {
                        false -> viewModel.handle(PcpSettingsLabsAction.DeleteRecordedClientInfo)
                        true -> viewModel.handle(PcpSettingsLabsAction.UpdateClientInfo)
                        else -> Unit
                    }
                    true
                }
    }
}
