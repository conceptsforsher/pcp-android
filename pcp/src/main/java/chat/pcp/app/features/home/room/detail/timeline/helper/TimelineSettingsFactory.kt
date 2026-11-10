/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.helper

import chat.pcp.app.core.resources.UserPreferencesProvider
import org.matrix.android.sdk.api.session.room.timeline.TimelineSettings
import javax.inject.Inject

class TimelineSettingsFactory @Inject constructor(private val userPreferencesProvider: UserPreferencesProvider) {

    fun create(rootThreadEventId: String?): TimelineSettings {
        return TimelineSettings(
                initialSize = 30,
                buildReadReceipts = userPreferencesProvider.shouldShowReadReceipts(),
                rootThreadEventId = rootThreadEventId,
                useLiveSenderInfo = userPreferencesProvider.showLiveSenderInfo()
        )
    }
}
