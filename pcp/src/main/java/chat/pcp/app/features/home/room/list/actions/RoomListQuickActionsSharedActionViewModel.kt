/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.home.room.list.actions

import chat.pcp.app.core.platform.PcpSharedAction
import chat.pcp.app.core.platform.PcpSharedActionViewModel
import javax.inject.Inject

/**
 * Activity shared view model to handle room list quick actions.
 */
class RoomListQuickActionsSharedActionViewModel @Inject constructor() : PcpSharedActionViewModel<RoomListQuickActionsSharedAction>()
