/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.login.terms

import org.matrix.android.sdk.api.auth.data.LocalizedFlowDataLoginTerms

data class LocalizedFlowDataLoginTermsChecked(
        val localizedFlowDataLoginTerms: LocalizedFlowDataLoginTerms,
        var checked: Boolean = false
)
