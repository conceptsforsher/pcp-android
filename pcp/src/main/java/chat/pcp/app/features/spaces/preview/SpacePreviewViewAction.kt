/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.preview

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class SpacePreviewViewAction : PcpViewModelAction {
    object ViewReady : SpacePreviewViewAction()
    object AcceptInvite : SpacePreviewViewAction()
    object DismissInvite : SpacePreviewViewAction()
}
