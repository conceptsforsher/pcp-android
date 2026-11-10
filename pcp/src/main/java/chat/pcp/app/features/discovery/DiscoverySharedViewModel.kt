/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.discovery

import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import chat.pcp.app.core.extensions.postLiveEvent
import chat.pcp.app.core.utils.LiveEvent
import javax.inject.Inject

class DiscoverySharedViewModel @Inject constructor() : ViewModel() {
    var navigateEvent = MutableLiveData<LiveEvent<DiscoverySharedViewModelAction>>()

    fun requestChangeToIdentityServer(serverUrl: String) {
        navigateEvent.postLiveEvent(DiscoverySharedViewModelAction.ChangeIdentityServer(serverUrl))
    }
}
