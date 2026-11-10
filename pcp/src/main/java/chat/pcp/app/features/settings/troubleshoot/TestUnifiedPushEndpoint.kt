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

class TestUnifiedPushEndpoint @Inject constructor(
        private val stringProvider: StringProvider,
        private val unifiedPushHelper: UnifiedPushHelper,
) : TroubleshootTest(CommonStrings.settings_troubleshoot_test_current_endpoint_title) {

    override fun perform(testParameters: TestParameters) {
        val endpoint = unifiedPushHelper.getPrivacyFriendlyUpEndpoint()
        if (endpoint != null) {
            description = stringProvider.getString(CommonStrings.settings_troubleshoot_test_current_endpoint_success, endpoint)
            status = TestStatus.SUCCESS
        } else {
            description = stringProvider.getString(CommonStrings.settings_troubleshoot_test_current_endpoint_failed)
            status = TestStatus.FAILED
        }
    }
}
