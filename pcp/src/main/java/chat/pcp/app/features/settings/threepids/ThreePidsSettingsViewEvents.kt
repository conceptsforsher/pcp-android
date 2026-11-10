/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.threepids

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse

sealed class ThreePidsSettingsViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : ThreePidsSettingsViewEvents()

    //    object RequestPassword : ThreePidsSettingsViewEvents()
    data class RequestReAuth(val registrationFlowResponse: RegistrationFlowResponse, val lastErrorCode: String?) : ThreePidsSettingsViewEvents()
}
