/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.core.resources.StringProvider
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.sync.InitialSyncStep
import javax.inject.Inject

class InitSyncStepFormatter @Inject constructor(
        private val stringProvider: StringProvider
) {
    fun format(initialSyncStep: InitialSyncStep): String {
        return stringProvider.getString(
                when (initialSyncStep) {
                    InitialSyncStep.ServerComputing -> CommonStrings.initial_sync_start_server_computing
                    InitialSyncStep.Downloading -> CommonStrings.initial_sync_start_downloading
                    InitialSyncStep.ImportingAccount -> CommonStrings.initial_sync_start_importing_account
                    InitialSyncStep.ImportingAccountCrypto -> CommonStrings.initial_sync_start_importing_account_crypto
                    InitialSyncStep.ImportingAccountRoom -> CommonStrings.initial_sync_start_importing_account_rooms
                    InitialSyncStep.ImportingAccountData -> CommonStrings.initial_sync_start_importing_account_data
                    InitialSyncStep.ImportingAccountJoinedRooms -> CommonStrings.initial_sync_start_importing_account_joined_rooms
                    InitialSyncStep.ImportingAccountInvitedRooms -> CommonStrings.initial_sync_start_importing_account_invited_rooms
                    InitialSyncStep.ImportingAccountLeftRooms -> CommonStrings.initial_sync_start_importing_account_left_rooms
                }
        )
    }
}
