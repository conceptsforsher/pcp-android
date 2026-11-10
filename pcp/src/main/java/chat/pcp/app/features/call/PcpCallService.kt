/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call

import chat.pcp.app.features.call.lookup.CallProtocolsChecker
import chat.pcp.app.features.call.lookup.CallUserMapper
import chat.pcp.app.features.session.SessionScopedProperty
import org.matrix.android.sdk.api.session.Session

interface PcpCallService {
    val protocolChecker: CallProtocolsChecker
    val userMapper: CallUserMapper
}

val Session.vectorCallService: PcpCallService by SessionScopedProperty {
    object : PcpCallService {
        override val protocolChecker = CallProtocolsChecker(it)
        override val userMapper = CallUserMapper(it, protocolChecker)
    }
}
