/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.configuration

import chat.pcp.app.features.voicebroadcast.VoiceBroadcastConstants
import org.matrix.android.sdk.api.provider.CustomEventTypesProvider
import javax.inject.Inject

class PcpEventTypes @Inject constructor() : CustomEventTypesProvider {

    override val customPreviewableEventTypes = listOf(
            VoiceBroadcastConstants.STATE_ROOM_VOICE_BROADCAST_INFO
    )
}
