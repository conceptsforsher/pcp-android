/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call.conference

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class JitsiCallViewActions : PcpViewModelAction {
    data class SwitchTo(
            val args: PcpJitsiActivity.Args,
            val withConfirmation: Boolean
    ) : JitsiCallViewActions()

    /**
     * The ViewModel will either ask the View to finish, or to join another conf.
     */
    object OnConferenceLeft : JitsiCallViewActions()
}
