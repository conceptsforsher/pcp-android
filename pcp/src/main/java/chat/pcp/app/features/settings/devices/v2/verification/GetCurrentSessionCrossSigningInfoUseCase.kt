/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.verification

import chat.pcp.app.core.di.ActiveSessionHolder
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.emptyFlow
import kotlinx.coroutines.flow.map
import org.matrix.android.sdk.api.extensions.orFalse
import org.matrix.android.sdk.api.session.crypto.crosssigning.MXCrossSigningInfo
import org.matrix.android.sdk.api.util.Optional
import org.matrix.android.sdk.flow.flow
import javax.inject.Inject

class GetCurrentSessionCrossSigningInfoUseCase @Inject constructor(
        private val activeSessionHolder: ActiveSessionHolder,
) {

    fun execute(): Flow<CurrentSessionCrossSigningInfo> {
        return activeSessionHolder.getSafeActiveSession()
                ?.let { session ->
                    session.flow().liveCrossSigningInfo(session.myUserId)
                            .map { convertToSigningInfo(session.sessionParams.deviceId.orEmpty(), it) }
                } ?: emptyFlow()
    }

    private fun convertToSigningInfo(deviceId: String, mxCrossSigningInfo: Optional<MXCrossSigningInfo>): CurrentSessionCrossSigningInfo {
        return CurrentSessionCrossSigningInfo(
                deviceId = deviceId,
                isCrossSigningInitialized = mxCrossSigningInfo.getOrNull() != null,
                isCrossSigningVerified = mxCrossSigningInfo.getOrNull()?.isTrusted().orFalse()
        )
    }
}
