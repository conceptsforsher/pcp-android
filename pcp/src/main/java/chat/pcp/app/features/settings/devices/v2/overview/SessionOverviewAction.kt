/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.overview

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class SessionOverviewAction : PcpViewModelAction {

    object VerifySession : SessionOverviewAction()
    object SignoutOtherSession : SessionOverviewAction()
    object SsoAuthDone : SessionOverviewAction()
    data class PasswordAuthDone(val password: String) : SessionOverviewAction()
    object ReAuthCancelled : SessionOverviewAction()
    data class TogglePushNotifications(
            val deviceId: String,
            val enabled: Boolean,
    ) : SessionOverviewAction()
    object ToggleIpAddressVisibility : SessionOverviewAction()
}
