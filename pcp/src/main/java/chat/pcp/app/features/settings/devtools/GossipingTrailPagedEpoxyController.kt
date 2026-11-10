/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devtools

import com.airbnb.epoxy.EpoxyModel
import com.airbnb.epoxy.paging.PagedListEpoxyController
import chat.pcp.app.core.date.DateFormatKind
import chat.pcp.app.core.date.PcpDateFormatter
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.core.ui.list.GenericItem_
import chat.pcp.app.core.utils.createUIHandler
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import me.gujun.android.span.span
import org.matrix.android.sdk.api.session.crypto.model.AuditTrail
import org.matrix.android.sdk.api.session.crypto.model.ForwardInfo
import org.matrix.android.sdk.api.session.crypto.model.TrailType
import org.matrix.android.sdk.api.session.crypto.model.WithheldInfo
import javax.inject.Inject

class GossipingTrailPagedEpoxyController @Inject constructor(
        private val vectorDateFormatter: PcpDateFormatter,
        private val colorProvider: ColorProvider
) : PagedListEpoxyController<AuditTrail>(
        // Important it must match the PageList builder notify Looper
        modelBuildingHandler = createUIHandler()
) {

    interface InteractionListener {
        fun didTap(event: AuditTrail)
    }

    var interactionListener: InteractionListener? = null

    override fun buildItemModel(currentPosition: Int, item: AuditTrail?): EpoxyModel<*> {
        val host = this
        val event = item ?: return GenericItem_().apply { id(currentPosition) }
        return GenericItem_().apply {
            id(event.hashCode())
            itemClickAction { host.interactionListener?.didTap(event) }
            title(event.type.name.toEpoxyCharSequence())
            description(
                    span {
                        +host.vectorDateFormatter.format(event.ageLocalTs, DateFormatKind.DEFAULT_DATE_AND_TIME)
                        span("\n${host.senderFromTo(event.type)}: ") {
                            textStyle = "bold"
                        }
                        +"${event.info.userId}|${event.info.deviceId}"
                        span("\nroomId: ") {
                            textStyle = "bold"
                        }
                        +event.info.roomId
                        span("\nsessionId: ") {
                            textStyle = "bold"
                        }
                        +event.info.sessionId
                        apply {
                            when (event.type) {
                                TrailType.OutgoingKeyForward -> {
                                    val fInfo = event.info as ForwardInfo
                                    span("\nchainIndex: ") {
                                        textStyle = "bold"
                                    }
                                    +"${fInfo.chainIndex}"
                                }
                                TrailType.OutgoingKeyWithheld -> {
                                    val fInfo = event.info as WithheldInfo
                                    span("\ncode: ") {
                                        textStyle = "bold"
                                    }
                                    +"${fInfo.code}"
                                }
                                TrailType.IncomingKeyRequest -> {
                                    // no additional info
                                }
                                TrailType.IncomingKeyForward -> {
                                    val fInfo = event.info as ForwardInfo
                                    span("\nchainIndex: ") {
                                        textStyle = "bold"
                                    }
                                    +"${fInfo.chainIndex}"
                                }
                                TrailType.Unknown -> {
                                }
                            }
                        }
                    }.toEpoxyCharSequence()
            )
        }
    }

    private fun senderFromTo(type: TrailType): String {
        return when (type) {
            TrailType.OutgoingKeyWithheld,
            TrailType.OutgoingKeyForward -> "to"
            TrailType.IncomingKeyRequest,
            TrailType.IncomingKeyForward -> "from"
            TrailType.Unknown -> ""
        }
    }
}
