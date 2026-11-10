/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.font

import chat.pcp.app.core.platform.PcpViewModelAction
import chat.pcp.app.features.settings.FontScaleValue

sealed class FontScaleSettingAction : PcpViewModelAction {
    data class UseSystemSettingChangedAction(val useSystemSettings: Boolean) : FontScaleSettingAction()
    data class FontScaleChangedAction(val fontScale: FontScaleValue) : FontScaleSettingAction()
}
