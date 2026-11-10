/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.uploads

import chat.pcp.app.core.platform.PcpViewEvents
import java.io.File

sealed class RoomUploadsViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : RoomUploadsViewEvents()

    data class FileReadyForSharing(val file: File) : RoomUploadsViewEvents()
    data class FileReadyForSaving(val file: File, val title: String) : RoomUploadsViewEvents()
}
