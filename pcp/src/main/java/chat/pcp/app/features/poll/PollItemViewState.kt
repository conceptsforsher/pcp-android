/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.poll

import chat.pcp.app.features.home.room.detail.timeline.item.PollOptionViewState

data class PollItemViewState(
        val question: String,
        val votesStatus: String,
        val canVote: Boolean,
        val optionViewStates: List<PollOptionViewState>?,
)
