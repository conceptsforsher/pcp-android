/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.notifications

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.media.RingtoneManager
import android.net.Uri
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.lifecycle.LiveData
import androidx.lifecycle.distinctUntilChanged
import androidx.lifecycle.lifecycleScope
import androidx.lifecycle.map
import androidx.preference.Preference
import androidx.preference.SwitchPreference
import com.airbnb.mvrx.fragmentViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.extensions.registerStartForActivityResult
import chat.pcp.app.core.preference.PcpEditTextPreference
import chat.pcp.app.core.preference.PcpBasePreference
import chat.pcp.app.core.preference.PcpPreferenceCategory
import chat.pcp.app.core.preference.PcpSwitchPreference
import chat.pcp.app.core.pushers.EnsureFcmTokenIsRetrievedUseCase
import chat.pcp.app.core.pushers.FcmHelper
import chat.pcp.app.core.pushers.PushersManager
import chat.pcp.app.core.pushers.UnifiedPushHelper
import chat.pcp.app.core.services.GuardServiceStarter
import chat.pcp.app.core.utils.combineLatest
import chat.pcp.app.core.utils.isIgnoringBatteryOptimizations
import chat.pcp.app.core.utils.registerForPermissionsResult
import chat.pcp.app.core.utils.requestDisablingBatteryOptimization
import chat.pcp.app.core.utils.startNotificationSettingsIntent
import chat.pcp.app.features.PcpFeatures
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.home.NotificationPermissionManager
import chat.pcp.app.features.notifications.NotificationUtils
import chat.pcp.app.features.settings.BackgroundSyncMode
import chat.pcp.app.features.settings.BackgroundSyncModeChooserDialog
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.settings.PcpSettingsBaseFragment
import chat.pcp.app.features.settings.PcpSettingsInteraction
import chat.pcp.lib.core.utils.compat.getParcelableExtraCompat
import chat.pcp.lib.strings.CommonPlurals
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.extensions.tryOrNull
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.identity.ThreePid
import org.matrix.android.sdk.api.session.pushers.Pusher
import org.matrix.android.sdk.api.session.pushrules.RuleIds
import org.matrix.android.sdk.api.session.pushrules.RuleKind
import javax.inject.Inject

