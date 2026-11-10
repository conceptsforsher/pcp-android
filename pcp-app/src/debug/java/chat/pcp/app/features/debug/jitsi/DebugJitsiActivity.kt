/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.debug.jitsi

import android.annotation.SuppressLint
import android.view.View
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.platform.PcpBaseActivity
import chat.pcp.application.databinding.ActivityDebugJitsiBinding
import org.jitsi.meet.sdk.JitsiMeet

@AndroidEntryPoint
class DebugJitsiActivity : PcpBaseActivity<ActivityDebugJitsiBinding>() {

    override fun getBinding() = ActivityDebugJitsiBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout
    override val rootView: View
        get() = views.coordinatorLayout

    @SuppressLint("SetTextI18n")
    override fun initUiAndData() {
        val isCrashReportingDisabled = JitsiMeet.isCrashReportingDisabled(this)
        views.status.text = "Jitsi crash reporting is disabled: $isCrashReportingDisabled"

        views.splash.setOnClickListener {
            JitsiMeet.showSplashScreen(this)
        }

        views.dev.setOnClickListener {
            JitsiMeet.showDevOptions()
        }
    }
}
