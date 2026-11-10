/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import com.airbnb.mvrx.MavericksState
import chat.pcp.app.features.home.room.list.UnreadCounterBadgeView

data class NewHomeDetailViewState(
        val spacesNotificationCounterBadgeState: UnreadCounterBadgeView.State = UnreadCounterBadgeView.State.Count(count = 0, highlighted = false),
) : MavericksState
