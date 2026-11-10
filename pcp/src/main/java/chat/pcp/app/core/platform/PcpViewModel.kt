/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.platform

import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.MavericksViewModel
import chat.pcp.app.core.utils.EventQueue
import chat.pcp.app.core.utils.SharedEvents

abstract class PcpViewModel<S : MavericksState, VA : PcpViewModelAction, VE : PcpViewEvents>(initialState: S) :
        MavericksViewModel<S>(initialState) {

    // Used to post transient events to the View
    protected val _viewEvents = EventQueue<VE>(capacity = 64)
    val viewEvents: SharedEvents<VE>
        get() = _viewEvents

    abstract fun handle(action: VA)
}
