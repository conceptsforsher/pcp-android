/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.troubleshoot

import chat.pcp.app.core.pushers.UnifiedPushHelper
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class TestCurrentUnifiedPushDistributor @Inject constructor(
        private val unifiedPushHelper: UnifiedPushHelper,
        private val stringProvider: StringProvider,
) : TroubleshootTest(CommonStrings.settings_troubleshoot_test_current_distributor_title) {

    override fun perform(testParameters: TestParameters) {
        description = stringProvider.getString(
                CommonStrings.settings_troubleshoot_test_current_distributor,
                unifiedPushHelper.getCurrentDistributorName()
        )
        status = TestStatus.SUCCESS
    }
}
