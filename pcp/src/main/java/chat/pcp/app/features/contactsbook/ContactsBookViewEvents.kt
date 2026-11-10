/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.contactsbook

import chat.pcp.app.core.platform.PcpViewEvents
import chat.pcp.app.features.discovery.ServerAndPolicies

sealed class ContactsBookViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : ContactsBookViewEvents()
    data class OnPoliciesRetrieved(val identityServerWithTerms: ServerAndPolicies?) : ContactsBookViewEvents()
}
