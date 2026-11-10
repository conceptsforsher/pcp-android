/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.pushers

import android.content.Context
import dagger.hilt.android.qualifiers.ApplicationContext
import chat.pcp.app.features.settings.BackgroundSyncMode
import chat.pcp.app.features.settings.PcpBasePreferences
import org.unifiedpush.android.connector.UnifiedPush
import timber.log.Timber
import javax.inject.Inject

class UnregisterUnifiedPushUseCase @Inject constructor(
        @ApplicationContext private val context: Context,
        private val pcpPreferences: PcpBasePreferences,
        private val unifiedPushStore: UnifiedPushStore,
        private val unifiedPushHelper: UnifiedPushHelper,
) {

    suspend fun execute(pushersManager: PushersManager?) {
        val mode = BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_REALTIME
        pcpPreferences.setFdroidSyncBackgroundMode(mode)
        try {
            unifiedPushHelper.getEndpointOrToken()?.let {
                Timber.d("Removing $it")
                pushersManager?.unregisterPusher(it)
            }
        } catch (e: Exception) {
            Timber.d(e, "Probably unregistering a non existing pusher")
        }
        unifiedPushStore.storeUpEndpoint(null)
        unifiedPushStore.storePushGateway(null)
        UnifiedPush.unregisterApp(context)
    }
}
