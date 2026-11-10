/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.share

import android.os.Parcelable
import kotlinx.parcelize.Parcelize
import org.matrix.android.sdk.api.session.content.ContentAttachmentData

sealed class SharedData : Parcelable {

    @Parcelize
    data class Text(val text: String) : SharedData()

    @Parcelize
    data class Attachments(val attachmentData: List<ContentAttachmentData>) : SharedData()
}
