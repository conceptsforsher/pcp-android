/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail

import android.annotation.SuppressLint
import android.app.Activity
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.text.method.LinkMovementMethod
import android.view.HapticFeedbackConstants
import android.view.LayoutInflater
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.TextView
import androidx.activity.addCallback
import androidx.annotation.StringRes
import androidx.appcompat.view.menu.MenuBuilder
import androidx.core.content.ContextCompat
import androidx.core.graphics.drawable.DrawableCompat
import androidx.core.net.toUri
import androidx.core.text.toSpannable
import androidx.core.util.Pair
import androidx.core.view.ViewCompat
import androidx.core.view.forEach
import androidx.core.view.isInvisible
import androidx.core.view.isVisible
import androidx.fragment.app.setFragmentResultListener
import androidx.lifecycle.lifecycleScope
import androidx.lifecycle.withResumed
import androidx.recyclerview.widget.ItemTouchHelper
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.transition.TransitionManager
import com.airbnb.epoxy.EpoxyModel
import com.airbnb.epoxy.EpoxyVisibilityTracker
import com.airbnb.epoxy.OnModelBuildFinishedListener
import com.airbnb.epoxy.addGlidePreloader
import com.airbnb.epoxy.glidePreloader
import com.airbnb.mvrx.Fail
import com.airbnb.mvrx.args
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.animations.play
import chat.pcp.app.core.dialogs.ConfirmationDialogBuilder
import chat.pcp.app.core.dialogs.GalleryOrCameraDialogHelper
import chat.pcp.app.core.dialogs.GalleryOrCameraDialogHelperFactory
import chat.pcp.app.core.epoxy.LayoutManagerStateRestorer
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.commitTransaction
import chat.pcp.app.core.extensions.containsRtLOverride
import chat.pcp.app.core.extensions.ensureEndsLeftToRight
import chat.pcp.app.core.extensions.filterDirectionOverrides
import chat.pcp.app.core.extensions.hideKeyboard
import chat.pcp.app.core.extensions.registerStartForActivityResult
import chat.pcp.app.core.extensions.setTextOrHide
import chat.pcp.app.core.glide.GlideApp
import chat.pcp.app.core.glide.GlideRequests
import chat.pcp.app.core.intent.getFilenameFromUri
import chat.pcp.app.core.intent.getMimeTypeFromUri
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.platform.PcpMenuProvider
import chat.pcp.app.core.platform.showOptimizedSnackbar
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.core.resources.UserPreferencesProvider
import chat.pcp.app.core.ui.views.CurrentCallsView
import chat.pcp.app.core.ui.views.CurrentCallsViewPresenter
import chat.pcp.app.core.ui.views.FailedMessagesWarningView
import chat.pcp.app.core.ui.views.JoinConferenceView
import chat.pcp.app.core.ui.views.NotificationAreaView
import chat.pcp.app.core.utils.Debouncer
import chat.pcp.app.core.utils.DimensionConverter
import chat.pcp.app.core.utils.KeyboardStateUtils
import chat.pcp.app.core.utils.PERMISSIONS_FOR_WRITING_FILES
import chat.pcp.app.core.utils.checkPermissions
import chat.pcp.app.core.utils.colorizeMatchingText
import chat.pcp.app.core.utils.copyToClipboard
import chat.pcp.app.core.utils.createJSonViewerStyleProvider
import chat.pcp.app.core.utils.createUIHandler
import chat.pcp.app.core.utils.isAnimationEnabled
import chat.pcp.app.core.utils.isValidUrl
import chat.pcp.app.core.utils.onPermissionDeniedDialog
import chat.pcp.app.core.utils.openLocation
import chat.pcp.app.core.utils.openUrlInExternalBrowser
import chat.pcp.app.core.utils.registerForPermissionsResult
import chat.pcp.app.core.utils.safeStartActivity
import chat.pcp.app.core.utils.saveMedia
import chat.pcp.app.core.utils.shareMedia
import chat.pcp.app.core.utils.shareText
import chat.pcp.app.core.utils.startInstallFromSourceIntent
import chat.pcp.app.core.utils.toast
import chat.pcp.app.databinding.DialogReportContentBinding
import chat.pcp.app.databinding.FragmentTimelineBinding
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.analytics.extensions.toAnalyticsInteraction
import im.vector.app.features.analytics.plan.Interaction
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.attachments.ShareIntentHandler
import chat.pcp.app.features.call.SharedKnownCallsViewModel
import chat.pcp.app.features.call.PcpCallActivity
import chat.pcp.app.features.call.conference.ConferenceEvent
import chat.pcp.app.features.call.conference.ConferenceEventEmitter
import chat.pcp.app.features.call.conference.ConferenceEventObserver
import chat.pcp.app.features.call.conference.JitsiCallViewModel
import chat.pcp.app.features.call.webrtc.WebRtcCallManager
import chat.pcp.app.features.crypto.keysbackup.restore.KeysBackupRestoreActivity
import chat.pcp.app.features.crypto.verification.user.UserVerificationBottomSheet
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.home.room.detail.arguments.TimelineArgs
import chat.pcp.app.features.home.room.detail.composer.CanSendStatus
import chat.pcp.app.features.home.room.detail.composer.MessageComposerAction
import chat.pcp.app.features.home.room.detail.composer.MessageComposerFragment
import chat.pcp.app.features.home.room.detail.composer.MessageComposerViewModel
import chat.pcp.app.features.home.room.detail.composer.boolean
import chat.pcp.app.features.home.room.detail.composer.voice.VoiceRecorderFragment
import chat.pcp.app.features.home.room.detail.error.RoomNotFound
import chat.pcp.app.features.home.room.detail.timeline.TimelineEventController
import chat.pcp.app.features.home.room.detail.timeline.action.EventSharedAction
import chat.pcp.app.features.home.room.detail.timeline.action.MessageActionsBottomSheet
import chat.pcp.app.features.home.room.detail.timeline.action.MessageSharedActionViewModel
import chat.pcp.app.native.PcpNative
import chat.pcp.app.features.home.room.detail.timeline.edithistory.ViewEditHistoryBottomSheet
import chat.pcp.app.features.home.room.detail.timeline.helper.AudioMessagePlaybackTracker
import chat.pcp.app.features.home.room.detail.timeline.helper.MatrixItemColorProvider
import chat.pcp.app.features.home.room.detail.timeline.item.AbsMessageItem
import chat.pcp.app.features.home.room.detail.timeline.item.MessageAudioItem
import chat.pcp.app.features.home.room.detail.timeline.item.MessageFileItem
import chat.pcp.app.features.home.room.detail.timeline.item.MessageImageVideoItem
import chat.pcp.app.features.home.room.detail.timeline.item.MessageInformationData
import chat.pcp.app.features.home.room.detail.timeline.item.MessageTextItem
import chat.pcp.app.features.home.room.detail.timeline.item.MessageVoiceItem
import chat.pcp.app.features.home.room.detail.timeline.item.ReadReceiptData
import chat.pcp.app.features.home.room.detail.timeline.reactions.ViewReactionsBottomSheet
import chat.pcp.app.features.home.room.detail.timeline.readreceipts.DisplayReadReceiptsBottomSheet
import chat.pcp.app.features.home.room.detail.timeline.url.PreviewUrlRetriever
import chat.pcp.app.features.home.room.detail.upgrade.MigrateRoomBottomSheet
import chat.pcp.app.features.home.room.detail.views.RoomDetailLazyLoadedViews
import chat.pcp.app.features.home.room.detail.widget.RoomWidgetsBottomSheet
import chat.pcp.app.features.home.room.threads.ThreadsManager
import chat.pcp.app.features.home.room.threads.arguments.ThreadTimelineArgs
import chat.pcp.app.features.html.EventHtmlRenderer
import chat.pcp.app.features.invite.PcpInviteView
import chat.pcp.app.features.location.LocationSharingMode
import chat.pcp.app.features.location.toLocationData
import chat.pcp.app.features.media.AttachmentData
import chat.pcp.app.features.media.ImageContentRenderer
import chat.pcp.app.features.media.VideoContentRenderer
import chat.pcp.app.features.notifications.NotificationDrawerManager
import chat.pcp.app.features.notifications.NotificationUtils
import chat.pcp.app.features.permalink.NavigationInterceptor
import chat.pcp.app.features.permalink.PermalinkFactory
import chat.pcp.app.features.permalink.PermalinkHandler
import chat.pcp.app.features.poll.PollMode
import chat.pcp.app.features.reactions.EmojiReactionPickerActivity
import chat.pcp.app.features.roomprofile.RoomProfileActivity
import chat.pcp.app.features.session.coroutineScope
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.settings.PcpSettingsActivity
import chat.pcp.app.features.spaces.share.ShareSpaceBottomSheet
import chat.pcp.app.features.themes.ThemeUtils
import chat.pcp.app.features.widgets.WidgetActivity
import chat.pcp.app.features.widgets.WidgetArgs
import chat.pcp.app.features.widgets.WidgetKind
import chat.pcp.app.features.widgets.permissions.RoomWidgetPermissionBottomSheet
import chat.pcp.lib.core.utils.timer.Clock
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import org.billcarsonfr.jsonviewer.JSonViewerDialog
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.events.model.EventType
import org.matrix.android.sdk.api.session.events.model.content.EncryptedEventContent
import org.matrix.android.sdk.api.session.events.model.content.WithHeldCode
import org.matrix.android.sdk.api.session.events.model.toModel
import org.matrix.android.sdk.api.session.room.model.Membership
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import org.matrix.android.sdk.api.session.room.model.message.MessageAudioContent
import org.matrix.android.sdk.api.session.room.model.message.MessageBeaconInfoContent
import org.matrix.android.sdk.api.session.room.model.message.MessageImageInfoContent
import org.matrix.android.sdk.api.session.room.model.message.MessageLocationContent
import org.matrix.android.sdk.api.session.room.model.message.MessageStickerContent
import org.matrix.android.sdk.api.session.room.model.message.MessageTextContent
import org.matrix.android.sdk.api.session.room.model.message.MessageVerificationRequestContent
import org.matrix.android.sdk.api.session.room.model.message.MessageVideoContent
import org.matrix.android.sdk.api.session.room.model.message.MessageWithAttachmentContent
import org.matrix.android.sdk.api.session.room.send.SendState
import org.matrix.android.sdk.api.session.room.timeline.Timeline
import org.matrix.android.sdk.api.session.room.timeline.TimelineEvent
import org.matrix.android.sdk.api.session.widgets.model.Widget
import org.matrix.android.sdk.api.session.widgets.model.WidgetType
import org.matrix.android.sdk.api.util.MatrixItem
import org.matrix.android.sdk.api.util.MimeTypes
import org.matrix.android.sdk.api.util.toMatrixItem
import timber.log.Timber
import java.net.URL
import java.util.UUID
import javax.inject.Inject
import okhttp3.MediaType.Companion.toMediaType
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import org.json.JSONObject
import java.util.concurrent.TimeUnit

