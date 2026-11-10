/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.discovery.change

import androidx.annotation.StringRes
import chat.pcp.app.core.platform.PcpViewEvents

sealed class SetIdentityServerViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : SetIdentityServerViewEvents()
    data class Failure(@StringRes val errorMessageId: Int, val forDefault: Boolean) : SetIdentityServerViewEvents()
    data class OtherFailure(val failure: Throwable) : SetIdentityServerViewEvents()

    data class ShowTerms(val identityServerUrl: String) : SetIdentityServerViewEvents()

    object NoTerms : SetIdentityServerViewEvents()
    object TermsAccepted : SetIdentityServerViewEvents()
}
