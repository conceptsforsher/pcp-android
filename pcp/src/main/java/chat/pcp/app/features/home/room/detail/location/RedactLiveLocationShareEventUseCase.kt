/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.location

import org.matrix.android.sdk.api.session.events.model.Event
import org.matrix.android.sdk.api.session.room.Room
import javax.inject.Inject

class RedactLiveLocationShareEventUseCase @Inject constructor() {

    suspend fun execute(event: Event, room: Room, reason: String?) {
        event.eventId
                ?.takeUnless { it.isEmpty() }
                ?.let { room.locationSharingService().redactLiveLocationShare(it, reason) }
    }
}
