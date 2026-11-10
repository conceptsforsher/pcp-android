/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call.conference

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.Uninitialized
import org.matrix.android.sdk.api.session.widgets.model.Widget

data class JitsiCallViewState(
        val roomId: String = "",
        val widgetId: String = "",
        val enableVideo: Boolean = false,
        val widget: Async<Widget> = Uninitialized
) : MavericksState {

    constructor(args: PcpJitsiActivity.Args) : this(
            roomId = args.roomId,
            widgetId = args.widgetId,
            enableVideo = args.enableVideo
    )
}
