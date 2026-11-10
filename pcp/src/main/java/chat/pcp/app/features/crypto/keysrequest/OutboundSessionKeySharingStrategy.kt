/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.keysrequest

enum class OutboundSessionKeySharingStrategy {
    /**
     * Keys will be sent for the first time when the first message is sent.
     * This is handled by the Matrix SDK so there's no need to do it in Vector.
     */
    WhenSendingEvent,

    /**
     * Keys will be sent for the first time when the timeline displayed.
     */
    WhenEnteringRoom,

    /**
     * Keys will be sent for the first time when a typing started.
     */
    WhenTyping
}
