/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.helper

import chat.pcp.app.core.resources.UserPreferencesProvider
import org.matrix.android.sdk.api.session.room.timeline.TimelineSettings
import javax.inject.Inject

class TimelineSettingsFactory @Inject constructor(private val userPreferencesProvider: UserPreferencesProvider) {

    fun create(rootThreadEventId: String?, isPublicRoom: Boolean = false): TimelineSettings {
        val initialSize = if (isPublicRoom) 10 else 30
        return TimelineSettings(
                initialSize = initialSize,
                buildReadReceipts = userPreferencesProvider.shouldShowReadReceipts(),
                rootThreadEventId = rootThreadEventId,
                useLiveSenderInfo = userPreferencesProvider.showLiveSenderInfo()
        )
    }
}
