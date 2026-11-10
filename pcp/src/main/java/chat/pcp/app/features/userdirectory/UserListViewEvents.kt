/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.userdirectory

import chat.pcp.app.core.platform.PcpViewEvents
import chat.pcp.app.features.discovery.ServerAndPolicies

/**
 * Transient events for invite users to room screen.
 */
sealed class UserListViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : UserListViewEvents()
    data class OnPoliciesRetrieved(val identityServerWithTerms: ServerAndPolicies?) : UserListViewEvents()
    data class OpenShareMatrixToLink(val link: String) : UserListViewEvents()
}
