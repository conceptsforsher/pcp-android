/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.animations

import android.content.Context
import android.util.AttributeSet
import androidx.transition.ChangeBounds
import androidx.transition.ChangeTransform
import androidx.transition.Fade
import androidx.transition.TransitionSet

class PcpTransitionSet : TransitionSet {

    constructor() {
        init()
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        init()
    }

    private fun init() {
        ordering = ORDERING_TOGETHER
        addTransition(Fade(Fade.OUT))
                .addTransition(ChangeBounds())
                .addTransition(ChangeTransform())
                .addTransition(Fade(Fade.IN))
    }
}
