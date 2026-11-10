/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.othersessions

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse

sealed class OtherSessionsViewEvents : PcpViewEvents {
    data class RequestReAuth(
            val registrationFlowResponse: RegistrationFlowResponse,
            val lastErrorCode: String?
    ) : OtherSessionsViewEvents()

    object SignoutSuccess : OtherSessionsViewEvents()
    data class SignoutError(val error: Throwable) : OtherSessionsViewEvents()
}
