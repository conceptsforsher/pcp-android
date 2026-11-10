/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.composer.link

import chat.pcp.app.core.platform.PcpViewEvents

sealed class SetLinkViewEvents : PcpViewEvents {

    data class SavedLink(
            val link: String,
    ) : SetLinkViewEvents()

    data class SavedLinkAndText(
            val link: String,
            val text: String,
    ) : SetLinkViewEvents()
}