// Referenced in pcp_settings_preferences_root.xml
@AndroidEntryPoint
class PcpSettingsNotifications :
        PcpSettingsBaseFragment(),
        BackgroundSyncModeChooserDialog.InteractionListener {

    @Inject lateinit var unifiedPushHelper: UnifiedPushHelper
    @Inject lateinit var pushersManager: PushersManager
    @Inject lateinit var fcmHelper: FcmHelper
    @Inject lateinit var activeSessionHolder: ActiveSessionHolder
    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var guardServiceStarter: GuardServiceStarter
    @Inject lateinit var vectorFeatures: PcpFeatures
    @Inject lateinit var notificationPermissionManager: NotificationPermissionManager
    @Inject lateinit var ensureFcmTokenIsRetrievedUseCase: EnsureFcmTokenIsRetrievedUseCase

    override var titleRes: Int = CommonStrings.settings_notifications
    override val preferenceXmlRes = R.xml.pcp_settings_notifications

    private var interactionListener: PcpSettingsInteraction? = null

    private val viewModel: PcpSettingsNotifViewModel by fragmentViewModel()

    private val notificationStartForActivityResult = registerStartForActivityResult { _ ->
        // No op
    }

    private val postPermissionLauncher = registerForPermissionsResult { _, deniedPermanently ->
        if (deniedPermanently) {
            // Open System setting, to give a chance to the user to enable notification. Sometimes the permission dialog is not displayed
            startNotificationSettingsIntent(requireContext(), notificationStartForActivityResult)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsNotifications
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        observeViewEvents()
    }

    private fun observeViewEvents() {
        viewModel.observeViewEvents {
            when (it) {
                PcpSettingsNotifEvent.NotificationsForDeviceEnabled -> onNotificationsForDeviceEnabled()
                PcpSettingsNotifEvent.NotificationsForDeviceDisabled -> onNotificationsForDeviceDisabled()
                is PcpSettingsNotifEvent.AskUserForPushDistributor -> askUserToSelectPushDistributor()
                PcpSettingsNotifEvent.NotificationMethodChanged -> onNotificationMethodChanged()
            }
        }
    }

    override fun bindPref() {
        findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_ENABLE_ALL_NOTIF_PREFERENCE_KEY)!!.let { pref ->
            val pushRuleService = session.pushRuleService()
            val mRuleMaster = pushRuleService.getPushRules().getAllRules()
                    .find { it.ruleId == RuleIds.RULE_ID_DISABLE_ALL }

            if (mRuleMaster == null) {
                // The homeserver does not support RULE_ID_DISABLE_ALL, so hide the preference
                pref.isVisible = false
                return
            }

            val areNotifEnabledAtAccountLevel = !mRuleMaster.enabled
            (pref as SwitchPreference).isChecked = areNotifEnabledAtAccountLevel
        }

        findPreference<SwitchPreference>(PcpBasePreferences.SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY)
                ?.setOnPreferenceChangeListener { _, isChecked ->
                    val action = if (isChecked as Boolean) {
                        PcpSettingsNotifAction.EnableNotificationsForDevice(pushDistributor = "")
                    } else {
                        PcpSettingsNotifAction.DisableNotificationsForDevice
                    }
                    viewModel.handle(action)
                    // preference will be updated on ViewEvent reception
                    false
                }

        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_FDROID_BACKGROUND_SYNC_MODE)?.let {
            it.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                val initialMode = pcpPreferences.getFdroidSyncBackgroundMode()
                val dialogFragment = BackgroundSyncModeChooserDialog.newInstance(initialMode)
                dialogFragment.interactionListener = this
                activity?.supportFragmentManager?.let { fm ->
                    dialogFragment.show(fm, "syncDialog")
                }
                true
            }
        }

        findPreference<PcpEditTextPreference>(PcpBasePreferences.SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY)?.let {
            it.isEnabled = pcpPreferences.isBackgroundSyncEnabled()
            it.summary = secondsToText(pcpPreferences.backgroundSyncTimeOut())
            it.onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                if (newValue is String) {
                    val syncTimeout = tryOrNull { Integer.parseInt(newValue) } ?: BackgroundSyncMode.DEFAULT_SYNC_TIMEOUT_SECONDS
                    pcpPreferences.setBackgroundSyncTimeout(maxOf(0, syncTimeout))
                    refreshBackgroundSyncPrefs()
                }
                true
            }
        }

        findPreference<PcpEditTextPreference>(PcpBasePreferences.SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY)?.let {
            it.isEnabled = pcpPreferences.isBackgroundSyncEnabled()
            it.summary = secondsToText(pcpPreferences.backgroundSyncDelay())
            it.onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                if (newValue is String) {
                    val syncDelay = tryOrNull { Integer.parseInt(newValue) } ?: BackgroundSyncMode.DEFAULT_SYNC_DELAY_SECONDS
                    pcpPreferences.setBackgroundSyncDelay(maxOf(0, syncDelay))
                    refreshBackgroundSyncPrefs()
                }
                true
            }
        }

        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_NOTIFICATION_METHOD_KEY)?.let {
            if (vectorFeatures.allowExternalUnifiedPushDistributors()) {
                it.summary = unifiedPushHelper.getCurrentDistributorName()
                it.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                    askUserToSelectPushDistributor(withUnregister = true)
                    true
                }
            } else {
                it.isVisible = false
            }
        }

        bindEmailNotifications()
        refreshBackgroundSyncPrefs()

        handleSystemPreference()
    }

    private fun onNotificationsForDeviceEnabled() {
        findPreference<SwitchPreference>(PcpBasePreferences.SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY)
                ?.isChecked = true
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_NOTIFICATION_METHOD_KEY)
                ?.summary = unifiedPushHelper.getCurrentDistributorName()

        notificationPermissionManager.eventuallyRequestPermission(
                requireActivity(),
                postPermissionLauncher,
                showRationale = false,
                ignorePreference = true
        )
    }

    private fun onNotificationsForDeviceDisabled() {
        findPreference<SwitchPreference>(PcpBasePreferences.SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY)
                ?.isChecked = false
        notificationPermissionManager.eventuallyRevokePermission(requireActivity())
    }

    private fun askUserToSelectPushDistributor(withUnregister: Boolean = false) {
        unifiedPushHelper.showSelectDistributorDialog(requireContext()) { selection ->
            if (withUnregister) {
                viewModel.handle(PcpSettingsNotifAction.RegisterPushDistributor(selection))
            } else {
                viewModel.handle(PcpSettingsNotifAction.EnableNotificationsForDevice(selection))
            }
        }
    }

    private fun onNotificationMethodChanged() {
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_NOTIFICATION_METHOD_KEY)?.summary = unifiedPushHelper.getCurrentDistributorName()
        session.pushersService().refreshPushers()
        refreshBackgroundSyncPrefs()
    }

    private fun bindEmailNotifications() {
        val initialEmails = session.getEmailsWithPushInformation()
        bindEmailNotificationCategory(initialEmails)
        session.getEmailsWithPushInformationLive().observe(this) { emails ->
            if (initialEmails != emails) {
                bindEmailNotificationCategory(emails)
            }
        }
    }

    private fun bindEmailNotificationCategory(emails: List<Pair<ThreePid.Email, Boolean>>) {
        findPreference<PcpPreferenceCategory>(PcpBasePreferences.SETTINGS_EMAIL_NOTIFICATION_CATEGORY_PREFERENCE_KEY)?.let { category ->
            category.removeAll()
            if (emails.isEmpty()) {
                val vectorPreference = PcpBasePreference(requireContext())
                vectorPreference.title = resources.getString(CommonStrings.settings_notification_emails_no_emails)
                category.addPreference(vectorPreference)
                vectorPreference.setOnPreferenceClickListener {
                    interactionListener?.navigateToEmailAndPhoneNumbers()
                    true
                }
            } else {
                emails.forEach { (emailPid, isEnabled) ->
                    val pref = PcpSwitchPreference(requireContext())
                    pref.title = resources.getString(CommonStrings.settings_notification_emails_enable_for_email, emailPid.email)
                    pref.isChecked = isEnabled
                    pref.setTransactionalSwitchChangeListener(lifecycleScope) { isChecked ->
                        if (isChecked) {
                            pushersManager.registerEmailForPush(emailPid.email)
                        } else {
                            pushersManager.unregisterEmailPusher(emailPid.email)
                        }
                    }
                    category.addPreference(pref)
                }
            }
        }
    }

    private val batteryStartForActivityResult = registerStartForActivityResult {
        // Noop
    }

    // BackgroundSyncModeChooserDialog.InteractionListener
    override fun onOptionSelected(mode: BackgroundSyncMode) {
        // option has change, need to act
        if (mode == BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_REALTIME) {
            // Important, Battery optim white listing is needed in this mode;
            // Even if using foreground service with foreground notif, it stops to work
            // in doze mode for certain devices :/
            if (!requireContext().isIgnoringBatteryOptimizations()) {
                requestDisablingBatteryOptimization(requireActivity(), batteryStartForActivityResult)
            }
        }
        pcpPreferences.setFdroidSyncBackgroundMode(mode)
        refreshBackgroundSyncPrefs()
    }

    private fun refreshBackgroundSyncPrefs() {
        findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_FDROID_BACKGROUND_SYNC_MODE)?.let {
            it.summary = when (pcpPreferences.getFdroidSyncBackgroundMode()) {
                BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_BATTERY -> getString(CommonStrings.settings_background_fdroid_sync_mode_battery)
                BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_REALTIME -> getString(CommonStrings.settings_background_fdroid_sync_mode_real_time)
                BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_DISABLED -> getString(CommonStrings.settings_background_fdroid_sync_mode_disabled)
            }
        }

        findPreference<PcpPreferenceCategory>(PcpBasePreferences.SETTINGS_BACKGROUND_SYNC_PREFERENCE_KEY)?.let {
            it.isVisible = unifiedPushHelper.isBackgroundSync()
        }

        val backgroundSyncEnabled = pcpPreferences.isBackgroundSyncEnabled()
        findPreference<PcpEditTextPreference>(PcpBasePreferences.SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY)?.let {
            it.isEnabled = backgroundSyncEnabled
            it.summary = secondsToText(pcpPreferences.backgroundSyncTimeOut())
        }
        findPreference<PcpEditTextPreference>(PcpBasePreferences.SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY)?.let {
            it.isEnabled = backgroundSyncEnabled
            it.summary = secondsToText(pcpPreferences.backgroundSyncDelay())
        }
        when {
            backgroundSyncEnabled -> guardServiceStarter.start()
            else -> guardServiceStarter.stop()
        }
    }

    /**
     * Convert a delay in seconds to string.
     *
     * @param seconds the delay in seconds
     * @return the text
     */
    private fun secondsToText(seconds: Int): String {
        return resources.getQuantityString(CommonPlurals.seconds, seconds, seconds)
    }

    private fun handleSystemPreference() {
        val callNotificationsSystemOptions = findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_SYSTEM_CALL_NOTIFICATION_PREFERENCE_KEY)!!
        if (NotificationUtils.supportNotificationChannels()) {
            callNotificationsSystemOptions.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                NotificationUtils.openSystemSettingsForCallCategory(this)
                false
            }
        } else {
            callNotificationsSystemOptions.isVisible = false
        }

        val noisyNotificationsSystemOptions = findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_SYSTEM_NOISY_NOTIFICATION_PREFERENCE_KEY)!!
        if (NotificationUtils.supportNotificationChannels()) {
            noisyNotificationsSystemOptions.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                NotificationUtils.openSystemSettingsForNoisyCategory(this)
                false
            }
        } else {
            noisyNotificationsSystemOptions.isVisible = false
        }

        val silentNotificationsSystemOptions = findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_SYSTEM_SILENT_NOTIFICATION_PREFERENCE_KEY)!!
        if (NotificationUtils.supportNotificationChannels()) {
            silentNotificationsSystemOptions.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                NotificationUtils.openSystemSettingsForSilentCategory(this)
                false
            }
        } else {
            silentNotificationsSystemOptions.isVisible = false
        }

        // Ringtone
        val ringtonePreference = findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_NOTIFICATION_RINGTONE_SELECTION_PREFERENCE_KEY)!!

        if (NotificationUtils.supportNotificationChannels()) {
            ringtonePreference.isVisible = false
        } else {
            ringtonePreference.summary = pcpPreferences.getNotificationRingToneName()
            ringtonePreference.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                val intent = Intent(RingtoneManager.ACTION_RINGTONE_PICKER)
                intent.putExtra(RingtoneManager.EXTRA_RINGTONE_TYPE, RingtoneManager.TYPE_NOTIFICATION)

                if (null != pcpPreferences.getNotificationRingTone()) {
                    intent.putExtra(RingtoneManager.EXTRA_RINGTONE_EXISTING_URI, pcpPreferences.getNotificationRingTone())
                }

                ringtoneStartForActivityResult.launch(intent)
                false
            }
        }
    }

    private val ringtoneStartForActivityResult = registerStartForActivityResult { activityResult ->
        if (activityResult.resultCode == Activity.RESULT_OK) {
            pcpPreferences.setNotificationRingTone(activityResult.data?.getParcelableExtraCompat<Uri>(RingtoneManager.EXTRA_RINGTONE_PICKED_URI))

            // test if the selected ring tone can be played
            val notificationRingToneName = pcpPreferences.getNotificationRingToneName()
            if (null != notificationRingToneName) {
                pcpPreferences.setNotificationRingTone(pcpPreferences.getNotificationRingTone())
                findPreference<PcpBasePreference>(PcpBasePreferences.SETTINGS_NOTIFICATION_RINGTONE_SELECTION_PREFERENCE_KEY)!!
                        .summary = notificationRingToneName
            }
        }
    }

    override fun onResume() {
        super.onResume()
        activeSessionHolder.getSafeActiveSession()?.pushersService()?.refreshPushers()

        interactionListener?.requestedKeyToHighlight()?.let { key ->
            interactionListener?.requestHighlightPreferenceKeyOnResume(null)
            val preference = findPreference<PcpSwitchPreference>(key)
            preference?.isHighlighted = true
        }

        refreshPref()
    }

    private fun refreshPref() {
        // This pref may have change from troubleshoot pref fragment
        if (unifiedPushHelper.isBackgroundSync()) {
            findPreference<PcpSwitchPreference>(PcpBasePreferences.SETTINGS_START_ON_BOOT_PREFERENCE_KEY)
                    ?.isChecked = pcpPreferences.autoStartOnBoot()
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is PcpSettingsInteraction) {
            interactionListener = context
        }
        (activity?.supportFragmentManager
                ?.findFragmentByTag("syncDialog") as BackgroundSyncModeChooserDialog?)
                ?.interactionListener = this
    }

    override fun onDetach() {
        interactionListener = null
        super.onDetach()
    }

    override fun onPreferenceTreeClick(preference: Preference): Boolean {
        return when (preference.key) {
            PcpBasePreferences.SETTINGS_ENABLE_ALL_NOTIF_PREFERENCE_KEY -> {
                updateEnabledForAccount(preference)
                true
            }
            else -> {
                return super.onPreferenceTreeClick(preference)
            }
        }
    }

    private fun updateEnabledForAccount(preference: Preference?) {
        val pushRuleService = session.pushRuleService()
        val switchPref = preference as SwitchPreference
        pushRuleService.getPushRules().getAllRules()
                .find { it.ruleId == RuleIds.RULE_ID_DISABLE_ALL }
                ?.let {
                    // Trick, we must enable this room to disable notifications
                    lifecycleScope.launch {
                        try {
                            pushRuleService.updatePushRuleEnableStatus(
                                    RuleKind.OVERRIDE,
                                    it,
                                    !switchPref.isChecked
                            )
                            // Push rules will be updated from the sync
                        } catch (failure: Throwable) {
                            if (!isAdded) {
                                return@launch
                            }

                            // revert the check box
                            switchPref.isChecked = !switchPref.isChecked
                            Toast.makeText(activity, CommonStrings.unknown_error, Toast.LENGTH_SHORT).show()
                        }
                    }
                }
    }
}

