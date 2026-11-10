/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding

object RegistrationStateFixture {

    fun aRegistrationState(
            email: String? = null,
            isUserNameAvailable: Boolean = false,
            selectedMatrixId: String? = null,
    ) = RegistrationState(email, isUserNameAvailable, selectedMatrixId)
}
