/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.attachments

import chat.pcp.app.core.utils.PERMISSIONS_EMPTY
import chat.pcp.app.core.utils.PERMISSIONS_FOR_FOREGROUND_LOCATION_SHARING
import chat.pcp.app.core.utils.PERMISSIONS_FOR_PICKING_CONTACT
import chat.pcp.app.core.utils.PERMISSIONS_FOR_TAKING_PHOTO
import chat.pcp.app.core.utils.PERMISSIONS_FOR_VOICE_BROADCAST

/**
 * The all possible types to pick with their required permissions.
 */
enum class AttachmentType(val permissions: List<String>) {
    CAMERA(PERMISSIONS_FOR_TAKING_PHOTO),
    GALLERY(PERMISSIONS_EMPTY),
    FILE(PERMISSIONS_EMPTY),
    STICKER(PERMISSIONS_EMPTY),
    CONTACT(PERMISSIONS_FOR_PICKING_CONTACT),
    POLL(PERMISSIONS_EMPTY),
    LOCATION(PERMISSIONS_FOR_FOREGROUND_LOCATION_SHARING),
    VOICE_BROADCAST(PERMISSIONS_FOR_VOICE_BROADCAST),
}
