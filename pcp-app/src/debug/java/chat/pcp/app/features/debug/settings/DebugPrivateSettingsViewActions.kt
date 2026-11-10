/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.debug.settings

import chat.pcp.app.core.platform.PcpViewModelAction

sealed interface DebugPrivateSettingsViewActions : PcpViewModelAction {
    data class SetDialPadVisibility(val force: Boolean) : DebugPrivateSettingsViewActions
    data class SetForceLoginFallbackEnabled(val force: Boolean) : DebugPrivateSettingsViewActions
    data class SetDisplayNameCapabilityOverride(val option: BooleanHomeserverCapabilitiesOverride?) : DebugPrivateSettingsViewActions
    data class SetAvatarCapabilityOverride(val option: BooleanHomeserverCapabilitiesOverride?) : DebugPrivateSettingsViewActions
    object ResetReleaseNotesActivityHasBeenDisplayed : DebugPrivateSettingsViewActions
}
