/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.arguments

import android.os.Parcelable
import chat.pcp.app.features.home.room.threads.arguments.ThreadTimelineArgs
import chat.pcp.app.features.share.SharedData
import kotlinx.parcelize.Parcelize

@Parcelize
data class TimelineArgs(
        val roomId: String,
        val eventId: String? = null,
        val sharedData: SharedData? = null,
        val openShareSpaceForId: String? = null,
        val threadTimelineArgs: ThreadTimelineArgs? = null,
        val switchToParentSpace: Boolean = false,
        val isInviteAlreadyAccepted: Boolean = false
) : Parcelable
