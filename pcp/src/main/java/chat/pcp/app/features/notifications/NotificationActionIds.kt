/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.notifications

import chat.pcp.app.core.resources.BuildMeta
import javax.inject.Inject

/**
 * Util class for creating notifications.
 * Note: Cannot inject ColorProvider in the constructor, because it requires an Activity
 */

data class NotificationActionIds @Inject constructor(
        private val buildMeta: BuildMeta,
) {

    val join = "${buildMeta.applicationId}.NotificationActions.JOIN_ACTION"
    val reject = "${buildMeta.applicationId}.NotificationActions.REJECT_ACTION"
    val quickLaunch = "${buildMeta.applicationId}.NotificationActions.QUICK_LAUNCH_ACTION"
    val markRoomRead = "${buildMeta.applicationId}.NotificationActions.MARK_ROOM_READ_ACTION"
    val smartReply = "${buildMeta.applicationId}.NotificationActions.SMART_REPLY_ACTION"
    val dismissSummary = "${buildMeta.applicationId}.NotificationActions.DISMISS_SUMMARY_ACTION"
    val dismissRoom = "${buildMeta.applicationId}.NotificationActions.DISMISS_ROOM_NOTIF_ACTION"
    val tapToView = "${buildMeta.applicationId}.NotificationActions.TAP_TO_VIEW_ACTION"
    val diagnostic = "${buildMeta.applicationId}.NotificationActions.DIAGNOSTIC"
    val push = "${buildMeta.applicationId}.PUSH"
}
