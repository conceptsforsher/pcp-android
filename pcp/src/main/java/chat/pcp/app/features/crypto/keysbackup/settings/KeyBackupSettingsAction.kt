/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.keysbackup.settings

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class KeyBackupSettingsAction : PcpViewModelAction {
    object Init : KeyBackupSettingsAction()
    object GetKeyBackupTrust : KeyBackupSettingsAction()
    object DeleteKeyBackup : KeyBackupSettingsAction()
    object SetUpKeyBackup : KeyBackupSettingsAction()
    data class StoreIn4SSuccess(val recoveryKey: String, val alias: String) : KeyBackupSettingsAction()
    object StoreIn4SReset : KeyBackupSettingsAction()
    object StoreIn4SFailure : KeyBackupSettingsAction()
}
