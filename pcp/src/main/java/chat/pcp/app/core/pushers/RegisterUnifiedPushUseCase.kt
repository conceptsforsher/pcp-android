/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.pushers

import android.content.Context
import dagger.hilt.android.qualifiers.ApplicationContext
import chat.pcp.app.features.PcpFeatures
import org.unifiedpush.android.connector.UnifiedPush
import javax.inject.Inject

class RegisterUnifiedPushUseCase @Inject constructor(
        @ApplicationContext private val context: Context,
        private val vectorFeatures: PcpFeatures,
) {

    sealed interface RegisterUnifiedPushResult {
        object Success : RegisterUnifiedPushResult
        object NeedToAskUserForDistributor : RegisterUnifiedPushResult
    }

    fun execute(distributor: String = ""): RegisterUnifiedPushResult {
        if (distributor.isNotEmpty()) {
            saveAndRegisterApp(distributor)
            return RegisterUnifiedPushResult.Success
        }

        if (!vectorFeatures.allowExternalUnifiedPushDistributors()) {
            saveAndRegisterApp(context.packageName)
            return RegisterUnifiedPushResult.Success
        }

        if (UnifiedPush.getDistributor(context).isNotEmpty()) {
            registerApp()
            return RegisterUnifiedPushResult.Success
        }

        val distributors = UnifiedPush.getDistributors(context)

        return if (distributors.size == 1) {
            saveAndRegisterApp(distributors.first())
            RegisterUnifiedPushResult.Success
        } else {
            RegisterUnifiedPushResult.NeedToAskUserForDistributor
        }
    }

    private fun saveAndRegisterApp(distributor: String) {
        UnifiedPush.saveDistributor(context, distributor)
        registerApp()
    }

    private fun registerApp() {
        UnifiedPush.registerApp(context)
    }
}
