/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call.webrtc

import android.content.Intent
import android.os.Binder
import android.os.IBinder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.startForegroundCompat
import chat.pcp.app.core.services.PcpService
import chat.pcp.app.features.notifications.NotificationUtils
import chat.pcp.lib.core.utils.timer.Clock
import javax.inject.Inject

@AndroidEntryPoint
class ScreenCaptureAndroidService : PcpService() {

    @Inject lateinit var notificationUtils: NotificationUtils
    @Inject lateinit var clock: Clock
    private val binder = LocalBinder()

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        showStickyNotification()

        return START_STICKY
    }

    private fun showStickyNotification() {
        val notificationId = clock.epochMillis().toInt()
        val notification = notificationUtils.buildScreenSharingNotification()
        startForegroundCompat(notificationId, notification)
    }

    override fun onBind(intent: Intent?): IBinder {
        return binder
    }

    fun stopService() {
        stopSelf()
    }

    inner class LocalBinder : Binder() {
        fun getService(): ScreenCaptureAndroidService = this@ScreenCaptureAndroidService
    }
}
