/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.helper

import com.airbnb.epoxy.VisibilityState
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.features.home.room.detail.timeline.TimelineEventController
import org.matrix.android.sdk.api.session.room.timeline.TimelineEvent

class ReadMarkerVisibilityStateChangedListener(private val callback: TimelineEventController.Callback?) :
        PcpEpoxyModel.OnVisibilityStateChangedListener {

    override fun onVisibilityStateChanged(visibilityState: Int) {
        if (visibilityState == VisibilityState.VISIBLE) {
            callback?.onReadMarkerVisible()
        }
    }
}

class TimelineEventVisibilityStateChangedListener(
        private val callback: TimelineEventController.Callback?,
        private val event: TimelineEvent
) :
        PcpEpoxyModel.OnVisibilityStateChangedListener {

    override fun onVisibilityStateChanged(visibilityState: Int) {
        if (visibilityState == VisibilityState.VISIBLE) {
            callback?.onEventVisible(event)
        } else if (visibilityState == VisibilityState.INVISIBLE) {
            callback?.onEventInvisible(event)
        }
    }
}

class MergedTimelineEventVisibilityStateChangedListener(
        private val callback: TimelineEventController.Callback?,
        private val events: List<TimelineEvent>
) :
        PcpEpoxyModel.OnVisibilityStateChangedListener {

    override fun onVisibilityStateChanged(visibilityState: Int) {
        if (visibilityState == VisibilityState.VISIBLE) {
            events.forEach { callback?.onEventVisible(it) }
        } else if (visibilityState == VisibilityState.INVISIBLE) {
            events.forEach { callback?.onEventInvisible(it) }
        }
    }
}
