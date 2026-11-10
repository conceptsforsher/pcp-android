/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.crosssigning

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse

/**
 * Transient events for cross signing settings screen.
 */
sealed class CrossSigningSettingsViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : CrossSigningSettingsViewEvents()
    data class RequestReAuth(val registrationFlowResponse: RegistrationFlowResponse, val lastErrorCode: String?) : CrossSigningSettingsViewEvents()
    data class ShowModalWaitingView(val status: String?) : CrossSigningSettingsViewEvents()
    object HideModalWaitingView : CrossSigningSettingsViewEvents()
}
