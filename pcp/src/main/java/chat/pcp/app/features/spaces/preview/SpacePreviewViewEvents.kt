/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.preview

import chat.pcp.app.core.platform.PcpViewEvents

sealed class SpacePreviewViewEvents : PcpViewEvents {
    object Dismiss : SpacePreviewViewEvents()
    object JoinSuccess : SpacePreviewViewEvents()
    data class JoinFailure(val message: String?) : SpacePreviewViewEvents()
}
