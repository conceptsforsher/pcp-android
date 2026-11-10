/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.contactsbook

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class ContactsBookAction : PcpViewModelAction {
    data class FilterWith(val filter: String) : ContactsBookAction()
    data class OnlyBoundContacts(val onlyBoundContacts: Boolean) : ContactsBookAction()
    object UserConsentRequest : ContactsBookAction()
    object UserConsentGranted : ContactsBookAction()
}
