/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.notification

import chat.pcp.app.core.pushers.UnifiedPushHelper
import chat.pcp.app.features.settings.PcpBasePreferences
import org.matrix.android.sdk.api.account.LocalNotificationSettingsContent
import org.matrix.android.sdk.api.session.Session
import javax.inject.Inject

/**
 * Update the notification settings account data for the current session depending on whether
 * the background sync is enabled or not.
 */
class UpdateNotificationSettingsAccountDataUseCase @Inject constructor(
        private val pcpPreferences: PcpBasePreferences,
        private val unifiedPushHelper: UnifiedPushHelper,
        private val getNotificationSettingsAccountDataUseCase: GetNotificationSettingsAccountDataUseCase,
        private val setNotificationSettingsAccountDataUseCase: SetNotificationSettingsAccountDataUseCase,
        private val deleteNotificationSettingsAccountDataUseCase: DeleteNotificationSettingsAccountDataUseCase,
) {

    suspend fun execute(session: Session) {
        if (unifiedPushHelper.isBackgroundSync()) {
            setCurrentNotificationStatus(session)
        } else {
            deleteCurrentNotificationStatus(session)
        }
    }

    private suspend fun setCurrentNotificationStatus(session: Session) {
        val deviceId = session.sessionParams.deviceId
        val areNotificationsSilenced = !pcpPreferences.areNotificationEnabledForDevice()
        val isSilencedAccountData = getNotificationSettingsAccountDataUseCase.execute(session, deviceId)?.isSilenced
        if (areNotificationsSilenced != isSilencedAccountData) {
            val notificationSettingsContent = LocalNotificationSettingsContent(
                    isSilenced = areNotificationsSilenced
            )
            setNotificationSettingsAccountDataUseCase.execute(session, deviceId, notificationSettingsContent)
        }
    }

    private suspend fun deleteCurrentNotificationStatus(session: Session) {
        deleteNotificationSettingsAccountDataUseCase.execute(session)
    }
}
