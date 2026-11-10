/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.animations

import androidx.transition.Transition

open class SimpleTransitionListener : Transition.TransitionListener {
    override fun onTransitionEnd(transition: Transition) {
        // No op
    }

    override fun onTransitionResume(transition: Transition) {
        // No op
    }

    override fun onTransitionPause(transition: Transition) {
        // No op
    }

    override fun onTransitionCancel(transition: Transition) {
        // No op
    }

    override fun onTransitionStart(transition: Transition) {
        // No op
    }
}
