/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.home.room.detail.timeline.action

import com.airbnb.epoxy.TypedEpoxyController
import com.airbnb.mvrx.Success
import chat.pcp.app.EmojiCompatFontProvider
import chat.pcp.app.R
import chat.pcp.app.core.date.DateFormatKind
import chat.pcp.app.core.date.PcpDateFormatter
import chat.pcp.app.core.epoxy.bottomSheetDividerItem
import chat.pcp.app.core.epoxy.bottomsheet.BottomSheetQuickReactionsItem
import chat.pcp.app.core.epoxy.bottomsheet.bottomSheetActionItem
import chat.pcp.app.core.epoxy.bottomsheet.bottomSheetMessagePreviewItem
import chat.pcp.app.core.epoxy.bottomsheet.bottomSheetQuickReactionsItem
import chat.pcp.app.core.epoxy.bottomsheet.bottomSheetSendStateItem
import chat.pcp.app.core.error.ErrorFormatter
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.utils.DimensionConverter
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.home.room.detail.timeline.TimelineEventController
import chat.pcp.app.features.home.room.detail.timeline.format.EventDetailsFormatter
import chat.pcp.app.features.home.room.detail.timeline.helper.LocationPinProvider
import chat.pcp.app.features.home.room.detail.timeline.image.buildImageContentRendererData
import chat.pcp.app.features.home.room.detail.timeline.item.E2EDecoration
import chat.pcp.app.features.home.room.detail.timeline.tools.createLinkMovementMethod
import chat.pcp.app.features.home.room.detail.timeline.tools.linkify
import chat.pcp.app.features.html.SpanUtils
import chat.pcp.app.features.location.INITIAL_MAP_ZOOM_IN_TIMELINE
import chat.pcp.app.features.location.UrlMapProvider
import chat.pcp.app.features.location.toLocationData
import chat.pcp.app.features.media.ImageContentRenderer
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.extensions.orFalse
import org.matrix.android.sdk.api.failure.Failure
import org.matrix.android.sdk.api.session.events.model.isLocationMessage
import org.matrix.android.sdk.api.session.events.model.toModel
import org.matrix.android.sdk.api.session.room.model.message.MessageLocationContent
import org.matrix.android.sdk.api.session.room.send.SendState
import javax.inject.Inject

/**
 * Epoxy controller for message action list.
 */
