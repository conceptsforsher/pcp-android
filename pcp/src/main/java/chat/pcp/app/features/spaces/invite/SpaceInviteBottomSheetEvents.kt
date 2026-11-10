/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.invite

import chat.pcp.app.core.platform.PcpViewEvents

sealed class SpaceInviteBottomSheetEvents : PcpViewEvents {
    data class ShowError(val message: String) : SpaceInviteBottomSheetEvents()
}