@AndroidEntryPoint
class TimelineFragment :
        PcpFragment<FragmentTimelineBinding>(),
        TimelineEventController.Callback,
        PcpInviteView.Callback,
        GalleryOrCameraDialogHelper.Listener,
        CurrentCallsView.Callback,
        PcpMenuProvider {

    @Inject lateinit var session: Session
    @Inject lateinit var avatarRenderer: AvatarRenderer
    @Inject lateinit var timelineEventController: TimelineEventController
    @Inject lateinit var permalinkHandler: PermalinkHandler
    @Inject lateinit var notificationDrawerManager: NotificationDrawerManager
    @Inject lateinit var eventHtmlRenderer: EventHtmlRenderer
    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var threadsManager: ThreadsManager
    @Inject lateinit var colorProvider: ColorProvider
    @Inject lateinit var dimensionConverter: DimensionConverter
    @Inject lateinit var userPreferencesProvider: UserPreferencesProvider
    @Inject lateinit var notificationUtils: NotificationUtils
    @Inject lateinit var matrixItemColorProvider: MatrixItemColorProvider
    @Inject lateinit var imageContentRenderer: ImageContentRenderer
    @Inject lateinit var roomDetailPendingActionStore: RoomDetailPendingActionStore
    @Inject lateinit var callManager: WebRtcCallManager
    @Inject lateinit var audioMessagePlaybackTracker: AudioMessagePlaybackTracker
    @Inject lateinit var shareIntentHandler: ShareIntentHandler
    @Inject lateinit var clock: Clock
    @Inject lateinit var vectorFeatures: PcpFeatures
    @Inject lateinit var galleryOrCameraDialogHelperFactory: GalleryOrCameraDialogHelperFactory
    @Inject lateinit var permalinkFactory: PermalinkFactory

    companion object {
        const val MAX_TYPING_MESSAGE_USERS_COUNT = 4
    }

    private lateinit var galleryOrCameraDialogHelper: GalleryOrCameraDialogHelper

    private val timelineArgs: TimelineArgs by args()

    private val timelineViewModel: TimelineViewModel by fragmentViewModel()
    private val messageComposerViewModel: MessageComposerViewModel by fragmentViewModel()
    private val debouncer = Debouncer(createUIHandler())
    private val itemVisibilityTracker = EpoxyVisibilityTracker()

    private lateinit var scrollOnNewMessageCallback: ScrollOnNewMessageCallback
    private lateinit var scrollOnHighlightedEventCallback: ScrollOnHighlightedEventCallback

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentTimelineBinding {
        return FragmentTimelineBinding.inflate(inflater, container, false)
    }

    override fun getMenuRes() = R.menu.menu_timeline

    private lateinit var sharedActionViewModel: MessageSharedActionViewModel
    private lateinit var sharedActivityActionViewModel: RoomDetailSharedActionViewModel

    private lateinit var knownCallsViewModel: SharedKnownCallsViewModel

    private lateinit var layoutManager: LinearLayoutManager
    private lateinit var jumpToBottomViewVisibilityManager: JumpToBottomViewVisibilityManager
    private var modelBuildListener: OnModelBuildFinishedListener? = null

    private lateinit var keyboardStateUtils: KeyboardStateUtils
    private lateinit var callActionsHandler: StartCallActionsHandler

    private val currentCallsViewPresenter = CurrentCallsViewPresenter()

    private val lazyLoadedViews = RoomDetailLazyLoadedViews()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.Room
        galleryOrCameraDialogHelper = galleryOrCameraDialogHelperFactory.create(this)
        setFragmentResultListener(MigrateRoomBottomSheet.REQUEST_KEY) { _, bundle ->
            bundle.getString(MigrateRoomBottomSheet.BUNDLE_KEY_REPLACEMENT_ROOM)?.let { replacementRoomId ->
                timelineViewModel.handle(RoomDetailAction.RoomUpgradeSuccess(replacementRoomId))
            }
        }

        if (childFragmentManager.findFragmentById(R.id.composerContainer) == null) {
            childFragmentManager.commitTransaction {
                replace(R.id.composerContainer, MessageComposerFragment())
            }
        }

        if (childFragmentManager.findFragmentById(R.id.voiceMessageRecorderContainer) == null) {
            childFragmentManager.commitTransaction {
                replace(R.id.voiceMessageRecorderContainer, VoiceRecorderFragment())
            }
        }
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        lifecycle.addObserver(ConferenceEventObserver(vectorBaseActivity, this::onBroadcastJitsiEvent))
        super.onViewCreated(view, savedInstanceState)
        sharedActionViewModel = activityViewModelProvider.get(MessageSharedActionViewModel::class.java)
        sharedActivityActionViewModel = activityViewModelProvider.get(RoomDetailSharedActionViewModel::class.java)
        knownCallsViewModel = activityViewModelProvider.get(SharedKnownCallsViewModel::class.java)
        callActionsHandler = StartCallActionsHandler(
                roomId = timelineArgs.roomId,
                fragment = this,
                pcpPreferences = pcpPreferences,
                timelineViewModel = timelineViewModel,
                callManager = callManager,
                startCallActivityResultLauncher = startCallActivityResultLauncher,
                showDialogWithMessage = ::showDialogWithMessage,
                onTapToReturnToCall = ::onTapToReturnToCall
        )
        keyboardStateUtils = KeyboardStateUtils(requireActivity())
        lazyLoadedViews.bind(views)
        setupToolbar(views.roomToolbar)
                .allowBack()
        setupRecyclerView()
        setupNotificationView()
        setupJumpToReadMarkerView()
        setupActiveCallView()
        setupJumpToBottomView()
        setupRemoveJitsiWidgetView()
        setupLiveLocationIndicator()
        setupBackPressHandling()

        views.includeRoomToolbar.roomToolbarContentView.debouncedClicks {
            navigator.openRoomProfile(requireActivity(), timelineArgs.roomId)
        }

        sharedActionViewModel
                .stream()
                .onEach {
                    handleActions(it)
                }
                .launchIn(viewLifecycleOwner.lifecycleScope)

        knownCallsViewModel
                .liveKnownCalls
                .observe(viewLifecycleOwner) {
                    currentCallsViewPresenter.updateCall(callManager.getCurrentCall(), it)
                    invalidateOptionsMenu()
                }

        timelineViewModel.onEach(RoomDetailViewState::canShowJumpToReadMarker, RoomDetailViewState::unreadState) { _, _ ->
            updateJumpToReadMarkerViewVisibility()
        }

        timelineViewModel.onEach(
                RoomDetailViewState::syncState,
                RoomDetailViewState::incrementalSyncRequestState,
                RoomDetailViewState::pushCounter
        ) { syncState, incrementalSyncStatus, pushCounter ->
            views.syncStateView.render(
                    syncState,
                    incrementalSyncStatus,
                    pushCounter,
                    pcpPreferences.developerShowDebugInfo()
            )
        }

        timelineViewModel.observeViewEvents {
            when (it) {
                is RoomDetailViewEvents.Failure -> displayErrorMessage(it)
                is RoomDetailViewEvents.OnNewTimelineEvents -> scrollOnNewMessageCallback.addNewTimelineEventIds(it.eventIds)
                is RoomDetailViewEvents.ActionSuccess -> displayRoomDetailActionSuccess(it)
                is RoomDetailViewEvents.ActionFailure -> displayRoomDetailActionFailure(it)
                is RoomDetailViewEvents.ShowMessage -> showSnackWithMessage(it.message)
                is RoomDetailViewEvents.NavigateToEvent -> navigateToEvent(it)
                is RoomDetailViewEvents.DownloadFileState -> handleDownloadFileState(it)
                is RoomDetailViewEvents.ShowE2EErrorMessage -> displayE2eError(it.withHeldCode)
                RoomDetailViewEvents.DisplayPromptForIntegrationManager -> displayPromptForIntegrationManager()
                is RoomDetailViewEvents.OpenStickerPicker -> openStickerPicker(it)
                is RoomDetailViewEvents.DisplayEnableIntegrationsWarning -> displayDisabledIntegrationDialog()
                is RoomDetailViewEvents.OpenIntegrationManager -> openIntegrationManager()
                is RoomDetailViewEvents.OpenFile -> startOpenFileIntent(it)
                RoomDetailViewEvents.OpenActiveWidgetBottomSheet -> onViewWidgetsClicked()
                is RoomDetailViewEvents.ShowInfoOkDialog -> showDialogWithMessage(it.message)
                is RoomDetailViewEvents.JoinJitsiConference -> joinJitsiRoom(it.widget, it.withVideo)
                RoomDetailViewEvents.LeaveJitsiConference -> leaveJitsiConference()
                is RoomDetailViewEvents.ShowWaitingView -> vectorBaseActivity.showWaitingView(it.text)
                RoomDetailViewEvents.HideWaitingView -> vectorBaseActivity.hideWaitingView()
                is RoomDetailViewEvents.RequestNativeWidgetPermission -> requestNativeWidgetPermission(it)
                is RoomDetailViewEvents.OpenRoom -> handleOpenRoom(it)
                RoomDetailViewEvents.OpenInvitePeople -> navigator.openInviteUsersToRoom(requireActivity(), timelineArgs.roomId)
                RoomDetailViewEvents.OpenSetRoomAvatarDialog -> galleryOrCameraDialogHelper.show()
                RoomDetailViewEvents.OpenRoomSettings -> handleOpenRoomSettings(RoomProfileActivity.EXTRA_DIRECT_ACCESS_ROOM_SETTINGS)
                RoomDetailViewEvents.OpenRoomProfile -> handleOpenRoomSettings()
                is RoomDetailViewEvents.ShowRoomAvatarFullScreen -> it.matrixItem?.let { item ->
                    navigator.openBigImageViewer(requireActivity(), it.view, item)
                }
                is RoomDetailViewEvents.StartChatEffect -> handleChatEffect(it.type)
                RoomDetailViewEvents.StopChatEffects -> handleStopChatEffects()
                is RoomDetailViewEvents.DisplayAndAcceptCall -> acceptIncomingCall(it)
                RoomDetailViewEvents.RoomReplacementStarted -> handleRoomReplacement()
                RoomDetailViewEvents.OpenElementCallWidget -> handleOpenElementCallWidget()
                RoomDetailViewEvents.DisplayPromptToStopVoiceBroadcast -> displayPromptToStopVoiceBroadcast()
                is RoomDetailViewEvents.RevokeFilePermission -> revokeFilePermission(it)
            }
        }
    }

    private fun setupBackPressHandling() {
        requireActivity().onBackPressedDispatcher.addCallback(viewLifecycleOwner) {
            withState(messageComposerViewModel) { state ->
                if (state.isFullScreen) {
                    messageComposerViewModel.handle(MessageComposerAction.SetFullScreen(false))
                } else {
                    remove() // Remove callback to avoid infinite loop
                    @Suppress("DEPRECATION")
                    requireActivity().onBackPressed()
                }
            }
        }
    }

    private fun setupRemoveJitsiWidgetView() {
        views.removeJitsiWidgetView.onCompleteSliding = {
            withState(timelineViewModel) {
                val jitsiWidgetId = it.jitsiState.widgetId ?: return@withState
                if (it.jitsiState.hasJoined) {
                    leaveJitsiConference()
                }
                timelineViewModel.handle(RoomDetailAction.RemoveWidget(jitsiWidgetId))
            }
        }
    }

    private fun leaveJitsiConference() {
        ConferenceEventEmitter(vectorBaseActivity).emitConferenceEnded()
    }

    private fun onBroadcastJitsiEvent(conferenceEvent: ConferenceEvent) {
        timelineViewModel.handle(RoomDetailAction.UpdateJoinJitsiCallStatus(conferenceEvent))
    }

    private fun acceptIncomingCall(event: RoomDetailViewEvents.DisplayAndAcceptCall) {
        val intent = PcpCallActivity.newIntent(
                context = vectorBaseActivity,
                call = event.call,
                mode = PcpCallActivity.INCOMING_ACCEPT
        )
        startActivity(intent)
    }

    private fun handleRoomReplacement() {
        // this will join a new room, it can take time and might fail
        // so we need to report progress and retry
        val tag = JoinReplacementRoomBottomSheet::javaClass.name
        JoinReplacementRoomBottomSheet().show(childFragmentManager, tag)
    }

    private fun handleChatEffect(chatEffect: ChatEffect) {
        if (!requireContext().isAnimationEnabled()) {
            Timber.d("Do not perform chat effect, animations are disabled.")
            return
        }
        when (chatEffect) {
            ChatEffect.CONFETTI -> {
                views.viewKonfetti.isVisible = true
                views.viewKonfetti.play()
            }
            ChatEffect.SNOWFALL -> {
                views.viewSnowFall.isVisible = true
                views.viewSnowFall.restartFalling()
            }
        }
    }

    private fun handleStopChatEffects() {
        TransitionManager.beginDelayedTransition(views.rootConstraintLayout)
        views.viewSnowFall.isVisible = false
        // when gone the effect is a bit buggy
        views.viewKonfetti.isInvisible = true
    }

    override fun onImageReady(uri: Uri?) {
        uri ?: return
        timelineViewModel.handle(
                RoomDetailAction.SetAvatarAction(
                        newAvatarUri = uri,
                        newAvatarFileName = getFilenameFromUri(requireContext(), uri) ?: UUID.randomUUID().toString()
                )
        )
    }

    private fun handleTranslateAction(action: EventSharedAction.Translate) {
        val endpoint = pcpPreferences.getTranslateApiEndpoint()
        val token = pcpPreferences.getTranslateApiToken()
        val targetLang = pcpPreferences.getTranslateTargetLanguage()
            .ifEmpty { java.util.Locale.getDefault().language }

        if (endpoint.isBlank() || token.isBlank()) {
            showSnackWithMessage("Configure API endpoint and token in Settings → Labs")
            return
        }

        viewLifecycleOwner.lifecycleScope.launch(Dispatchers.IO) {
            try {
                val requestJson = JSONObject(PcpNative.nativeBuildTranslateRequest(
                    action.text, "", targetLang, endpoint, token, "gpt-4o-mini"
                ))

                val client = OkHttpClient.Builder()
                    .connectTimeout(15, TimeUnit.SECONDS)
                    .readTimeout(30, TimeUnit.SECONDS)
                    .build()

                val request = okhttp3.Request.Builder()
                    .url(endpoint)
                    .header("Authorization", "Bearer $token")
                    .header("Content-Type", "application/json")
                    .post(requestJson.toString().toRequestBody("application/json".toMediaType()))
                    .build()

                val response = client.newCall(request).execute()
                val body = response.body?.string() ?: ""

                val result = JSONObject(PcpNative.nativeParseTranslateResponse(body, response.code))

                withContext(Dispatchers.Main) {
                    if (result.optBoolean("success", false)) {
                        showTranslatedText(result.getString("translatedText"))
                    } else {
                        val error = result.optString("error", "Translation failed")
                        showSnackWithMessage(error)
                    }
                }
            } catch (e: Exception) {
                withContext(Dispatchers.Main) { showErrorInSnackbar(e) }
            }
        }
    }

    private fun showTranslatedText(text: String) {
        MaterialAlertDialogBuilder(requireContext())
            .setTitle(CommonStrings.translate_action)
            .setMessage(text)
            .setPositiveButton(CommonStrings.action_copy) { _, _ ->
                copyToClipboard(requireContext(), text, false, CommonStrings.translate_action)
            }
            .setNegativeButton(CommonStrings.action_cancel, null)
            .show()
    }

    private fun handleOpenRoomSettings(directAccess: Int? = null) {
        navigator.openRoomProfile(
                requireContext(),
                timelineArgs.roomId,
                directAccess
        )
    }

    private fun handleOpenRoom(openRoom: RoomDetailViewEvents.OpenRoom) {
        navigator.openRoom(requireContext(), openRoom.roomId, null)
        if (openRoom.closeCurrentRoom) {
            requireActivity().finish()
        }
    }

    private fun handleShowLocationPreview(locationContent: MessageLocationContent, senderId: String) {
        val isSelfLocation = locationContent.isSelfLocation()
        navigator
                .openLocationSharing(
                        context = requireContext(),
                        roomId = timelineArgs.roomId,
                        mode = LocationSharingMode.PREVIEW,
                        initialLocationData = locationContent.toLocationData(),
                        locationOwnerId = if (isSelfLocation) senderId else null
                )
    }

    private fun navigateToLiveLocationMap() {
        navigator.openLiveLocationMap(
                context = requireContext(),
                roomId = timelineArgs.roomId
        )
    }

    private fun displayErrorMessage(error: RoomDetailViewEvents.Failure) {
        if (error.showInDialog) displayErrorDialog(error.throwable) else showErrorInSnackbar(error.throwable)
    }

    private fun requestNativeWidgetPermission(it: RoomDetailViewEvents.RequestNativeWidgetPermission) {
        val tag = RoomWidgetPermissionBottomSheet::class.java.name
        val dFrag = childFragmentManager.findFragmentByTag(tag) as? RoomWidgetPermissionBottomSheet
        if (dFrag != null && dFrag.dialog?.isShowing == true && !dFrag.isRemoving) {
            return
        } else {
            RoomWidgetPermissionBottomSheet.newInstance(
                    WidgetArgs(
                            baseUrl = it.domain,
                            kind = WidgetKind.ROOM,
                            roomId = timelineArgs.roomId,
                            widgetId = it.widget.widgetId
                    )
            ).apply {
                directListener = { granted ->
                    if (granted) {
                        timelineViewModel.handle(
                                RoomDetailAction.EnsureNativeWidgetAllowed(
                                        widget = it.widget,
                                        userJustAccepted = true,
                                        grantedEvents = it.grantedEvents
                                )
                        )
                    }
                }
            }
                    .show(childFragmentManager, tag)
        }
    }

    private val integrationManagerActivityResultLauncher = registerStartForActivityResult {
        // Noop
    }

    private fun openIntegrationManager(screen: String? = null) {
        navigator.openIntegrationManager(
                context = requireContext(),
                activityResultLauncher = integrationManagerActivityResultLauncher,
                roomId = timelineArgs.roomId,
                integId = null,
                screen = screen
        )
    }

    private fun createFailedMessagesWarningCallback(): FailedMessagesWarningView.Callback {
        return object : FailedMessagesWarningView.Callback {
            override fun onDeleteAllClicked() {
                MaterialAlertDialogBuilder(requireContext())
                        .setTitle(CommonStrings.event_status_delete_all_failed_dialog_title)
                        .setMessage(getString(CommonStrings.event_status_delete_all_failed_dialog_message))
                        .setNegativeButton(CommonStrings.no, null)
                        .setPositiveButton(CommonStrings.yes) { _, _ ->
                            timelineViewModel.handle(RoomDetailAction.RemoveAllFailedMessages)
                        }
                        .show()
            }

            override fun onRetryClicked() {
                timelineViewModel.handle(RoomDetailAction.ResendAll)
            }
        }
    }

    private fun setupLiveLocationIndicator() {
        views.liveLocationStatusIndicator.stopButton.debouncedClicks {
            timelineViewModel.handle(RoomDetailAction.StopLiveLocationSharing)
        }
        views.liveLocationStatusIndicator.debouncedClicks {
            navigateToLiveLocationMap()
        }
    }

    private fun joinJitsiRoom(jitsiWidget: Widget, enableVideo: Boolean) {
        navigator.openRoomWidget(requireContext(), timelineArgs.roomId, jitsiWidget, mapOf(JitsiCallViewModel.ENABLE_VIDEO_OPTION to enableVideo))
    }

    private fun openStickerPicker(event: RoomDetailViewEvents.OpenStickerPicker) {
        navigator.openStickerPicker(requireContext(), stickerActivityResultLauncher, timelineArgs.roomId, event.widget)
    }

    private fun startOpenFileIntent(action: RoomDetailViewEvents.OpenFile) {
        if (action.mimeType == MimeTypes.Apk) {
            installApk(action)
        } else {
            openFile(action)
        }
    }

    private fun openFile(action: RoomDetailViewEvents.OpenFile) {
        val intent = Intent(Intent.ACTION_VIEW).apply {
            setDataAndTypeAndNormalize(action.uri, action.mimeType)
            addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION or Intent.FLAG_ACTIVITY_NEW_TASK)
        }

        requireActivity().safeStartActivity(intent)
    }

    private fun installApk(action: RoomDetailViewEvents.OpenFile) {
        val safeContext = context ?: return
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            if (!safeContext.packageManager.canRequestPackageInstalls()) {
                timelineViewModel.pendingEvent = action
                startInstallFromSourceIntent(safeContext, installApkActivityResultLauncher)
            } else {
                openFile(action)
            }
        } else {
            openFile(action)
        }
    }

    private val installApkActivityResultLauncher = registerStartForActivityResult { activityResult ->
        if (activityResult.resultCode == Activity.RESULT_OK) {
            timelineViewModel.pendingEvent?.let {
                if (it is RoomDetailViewEvents.OpenFile) {
                    openFile(it)
                }
            }
        } else {
            // User cancelled
        }
        timelineViewModel.pendingEvent = null
    }

    private fun displayPromptForIntegrationManager() {
        // The Sticker picker widget is not installed yet. Propose the user to install it
        val builder = MaterialAlertDialogBuilder(requireContext())
        val v: View = LayoutInflater.from(requireContext()).inflate(R.layout.dialog_no_sticker_pack, null)
        builder
                .setView(v)
                .setPositiveButton(CommonStrings.yes) { _, _ ->
                    // Open integration manager, to the sticker installation page
                    openIntegrationManager(
                            screen = WidgetType.StickerPicker.preferred
                    )
                }
                .setNegativeButton(CommonStrings.no, null)
                .show()
    }

    private fun handleSpaceShare() {
        timelineArgs.openShareSpaceForId?.let { spaceId ->
            ShareSpaceBottomSheet.show(childFragmentManager, spaceId, true)
            view?.post {
                handleChatEffect(ChatEffect.CONFETTI)
            }
        }
    }

    override fun onDestroyView() {
        lazyLoadedViews.unBind()
        timelineEventController.callback = null
        timelineEventController.removeModelBuildListener(modelBuildListener)
        currentCallsViewPresenter.unBind()
        modelBuildListener = null
        debouncer.cancelAll()
        views.timelineRecyclerView.cleanup()
        super.onDestroyView()
    }

    override fun onDestroy() {
        timelineViewModel.handle(RoomDetailAction.ExitTrackingUnreadMessagesState)
        super.onDestroy()
    }

    private fun setupJumpToBottomView() {
        views.jumpToBottomView.visibility = View.INVISIBLE
        views.jumpToBottomView.debouncedClicks {
            timelineViewModel.handle(RoomDetailAction.ExitTrackingUnreadMessagesState)
            views.jumpToBottomView.visibility = View.INVISIBLE
            if (timelineViewModel.timeline?.isLive == false) {
                scrollOnNewMessageCallback.forceScrollOnNextUpdate()
                timelineViewModel.timeline?.restartWithEventId(null)
            } else {
                layoutManager.scrollToPosition(0)
            }
        }

        jumpToBottomViewVisibilityManager = JumpToBottomViewVisibilityManager(
                views.jumpToBottomView,
                debouncer,
                views.timelineRecyclerView,
                layoutManager
        )
    }

    private fun setupJumpToReadMarkerView() {
        views.jumpToReadMarkerView.debouncedClicks {
            onJumpToReadMarkerClicked()
        }
        views.jumpToReadMarkerView.setOnCloseIconClickListener {
            timelineViewModel.handle(RoomDetailAction.MarkAllAsRead)
        }
    }

    private fun setupActiveCallView() {
        currentCallsViewPresenter.bind(views.currentCallsView, this)
    }

    private fun navigateToEvent(action: RoomDetailViewEvents.NavigateToEvent) {
        val scrollPosition = timelineEventController.getPositionOfReadMarker().takeIf { action.isFirstUnreadEvent }
                ?: timelineEventController.searchPositionOfEvent(action.eventId)

        if (scrollPosition == null) {
            scrollOnHighlightedEventCallback.scheduleScrollTo(action.eventId)
        } else {
            views.timelineRecyclerView.stopScroll()
            layoutManager.scrollToPosition(scrollPosition)
        }
    }

    private fun handleDownloadFileState(action: RoomDetailViewEvents.DownloadFileState) {
        val activity = requireActivity()
        if (action.throwable != null) {
            activity.toast(errorFormatter.toHumanReadable(action.throwable))
        }
//        else if (action.file != null) {
//            addEntryToDownloadManager(activity, action.file, action.mimeType ?: "application/octet-stream")?.let {
//                // This is a temporary solution to help users find downloaded files
//                // there is a better way to do that
//                // On android Q+ this method returns the file URI, on older
//                // it returns null, and the download manager handles the notification
//                notificationUtils.buildDownloadFileNotification(
//                        it,
//                        action.file.name ?: "file",
//                        action.mimeType ?: "application/octet-stream"
//                ).let { notification ->
//                    notificationUtils.showNotificationMessage("DL", action.file.absolutePath.hashCode(), notification)
//                }
//            }
//        }
    }

    private fun setupNotificationView() {
        views.notificationAreaView.delegate = object : NotificationAreaView.Delegate {
            override fun onTombstoneEventClicked() {
                timelineViewModel.handle(RoomDetailAction.JoinAndOpenReplacementRoom)
            }

            override fun onMisconfiguredEncryptionClicked() {
                timelineViewModel.handle(RoomDetailAction.OnClickMisconfiguredEncryption)
            }
        }
    }

    @SuppressLint("RestrictedApi")
    override fun handlePostCreateMenu(menu: Menu) {
        if (isThreadTimeLine()) {
            if (menu is MenuBuilder) menu.setOptionalIconsVisible(true)
        }
        // We use a custom layout for this menu item, so we need to set a ClickListener
        menu.findItem(R.id.open_matrix_apps)?.let { menuItem ->
            menuItem.actionView?.setOnClickListener {
                handleMenuItemSelected(menuItem)
            }
        }
        val joinConfItem = menu.findItem(R.id.join_conference)
        (joinConfItem.actionView as? JoinConferenceView)?.onJoinClicked = {
            timelineViewModel.handle(RoomDetailAction.JoinJitsiCall)
        }

        // Custom thread notification menu item
        menu.findItem(R.id.menu_timeline_thread_list)?.let { menuItem ->
            menuItem.actionView?.setOnClickListener {
                handleMenuItemSelected(menuItem)
            }
        }
    }

    override fun handlePrepareMenu(menu: Menu) {
        menu.forEach {
            it.isVisible = timelineViewModel.isMenuItemVisible(it.itemId)
        }

        withState(timelineViewModel) { state ->
            // Set the visual state of the call buttons (voice/video) to enabled/disabled according to user permissions
            val hasCallInRoom = callManager.getCallsByRoomId(state.roomId).isNotEmpty() || state.jitsiState.hasJoined
            val callButtonsEnabled = !hasCallInRoom && when (state.asyncRoomSummary.invoke()?.joinedMembersCount) {
                1 -> false
                2 -> state.isAllowedToStartWebRTCCall
                else -> state.isAllowedToManageWidgets
            }
            menu.findItem(R.id.video_call).icon?.alpha = if (callButtonsEnabled) 0xFF else 0x40
            menu.findItem(R.id.voice_call).icon?.alpha = if (callButtonsEnabled || state.hasActiveElementCallWidget()) 0xFF else 0x40

            val matrixAppsMenuItem = menu.findItem(R.id.open_matrix_apps)
            val widgetsCount = state.activeRoomWidgets.invoke()?.size ?: 0
            val hasOnlyJitsiWidget = widgetsCount == 1 && state.hasActiveJitsiWidget()
            if (widgetsCount == 0 || hasOnlyJitsiWidget) {
                // icon should be default color no badge
                val actionView = matrixAppsMenuItem.actionView
                actionView
                        ?.findViewById<ImageView>(R.id.action_view_icon_image)
                        ?.setColorFilter(ThemeUtils.getColor(requireContext(), chat.pcp.lib.ui.styles.R.attr.vctr_content_secondary))
                actionView?.findViewById<TextView>(R.id.cart_badge)?.isVisible = false
                matrixAppsMenuItem.setShowAsAction(MenuItem.SHOW_AS_ACTION_NEVER)
            } else {
                val actionView = matrixAppsMenuItem.actionView
                actionView
                        ?.findViewById<ImageView>(R.id.action_view_icon_image)
                        ?.setColorFilter(colorProvider.getColorFromAttribute(com.google.android.material.R.attr.colorPrimary))
                actionView?.findViewById<TextView>(R.id.cart_badge)?.setTextOrHide("$widgetsCount")
                @Suppress("AlwaysShowAction")
                matrixAppsMenuItem.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS)
            }

            // Handle custom threads badge notification
            updateMenuThreadNotificationBadge(menu, state)
        }
    }

    override fun handleMenuItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.invite -> {
                navigator.openInviteUsersToRoom(requireActivity(), timelineArgs.roomId)
                true
            }
            R.id.timeline_setting -> {
                navigator.openRoomProfile(requireActivity(), timelineArgs.roomId)
                true
            }
            R.id.open_matrix_apps -> {
                timelineViewModel.handle(RoomDetailAction.ManageIntegrations)
                true
            }
            R.id.voice_call -> {
                callActionsHandler.onVoiceCallClicked()
                true
            }
            R.id.video_call -> {
                callActionsHandler.onVideoCallClicked()
                true
            }
            R.id.menu_timeline_thread_list -> {
                navigateToThreadList()
                true
            }
            R.id.search -> {
                handleSearchAction()
                true
            }
            R.id.dev_tools -> {
                navigator.openDevTools(requireContext(), timelineArgs.roomId)
                true
            }
            R.id.menu_thread_timeline_copy_link -> {
                getRootThreadEventId()?.let {
                    val permalink = permalinkFactory.createPermalink(timelineArgs.roomId, it)
                    copyToClipboard(requireContext(), permalink, false)
                    showSnackWithMessage(getString(CommonStrings.copied_to_clipboard))
                }
                true
            }
            R.id.menu_thread_timeline_view_in_room -> {
                handleViewInRoomAction()
                true
            }
            R.id.menu_thread_timeline_share -> {
                getRootThreadEventId()?.let {
                    val permalink = permalinkFactory.createPermalink(timelineArgs.roomId, it)
                    shareText(requireContext(), permalink)
                }
                true
            }
            else -> false
        }
    }

    /**
     * Update menu thread notification badge appropriately.
     */
    @SuppressLint("SetTextI18n")
    private fun updateMenuThreadNotificationBadge(menu: Menu, state: RoomDetailViewState) {
        val menuThreadList = menu.findItem(R.id.menu_timeline_thread_list).actionView
        val badgeFrameLayout = menuThreadList?.findViewById<FrameLayout>(R.id.threadNotificationBadgeFrameLayout) ?: return
        val badgeTextView = menuThreadList.findViewById<TextView>(R.id.threadNotificationBadgeTextView)

        val unreadThreadMessages = state.threadNotificationBadgeState.numberOfLocalUnreadThreads
        val userIsMentioned = state.threadNotificationBadgeState.isUserMentioned

        if (unreadThreadMessages > 0) {
            badgeFrameLayout.isVisible = true
            badgeTextView.text = "$unreadThreadMessages"
            val badgeDrawable = DrawableCompat.wrap(badgeFrameLayout.background)
            val color = ContextCompat.getColor(
                    requireContext(),
                    if (userIsMentioned) chat.pcp.lib.ui.styles.R.color.palette_vermilion else chat.pcp.lib.ui.styles.R.color.palette_gray_200
            )
            DrawableCompat.setTint(badgeDrawable, color)
            badgeFrameLayout.background = badgeDrawable
        } else {
            badgeFrameLayout.isVisible = false
        }
    }

    /**
     * View and highlight the original root thread message in the main timeline.
     */
    private fun handleViewInRoomAction() {
        getRootThreadEventId()?.let {
            val newRoom = timelineArgs.copy(threadTimelineArgs = null, eventId = it)
            context?.let { con ->
                val intent = RoomDetailActivity.newIntent(con, newRoom, false)
                intent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP or Intent.FLAG_ACTIVITY_NEW_TASK
                con.startActivity(intent)
            }
        }
    }

    private fun handleSearchAction() = withState(timelineViewModel) { state ->
        navigator.openSearch(
                context = requireContext(),
                roomId = timelineArgs.roomId,
                roomDisplayName = state.asyncRoomSummary()?.displayName,
                roomAvatarUrl = state.asyncRoomSummary()?.avatarUrl
        )
    }

    private fun displayDisabledIntegrationDialog() {
        MaterialAlertDialogBuilder(requireActivity())
                .setTitle(CommonStrings.disabled_integration_dialog_title)
                .setMessage(CommonStrings.disabled_integration_dialog_content)
                .setPositiveButton(CommonStrings.settings) { _, _ ->
                    navigator.openSettings(requireActivity(), PcpSettingsActivity.EXTRA_DIRECT_ACCESS_GENERAL)
                }
                .setNegativeButton(CommonStrings.action_cancel, null)
                .show()
    }

    override fun onResume() {
        super.onResume()
        itemVisibilityTracker.attach(views.timelineRecyclerView)
        notificationDrawerManager.setCurrentRoom(timelineArgs.roomId)
        notificationDrawerManager.setCurrentThread(timelineArgs.threadTimelineArgs?.rootThreadEventId)
        roomDetailPendingActionStore.data?.let { handlePendingAction(it) }
        roomDetailPendingActionStore.data = null
    }

    private fun handlePendingAction(roomDetailPendingAction: RoomDetailPendingAction) {
        when (roomDetailPendingAction) {
            RoomDetailPendingAction.DoNothing -> Unit
            is RoomDetailPendingAction.JumpToReadReceipt ->
                timelineViewModel.handle(RoomDetailAction.JumpToReadReceipt(roomDetailPendingAction.userId))
            is RoomDetailPendingAction.MentionUser ->
                messageComposerViewModel.handle(MessageComposerAction.InsertUserDisplayName(roomDetailPendingAction.userId))
            is RoomDetailPendingAction.OpenRoom ->
                handleOpenRoom(RoomDetailViewEvents.OpenRoom(roomDetailPendingAction.roomId, roomDetailPendingAction.closeCurrentRoom))
        }
    }

    override fun onPause() {
        super.onPause()
        itemVisibilityTracker.detach(views.timelineRecyclerView)
        notificationDrawerManager.setCurrentRoom(null)
        notificationDrawerManager.setCurrentThread(null)
    }

    private val emojiActivityResultLauncher = registerStartForActivityResult { activityResult ->
        if (activityResult.resultCode == Activity.RESULT_OK) {
            val eventId = EmojiReactionPickerActivity.getOutputEventId(activityResult.data)
            val reaction = EmojiReactionPickerActivity.getOutputReaction(activityResult.data)
            if (eventId != null && reaction != null) {
                timelineViewModel.handle(RoomDetailAction.SendReaction(eventId, reaction))
            }
        }
    }

    private val stickerActivityResultLauncher = registerStartForActivityResult { activityResult ->
        val data = activityResult.data ?: return@registerStartForActivityResult
        if (activityResult.resultCode == Activity.RESULT_OK) {
            WidgetActivity.getOutput(data).toModel<MessageStickerContent>()
                    ?.let { content ->
                        timelineViewModel.handle(RoomDetailAction.SendSticker(content))
                    }
        }
    }

    private val startCallActivityResultLauncher = registerForPermissionsResult { allGranted, deniedPermanently ->
        if (allGranted) {
            (timelineViewModel.pendingAction as? RoomDetailAction.StartCall)?.let {
                timelineViewModel.pendingAction = null
                timelineViewModel.handle(it)
            }
        } else {
            if (deniedPermanently) {
                activity?.onPermissionDeniedDialog(CommonStrings.denied_permission_generic)
            }
            cleanUpAfterPermissionNotGranted()
        }
    }

