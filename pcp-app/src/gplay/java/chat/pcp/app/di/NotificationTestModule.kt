/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.di

import dagger.Binds
import dagger.Module
import dagger.hilt.InstallIn
import dagger.hilt.android.components.ActivityComponent
import chat.pcp.app.features.push.NotificationTroubleshootTestManagerFactory
import chat.pcp.app.push.fcm.GoogleNotificationTroubleshootTestManagerFactory

@InstallIn(ActivityComponent::class)
@Module
abstract class NotificationTestModule {
    @Binds
    abstract fun bindsNotificationTestFactory(factory: GoogleNotificationTroubleshootTestManagerFactory): NotificationTroubleshootTestManagerFactory
}
