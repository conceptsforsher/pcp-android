/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices

import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.features.settings.devices.v2.verification.CurrentSessionCrossSigningInfo
import javax.inject.Inject

class GetCurrentSessionCrossSigningInfoUseCase @Inject constructor(
        private val activeSessionHolder: ActiveSessionHolder,
) {

    suspend fun execute(): CurrentSessionCrossSigningInfo {
        val session = activeSessionHolder.getActiveSession()
        val isCrossSigningInitialized = session.cryptoService().crossSigningService().isCrossSigningInitialized()
        val isCrossSigningVerified = session.cryptoService().crossSigningService().isCrossSigningVerified()
        return CurrentSessionCrossSigningInfo(
                deviceId = session.sessionParams.deviceId,
                isCrossSigningInitialized = isCrossSigningInitialized,
                isCrossSigningVerified = isCrossSigningVerified
        )
    }
}
