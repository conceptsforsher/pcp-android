/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.home.room.detail.composer.link

import chat.pcp.app.core.platform.PcpSharedAction
import chat.pcp.app.core.platform.PcpSharedActionViewModel
import javax.inject.Inject

class SetLinkSharedActionViewModel @Inject constructor() :
        PcpSharedActionViewModel<SetLinkSharedAction>()

sealed interface SetLinkSharedAction : PcpSharedAction {
    data class Set(
            val link: String,
    ) : SetLinkSharedAction

    data class Insert(
            val text: String,
            val link: String,
    ) : SetLinkSharedAction

    object Remove : SetLinkSharedAction
}
