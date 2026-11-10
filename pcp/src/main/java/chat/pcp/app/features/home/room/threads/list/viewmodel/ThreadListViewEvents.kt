/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.threads.list.viewmodel

import chat.pcp.app.core.platform.PcpViewEvents

sealed interface ThreadListViewEvents : PcpViewEvents {
    data class ShowError(val throwable: Throwable) : ThreadListViewEvents
}
