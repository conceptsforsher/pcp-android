/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.push.fcm

import androidx.fragment.app.Fragment
import chat.pcp.app.core.pushers.UnifiedPushHelper
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.push.NotificationTroubleshootTestManagerFactory
import chat.pcp.app.features.settings.troubleshoot.NotificationTroubleshootTestManager
import chat.pcp.app.features.settings.troubleshoot.TestAccountSettings
import chat.pcp.app.features.settings.troubleshoot.TestAvailableUnifiedPushDistributors
import chat.pcp.app.features.settings.troubleshoot.TestCurrentUnifiedPushDistributor
import chat.pcp.app.features.settings.troubleshoot.TestDeviceSettings
import chat.pcp.app.features.settings.troubleshoot.TestEndpointAsTokenRegistration
import chat.pcp.app.features.settings.troubleshoot.TestNotification
import chat.pcp.app.features.settings.troubleshoot.TestPushFromPushGateway
import chat.pcp.app.features.settings.troubleshoot.TestPushRulesSettings
import chat.pcp.app.features.settings.troubleshoot.TestSystemSettings
import chat.pcp.app.features.settings.troubleshoot.TestUnifiedPushEndpoint
import chat.pcp.app.features.settings.troubleshoot.TestUnifiedPushGateway
import chat.pcp.app.gplay.features.settings.troubleshoot.TestFirebaseToken
import chat.pcp.app.gplay.features.settings.troubleshoot.TestPlayServices
import chat.pcp.app.gplay.features.settings.troubleshoot.TestTokenRegistration
import javax.inject.Inject

class GoogleNotificationTroubleshootTestManagerFactory @Inject constructor(
        private val unifiedPushHelper: UnifiedPushHelper,
        private val testSystemSettings: TestSystemSettings,
        private val testAccountSettings: TestAccountSettings,
        private val testDeviceSettings: TestDeviceSettings,
        private val testPushRulesSettings: TestPushRulesSettings,
        private val testPlayServices: TestPlayServices,
        private val testFirebaseToken: TestFirebaseToken,
        private val testTokenRegistration: TestTokenRegistration,
        private val testCurrentUnifiedPushDistributor: TestCurrentUnifiedPushDistributor,
        private val testUnifiedPushGateway: TestUnifiedPushGateway,
        private val testUnifiedPushEndpoint: TestUnifiedPushEndpoint,
        private val testAvailableUnifiedPushDistributors: TestAvailableUnifiedPushDistributors,
        private val testEndpointAsTokenRegistration: TestEndpointAsTokenRegistration,
        private val testPushFromPushGateway: TestPushFromPushGateway,
        private val testNotification: TestNotification,
        private val vectorFeatures: PcpFeatures,
) : NotificationTroubleshootTestManagerFactory {

    override fun create(fragment: Fragment): NotificationTroubleshootTestManager {
        val mgr = NotificationTroubleshootTestManager(fragment)
        mgr.addTest(testSystemSettings)
        mgr.addTest(testAccountSettings)
        mgr.addTest(testDeviceSettings)
        mgr.addTest(testPushRulesSettings)
        if (vectorFeatures.allowExternalUnifiedPushDistributors()) {
            mgr.addTest(testAvailableUnifiedPushDistributors)
            mgr.addTest(testCurrentUnifiedPushDistributor)
        }
        if (unifiedPushHelper.isEmbeddedDistributor()) {
            mgr.addTest(testPlayServices)
            mgr.addTest(testFirebaseToken)
            mgr.addTest(testTokenRegistration)
        } else {
            mgr.addTest(testUnifiedPushGateway)
            mgr.addTest(testUnifiedPushEndpoint)
            mgr.addTest(testEndpointAsTokenRegistration)
        }
        mgr.addTest(testPushFromPushGateway)
        mgr.addTest(testNotification)
        return mgr
    }
}
