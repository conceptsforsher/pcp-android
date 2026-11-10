/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.discovery.change

import com.airbnb.mvrx.MavericksState

data class SetIdentityServerState(
        val homeServerUrl: String = "",
        // Will contain the default identity server url if any
        val defaultIdentityServerUrl: String? = null
) : MavericksState
