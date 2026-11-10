/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call.conference

import chat.pcp.app.core.platform.PcpViewEvents
import org.jitsi.meet.sdk.JitsiMeetUserInfo

sealed class JitsiCallViewEvents : PcpViewEvents {
    data class JoinConference(
            val enableVideo: Boolean,
            val jitsiUrl: String,
            val subject: String,
            val confId: String,
            val userInfo: JitsiMeetUserInfo,
            val token: String?
    ) : JitsiCallViewEvents()

    data class ConfirmSwitchingConference(
            val args: PcpJitsiActivity.Args
    ) : JitsiCallViewEvents()

    object LeaveConference : JitsiCallViewEvents()
    object FailJoiningConference : JitsiCallViewEvents()
    object Finish : JitsiCallViewEvents()
}
