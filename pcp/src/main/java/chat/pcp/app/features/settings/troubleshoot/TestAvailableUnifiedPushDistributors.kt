/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.troubleshoot

import chat.pcp.app.core.pushers.FcmHelper
import chat.pcp.app.core.pushers.UnifiedPushHelper
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.lib.strings.CommonPlurals
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class TestAvailableUnifiedPushDistributors @Inject constructor(
        private val unifiedPushHelper: UnifiedPushHelper,
        private val stringProvider: StringProvider,
        private val fcmHelper: FcmHelper,
) : TroubleshootTest(CommonStrings.settings_troubleshoot_test_distributors_title) {

    override fun perform(testParameters: TestParameters) {
        val distributors = unifiedPushHelper.getExternalDistributors()
        description = if (distributors.isEmpty()) {
            stringProvider.getString(
                    if (fcmHelper.isFirebaseAvailable()) {
                        CommonStrings.settings_troubleshoot_test_distributors_gplay
                    } else {
                        CommonStrings.settings_troubleshoot_test_distributors_fdroid
                    }
            )
        } else {
            val quantity = distributors.size + 1
            stringProvider.getQuantityString(CommonPlurals.settings_troubleshoot_test_distributors_many, quantity, quantity)
        }
        status = TestStatus.SUCCESS
    }
}
