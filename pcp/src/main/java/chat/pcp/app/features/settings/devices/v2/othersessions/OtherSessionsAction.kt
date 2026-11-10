/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.othersessions

import chat.pcp.app.core.platform.PcpViewModelAction
import chat.pcp.app.features.settings.devices.v2.filter.DeviceManagerFilterType

sealed class OtherSessionsAction : PcpViewModelAction {
    // ReAuth
    object SsoAuthDone : OtherSessionsAction()
    data class PasswordAuthDone(val password: String) : OtherSessionsAction()
    object ReAuthCancelled : OtherSessionsAction()

    // Others
    data class FilterDevices(val filterType: DeviceManagerFilterType) : OtherSessionsAction()
    data class EnableSelectMode(val deviceId: String?) : OtherSessionsAction()
    object DisableSelectMode : OtherSessionsAction()
    data class ToggleSelectionForDevice(val deviceId: String) : OtherSessionsAction()
    object SelectAll : OtherSessionsAction()
    object DeselectAll : OtherSessionsAction()
    object MultiSignout : OtherSessionsAction()
    object ToggleIpAddressVisibility : OtherSessionsAction()
}
