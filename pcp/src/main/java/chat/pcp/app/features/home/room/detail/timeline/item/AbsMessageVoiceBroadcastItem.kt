/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.item

import android.widget.ImageView
import android.widget.TextView
import androidx.annotation.IdRes
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import chat.pcp.app.R
import chat.pcp.app.core.error.ErrorFormatter
import chat.pcp.app.core.extensions.tintBackground
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.core.resources.DrawableProvider
import chat.pcp.app.features.home.room.detail.timeline.helper.AudioMessagePlaybackTracker
import chat.pcp.app.features.voicebroadcast.listening.VoiceBroadcastPlayer
import chat.pcp.app.features.voicebroadcast.model.VoiceBroadcast
import chat.pcp.app.features.voicebroadcast.model.VoiceBroadcastState
import chat.pcp.app.features.voicebroadcast.recording.VoiceBroadcastRecorder
import org.matrix.android.sdk.api.util.MatrixItem

abstract class AbsMessageVoiceBroadcastItem<H : AbsMessageVoiceBroadcastItem.Holder> : AbsMessageItem<H>() {

    @EpoxyAttribute
    lateinit var voiceBroadcastAttributes: Attributes

    protected val voiceBroadcast get() = voiceBroadcastAttributes.voiceBroadcast
    protected val voiceBroadcastState get() = voiceBroadcastAttributes.voiceBroadcastState
    protected val recorderName get() = voiceBroadcastAttributes.recorderName
    protected val recorder get() = voiceBroadcastAttributes.recorder
    protected val player get() = voiceBroadcastAttributes.player
    protected val playbackTracker get() = voiceBroadcastAttributes.playbackTracker
    protected val duration get() = voiceBroadcastAttributes.duration
    protected val hasUnableToDecryptEvent get() = voiceBroadcastAttributes.hasUnableToDecryptEvent
    protected val roomItem get() = voiceBroadcastAttributes.roomItem
    protected val colorProvider get() = voiceBroadcastAttributes.colorProvider
    protected val drawableProvider get() = voiceBroadcastAttributes.drawableProvider
    protected val avatarRenderer get() = attributes.avatarRenderer
    protected val errorFormatter get() = voiceBroadcastAttributes.errorFormatter
    protected val callback get() = attributes.callback

    override fun isCacheable(): Boolean = false

    override fun bind(holder: H) {
        super.bind(holder)
        renderHeader(holder)
    }

    private fun renderHeader(holder: H) {
        with(holder) {
            roomItem?.let {
                avatarRenderer.render(it, roomAvatarImageView)
                titleText.text = it.displayName
            }
        }
        renderLiveIndicator(holder)
        renderMetadata(holder)
    }

    abstract fun renderLiveIndicator(holder: H)

    protected fun renderPlayingLiveIndicator(holder: H) {
        with(holder) {
            liveIndicator.tintBackground(colorProvider.getColorFromAttribute(com.google.android.material.R.attr.colorError))
            liveIndicator.isVisible = true
        }
    }

    protected fun renderPausedLiveIndicator(holder: H) {
        with(holder) {
            liveIndicator.tintBackground(colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_quaternary))
            liveIndicator.isVisible = true
        }
    }

    protected fun renderNoLiveIndicator(holder: H) {
        holder.liveIndicator.isVisible = false
    }

    abstract fun renderMetadata(holder: H)

    abstract class Holder(@IdRes stubId: Int) : AbsMessageItem.Holder(stubId) {
        val liveIndicator by bind<TextView>(R.id.liveIndicator)
        val roomAvatarImageView by bind<ImageView>(R.id.roomAvatarImageView)
        val titleText by bind<TextView>(R.id.titleText)
    }

    data class Attributes(
            val voiceBroadcast: VoiceBroadcast,
            val voiceBroadcastState: VoiceBroadcastState?,
            val duration: Int,
            val hasUnableToDecryptEvent: Boolean,
            val recorderName: String,
            val recorder: VoiceBroadcastRecorder?,
            val player: VoiceBroadcastPlayer,
            val playbackTracker: AudioMessagePlaybackTracker,
            val roomItem: MatrixItem?,
            val colorProvider: ColorProvider,
            val drawableProvider: DrawableProvider,
            val errorFormatter: ErrorFormatter,
    )
}
