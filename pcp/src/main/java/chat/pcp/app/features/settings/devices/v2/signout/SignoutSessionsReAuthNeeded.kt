/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.signout

import org.matrix.android.sdk.api.auth.UIABaseAuth
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse
import kotlin.coroutines.Continuation

data class SignoutSessionsReAuthNeeded(
        val pendingAuth: UIABaseAuth,
        val uiaContinuation: Continuation<UIABaseAuth>,
        val flowResponse: RegistrationFlowResponse,
        val errCode: String?
)