class MessageActionsEpoxyController @Inject constructor(
        private val stringProvider: StringProvider,
        private val avatarRenderer: AvatarRenderer,
        private val fontProvider: EmojiCompatFontProvider,
        private val imageContentRenderer: ImageContentRenderer,
        private val dimensionConverter: DimensionConverter,
        private val errorFormatter: ErrorFormatter,
        private val spanUtils: SpanUtils,
        private val eventDetailsFormatter: EventDetailsFormatter,
        private val pcpPreferences: PcpBasePreferences,
        private val dateFormatter: PcpDateFormatter,
        private val urlMapProvider: UrlMapProvider,
        private val locationPinProvider: LocationPinProvider
) : TypedEpoxyController<MessageActionState>() {

    var listener: MessageActionsEpoxyControllerListener? = null

    override fun buildModels(state: MessageActionState) {
        val host = this
        // Message preview
        val date = state.timelineEvent()?.root?.originServerTs
        val formattedDate = dateFormatter.format(date, DateFormatKind.MESSAGE_DETAIL)
        val body = state.messageBody.linkify(host.listener)
        val bindingOptions = spanUtils.getBindingOptions(body)
        val locationUiData = buildLocationUiData(state)

        bottomSheetMessagePreviewItem {
            id("preview")
            avatarRenderer(host.avatarRenderer)
            matrixItem(state.informationData.matrixItem)
            movementMethod(createLinkMovementMethod(host.listener))
            imageContentRenderer(host.imageContentRenderer)
            data(state.timelineEvent()?.buildImageContentRendererData(host.dimensionConverter.dpToPx(66)))
            userClicked { host.listener?.didSelectMenuAction(EventSharedAction.OpenUserProfile(state.informationData.senderId)) }
            bindingOptions(bindingOptions)
            body(body.toEpoxyCharSequence())
            bodyDetails(host.eventDetailsFormatter.format(state.timelineEvent()?.root)?.toEpoxyCharSequence())
            time(formattedDate)
            locationUiData(locationUiData)
        }

        // Send state
        val sendState = state.sendState()
        if (sendState?.hasFailed().orFalse()) {
            // Get more details about the error
            val errorMessage = state.timelineEvent()?.root?.sendStateError()
                    ?.let { errorFormatter.toHumanReadable(Failure.ServerError(it, 0)) }
                    ?: stringProvider.getString(CommonStrings.unable_to_send_message)
            bottomSheetSendStateItem {
                id("send_state")
                showProgress(false)
                text(errorMessage)
                drawableStart(R.drawable.ic_warning_badge)
            }
        } else if (sendState?.isSending().orFalse()) {
            bottomSheetSendStateItem {
                id("send_state")
                showProgress(true)
                text(host.stringProvider.getString(CommonStrings.event_status_sending_message))
            }
        } else if (sendState == SendState.SENT) {
            bottomSheetSendStateItem {
                id("send_state")
                showProgress(false)
                drawableStart(R.drawable.ic_message_sent)
                text(host.stringProvider.getString(CommonStrings.event_status_sent_message))
            }
        }

        when (state.informationData.e2eDecoration) {
            E2EDecoration.WARN_IN_CLEAR -> {
                bottomSheetSendStateItem {
                    id("e2e_clear")
                    showProgress(false)
                    text(host.stringProvider.getString(CommonStrings.unencrypted))
                    drawableStart(R.drawable.ic_shield_warning_small)
                }
            }
            E2EDecoration.WARN_SENT_BY_UNVERIFIED,
            E2EDecoration.WARN_SENT_BY_UNKNOWN -> {
                bottomSheetSendStateItem {
                    id("e2e_unverified")
                    showProgress(false)
                    text(host.stringProvider.getString(CommonStrings.encrypted_unverified))
                    drawableStart(R.drawable.ic_shield_warning_small)
                }
            }
            E2EDecoration.WARN_UNSAFE_KEY -> {
                bottomSheetSendStateItem {
                    id("e2e_unsafe")
                    showProgress(false)
                    text(host.stringProvider.getString(CommonStrings.key_authenticity_not_guaranteed))
                    drawableStart(R.drawable.ic_shield_gray)
                }
            }
            E2EDecoration.WARN_SENT_BY_DELETED_SESSION -> {
                bottomSheetSendStateItem {
                    id("e2e_deleted")
                    showProgress(false)
                    text(host.stringProvider.getString(CommonStrings.encrypted_by_deleted))
                    drawableStart(R.drawable.ic_shield_warning_small)
                }
            }
            E2EDecoration.NONE -> {
            }
        }

        // Quick reactions
        if (state.canReact() && state.quickStates is Success) {
            // Separator
            bottomSheetDividerItem {
                id("reaction_separator")
            }

            bottomSheetQuickReactionsItem {
                id("quick_reaction")
                fontProvider(host.fontProvider)
                texts(state.quickStates()?.map { it.reaction }.orEmpty())
                selecteds(state.quickStates.invoke().map { it.isSelected })
                listener(object : BottomSheetQuickReactionsItem.Listener {
                    override fun didSelect(emoji: String, selected: Boolean) {
                        host.listener?.didSelectMenuAction(EventSharedAction.QuickReact(state.eventId, emoji, selected))
                    }
                })
            }
        }

        if (state.actions.isNotEmpty()) {
            // Separator
            bottomSheetDividerItem {
                id("actions_separator")
            }
        }

        // Action
        state.actions.forEachIndexed { index, action ->
            if (action is EventSharedAction.Separator) {
                bottomSheetDividerItem {
                    id("separator_$index")
                }
            } else {
                val showBetaLabel = action.shouldShowBetaLabel()

                bottomSheetActionItem {
                    id("action_$index")
                    iconRes(action.iconResId)
                    textRes(action.titleRes)
                    showExpand(action is EventSharedAction.ReportContent)
                    expanded(state.expendedReportContentMenu)
                    listener { host.listener?.didSelectMenuAction(action) }
                    destructive(action.destructive)
                    showBetaLabel(showBetaLabel)
                }

                if (action is EventSharedAction.ReportContent && state.expendedReportContentMenu) {
                    // Special case for report content menu: add the submenu
                    listOf(
                            EventSharedAction.ReportContentSpam(action.eventId, action.senderId),
                            EventSharedAction.ReportContentInappropriate(action.eventId, action.senderId),
                            EventSharedAction.ReportContentCustom(action.eventId, action.senderId)
                    ).forEachIndexed { indexReport, actionReport ->
                        bottomSheetActionItem {
                            id("actionReport_$indexReport")
                            subMenuItem(true)
                            iconRes(actionReport.iconResId)
                            textRes(actionReport.titleRes)
                            listener { host.listener?.didSelectMenuAction(actionReport) }
                        }
                    }
                }
            }
        }
    }

    private fun buildLocationUiData(state: MessageActionState): LocationUiData? {
        if (state.timelineEvent()?.root?.isLocationMessage() != true) return null

        val locationContent = state.timelineEvent()?.root?.getClearContent().toModel<MessageLocationContent>(catchError = true)
                ?: return null
        val locationUrl = locationContent.toLocationData()
                ?.let { urlMapProvider.buildStaticMapUrl(it, INITIAL_MAP_ZOOM_IN_TIMELINE, 1200, 800) }
                ?: return null
        val locationOwnerId = if (locationContent.isSelfLocation()) state.informationData.senderId else null

        return LocationUiData(
                locationUrl = locationUrl,
                locationOwnerId = locationOwnerId,
                locationPinProvider = locationPinProvider,
        )
    }

    private fun EventSharedAction.shouldShowBetaLabel(): Boolean =
            this is EventSharedAction.ReplyInThread && !pcpPreferences.areThreadMessagesEnabled()

    interface MessageActionsEpoxyControllerListener : TimelineEventController.UrlClickCallback {
        fun didSelectMenuAction(eventAction: EventSharedAction)
    }
}
