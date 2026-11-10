/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

object RoomSummaryFormatter {

    /**
     * Format the unread messages counter.
     *
     * @param count the count
     * @return the formatted value
     */
    fun formatUnreadMessagesCounter(count: Int): String {
        return if (count > 999) {
            "${count / 1000}.${count % 1000 / 100}k"
        } else {
            count.toString()
        }
    }
}
