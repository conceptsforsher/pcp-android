/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.notifications

import chat.pcp.app.core.platform.PcpViewModelAction

sealed interface PcpSettingsNotifAction : PcpViewModelAction {
    data class EnableNotificationsForDevice(val pushDistributor: String) : PcpSettingsNotifAction
    object DisableNotificationsForDevice : PcpSettingsNotifAction
    data class RegisterPushDistributor(val pushDistributor: String) : PcpSettingsNotifAction
}
