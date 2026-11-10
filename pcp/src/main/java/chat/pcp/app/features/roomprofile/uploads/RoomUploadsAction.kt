/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.uploads

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.uploads.UploadEvent

sealed class RoomUploadsAction : PcpViewModelAction {
    data class Download(val uploadEvent: UploadEvent) : RoomUploadsAction()
    data class Share(val uploadEvent: UploadEvent) : RoomUploadsAction()

    object Retry : RoomUploadsAction()
    object LoadMore : RoomUploadsAction()
}
