/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.debug.di

import android.content.Context
import android.content.Intent
import dagger.Binds
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.pcp.app.core.debug.DebugNavigator
import chat.pcp.app.core.debug.DebugReceiver
import chat.pcp.app.core.debug.LeakDetector
import chat.pcp.app.features.debug.DebugMenuActivity
import chat.pcp.app.leakcanary.LeakCanaryLeakDetector
import chat.pcp.app.receivers.PcpDebugReceiver

@InstallIn(SingletonComponent::class)
@Module
abstract class DebugModule {

    companion object {

        @Provides
        fun providesDebugNavigator() = object : DebugNavigator {
            override fun openDebugMenu(context: Context) {
                context.startActivity(Intent(context, DebugMenuActivity::class.java))
            }
        }
    }

    @Binds
    abstract fun bindsDebugReceiver(receiver: PcpDebugReceiver): DebugReceiver

    @Binds
    abstract fun bindsLeakDetector(leakDetector: LeakCanaryLeakDetector): LeakDetector
}
