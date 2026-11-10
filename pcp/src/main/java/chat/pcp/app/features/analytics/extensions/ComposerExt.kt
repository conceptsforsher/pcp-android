/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.analytics.extensions

import im.vector.app.features.analytics.plan.Composer
import chat.pcp.app.features.home.room.detail.composer.MessageComposerViewState
import chat.pcp.app.features.home.room.detail.composer.SendMode

fun MessageComposerViewState.toAnalyticsComposer(): Composer =
        Composer(
                inThread = isInThreadTimeline(),
                isEditing = sendMode is SendMode.Edit,
                isReply = sendMode is SendMode.Reply,
                messageType = Composer.MessageType.Text,
                startsThread = startsThread,
        )
