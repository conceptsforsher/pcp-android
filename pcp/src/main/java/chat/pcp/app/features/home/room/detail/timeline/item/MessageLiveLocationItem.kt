/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.item

import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.date.DateFormatKind
import chat.pcp.app.core.date.PcpDateFormatter
import chat.pcp.app.core.resources.toTimestamp
import chat.pcp.app.core.utils.DimensionConverter
import chat.pcp.app.features.home.room.detail.RoomDetailAction
import chat.pcp.app.features.home.room.detail.timeline.style.TimelineMessageLayout
import chat.pcp.app.features.location.live.LiveLocationMessageBannerViewState
import chat.pcp.app.features.location.live.LiveLocationRunningBannerView
import org.threeten.bp.LocalDateTime

@EpoxyModelClass
abstract class MessageLiveLocationItem : AbsMessageLocationItem<MessageLiveLocationItem.Holder>() {

    @EpoxyAttribute
    var currentUserId: String? = null

    @EpoxyAttribute
    var endOfLiveDateTime: LocalDateTime? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    lateinit var vectorDateFormatter: PcpDateFormatter

    override fun bind(holder: Holder) {
        super.bind(holder)
        bindLiveLocationBanner(holder)
    }

    private fun bindLiveLocationBanner(holder: Holder) {
        // TODO in a future PR add check on device id to confirm that is the one that sent the beacon
        val isEmitter = currentUserId != null && currentUserId == pinMatrixItem?.id
        val messageLayout = attributes.informationData.messageLayout
        val viewState = buildViewState(holder, messageLayout, isEmitter)
        holder.liveLocationRunningBanner.isVisible = true
        holder.liveLocationRunningBanner.render(viewState)
        holder.liveLocationRunningBanner.stopButton.setOnClickListener {
            attributes.callback?.onTimelineItemAction(RoomDetailAction.StopLiveLocationSharing)
        }
    }

    private fun buildViewState(
            holder: Holder,
            messageLayout: TimelineMessageLayout,
            isEmitter: Boolean
    ): LiveLocationMessageBannerViewState {
        return when {
            messageLayout is TimelineMessageLayout.Bubble && isEmitter ->
                LiveLocationMessageBannerViewState.Emitter(
                        remainingTimeInMillis = getRemainingTimeOfLiveInMillis(),
                        bottomStartCornerRadiusInDp = messageLayout.cornersRadius.bottomStartRadius,
                        bottomEndCornerRadiusInDp = messageLayout.cornersRadius.bottomEndRadius,
                        isStopButtonCenteredVertically = false
                )
            messageLayout is TimelineMessageLayout.Bubble ->
                LiveLocationMessageBannerViewState.Watcher(
                        bottomStartCornerRadiusInDp = messageLayout.cornersRadius.bottomStartRadius,
                        bottomEndCornerRadiusInDp = messageLayout.cornersRadius.bottomEndRadius,
                        formattedLocalTimeOfEndOfLive = getFormattedLocalTimeEndOfLive(),
                )
            isEmitter -> {
                val cornerRadius = getBannerCornerRadiusForDefaultLayout(holder)
                LiveLocationMessageBannerViewState.Emitter(
                        remainingTimeInMillis = getRemainingTimeOfLiveInMillis(),
                        bottomStartCornerRadiusInDp = cornerRadius,
                        bottomEndCornerRadiusInDp = cornerRadius,
                        isStopButtonCenteredVertically = true
                )
            }
            else -> {
                val cornerRadius = getBannerCornerRadiusForDefaultLayout(holder)
                LiveLocationMessageBannerViewState.Watcher(
                        bottomStartCornerRadiusInDp = cornerRadius,
                        bottomEndCornerRadiusInDp = cornerRadius,
                        formattedLocalTimeOfEndOfLive = getFormattedLocalTimeEndOfLive(),
                )
            }
        }
    }

    private fun getBannerCornerRadiusForDefaultLayout(holder: Holder): Float {
        val dimensionConverter = DimensionConverter(holder.view.resources)
        return dimensionConverter.dpToPx(8).toFloat()
    }

    private fun getFormattedLocalTimeEndOfLive() =
            endOfLiveDateTime?.toTimestamp()?.let { vectorDateFormatter.format(it, DateFormatKind.MESSAGE_SIMPLE) }.orEmpty()

    private fun getRemainingTimeOfLiveInMillis() =
            (endOfLiveDateTime?.toTimestamp() ?: 0) - LocalDateTime.now().toTimestamp()

    override fun getViewStubId() = STUB_ID

    class Holder : AbsMessageLocationItem.Holder(STUB_ID) {
        val liveLocationRunningBanner by bind<LiveLocationRunningBannerView>(R.id.liveLocationRunningBanner)
    }

    companion object {
        private val STUB_ID = R.id.messageContentLiveLocationStub
    }
}