private fun SwitchPreference.setTransactionalSwitchChangeListener(scope: CoroutineScope, transaction: suspend (Boolean) -> Unit) {
    setOnPreferenceChangeListener { switchPreference, isChecked ->
        require(switchPreference is SwitchPreference)
        val originalState = switchPreference.isChecked
        scope.launch {
            try {
                transaction(isChecked as Boolean)
            } catch (failure: Throwable) {
                switchPreference.isChecked = originalState
                Toast.makeText(switchPreference.context, CommonStrings.unknown_error, Toast.LENGTH_SHORT).show()
            }
        }
        true
    }
}

/**
 * Fetches the current users 3pid emails and pairs them with their enabled state.
 * If no pusher is available for a given email we can infer that push is not registered for the email.
 * @return a list of ThreePid emails paired with the email notification enabled state. true if email notifications are enabled, false if not.
 * @see ThreePid.Email
 */
private fun Session.getEmailsWithPushInformation(): List<Pair<ThreePid.Email, Boolean>> {
    val emailPushers = pushersService().getPushers().filter { it.kind == Pusher.KIND_EMAIL }
    return profileService().getThreePids()
            .filterIsInstance<ThreePid.Email>()
            .map { it to emailPushers.any { pusher -> pusher.pushKey == it.email } }
}

private fun Session.getEmailsWithPushInformationLive(): LiveData<List<Pair<ThreePid.Email, Boolean>>> {
    val emailThreePids = profileService().getThreePidsLive(refreshData = true).map { it.filterIsInstance<ThreePid.Email>() }
    val emailPushers = pushersService().getPushersLive().map { it.filter { pusher -> pusher.kind == Pusher.KIND_EMAIL } }
    return combineLatest(emailThreePids, emailPushers) { emailThreePidsResult, emailPushersResult ->
        emailThreePidsResult.map { it to emailPushersResult.any { pusher -> pusher.pushKey == it.email } }
    }.distinctUntilChanged()
}
