/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location.live.map

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class LiveLocationMapAction : PcpViewModelAction {
    data class AddMapSymbol(val key: String, val value: Long) : LiveLocationMapAction()
    data class RemoveMapSymbol(val key: String) : LiveLocationMapAction()
    object StopSharing : LiveLocationMapAction()
    object ShowMapLoadingError : LiveLocationMapAction()
    object ZoomToUserLocation : LiveLocationMapAction()
}
