/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomdirectory

data class RoomDirectoryServer(
        val serverName: String,

        /**
         * True if this is the current user server.
         */
        val isUserServer: Boolean,

        /**
         * True if manually added, so it can be removed by the user.
         */
        val isManuallyAdded: Boolean,

        /**
         * Supported protocols.
         * TODO Rename RoomDirectoryData to RoomDirectoryProtocols
         */
        val protocols: List<RoomDirectoryData>
)
