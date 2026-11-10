/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.item

interface ItemWithEvents {
    /**
     * Returns the eventIds associated with the EventItem.
     * Will generally get only one, but it handles the merged items.
     */
    fun getEventIds(): List<String>

    fun canAppendReadMarker(): Boolean = true

    fun isVisible(): Boolean = true

    /**
     * Returns false if you want epoxy controller to rebuild the event each time a built is triggered.
     */
    fun isCacheable(): Boolean = true
}
