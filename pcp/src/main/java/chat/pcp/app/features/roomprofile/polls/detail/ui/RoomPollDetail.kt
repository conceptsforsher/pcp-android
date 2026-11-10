/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.detail.ui

import chat.pcp.app.features.poll.PollItemViewState

data class RoomPollDetail(
        val creationTimestamp: Long,
        val isEnded: Boolean,
        val endedPollEventId: String?,
        val pollItemViewState: PollItemViewState,
)
