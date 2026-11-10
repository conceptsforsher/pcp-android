/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces

import chat.pcp.app.core.platform.PcpSharedAction

sealed class SpacePreviewSharedAction : PcpSharedAction {
    object DismissAction : SpacePreviewSharedAction()
    object ShowModalLoading : SpacePreviewSharedAction()
    object HideModalLoading : SpacePreviewSharedAction()
    data class ShowErrorMessage(val error: String? = null) : SpacePreviewSharedAction()
}
