/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home

import com.airbnb.mvrx.MavericksState
import chat.pcp.app.core.platform.StateView
import chat.pcp.app.features.home.room.list.home.header.RoomsHeadersData

data class HomeRoomListViewState(
        val emptyState: StateView.State.Empty? = null,
        val headersData: RoomsHeadersData = RoomsHeadersData(),
) : MavericksState