// PRIVATE METHODS *****************************************************************************

    private fun setupRecyclerView() {
        timelineEventController.callback = this
        timelineEventController.timeline = timelineViewModel.timeline

        layoutManager = object : LinearLayoutManager(requireContext(), RecyclerView.VERTICAL, true) {
            override fun onLayoutCompleted(state: RecyclerView.State) {
                super.onLayoutCompleted(state)
                updateJumpToReadMarkerViewVisibility()
                jumpToBottomViewVisibilityManager.maybeShowJumpToBottomViewVisibilityWithDelay()
            }
        }.apply {
            // For local rooms, pin the view's content to the top edge (the layout is reversed)
            stackFromEnd = isLocalRoom()
        }
        val stateRestorer = LayoutManagerStateRestorer(layoutManager).register()
        scrollOnNewMessageCallback = ScrollOnNewMessageCallback(layoutManager, timelineEventController)
        scrollOnHighlightedEventCallback = ScrollOnHighlightedEventCallback(views.timelineRecyclerView, layoutManager, timelineEventController)
        views.timelineRecyclerView.layoutManager = layoutManager
        views.timelineRecyclerView.itemAnimator = null
        views.timelineRecyclerView.setHasFixedSize(true)
        modelBuildListener = OnModelBuildFinishedListener {
            it.dispatchTo(stateRestorer)
            it.dispatchTo(scrollOnNewMessageCallback)
            it.dispatchTo(scrollOnHighlightedEventCallback)
        }
        timelineEventController.addModelBuildListener(modelBuildListener)
        views.timelineRecyclerView.adapter = timelineEventController.adapter

        if (pcpPreferences.swipeToReplyIsEnabled()) {
            val quickReplyHandler = object : RoomMessageTouchHelperCallback.QuickReplayHandler {
                override fun performQuickReplyOnHolder(model: EpoxyModel<*>) {
                    (model as? AbsMessageItem)?.attributes?.informationData?.let {
                        val eventId = it.eventId
                        messageComposerViewModel.handle(MessageComposerAction.EnterReplyMode(eventId))
                    }
                }

                override fun canSwipeModel(model: EpoxyModel<*>): Boolean {
                    val canSendMessage = withState(messageComposerViewModel) {
                        it.canSendMessage
                    }
                    if (!canSendMessage.boolean()) {
                        return false
                    }
                    return when (model) {
                        is MessageFileItem,
                        is MessageAudioItem,
                        is MessageVoiceItem,
                        is MessageImageVideoItem,
                        is MessageTextItem -> {
                            return (model as AbsMessageItem).attributes.informationData.sendState == SendState.SYNCED
                        }
                        else -> false
                    }
                }
            }
            val swipeCallback = RoomMessageTouchHelperCallback(requireContext(), R.drawable.ic_reply, quickReplyHandler, clock)
            val touchHelper = ItemTouchHelper(swipeCallback)
            touchHelper.attachToRecyclerView(views.timelineRecyclerView)
        }
        views.timelineRecyclerView.addGlidePreloader(
                epoxyController = timelineEventController,
                requestManager = GlideApp.with(this),
                preloader = glidePreloader { requestManager, epoxyModel: MessageImageVideoItem, _ ->
                    imageContentRenderer.createGlideRequest(
                            epoxyModel.mediaData,
                            ImageContentRenderer.Mode.THUMBNAIL,
                            requestManager as GlideRequests
                    )
                })
    }

    private fun updateJumpToReadMarkerViewVisibility() {
        if (isThreadTimeLine()) return
        viewLifecycleOwner.lifecycleScope.launch {
            withResumed {
                viewLifecycleOwner.lifecycleScope.launch {
                    val state = timelineViewModel.awaitState()
                    val showJumpToUnreadBanner = when (state.unreadState) {
                        UnreadState.Unknown,
                        UnreadState.HasNoUnread -> false
                        is UnreadState.ReadMarkerNotLoaded -> true
                        is UnreadState.HasUnread -> {
                            if (state.canShowJumpToReadMarker) {
                                val lastVisibleItem = layoutManager.findLastCompletelyVisibleItemPosition()
                                val positionOfReadMarker = withContext(Dispatchers.Default) {
                                    timelineEventController.getPositionOfReadMarker()
                                }
                                if (positionOfReadMarker == null) {
                                    false
                                } else {
                                    positionOfReadMarker > lastVisibleItem
                                }
                            } else {
                                false
                            }
                        }
                    }
                    views.jumpToReadMarkerView.isVisible = showJumpToUnreadBanner
                }
            }
        }
    }

    override fun invalidate() = withState(timelineViewModel, messageComposerViewModel) { mainState, messageComposerState ->
        invalidateOptionsMenu()
        if (mainState.asyncRoomSummary is Fail) {
            handleRoomSummaryFailure(mainState.asyncRoomSummary)
            return@withState
        }
        val summary = mainState.asyncRoomSummary()
        renderToolbar(summary)
        views.removeJitsiWidgetView.render(mainState)
        if (mainState.hasFailedSending) {
            lazyLoadedViews.failedMessagesWarningView(inflateIfNeeded = true, createFailedMessagesWarningCallback())?.isVisible = true
        } else {
            lazyLoadedViews.failedMessagesWarningView(inflateIfNeeded = false)?.isVisible = false
        }
        val inviter = mainState.asyncInviter()
        if (summary?.membership == Membership.JOIN) {
            views.jumpToBottomView.count = summary.notificationCount
            views.jumpToBottomView.drawBadge = summary.hasUnreadMessages
            timelineEventController.update(mainState)
            lazyLoadedViews.inviteView(false)?.isVisible = false

            if (mainState.tombstoneEvent == null) {
                views.composerContainer.isInvisible = !messageComposerState.isComposerVisible
                views.voiceMessageRecorderContainer.isVisible = messageComposerState.isVoiceMessageRecorderVisible
                when (messageComposerState.canSendMessage) {
                    CanSendStatus.Allowed -> {
                        NotificationAreaView.State.Hidden
                    }
                    CanSendStatus.NoPermission -> {
                        NotificationAreaView.State.NoPermissionToPost
                    }
                    is CanSendStatus.UnSupportedE2eAlgorithm -> {
                        NotificationAreaView.State.UnsupportedAlgorithm(mainState.isAllowedToSetupEncryption)
                    }
                }.let {
                    views.notificationAreaView.render(it)
                }
            } else {
                views.hideComposerViews()
                views.notificationAreaView.render(NotificationAreaView.State.Tombstone(mainState.tombstoneEvent))
            }

            if (summary.isDirect && summary.isEncrypted && summary.joinedMembersCount == 1 && summary.invitedMembersCount == 0) {
                views.hideComposerViews()
            }
        } else if (summary?.membership == Membership.INVITE && inviter != null) {
            views.hideComposerViews()
            lazyLoadedViews.inviteView(true)?.apply {
                callback = this@TimelineFragment
                isVisible = true
                render(inviter, PcpInviteView.Mode.LARGE, mainState.changeMembershipState)
                setOnClickListener(null)
            }
            Unit
        } else if (mainState.asyncInviter.complete) {
            vectorBaseActivity.finish()
        }
        updateLiveLocationIndicator(mainState.isSharingLiveLocation)
    }

    private fun handleRoomSummaryFailure(asyncRoomSummary: Fail<RoomSummary>) {
        views.roomNotFound.isVisible = true
        views.roomNotFoundText.text = when (asyncRoomSummary.error) {
            is RoomNotFound -> {
                getString(
                        CommonStrings.timeline_error_room_not_found,
                        if (pcpPreferences.developerMode()) {
                            "\nDeveloper info: $timelineArgs"
                        } else {
                            ""
                        }
                )
            }
            else -> errorFormatter.toHumanReadable(asyncRoomSummary.error)
        }
    }

    private fun updateLiveLocationIndicator(isSharingLiveLocation: Boolean) {
        views.liveLocationStatusIndicator.isVisible = isSharingLiveLocation
    }

    private fun FragmentTimelineBinding.hideComposerViews() {
        composerContainer.isVisible = false
        voiceMessageRecorderContainer.isVisible = false
    }

    private fun renderToolbar(roomSummary: RoomSummary?) {
        when {
            isLocalRoom() -> {
                views.includeRoomToolbar.roomToolbarContentView.isVisible = false
                views.includeThreadToolbar.roomToolbarThreadConstraintLayout.isVisible = false
                setupToolbar(views.roomToolbar)
                        .setTitle(CommonStrings.room_member_open_or_create_dm)
                        .allowBack(useCross = true)
            }
            isThreadTimeLine() -> {
                views.includeRoomToolbar.roomToolbarContentView.isVisible = false
                views.includeThreadToolbar.roomToolbarThreadConstraintLayout.isVisible = true
                timelineArgs.threadTimelineArgs?.let {
                    val matrixItem = MatrixItem.RoomItem(it.roomId, it.displayName, it.avatarUrl)
                    avatarRenderer.render(matrixItem, views.includeThreadToolbar.roomToolbarThreadImageView)
                    views.includeThreadToolbar.roomToolbarThreadShieldImageView.render(it.roomEncryptionTrustLevel)
                    views.includeThreadToolbar.roomToolbarThreadSubtitleTextView.text = it.displayName
                }
                views.includeThreadToolbar.roomToolbarThreadTitleTextView.text = resources.getText(CommonStrings.thread_timeline_title)
            }
            else -> {
                views.includeRoomToolbar.roomToolbarContentView.isVisible = true
                views.includeThreadToolbar.roomToolbarThreadConstraintLayout.isVisible = false
                if (roomSummary == null) {
                    views.includeRoomToolbar.roomToolbarContentView.isClickable = false
                } else {
                    views.includeRoomToolbar.roomToolbarContentView.isClickable = roomSummary.membership == Membership.JOIN
                    views.includeRoomToolbar.roomToolbarTitleView.text = roomSummary.displayName
                    avatarRenderer.render(roomSummary.toMatrixItem(), views.includeRoomToolbar.roomToolbarAvatarImageView)
                    val showPresence = roomSummary.isDirect
                    views.includeRoomToolbar.roomToolbarPresenceImageView.render(showPresence, roomSummary.directUserPresence)
                    val shieldView = if (showPresence) views.includeRoomToolbar.roomToolbarTitleShield else views.includeRoomToolbar.roomToolbarAvatarShield
                    shieldView.render(roomSummary.roomEncryptionTrustLevel)
                    views.includeRoomToolbar.roomToolbarPublicImageView.isVisible = roomSummary.isPublic && !roomSummary.isDirect
                }
            }
        }
    }

    private fun displayE2eError(withHeldCode: WithHeldCode?) {
        val msgId = when (withHeldCode) {
            WithHeldCode.BLACKLISTED -> CommonStrings.crypto_error_withheld_blacklisted
            WithHeldCode.UNVERIFIED -> CommonStrings.crypto_error_withheld_unverified
            WithHeldCode.UNAUTHORISED,
            WithHeldCode.UNAVAILABLE -> CommonStrings.crypto_error_withheld_generic
            else -> CommonStrings.notice_crypto_unable_to_decrypt_friendly_desc
        }
        MaterialAlertDialogBuilder(requireActivity())
                .setMessage(msgId)
                .setPositiveButton(CommonStrings.ok, null)
                .show()
    }

    private fun promptReasonToReportContent(action: EventSharedAction.ReportContentCustom) {
        val inflater = requireActivity().layoutInflater
        val layout = inflater.inflate(R.layout.dialog_report_content, null)
        val views = DialogReportContentBinding.bind(layout)

        MaterialAlertDialogBuilder(requireActivity())
                .setTitle(CommonStrings.report_content_custom_title)
                .setView(layout)
                .setPositiveButton(CommonStrings.report_content_custom_submit) { _, _ ->
                    val reason = views.dialogReportContentInput.text.toString()
                    timelineViewModel.handle(RoomDetailAction.ReportContent(action.eventId, action.senderId, reason))
                }
                .setNegativeButton(CommonStrings.action_cancel, null)
                .show()
    }

    private fun promptConfirmationToRedactEvent(action: EventSharedAction.Redact) {
        ConfirmationDialogBuilder
                .show(
                        activity = requireActivity(),
                        askForReason = action.askForReason,
                        confirmationRes = action.dialogDescriptionRes,
                        positiveRes = CommonStrings.action_remove,
                        reasonHintRes = CommonStrings.delete_event_dialog_reason_hint,
                        titleRes = action.dialogTitleRes
                ) { reason ->
                    timelineViewModel.handle(RoomDetailAction.RedactAction(action.eventId, reason))
                }
    }

    private fun displayRoomDetailActionFailure(result: RoomDetailViewEvents.ActionFailure) {
        @StringRes val titleResId = when (result.action) {
            RoomDetailAction.VoiceBroadcastAction.Recording.Start -> CommonStrings.error_voice_broadcast_unauthorized_title
            else -> CommonStrings.dialog_title_error
        }
        MaterialAlertDialogBuilder(requireActivity())
                .setTitle(titleResId)
                .setMessage(errorFormatter.toHumanReadable(result.throwable))
                .setPositiveButton(CommonStrings.ok, null)
                .show()
    }

    private fun displayRoomDetailActionSuccess(result: RoomDetailViewEvents.ActionSuccess) {
        when (val data = result.action) {
            is RoomDetailAction.ReportContent -> {
                when {
                    data.spam -> {
                        MaterialAlertDialogBuilder(
                                requireActivity(),
                                chat.pcp.lib.ui.styles.R.style.ThemeOverlay_Vector_MaterialAlertDialog_NegativeDestructive
                        )
                                .setTitle(CommonStrings.content_reported_as_spam_title)
                                .setMessage(CommonStrings.content_reported_as_spam_content)
                                .setPositiveButton(CommonStrings.ok, null)
                                .setNegativeButton(CommonStrings.block_user) { _, _ ->
                                    timelineViewModel.handle(RoomDetailAction.IgnoreUser(data.senderId))
                                }
                                .show()
                    }
                    data.inappropriate -> {
                        MaterialAlertDialogBuilder(
                                requireActivity(),
                                chat.pcp.lib.ui.styles.R.style.ThemeOverlay_Vector_MaterialAlertDialog_NegativeDestructive
                        )
                                .setTitle(CommonStrings.content_reported_as_inappropriate_title)
                                .setMessage(CommonStrings.content_reported_as_inappropriate_content)
                                .setPositiveButton(CommonStrings.ok, null)
                                .setNegativeButton(CommonStrings.block_user) { _, _ ->
                                    timelineViewModel.handle(RoomDetailAction.IgnoreUser(data.senderId))
                                }
                                .show()
                    }
                    data.user -> {
                        MaterialAlertDialogBuilder(
                                requireActivity(),
                                chat.pcp.lib.ui.styles.R.style.ThemeOverlay_Vector_MaterialAlertDialog_NegativeDestructive
                        )
                                .setTitle(CommonStrings.user_reported_as_inappropriate_title)
                                .setMessage(CommonStrings.user_reported_as_inappropriate_content)
                                .setPositiveButton(CommonStrings.ok, null)
                                .setNegativeButton(CommonStrings.block_user) { _, _ ->
                                    timelineViewModel.handle(RoomDetailAction.IgnoreUser(data.senderId))
                                }
                                .show()
                    }
                    else -> {
                        MaterialAlertDialogBuilder(
                                requireActivity(),
                                chat.pcp.lib.ui.styles.R.style.ThemeOverlay_Vector_MaterialAlertDialog_NegativeDestructive
                        )
                                .setTitle(CommonStrings.content_reported_title)
                                .setMessage(CommonStrings.content_reported_content)
                                .setPositiveButton(CommonStrings.ok, null)
                                .setNegativeButton(CommonStrings.block_user) { _, _ ->
                                    timelineViewModel.handle(RoomDetailAction.IgnoreUser(data.senderId))
                                }
                                .show()
                    }
                }
            }
            is RoomDetailAction.RequestVerification -> {
                Timber.v("## SAS RequestVerification action $data")
                UserVerificationBottomSheet.verifyUser(
                        timelineArgs.roomId,
                        data.userId
                ).show(parentFragmentManager, "REQ")
            }
            is RoomDetailAction.AcceptVerificationRequest -> {
                Timber.v("## SAS AcceptVerificationRequest action $data")
                UserVerificationBottomSheet.verifyUser(
                        data.otherUserId,
                        data.transactionId
                ).show(parentFragmentManager, "REQ")
            }
            is RoomDetailAction.ResumeVerification -> {
                val otherUserId = data.otherUserId ?: return
                UserVerificationBottomSheet.verifyUser(
//                        roomId = timelineArgs.roomId,
                        otherUserId = otherUserId,
                        transactionId = data.transactionId,
                ).show(parentFragmentManager, "REQ")
            }
            else -> Unit
        }
    }

    // TimelineEventController.Callback ************************************************************
    override fun onUrlClicked(url: String, title: String): Boolean {
        viewLifecycleOwner.lifecycleScope.launch {
            val isManaged = permalinkHandler
                    .launch(requireActivity(), url, object : NavigationInterceptor {
                        override fun navToRoom(roomId: String?, eventId: String?, deepLink: Uri?, rootThreadEventId: String?): Boolean {
                            // Same room?
                            if (roomId != timelineArgs.roomId) return false
                            // Navigation to same room
                            if (!isThreadTimeLine()) {
                                if (rootThreadEventId != null && userPreferencesProvider.areThreadMessagesEnabled()) {
                                    // Thread link, so PermalinkHandler will handle the navigation
                                    return false
                                }
                                return if (eventId == null) {
                                    showSnackWithMessage(getString(CommonStrings.navigate_to_room_when_already_in_the_room))
                                    true
                                } else {
                                    // Highlight and scroll to this event
                                    timelineViewModel.handle(RoomDetailAction.NavigateToEvent(eventId, true))
                                    true
                                }
                            } else {
                                return if (rootThreadEventId == getRootThreadEventId() && eventId == null) {
                                    showSnackWithMessage(getString(CommonStrings.navigate_to_thread_when_already_in_the_thread))
                                    true
                                } else if (rootThreadEventId == getRootThreadEventId() && eventId != null) {
                                    // we are in the same thread
                                    timelineViewModel.handle(RoomDetailAction.NavigateToEvent(eventId, true))
                                    true
                                } else {
                                    false
                                }
                            }
                        }

                        override fun navToMemberProfile(userId: String, deepLink: Uri): Boolean {
                            openRoomMemberProfile(userId)
                            return true
                        }
                    })
            if (!isManaged) {
                when {
                    url.containsRtLOverride() -> {
                        displayUrlConfirmationDialog(
                                seenUrl = title.ensureEndsLeftToRight(),
                                actualUrl = url.filterDirectionOverrides(),
                                continueTo = url
                        )
                    }
                    title.isValidUrl() && url.isValidUrl() && URL(title).host != URL(url).host -> {
                        displayUrlConfirmationDialog(title, url)
                    }
                    else -> {
                        openUrlInExternalBrowser(requireContext(), url)
                    }
                }
            }
        }
        // In fact it is always managed
        return true
    }

    private fun displayUrlConfirmationDialog(seenUrl: String, actualUrl: String, continueTo: String = actualUrl) {
        MaterialAlertDialogBuilder(requireActivity(), chat.pcp.lib.ui.styles.R.style.ThemeOverlay_Vector_MaterialAlertDialog_NegativeDestructive)
                .setTitle(CommonStrings.external_link_confirmation_title)
                .setMessage(
                        getString(CommonStrings.external_link_confirmation_message, seenUrl, actualUrl)
                                .toSpannable()
                                .colorizeMatchingText(actualUrl, colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_tertiary))
                                .colorizeMatchingText(seenUrl, colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_tertiary))
                )
                .setPositiveButton(CommonStrings._continue) { _, _ ->
                    openUrlInExternalBrowser(requireContext(), continueTo)
                }
                .setNegativeButton(CommonStrings.action_cancel, null)
                .show()
    }

    override fun onUrlLongClicked(url: String): Boolean {
        if (url != getString(CommonStrings.edited_suffix) && url.isValidUrl()) {
            // Copy the url to the clipboard
            copyToClipboard(requireContext(), url, true, CommonStrings.link_copied_to_clipboard)
        }
        return true
    }

    override fun onEventVisible(event: TimelineEvent) {
        timelineViewModel.handle(RoomDetailAction.TimelineEventTurnsVisible(event))
    }

    override fun onEventInvisible(event: TimelineEvent) {
        timelineViewModel.handle(RoomDetailAction.TimelineEventTurnsInvisible(event))
    }

    override fun onEncryptedMessageClicked(informationData: MessageInformationData, view: View) {
        vectorBaseActivity.notImplemented("encrypted message click")
    }

    override fun onImageMessageClicked(
            messageImageContent: MessageImageInfoContent,
            mediaData: ImageContentRenderer.Data,
            view: View,
            inMemory: List<AttachmentData>
    ) {
        navigator.openMediaViewer(
                activity = requireActivity(),
                roomId = timelineArgs.roomId,
                mediaData = mediaData,
                view = view,
                inMemory = inMemory
        ) { pairs ->
            pairs.add(Pair(views.roomToolbar, ViewCompat.getTransitionName(views.roomToolbar) ?: ""))
            pairs.add(Pair(views.composerContainer, ViewCompat.getTransitionName(views.composerContainer) ?: ""))
        }
    }

    override fun onVideoMessageClicked(messageVideoContent: MessageVideoContent, mediaData: VideoContentRenderer.Data, view: View) {
        navigator.openMediaViewer(
                activity = requireActivity(),
                roomId = timelineArgs.roomId,
                mediaData = mediaData,
                view = view
        ) { pairs ->
            pairs.add(Pair(views.roomToolbar, ViewCompat.getTransitionName(views.roomToolbar) ?: ""))
            pairs.add(Pair(views.composerContainer, ViewCompat.getTransitionName(views.composerContainer) ?: ""))
        }
    }

    override fun onLoadMore(direction: Timeline.Direction) {
        timelineViewModel.handle(RoomDetailAction.LoadMoreTimelineEvents(direction))
    }

    override fun onAddMoreReaction(event: TimelineEvent) {
        openEmojiReactionPicker(event.eventId)
    }

    override fun onEventCellClicked(informationData: MessageInformationData, messageContent: Any?, view: View, isRootThreadEvent: Boolean) {
        when (messageContent) {
            is MessageVerificationRequestContent -> {
                timelineViewModel.handle(RoomDetailAction.ResumeVerification(informationData.eventId, null))
            }
            is MessageWithAttachmentContent -> {
                val action = RoomDetailAction.DownloadOrOpen(informationData.eventId, informationData.senderId, messageContent)
                timelineViewModel.handle(action)
            }
            is EncryptedEventContent -> {
                timelineViewModel.handle(RoomDetailAction.TapOnFailedToDecrypt(informationData.eventId))
            }
            is MessageLocationContent -> {
                handleShowLocationPreview(messageContent, informationData.senderId)
            }
            is MessageBeaconInfoContent -> {
                navigateToLiveLocationMap()
            }
            else -> {
                val handled = onThreadSummaryClicked(informationData.eventId, isRootThreadEvent)
                if (!handled) {
                    Timber.d("No click action defined for this message content")
                }
            }
        }
    }

    override fun onEventLongClicked(informationData: MessageInformationData, messageContent: Any?, view: View): Boolean {
        view.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS)
        val roomId = timelineArgs.roomId
        this.view?.hideKeyboard()

        MessageActionsBottomSheet
                .newInstance(roomId, informationData, isThreadTimeLine())
                .show(requireActivity().supportFragmentManager, "MESSAGE_CONTEXTUAL_ACTIONS")

        return true
    }

    private fun handleCancelSend(action: EventSharedAction.Cancel) {
        if (action.force) {
            timelineViewModel.handle(RoomDetailAction.CancelSend(action.event, true))
        } else {
            MaterialAlertDialogBuilder(requireContext())
                    .setTitle(CommonStrings.dialog_title_confirmation)
                    .setMessage(getString(CommonStrings.event_status_cancel_sending_dialog_message))
                    .setNegativeButton(CommonStrings.no, null)
                    .setPositiveButton(CommonStrings.yes) { _, _ ->
                        timelineViewModel.handle(RoomDetailAction.CancelSend(action.event, false))
                    }
                    .show()
        }
    }

    override fun onThreadSummaryClicked(eventId: String, isRootThreadEvent: Boolean): Boolean {
        return if (pcpPreferences.areThreadMessagesEnabled() && isRootThreadEvent && !isThreadTimeLine()) {
            navigateToThreadTimeline(eventId)
            true
        } else {
            false
        }
    }

    override fun onAvatarClicked(informationData: MessageInformationData) {
        // roomDetailViewModel.handle(RoomDetailAction.RequestVerification(informationData.userId))
        openRoomMemberProfile(informationData.senderId)
    }

    private fun openRoomMemberProfile(userId: String) {
        navigator.openRoomMemberProfile(userId = userId, roomId = timelineArgs.roomId, context = requireActivity())
    }

    override fun onMemberNameClicked(informationData: MessageInformationData) {
        messageComposerViewModel.handle(MessageComposerAction.InsertUserDisplayName(informationData.senderId))
    }

    override fun onClickOnReactionPill(informationData: MessageInformationData, reaction: String, on: Boolean) {
        if (on) {
            // we should test the current real state of reaction on this event
            timelineViewModel.handle(RoomDetailAction.SendReaction(informationData.eventId, reaction))
        } else {
            // I need to redact a reaction
            timelineViewModel.handle(RoomDetailAction.UndoReaction(informationData.eventId, reaction))
        }
    }

    override fun onLongClickOnReactionPill(informationData: MessageInformationData, reaction: String) {
        ViewReactionsBottomSheet.newInstance(timelineArgs.roomId, informationData)
                .show(requireActivity().supportFragmentManager, "DISPLAY_REACTIONS")
    }

    override fun onEditedDecorationClicked(informationData: MessageInformationData) {
        ViewEditHistoryBottomSheet.newInstance(timelineArgs.roomId, informationData)
                .show(requireActivity().supportFragmentManager, "DISPLAY_EDITS")
    }

    override fun onTimelineItemAction(itemAction: RoomDetailAction) {
        timelineViewModel.handle(itemAction)
    }

    override fun getPreviewUrlRetriever() = timelineViewModel.previewUrlRetriever

    override fun onRoomCreateLinkClicked(url: String) {
        viewLifecycleOwner.lifecycleScope.launch {
            withResumed {
                viewLifecycleOwner.lifecycleScope.launch {
                    permalinkHandler
                            .launch(requireActivity(), url, object : NavigationInterceptor {
                                override fun navToRoom(roomId: String?, eventId: String?, deepLink: Uri?, rootThreadEventId: String?): Boolean {
                                    requireActivity().finish()
                                    return false
                                }
                            })
                }
            }
        }
    }

    override fun onReadReceiptsClicked(readReceipts: List<ReadReceiptData>) {
        DisplayReadReceiptsBottomSheet.newInstance(readReceipts)
                .show(requireActivity().supportFragmentManager, "DISPLAY_READ_RECEIPTS")
    }

    override fun onReadMarkerVisible() {
        timelineViewModel.handle(RoomDetailAction.EnterTrackingUnreadMessagesState)
    }

    override fun onPreviewUrlClicked(url: String) {
        onUrlClicked(url, url)
    }

    override fun onPreviewUrlCloseClicked(eventId: String, url: String) {
        timelineViewModel.handle(RoomDetailAction.DoNotShowPreviewUrlFor(eventId, url))
    }

    override fun onPreviewUrlImageClicked(sharedView: View?, mxcUrl: String?, title: String?) {
        navigator.openBigImageViewer(requireActivity(), sharedView, mxcUrl, title)
    }

    override fun onVoiceControlButtonClicked(eventId: String, messageAudioContent: MessageAudioContent) {
        messageComposerViewModel.handle(MessageComposerAction.PlayOrPauseVoicePlayback(eventId, messageAudioContent))
    }

    override fun onVoiceWaveformTouchedUp(eventId: String, duration: Int, percentage: Float) {
        messageComposerViewModel.handle(MessageComposerAction.VoiceWaveformTouchedUp(eventId, duration, percentage))
    }

    override fun onVoiceWaveformMovedTo(eventId: String, duration: Int, percentage: Float) {
        messageComposerViewModel.handle(MessageComposerAction.VoiceWaveformMovedTo(eventId, duration, percentage))
    }

    override fun onAudioSeekBarMovedTo(eventId: String, duration: Int, percentage: Float) {
        messageComposerViewModel.handle(MessageComposerAction.AudioSeekBarMovedTo(eventId, duration, percentage))
    }

    private fun onShareActionClicked(action: EventSharedAction.Share) {
        when (action.messageContent) {
            is MessageTextContent -> shareText(requireContext(), action.messageContent.body)
            is MessageLocationContent -> {
                action.messageContent.toLocationData()?.let {
                    openLocation(requireActivity(), it.latitude, it.longitude)
                }
            }
            is MessageWithAttachmentContent -> {
                lifecycleScope.launch {
                    val result = runCatching { session.fileService().downloadFile(messageContent = action.messageContent) }
                    if (!isAdded) return@launch
                    result.fold(
                            { shareMedia(requireContext(), it, getMimeTypeFromUri(requireContext(), it.toUri())) },
                            { showErrorInSnackbar(it) }
                    )
                }
            }
        }
    }

    private val saveActionActivityResultLauncher = registerForPermissionsResult { allGranted, deniedPermanently ->
        if (allGranted) {
            sharedActionViewModel.pendingAction?.let {
                handleActions(it)
                sharedActionViewModel.pendingAction = null
            }
        } else {
            if (deniedPermanently) {
                activity?.onPermissionDeniedDialog(CommonStrings.denied_permission_generic)
            }
            cleanUpAfterPermissionNotGranted()
        }
    }

    private fun cleanUpAfterPermissionNotGranted() {
        // Reset all pending data
        timelineViewModel.pendingAction = null
    }

    private fun onSaveActionClicked(action: EventSharedAction.Save) {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.Q &&
                !checkPermissions(PERMISSIONS_FOR_WRITING_FILES, requireActivity(), saveActionActivityResultLauncher)) {
            sharedActionViewModel.pendingAction = action
            return
        }
        session.coroutineScope.launch {
            val result = runCatching { session.fileService().downloadFile(messageContent = action.messageContent) }
            if (!isAdded) return@launch
            result.mapCatching {
                saveMedia(
                        context = requireContext(),
                        file = it,
                        title = action.messageContent.body,
                        mediaMimeType = action.messageContent.mimeType ?: getMimeTypeFromUri(requireContext(), it.toUri()),
                        notificationUtils = notificationUtils,
                        currentTimeMillis = clock.epochMillis()
                )
            }
                    .onFailure {
                        if (!isAdded) return@onFailure
                        showErrorInSnackbar(it)
                    }
        }
    }

    private fun handleActions(action: EventSharedAction) {
        when (action) {
            is EventSharedAction.OpenUserProfile -> {
                openRoomMemberProfile(action.userId)
            }
            is EventSharedAction.AddReaction -> {
                openEmojiReactionPicker(action.eventId)
            }
            is EventSharedAction.ViewReactions -> {
                ViewReactionsBottomSheet.newInstance(timelineArgs.roomId, action.messageInformationData)
                        .show(requireActivity().supportFragmentManager, "DISPLAY_REACTIONS")
            }
            is EventSharedAction.Copy -> {
                // I need info about the current selected message :/
                copyToClipboard(requireContext(), action.content, false)
                showSnackWithMessage(getString(CommonStrings.copied_to_clipboard))
            }
            is EventSharedAction.Redact -> {
                promptConfirmationToRedactEvent(action)
            }
            is EventSharedAction.Share -> {
                onShareActionClicked(action)
            }
            is EventSharedAction.Save -> {
                onSaveActionClicked(action)
            }
            is EventSharedAction.ViewEditHistory -> {
                onEditedDecorationClicked(action.messageInformationData)
            }
            is EventSharedAction.ViewSource -> {
                JSonViewerDialog.newInstance(
                        action.content,
                        -1,
                        createJSonViewerStyleProvider(colorProvider)
                ).show(childFragmentManager, "JSON_VIEWER")
            }
            is EventSharedAction.ViewDecryptedSource -> {
                JSonViewerDialog.newInstance(
                        action.content,
                        -1,
                        createJSonViewerStyleProvider(colorProvider)
                ).show(childFragmentManager, "JSON_VIEWER")
            }
            is EventSharedAction.QuickReact -> {
                // eventId,ClickedOn,Add
                timelineViewModel.handle(RoomDetailAction.UpdateQuickReactAction(action.eventId, action.clickedOn, action.add))
            }
            is EventSharedAction.Edit -> {
                if (action.eventType in EventType.POLL_START.values) {
                    navigator.openCreatePoll(requireContext(), timelineArgs.roomId, action.eventId, PollMode.EDIT)
                } else if (withState(messageComposerViewModel) { it.isVoiceMessageIdle }) {
                    messageComposerViewModel.handle(MessageComposerAction.EnterEditMode(action.eventId))
                } else {
                    requireActivity().toast(CommonStrings.error_voice_message_cannot_reply_or_edit)
                }
            }
            is EventSharedAction.Quote -> {
                messageComposerViewModel.handle(MessageComposerAction.EnterQuoteMode(action.eventId))
            }
            is EventSharedAction.Reply -> {
                if (withState(messageComposerViewModel) { it.isVoiceMessageIdle }) {
                    messageComposerViewModel.handle(MessageComposerAction.EnterReplyMode(action.eventId))
                } else {
                    requireActivity().toast(CommonStrings.error_voice_message_cannot_reply_or_edit)
                }
            }
            is EventSharedAction.ReplyInThread -> {
                if (withState(messageComposerViewModel) { it.isVoiceMessageIdle }) {
                    onReplyInThreadClicked(action)
                } else {
                    requireActivity().toast(CommonStrings.error_voice_message_cannot_reply_or_edit)
                }
            }
            is EventSharedAction.ViewInRoom -> {
                if (withState(messageComposerViewModel) { it.isVoiceMessageIdle }) {
                    handleViewInRoomAction()
                } else {
                    requireActivity().toast(CommonStrings.error_voice_message_cannot_reply_or_edit)
                }
            }
            is EventSharedAction.CopyPermalink -> {
                val permalink = permalinkFactory.createPermalink(timelineArgs.roomId, action.eventId)
                copyToClipboard(requireContext(), permalink, false)
                showSnackWithMessage(getString(CommonStrings.copied_to_clipboard))
            }
            is EventSharedAction.Resend -> {
                timelineViewModel.handle(RoomDetailAction.ResendMessage(action.eventId))
            }
            is EventSharedAction.Remove -> {
                timelineViewModel.handle(RoomDetailAction.RemoveFailedEcho(action.eventId))
            }
            is EventSharedAction.Cancel -> {
                handleCancelSend(action)
            }
            is EventSharedAction.ReportContentSpam -> {
                timelineViewModel.handle(
                        RoomDetailAction.ReportContent(
                                action.eventId, action.senderId, "This message is spam", spam = true
                        )
                )
            }
            is EventSharedAction.ReportContentInappropriate -> {
                timelineViewModel.handle(
                        RoomDetailAction.ReportContent(
                                action.eventId, action.senderId, "This message is inappropriate", inappropriate = true
                        )
                )
            }
            is EventSharedAction.ReportContentCustom -> {
                promptReasonToReportContent(action)
            }
            is EventSharedAction.IgnoreUser -> {
                action.senderId?.let { askConfirmationToIgnoreUser(it) }
            }
            is EventSharedAction.ReportUser -> {
                timelineViewModel.handle(
                        RoomDetailAction.ReportContent(
                                action.eventId, action.senderId, "Reporting user ${action.senderId}", user = true
                        )
                )
            }
            is EventSharedAction.OnUrlClicked -> {
                onUrlClicked(action.url, action.title)
            }
            is EventSharedAction.OnUrlLongClicked -> {
                onUrlLongClicked(action.url)
            }
            is EventSharedAction.ReRequestKey -> {
                timelineViewModel.handle(RoomDetailAction.ReRequestKeys(action.eventId))
            }
            is EventSharedAction.UseKeyBackup -> {
                context?.let {
                    startActivity(KeysBackupRestoreActivity.intent(it))
                }
            }
            is EventSharedAction.EndPoll -> {
                askConfirmationToEndPoll(action.eventId)
            }
            is EventSharedAction.JumpToSource -> {
                timelineViewModel.handle(RoomDetailAction.NavigateToEvent(action.sourceEventId, highlight = true))
            }
            is EventSharedAction.Translate -> {
                handleTranslateAction(action)
            }
            is EventSharedAction.ReportContent -> Unit /* Not clickable */
            EventSharedAction.Separator -> Unit /* Not clickable */
        }
    }

    private fun openEmojiReactionPicker(eventId: String) {
        emojiActivityResultLauncher.launch(EmojiReactionPickerActivity.intent(requireContext(), eventId))
    }

    private fun askConfirmationToEndPoll(eventId: String) {
        MaterialAlertDialogBuilder(requireContext(), chat.pcp.lib.ui.styles.R.style.ThemeOverlay_Vector_MaterialAlertDialog)
                .setTitle(CommonStrings.end_poll_confirmation_title)
                .setMessage(CommonStrings.end_poll_confirmation_description)
                .setNegativeButton(CommonStrings.action_cancel, null)
                .setPositiveButton(CommonStrings.end_poll_confirmation_approve_button) { _, _ ->
                    timelineViewModel.handle(RoomDetailAction.EndPoll(eventId))
                }
                .show()
    }

    private fun askConfirmationToIgnoreUser(senderId: String) {
        MaterialAlertDialogBuilder(requireContext(), chat.pcp.lib.ui.styles.R.style.ThemeOverlay_Vector_MaterialAlertDialog_Destructive)
                .setTitle(CommonStrings.room_participants_action_ignore_title)
                .setMessage(CommonStrings.room_participants_action_ignore_prompt_msg)
                .setNegativeButton(CommonStrings.action_cancel, null)
                .setPositiveButton(CommonStrings.room_participants_action_ignore) { _, _ ->
                    timelineViewModel.handle(RoomDetailAction.IgnoreUser(senderId))
                }
                .show()
    }

    private fun showSnackWithMessage(message: String) {
        view?.showOptimizedSnackbar(message)
    }

    private fun showDialogWithMessage(message: String) {
        MaterialAlertDialogBuilder(requireContext())
                .setMessage(message)
                .setPositiveButton(getString(CommonStrings.ok), null)
                .show()
    }

    private fun onReplyInThreadClicked(action: EventSharedAction.ReplyInThread) {
        if (pcpPreferences.areThreadMessagesEnabled()) {
            navigateToThreadTimeline(
                    rootThreadEventId = action.eventId,
                    startsThread = action.startsThread,
                    showKeyboard = true
            )
        } else {
            displayThreadsBetaOptInDialog()
        }
    }

    /**
     * Navigate to Threads timeline for the specified rootThreadEventId
     * using the ThreadsActivity.
     */
    private fun navigateToThreadTimeline(
            rootThreadEventId: String,
            startsThread: Boolean = false,
            showKeyboard: Boolean = false,
    ) = withState(timelineViewModel) { state ->
        analyticsTracker.capture(Interaction.Name.MobileRoomThreadSummaryItem.toAnalyticsInteraction())
        context?.let {
            val roomThreadDetailArgs = ThreadTimelineArgs(
                    startsThread = startsThread,
                    roomId = timelineArgs.roomId,
                    displayName = state.asyncRoomSummary()?.displayName,
                    avatarUrl = state.asyncRoomSummary()?.avatarUrl,
                    roomEncryptionTrustLevel = state.asyncRoomSummary()?.roomEncryptionTrustLevel,
                    rootThreadEventId = rootThreadEventId,
                    showKeyboard = showKeyboard
            )
            navigator.openThread(it, roomThreadDetailArgs)
        }
    }

    private fun displayThreadsBetaOptInDialog() {
        activity?.let {
            MaterialAlertDialogBuilder(it)
                    .setTitle(CommonStrings.threads_beta_enable_notice_title)
                    .setMessage(threadsManager.getBetaEnableThreadsMessage())
                    .setCancelable(true)
                    .setNegativeButton(CommonStrings.action_not_now) { _, _ -> }
                    .setPositiveButton(CommonStrings.action_try_it_out) { _, _ ->
                        threadsManager.enableThreadsAndRestart(it)
                    }
                    .show()
                    ?.findViewById<TextView>(android.R.id.message)
                    ?.apply {
                        linksClickable = true
                        movementMethod = LinkMovementMethod.getInstance()
                    }
        }
    }

    /**
     * Navigate to Threads list for the current room
     * using the ThreadsActivity.
     */
    private fun navigateToThreadList() = withState(timelineViewModel) { state ->
        analyticsTracker.capture(Interaction.Name.MobileRoomThreadListButton.toAnalyticsInteraction())
        context?.let {
            val roomThreadDetailArgs = ThreadTimelineArgs(
                    roomId = timelineArgs.roomId,
                    displayName = state.asyncRoomSummary()?.displayName,
                    roomEncryptionTrustLevel = state.asyncRoomSummary()?.roomEncryptionTrustLevel,
                    avatarUrl = state.asyncRoomSummary()?.avatarUrl
            )
            navigator.openThreadList(it, roomThreadDetailArgs)
        }
    }

    // PcpInviteView.Callback
    override fun onAcceptInvite() {
        timelineViewModel.handle(RoomDetailAction.AcceptInvite)
    }

    override fun onRejectInvite() {
        timelineViewModel.handle(RoomDetailAction.RejectInvite)
    }

    private fun onJumpToReadMarkerClicked() = withState(timelineViewModel) {
        if (it.unreadState is UnreadState.HasUnread) {
            timelineViewModel.handle(RoomDetailAction.NavigateToEvent(it.unreadState.firstUnreadEventId, highlight = false, isFirstUnreadEvent = true))
        }
        if (it.unreadState is UnreadState.ReadMarkerNotLoaded) {
            timelineViewModel.handle(RoomDetailAction.NavigateToEvent(it.unreadState.readMarkerId, highlight = false))
        }
    }

    private fun onViewWidgetsClicked() {
        RoomWidgetsBottomSheet.newInstance()
                .show(childFragmentManager, "ROOM_WIDGETS_BOTTOM_SHEET")
    }

    private fun handleOpenElementCallWidget() = withState(timelineViewModel) { state ->
        state
                .activeRoomWidgets()
                ?.find { it.type == WidgetType.ElementCall }
                ?.also { widget ->
                    navigator.openRoomWidget(requireContext(), state.roomId, widget)
                }
    }

    private fun displayPromptToStopVoiceBroadcast() {
        ConfirmationDialogBuilder
                .show(
                        activity = requireActivity(),
                        askForReason = false,
                        confirmationRes = CommonStrings.stop_voice_broadcast_content,
                        positiveRes = CommonStrings.action_stop,
                        reasonHintRes = 0,
                        titleRes = CommonStrings.stop_voice_broadcast_dialog_title
                ) {
                    timelineViewModel.handle(RoomDetailAction.VoiceBroadcastAction.Recording.StopConfirmed)
                }
    }

    private fun revokeFilePermission(revokeFilePermission: RoomDetailViewEvents.RevokeFilePermission) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            requireContext().revokeUriPermission(
                    requireContext().applicationContext.packageName,
                    revokeFilePermission.uri,
                    Intent.FLAG_GRANT_READ_URI_PERMISSION
            )
        } else {
            requireContext().revokeUriPermission(
                    revokeFilePermission.uri,
                    Intent.FLAG_GRANT_READ_URI_PERMISSION
            )
        }
    }

    override fun onTapToReturnToCall() {
        callManager.getCurrentCall()?.let { call ->
            PcpCallActivity.newIntent(
                    context = requireContext(),
                    callId = call.callId,
                    signalingRoomId = call.signalingRoomId,
                    otherUserId = call.mxCall.opponentUserId,
                    isIncomingCall = !call.mxCall.isOutgoing,
                    isVideoCall = call.mxCall.isVideoCall,
                    mode = null
            ).let {
                startActivity(it)
            }
        }
    }

    /**
     * Returns true if the current room is a Thread room, false otherwise.
     */
    private fun isThreadTimeLine(): Boolean = withState(timelineViewModel) { it.isThreadTimeline() }

    /**
     * Returns true if the current room is a local room, false otherwise.
     */
    private fun isLocalRoom(): Boolean = withState(timelineViewModel) { it.isLocalRoom() }

    /**
     * Returns the root thread event if we are in a thread room, otherwise returns null.
     */
    fun getRootThreadEventId(): String? = withState(timelineViewModel) { it.rootThreadEventId }
}
