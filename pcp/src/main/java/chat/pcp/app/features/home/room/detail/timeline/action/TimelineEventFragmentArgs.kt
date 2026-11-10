/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.action

import android.os.Parcelable
import chat.pcp.app.features.home.room.detail.timeline.item.MessageInformationData
import kotlinx.parcelize.Parcelize

@Parcelize
data class TimelineEventFragmentArgs(
        val eventId: String,
        val roomId: String,
        val informationData: MessageInformationData,
        val isFromThreadTimeline: Boolean = false
) : Parcelable
