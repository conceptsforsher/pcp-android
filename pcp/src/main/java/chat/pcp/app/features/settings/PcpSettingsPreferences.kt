/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings

import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.lifecycle.lifecycleScope
import androidx.preference.Preference
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.dialogs.PhotoOrVideoDialog
import chat.pcp.app.core.extensions.restart
import chat.pcp.app.core.preference.PcpListPreference
import chat.pcp.app.core.preference.PcpBasePreference
import chat.pcp.app.core.preference.PcpSwitchPreference
import chat.pcp.app.features.MainActivity
import chat.pcp.app.features.MainActivityArgs
import chat.pcp.app.features.PcpFeatures
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.settings.font.FontScaleSettingActivity
import chat.pcp.app.features.themes.ThemeUtils
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.session.presence.model.PresenceEnum
import javax.inject.Inject

@AndroidEntryPoint
class PcpSettingsPreferences :
        PcpSettingsBaseFragment() {

    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var fontScalePreferences: FontScalePreferences
    @Inject lateinit var vectorFeatures: PcpFeatures
    @Inject lateinit var vectorLocale: PcpLocale

    override var titleRes = CommonStrings.settings_preferences
    override val preferenceXmlRes = R.xml.pcp_settings_preferences

    private val selectedLanguagePreference by lazy {
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_INTERFACE_LANGUAGE_PREFERENCE_KEY)!!
    }
    private val textSizePreference by lazy {
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_INTERFACE_TEXT_SIZE_KEY)!!
    }
    private val takePhotoOrVideoPreference by lazy {
        findPreference<PcpBasePreference>("SETTINGS_INTERFACE_TAKE_PHOTO_VIDEO")!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsPreferences
    }

    override fun bindPref() {
        // user interface preferences
        setUserInterfacePreferences()

        // Themes
        findPreference<PcpListPreference>(ThemeUtils.APPLICATION_THEME_KEY)!!
                .onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
            if (newValue is String) {
                ThemeUtils.setApplicationTheme(requireContext().applicationContext, newValue)
                // Restart the Activity
                activity?.restart()
                true
            } else {
                false
            }
        }

        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_PRESENCE_USER_ALWAYS_APPEARS_OFFLINE)!!.let { pref ->
            pref.isChecked = pcpPreferences.userAlwaysAppearsOffline()
            pref.setOnPreferenceChangeListener { _, newValue ->
                val presenceOfflineModeEnabled = newValue as? Boolean ?: false
                lifecycleScope.launch {
                    session.presenceService().setMyPresence(if (presenceOfflineModeEnabled) PresenceEnum.OFFLINE else PresenceEnum.ONLINE)
                }
                true
            }
        }

        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_PREF_SPACE_SHOW_ALL_ROOM_IN_HOME)!!.let { pref ->
            pref.isChecked = pcpPreferences.prefSpacesShowAllRoomInHome()
            pref.setOnPreferenceChangeListener { _, _ ->
                MainActivity.restartApp(requireActivity(), MainActivityArgs(clearCache = false))
                true
            }
        }

        findPreference<Preference>(PcpBasePreferences.SETTINGS_PREF_SPACE_CATEGORY)!!.let { pref ->
            pref.isVisible = !pcpPreferences.isNewAppLayoutEnabled()
            pref.isEnabled = !pcpPreferences.isNewAppLayoutEnabled()
        }

        // Url preview
        /*
        TODO Note: we keep the setting client side for now
        findPreference<SwitchPreference>(PcpBasePreferences.SETTINGS_SHOW_URL_PREVIEW_KEY)!!.let {
            it.isChecked = session.isURLPreviewEnabled

            it.onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                if (null != newValue && newValue as Boolean != session.isURLPreviewEnabled) {
                    displayLoadingView()
                    session.setURLPreviewStatus(newValue, object : MatrixCallback<Unit> {
                        override fun onSuccess(info: Void?) {
                            it.isChecked = session.isURLPreviewEnabled
                            hideLoadingView()
                        }

                        private fun onError(errorMessage: String) {
                            activity?.toast(errorMessage)

                            onSuccess(null)
                        }

                        override fun onNetworkError(e: Exception) {
                            onError(e.localizedMessage)
                        }

                        override fun onMatrixError(e: MatrixError) {
                            onError(e.localizedMessage)
                        }

                        override fun onUnexpectedError(e: Exception) {
                            onError(e.localizedMessage)
                        }
                    })
                }

                false
            }
        }
         */

        // update keep medias period
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_MEDIA_SAVING_PERIOD_KEY)!!.let {
            it.summary = pcpPreferences.getSelectedMediasSavingPeriodString()

            it.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                context?.let { context: Context ->
                    MaterialAlertDialogBuilder(context)
                            .setSingleChoiceItems(
                                    chat.pcp.lib.strings.R.array.media_saving_choice,
                                    pcpPreferences.getSelectedMediasSavingPeriod()
                            ) { d, n ->
                                pcpPreferences.setSelectedMediasSavingPeriod(n)
                                d.cancel()

                                it.summary = pcpPreferences.getSelectedMediasSavingPeriodString()
                            }
                            .show()
                }

                false
            }
        }

        // Take photo or video
        updateTakePhotoOrVideoPreferenceSummary()
        takePhotoOrVideoPreference.onPreferenceClickListener = Preference.OnPreferenceClickListener {
            PhotoOrVideoDialog(requireActivity(), pcpPreferences).showForSettings(object : PhotoOrVideoDialog.PhotoOrVideoDialogSettingsListener {
                override fun onUpdated() {
                    updateTakePhotoOrVideoPreferenceSummary()
                }
            })
            true
        }
    }

    private fun updateTakePhotoOrVideoPreferenceSummary() {
        takePhotoOrVideoPreference.summary = getString(
                when (pcpPreferences.getTakePhotoVideoMode()) {
                    PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_PHOTO -> CommonStrings.option_take_photo
                    PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_VIDEO -> CommonStrings.option_take_video
                    /* PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_ALWAYS_ASK */
                    else -> CommonStrings.option_always_ask
                }
        )
    }

    // ==============================================================================================================
    // user interface management
    // ==============================================================================================================

    private fun setUserInterfacePreferences() {
        // Selected language
        selectedLanguagePreference.summary = vectorLocale.localeToLocalisedString(vectorLocale.applicationLocale)

        // Text size
        textSizePreference.summary = getString(fontScalePreferences.getResolvedFontScaleValue().nameResId)

        textSizePreference.onPreferenceClickListener = Preference.OnPreferenceClickListener {
            startActivity(Intent(activity, FontScaleSettingActivity::class.java))
            true
        }
    }
}
