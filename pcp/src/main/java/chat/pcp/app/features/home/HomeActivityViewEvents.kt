/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.util.MatrixItem

sealed interface HomeActivityViewEvents : PcpViewEvents {
    data class AskPasswordToInitCrossSigning(val userItem: MatrixItem.UserItem) : HomeActivityViewEvents
    data class CurrentSessionNotVerified(
            val userItem: MatrixItem.UserItem,
            val afterMigration: Boolean
    ) : HomeActivityViewEvents

    data class CurrentSessionCannotBeVerified(
            val userItem: MatrixItem.UserItem,
    ) : HomeActivityViewEvents

    data class OnCrossSignedInvalidated(val userItem: MatrixItem.UserItem) : HomeActivityViewEvents
    object PromptToEnableSessionPush : HomeActivityViewEvents
    object ShowAnalyticsOptIn : HomeActivityViewEvents
    object ShowNotificationDialog : HomeActivityViewEvents
    object ShowReleaseNotes : HomeActivityViewEvents
    object NotifyUserForThreadsMigration : HomeActivityViewEvents
    data class MigrateThreads(val checkSession: Boolean) : HomeActivityViewEvents
    object StartRecoverySetupFlow : HomeActivityViewEvents
    data class ForceVerification(val sendRequest: Boolean) : HomeActivityViewEvents
    object AskUserForPushDistributor : HomeActivityViewEvents
}
