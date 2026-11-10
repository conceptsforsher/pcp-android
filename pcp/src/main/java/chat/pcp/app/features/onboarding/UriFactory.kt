/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding

import android.net.Uri
import javax.inject.Inject

class UriFactory @Inject constructor() {

    fun parse(value: String): Uri {
        return Uri.parse(value)
    }
}
