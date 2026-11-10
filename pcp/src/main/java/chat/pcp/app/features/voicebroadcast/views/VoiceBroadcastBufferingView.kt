/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.voicebroadcast.views

import android.content.Context
import android.util.AttributeSet
import android.view.LayoutInflater
import android.widget.LinearLayout
import chat.pcp.app.databinding.ViewVoiceBroadcastBufferingBinding

class VoiceBroadcastBufferingView @JvmOverloads constructor(
        context: Context,
        attrs: AttributeSet? = null,
        defStyleAttr: Int = 0
) : LinearLayout(context, attrs, defStyleAttr) {

    init {
        ViewVoiceBroadcastBufferingBinding.inflate(
                LayoutInflater.from(context),
                this
        )
    }
}
