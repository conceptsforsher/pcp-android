/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import chat.pcp.app.test.fixtures.aDummyStage
import chat.pcp.app.test.fixtures.aMsisdnStage
import chat.pcp.app.test.fixtures.aRecaptchaStage
import chat.pcp.app.test.fixtures.aTermsStage
import chat.pcp.app.test.fixtures.anEmailStage
import chat.pcp.app.test.fixtures.anOtherStage
import org.amshove.kluent.shouldBeEqualTo
import org.junit.Test

class MatrixOrgRegistrationStagesComparatorTest {

    @Test
    fun `when ordering stages, then prioritizes email`() {
        val input = listOf(
                aDummyStage(),
                anOtherStage(),
                aMsisdnStage(),
                anEmailStage(),
                aRecaptchaStage(),
                aTermsStage()
        )

        val result = input.sortedWith(MatrixOrgRegistrationStagesComparator())

        result shouldBeEqualTo listOf(
                anEmailStage(),
                aMsisdnStage(),
                aTermsStage(),
                aRecaptchaStage(),
                anOtherStage(),
                aDummyStage()
        )
    }
}
