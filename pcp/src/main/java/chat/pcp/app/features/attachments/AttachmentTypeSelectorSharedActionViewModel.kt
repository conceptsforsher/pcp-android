/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.attachments

import chat.pcp.app.core.platform.PcpSharedAction
import chat.pcp.app.core.platform.PcpSharedActionViewModel
import javax.inject.Inject

class AttachmentTypeSelectorSharedActionViewModel @Inject constructor() :
        PcpSharedActionViewModel<AttachmentTypeSelectorSharedAction>()

sealed interface AttachmentTypeSelectorSharedAction : PcpSharedAction {
    data class SelectAttachmentTypeAction(
            val attachmentType: AttachmentType
    ) : AttachmentTypeSelectorSharedAction
}
