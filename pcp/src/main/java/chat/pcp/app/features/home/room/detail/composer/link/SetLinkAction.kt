/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.composer.link

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class SetLinkAction : PcpViewModelAction {
    data class LinkChanged(
            val newLink: String
    ) : SetLinkAction()

    data class Save(
            val link: String,
            val text: String,
    ) : SetLinkAction()
}
