/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.widgets.permissions

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class RoomWidgetPermissionActions : PcpViewModelAction {
    object AllowWidget : RoomWidgetPermissionActions()
    object BlockWidget : RoomWidgetPermissionActions()
}
