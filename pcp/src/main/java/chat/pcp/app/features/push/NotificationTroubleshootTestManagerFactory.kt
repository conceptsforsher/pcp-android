/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.push

import androidx.fragment.app.Fragment
import chat.pcp.app.features.settings.troubleshoot.NotificationTroubleshootTestManager

interface NotificationTroubleshootTestManagerFactory {
    fun create(fragment: Fragment): NotificationTroubleshootTestManager
}
