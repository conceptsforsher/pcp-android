/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.pushers

import android.content.Context
import android.widget.Toast
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.services.GuardServiceStarter
import chat.pcp.app.features.settings.BackgroundSyncMode
import chat.pcp.app.features.settings.PcpBasePreferences
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import org.matrix.android.sdk.api.logger.LoggerTag
import org.unifiedpush.android.connector.MessagingReceiver
import timber.log.Timber
import javax.inject.Inject

private val loggerTag = LoggerTag("Push", LoggerTag.SYNC)

/**
 * Hilt injection happen at super.onReceive().
 */
@AndroidEntryPoint
class PcpUnifiedPush : MessagingReceiver() {
    @Inject lateinit var pushersManager: PushersManager
    @Inject lateinit var pushParser: PushParser
    @Inject lateinit var activeSessionHolder: ActiveSessionHolder
    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var vectorPushHandler: PcpPushHandler
    @Inject lateinit var guardServiceStarter: GuardServiceStarter
    @Inject lateinit var unifiedPushStore: UnifiedPushStore
    @Inject lateinit var unifiedPushHelper: UnifiedPushHelper

    private val coroutineScope = CoroutineScope(SupervisorJob())

    /**
     * Called when message is received.
     *
     * @param context the Android context
     * @param message the message
     * @param instance connection, for multi-account
     */
    override fun onMessage(context: Context, message: ByteArray, instance: String) {
        Timber.tag(loggerTag.value).d("New message")
        pushParser.parsePushDataUnifiedPush(message)?.let {
            vectorPushHandler.handle(it)
        } ?: run {
            Timber.tag(loggerTag.value).w("Invalid received data Json format")
        }
    }

    override fun onNewEndpoint(context: Context, endpoint: String, instance: String) {
        Timber.tag(loggerTag.value).i("onNewEndpoint: adding $endpoint")
        if (pcpPreferences.areNotificationEnabledForDevice() && activeSessionHolder.hasActiveSession()) {
            // If the endpoint has changed
            // or the gateway has changed
            if (unifiedPushHelper.getEndpointOrToken() != endpoint) {
                unifiedPushStore.storeUpEndpoint(endpoint)
                coroutineScope.launch {
                    unifiedPushHelper.storeCustomOrDefaultGateway(endpoint) {
                        unifiedPushHelper.getPushGateway()?.let {
                            coroutineScope.launch {
                                pushersManager.enqueueRegisterPusher(endpoint, it)
                            }
                        }
                    }
                }
            } else {
                Timber.tag(loggerTag.value).i("onNewEndpoint: skipped")
            }
        }
        val mode = BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_DISABLED
        pcpPreferences.setFdroidSyncBackgroundMode(mode)
        guardServiceStarter.stop()
    }

    override fun onRegistrationFailed(context: Context, instance: String) {
        Toast.makeText(context, "Push service registration failed", Toast.LENGTH_SHORT).show()
        val mode = BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_REALTIME
        pcpPreferences.setFdroidSyncBackgroundMode(mode)
        guardServiceStarter.start()
    }

    override fun onUnregistered(context: Context, instance: String) {
        Timber.tag(loggerTag.value).d("Unifiedpush: Unregistered")
        val mode = BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_REALTIME
        pcpPreferences.setFdroidSyncBackgroundMode(mode)
        guardServiceStarter.start()
        runBlocking {
            try {
                pushersManager.unregisterPusher(unifiedPushHelper.getEndpointOrToken().orEmpty())
            } catch (e: Exception) {
                Timber.tag(loggerTag.value).d("Probably unregistering a non existing pusher")
            }
        }
    }
}
