/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.more

import com.airbnb.mvrx.MavericksState

data class SessionLearnMoreViewState(
        val title: String,
        val description: String,
) : MavericksState {
    constructor(args: SessionLearnMoreBottomSheet.Args) : this(
            title = args.title,
            description = args.description,
    )
}
