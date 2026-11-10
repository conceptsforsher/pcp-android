/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.pushers.model

/**
 * Represent parsed data that the app has received from a Push content.
 *
 * @property eventId The Event ID. If not null, it will not be empty, and will have a valid format.
 * @property roomId The Room ID. If not null, it will not be empty, and will have a valid format.
 * @property unread Number of unread message.
 */
data class PushData(
        val eventId: String?,
        val roomId: String?,
        val unread: Int?,
)
