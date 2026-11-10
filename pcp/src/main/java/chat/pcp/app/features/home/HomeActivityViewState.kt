/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import com.airbnb.mvrx.MavericksState
import chat.pcp.app.features.onboarding.AuthenticationDescription
import org.matrix.android.sdk.api.session.sync.SyncRequestState

data class HomeActivityViewState(
        val syncRequestState: SyncRequestState = SyncRequestState.Idle,
        val authenticationDescription: AuthenticationDescription? = null,
) : MavericksState
