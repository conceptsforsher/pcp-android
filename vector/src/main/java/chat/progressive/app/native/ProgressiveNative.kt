package chat.progressive.app.native

import org.json.JSONObject
import org.json.JSONArray
import timber.log.Timber

/**
 * JNI bridge to the progressive_native C++ library.
 * Handles date validation, MSC3030 URL construction, and response parsing.
 */
object ProgressiveNative {

    private var isLoaded = false

    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                System.loadLibrary("progressive_native")
                isLoaded = true
                Timber.d("progressive_native loaded successfully")
            } catch (e: UnsatisfiedLinkError) {
                Timber.e(e, "Failed to load progressive_native")
                // Fallback: use pure Kotlin implementation
            }
        }
    }

    /**
     * Validates the date string (YYYY-MM-DD) and builds the MSC3030 URL.
     * Gates on the feature flag from C++ side.
     *
     * @param roomId Target room ID
     * @param dateString Date in YYYY-MM-DD format
     * @param serverUrl Base URL of the homeserver
     * @param accessToken User's access token
     * @param isEnabled Whether jumptodate labs setting is enabled
     *
     * @return JSON string with url, accessToken, timestamp — or {"error":"..."} on failure
     */
    @JvmStatic
    external fun nativeValidateAndBuild(
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): String

    /**
     * Parses the UTC timestamp_to_event response from the homeserver.
     *
     * @param responseBody HTTP response body
     * @param httpStatus HTTP status code
     *
     * @return JSON string with eventId — or error info on failure
     */
    @JvmStatic
    external fun nativeParseResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    /**
     * Parses a Matrix event JSON to find the source event it relates to
     * (reply, reaction, edit, thread root).
     *
     * @param eventJson Full JSON of the Matrix event
     * @param allowedTypes Comma-separated list of allowed relation types, or empty for all
     *
     * @return JSON with sourceEventId and relationType, or {"isRelation": false}
     */
    @JvmStatic
    external fun nativeParseRelation(
        eventJson: String,
        allowedTypes: String
    ): String

    // --- Export functions ---

    @JvmStatic
    external fun nativeFormatEventHtml(
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        mediaSize: String,
        relationType: String,
        isContinuation: Boolean
    ): String

    @JvmStatic
    external fun nativeFormatEventPlainText(
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        relationType: String
    ): String

    @JvmStatic
    external fun nativeBuildHtmlExport(
        roomName: String,
        roomTopic: String,
        exportDate: String,
        eventHtmls: Array<String>
    ): String

    // --- Event Cache ---

    @JvmStatic
    external fun nativeCachePut(
        eventId: String,
        senderId: String,
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        eventType: String,
        relationType: String,
        sourceEventId: String,
        sentByMe: Boolean
    )

    @JvmStatic
    external fun nativeCacheGetContext(eventId: String): String

    @JvmStatic
    external fun nativeCacheClear()

    @JvmStatic
    external fun nativeCacheSize(): Int

    // --- SQLite Event Database ---

    @JvmStatic
    external fun nativeDbOpen(dbPath: String): Boolean

    @JvmStatic
    external fun nativeDbClose()

    @JvmStatic
    external fun nativeDbInsertEvent(
        eventId: String, roomId: String, senderId: String, senderName: String,
        timestamp: String, body: String, msgType: String, eventType: String,
        relationType: String, sourceEventId: String,
        originServerTs: Long, displayIndex: Int, sentByMe: Boolean
    )

    @JvmStatic
    external fun nativeDbGetContext(eventId: String): String

    @JvmStatic
    external fun nativeDbClearRoom(roomId: String)

    @JvmStatic
    external fun nativeDbCount(): Int

    // --- Native SQLite DB (SqliteDB - richer API with room summaries, transactions) ---

    @JvmStatic external fun nativeSqliteDbOpen(dbPath: String, key: String): Boolean
    @JvmStatic external fun nativeSqliteDbClose(key: String)
    @JvmStatic external fun nativeSqliteDbInsertEvent(
        key: String, eventId: String, roomId: String, type: String, senderId: String,
        contentJson: String, originTs: Long, ageTs: Long, displayIndex: Int
    ): Boolean
    @JvmStatic external fun nativeSqliteDbInsertEventRel(
        key: String, eventId: String, roomId: String, type: String, senderId: String,
        contentJson: String, originTs: Long, ageTs: Long, displayIndex: Int,
        stateKey: String, redacts: String, relType: String, relatesToId: String
    ): Boolean
    @JvmStatic external fun nativeSqliteDbQueryEvents(key: String, roomId: String, limit: Int, offset: Int, ascending: Boolean): String
    @JvmStatic external fun nativeSqliteDbQueryEvent(key: String, eventId: String): String
    @JvmStatic external fun nativeSqliteDbDeleteEvent(key: String, eventId: String)
    @JvmStatic external fun nativeSqliteDbCountEvents(key: String, roomId: String): Int
    @JvmStatic external fun nativeSqliteDbMaxDisplayIndex(key: String, roomId: String): Int
    @JvmStatic external fun nativeSqliteDbUpsertRoom(
        key: String, roomId: String, displayName: String, avatarUrl: String,
        topic: String, membership: String, notifCount: Int, highlightCount: Int,
        lastActivityMs: Long, isDirect: Boolean, isSpace: Boolean, isFavourite: Boolean, isEncrypted: Boolean
    ): Boolean
    @JvmStatic external fun nativeSqliteDbQueryRooms(key: String): String
    @JvmStatic external fun nativeSqliteDbBeginTransaction(key: String)
    @JvmStatic external fun nativeSqliteDbCommitTransaction(key: String)
    @JvmStatic external fun nativeSqliteDbSchemaVersion(key: String): Int

    // --- Translation ---

    @JvmStatic
    external fun nativeBuildTranslateRequest(
        text: String,
        sourceLang: String,
        targetLang: String,
        apiEndpoint: String,
        apiToken: String,
        model: String
    ): String

    @JvmStatic
    external fun nativeParseTranslateResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    // --- Proxy / Tor / I2P ---

    @JvmStatic
    external fun nativeComputeProxyConfig(
        connType: Int,
        proxyType: Int,
        host: String,
        port: Int,
        username: String,
        password: String
    ): String

    // --- Yggdrasil ---

    @JvmStatic external fun nativeIsYggdrasilAddress(addr: String): Boolean
    @JvmStatic external fun nativeIsYggdrasilDomain(host: String): Boolean
    @JvmStatic external fun nativeBuildYggHomeserverUrl(addr: String, port: Int, tls: Boolean): String
    @JvmStatic external fun nativeRewriteHomeserverUrl(originalUrl: String, yggAddr: String): String

    // --- Markdown ---

    @JvmStatic external fun nativeMarkdownToHtml(markdown: String, enableTables: Boolean): String
    @JvmStatic external fun nativeParseMarkdownTable(tableBlock: String, withScroll: Boolean): String

    // --- Event Relations ---

    @JvmStatic external fun nativeIsReply(contentJson: String): Boolean
    @JvmStatic external fun nativeIsEdit(contentJson: String): Boolean
    @JvmStatic external fun nativeIsReaction(contentJson: String): Boolean
    @JvmStatic external fun nativeIsThreadRoot(contentJson: String): Boolean
    @JvmStatic external fun nativeExtractThreadRoot(contentJson: String): String
    @JvmStatic external fun nativeExtractReplySource(contentJson: String): String
    @JvmStatic external fun nativeExtractEditSource(contentJson: String): String
    @JvmStatic external fun nativeBuildReplyRelationWithThread(eventId: String, threadRoot: String): String

    // --- Room Content Parsers ---

    @JvmStatic external fun nativeParseRoomNameContent(contentJson: String): String
    @JvmStatic external fun nativeParseRoomTopicContent(contentJson: String): String
    @JvmStatic external fun nativeParseRoomAvatarContent(contentJson: String): String

    // --- Connection Monitor ---

    @JvmStatic external fun nativeGetBannerColor(downtimeMs: Long): String

    // --- Content Guard ---

    @JvmStatic external fun nativeCountEmojis(text: String): Int
    @JvmStatic external fun nativeCountUniqueEmojis(text: String): Int
    @JvmStatic external fun nativeFormatMediaCollapseLabel(count: Int): String
    @JvmStatic external fun nativeIsEmojiCodePoint(codepoint: Int): Boolean

    // --- MIME Normalization ---

    @JvmStatic external fun nativeNormalizeMimeType(mimeType: String): String

    // --- Room State Parse ---

    @JvmStatic external fun nativeParseJoinRules(contentJson: String): String
    @JvmStatic external fun nativeParseHistoryVisibility(contentJson: String): String
    @JvmStatic external fun nativeParseGuestAccess(contentJson: String): String

    // --- Push Rules ---

    @JvmStatic external fun nativeIsKnownPushRuleKind(kind: String): Boolean
    @JvmStatic external fun nativeGetRuleKindDescription(kind: String, enabled: Boolean): String
    @JvmStatic external fun nativeIsMsc3061SharedKey(roomKeyContentJson: String): Boolean
    @JvmStatic external fun nativeFormatMsc3061Status(isShared: Boolean, visibilitySetting: String): String
    @JvmStatic external fun nativeCanShareHistory(roomVisibility: String): Boolean

    // --- Poll Utilities ---

    @JvmStatic external fun nativeGeneratePollOptionId(): String

    // --- Identity Utilities ---

    @JvmStatic external fun nativeDisambiguateName(displayName: String, mxid: String): String
    @JvmStatic external fun nativeGetIdentityInitials(displayName: String): String
    @JvmStatic external fun nativeIsCanonicalAlias(alias: String, expectedRoomId: String): Boolean
    @JvmStatic external fun nativeSuggestAliases(roomName: String): String

    // --- Presence / Backup / Cross-Signing (JSON round-trip) ---

    @JvmStatic external fun nativeParsePresenceInfo(userId: String, apiResponseJson: String): String
    @JvmStatic external fun nativeParseBackupInfo(apiResponseJson: String): String
    @JvmStatic external fun nativeParseCrossSigningStatus(accountDataJson: String, userId: String): String
    @JvmStatic external fun nativeParseKeyBackupVersion(json: String): String

    // --- Device List ---

    @JvmStatic external fun nativeParseDeviceList(apiResponseJson: String, currentDeviceId: String): String

    // --- Room Permissions ---

    @JvmStatic external fun nativeComputePermissions(powerLevelsJson: String, myUserId: String): String

    // --- Room Tombstone ---

    @JvmStatic external fun nativeParseTombstone(contentJson: String): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeParseScanResult(apiResponseJson: String): String

    // --- Server Notice ---

    @JvmStatic external fun nativeParseServerNotice(eventContentJson: String, eventId: String): String

    // --- Member List ---

    @JvmStatic external fun nativeParseMemberList(roomId: String, apiResponseJson: String, isTruncated: Boolean): String

    // --- Public Room ---

    @JvmStatic external fun nativeParsePublicRoom(json: String): String

    // --- Event Relation Info ---

    @JvmStatic external fun nativeParseEventRelation(contentJson: String): String

    // --- Public Rooms / Thread ---

    @JvmStatic external fun nativeParsePublicRoomsResponse(json: String): String
    @JvmStatic external fun nativeComputeThreadSummary(rootEventId: String, eventsJson: String): String

    // --- Relation Description ---

    @JvmStatic external fun nativeFormatRelationDescription(relType: String, eventId: String, key: String): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeBuildScanRequestBody(mxcUri: String): String

    // --- Event Content ---

    @JvmStatic external fun nativeParseEventContent(eventType: String, contentJson: String): String

    // --- Canonical JSON ---

    @JvmStatic external fun nativeCanonicalizeJson(json: String): String

    // --- Chunked Uploader ---

    @JvmStatic external fun nativeUploaderSetChunkSizeMb(mb: Int)
    @JvmStatic external fun nativeUploaderComputeChunks(fileSize: Long): Int
    @JvmStatic external fun nativeUploaderGetChunkInfo(index: Int): String
    @JvmStatic external fun nativeUploaderContentRange(index: Int): String
    @JvmStatic external fun nativeUploaderAdvance()
    @JvmStatic external fun nativeUploaderCancel()
    @JvmStatic external fun nativeUploaderReset()
    @JvmStatic external fun nativeUploaderProgress(): String
    @JvmStatic external fun nativeSuggestChunkSizeMb(fileSize: Long): Int

    // --- Thread List ---

    @JvmStatic external fun nativeBuildThreadListJson(eventsJson: String): String

    // --- Thread Unread Counter ---

    @JvmStatic external fun nativeComputeThreadUnreadCount(eventIdsJson: String, readReceiptId: String, highlightIdsJson: String): String

    // --- Sync Filter ---

    @JvmStatic external fun nativeBuildSyncFilter(includeThreads: Boolean, includePresence: Boolean, timelineLimit: Int, lazyLoadMembers: Boolean): String

    // --- Bidi Text Security ---

    @JvmStatic external fun nativeContainsRtlOverride(text: String): Boolean
    @JvmStatic external fun nativeContainsBidiOverride(text: String): Boolean
    @JvmStatic external fun nativeFilterBidiOverrides(text: String): String
    @JvmStatic external fun nativeSanitizeDisplayText(text: String): String

    // --- Read Receipts ---

    @JvmStatic external fun nativeFormatReceiptAccessibility(receiptsJson: String, overflowCount: Int): String
    @JvmStatic external fun nativeFormatOverflowLabel(count: Int): String

    // --- Space Hierarchy ---

    @JvmStatic external fun nativeSearchSpaceChildren(childrenJson: String, query: String): String

    // --- 3PID / Presence ---

    @JvmStatic external fun nativeParseThreePid(input: String): String
    @JvmStatic external fun nativeFormatPresenceAggregation(userNamesJson: String, maxNames: Int): String

    // --- Reaction / Poll ---

    @JvmStatic external fun nativeFormatReactionAggregation(key: String, count: Int, reactorsJson: String): String
    @JvmStatic external fun nativeTrackPollResponse(optionId: String, userId: String): String

    // --- Audio / Voice ---

    @JvmStatic external fun nativeIsSupportedAudioType(mimeType: String): Boolean
    @JvmStatic external fun nativeFormatPositionInfo(positionMs: Long, durationMs: Long): String

    // --- Direct Messages ---

    @JvmStatic external fun nativeParseDirectMessageMap(json: String): String

    // --- Edit History ---

    @JvmStatic external fun nativeGetEditCountBadge(editCount: Int): String
    @JvmStatic external fun nativeComputeEditDiffSummary(oldBody: String, newBody: String): String

    // --- Notification State ---

    @JvmStatic external fun nativeComputeNotificationState(roomJson: String): String

    // --- Room List Search ---

    @JvmStatic external fun nativeSearchRoomList(roomsJson: String, query: String): String

    // --- Event Classifier ---

    @JvmStatic external fun nativeIsStateEvent(eventType: String): Boolean

    // --- Poll Results ---

    @JvmStatic external fun nativeComputePollResults(pollJson: String): String

    // --- Location Sharing ---

    @JvmStatic external fun nativeLocationStartSession(sessionId: String, roomId: String, userId: String, intervalSec: Int): String
    @JvmStatic external fun nativeLocationStopSession(sessionId: String)
    @JvmStatic external fun nativeLocationIsDue(sessionId: String): Boolean
    @JvmStatic external fun nativeLocationExportJson(): String

    // --- AI Agent ---

    @JvmStatic external fun nativeAgentHasToolCalls(llmResponse: String): Boolean
    @JvmStatic external fun nativeAgentExtractTextAnswer(llmResponse: String): String
    @JvmStatic external fun nativeAgentGetToolsSchema(): String

    // --- Notification Formatting ---

    @JvmStatic external fun nativeFormatThreadNotificationCount(threadCount: Int, highlightCount: Int): String
    @JvmStatic external fun nativeFormatUnreadCounter(count: Int): String

    // --- Push Notification Evaluator ---

    @JvmStatic external fun nativeEvaluatePushNotification(eventJson: String, rulesJson: String, myDisplayName: String, myUserId: String): String

    // --- Room Upgrade ---

    @JvmStatic external fun nativeProcessRoomUpgrade(tombstoneEventJson: String): String

    // --- Redaction ---

    @JvmStatic external fun nativeFormatRedactionNotice(reason: String, redactedBySelf: Boolean, isStateEvent: Boolean): String

    // --- Key Backup ---

    @JvmStatic external fun nativeValidateAndFormatRecoveryKey(rawKey: String): String

    // --- Member / Call Notices ---

    @JvmStatic external fun nativeFormatMemberNotice(membership: String, prevMembership: String, senderId: String, senderName: String, targetId: String, targetName: String, reason: String, isDirect: Boolean, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatCallNotice(eventType: String, isVideo: Boolean, senderName: String, sentBySelf: Boolean): String
    @JvmStatic external fun nativeAnnotateEdited(body: String, isEdited: Boolean): String

    // --- Room State Notices ---

    @JvmStatic external fun nativeFormatRoomNameNotice(senderName: String, newName: String, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomTopicNotice(senderName: String, newTopic: String, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomAvatarNotice(senderName: String, isRemoved: Boolean, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomCreateNotice(senderName: String, predecessorRoomId: String, isDirect: Boolean, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomTombstoneNotice(senderName: String, replacementRoom: String, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomEncryptionNotice(senderName: String, isEnabled: Boolean, sentBySelf: Boolean): String

    // --- Power Level Diff ---

    @JvmStatic external fun nativeFormatPowerLevelDiff(senderName: String, oldLevelsJson: String, newLevelsJson: String, userNamesJson: String, sentBySelf: Boolean): String

    // --- Poll Validation ---

    @JvmStatic external fun nativeIsValidPollQuestion(question: String): Boolean

    // --- Room Uploads ---

    @JvmStatic external fun nativeIsStickerEvent(eventType: String): Boolean
    @JvmStatic external fun nativeHasAttachmentUrl(decryptedContentJson: String): Boolean
    @JvmStatic external fun nativeCreateUploadsFilterJson(numberOfEvents: Int): String

    // --- Matrix Error ---

    @JvmStatic external fun nativeGetErrorDescription(errorCode: String): String
    @JvmStatic external fun nativeIsPasswordError(errorCode: String): Boolean
    @JvmStatic external fun nativeGetAllErrorCodes(): String
    @JvmStatic external fun nativeIsRateLimitError(errorJson: String): Boolean
    @JvmStatic external fun nativeGetRetryAfterMs(errorJson: String): Long
    @JvmStatic external fun nativeIsSoftLogout(errorJson: String): Boolean
    @JvmStatic external fun nativeNeedsConsent(errorJson: String): Boolean
    @JvmStatic external fun nativeIsUserDeactivated(errorJson: String): Boolean

    // --- Error Classification (login/auth flow) ---

    @JvmStatic external fun nativeErrorIsTokenError(errorCode: String): Boolean
    @JvmStatic external fun nativeErrorShouldBeRetried(errorCode: String, httpCode: Int, isNetworkError: Boolean): Boolean
    @JvmStatic external fun nativeErrorIsInvalidUsername(errorCode: String, errorMessage: String): Boolean
    @JvmStatic external fun nativeErrorIsInvalidPassword(errorCode: String, errorMessage: String): Boolean
    @JvmStatic external fun nativeErrorIsWeakPassword(errorCode: String): Boolean
    @JvmStatic external fun nativeErrorIsLoginEmailUnknown(errorCode: String, errorMessage: String): Boolean
    @JvmStatic external fun nativeErrorIsHomeserverUnavailable(isNetworkError: Boolean, isUnknownHost: Boolean): Boolean
    @JvmStatic external fun nativeErrorIsRegistrationAvailability(errorCode: String, httpCode: Int): Boolean
    @JvmStatic external fun nativeClassifyError(errorCode: String, httpCode: Int, errorMessage: String, isNetworkError: Boolean, isUnknownHost: Boolean): String

    // --- Server ACL ---

    @JvmStatic external fun nativeWildcardMatch(pattern: String, value: String): Boolean
    @JvmStatic external fun nativeIsServerAllowed(serverName: String, aclJson: String): Boolean
    @JvmStatic external fun nativeCanJoinRoom(joinRulesJson: String, isInvited: Boolean, isMember: Boolean, isMemberOfAllowedRoom: Boolean): Boolean

    // --- Notification Formatter ---

    @JvmStatic external fun nativeFormatImageNotification(senderName: String): String
    @JvmStatic external fun nativeFormatFileNotification(senderName: String, fileName: String): String
    @JvmStatic external fun nativeFormatVideoNotification(senderName: String): String
    @JvmStatic external fun nativeFormatAudioNotification(senderName: String, isVoice: Boolean): String
    @JvmStatic external fun nativeFormatInviteNotification(inviterName: String, roomName: String): String
    @JvmStatic external fun nativeFormatRoomNotification(messageCount: Int, roomName: String): String
    @JvmStatic external fun nativeFormatStickerNotification(senderName: String): String
    @JvmStatic external fun nativeFormatLocationNotification(senderName: String): String
    @JvmStatic external fun nativeFormatPollNotification(senderName: String, isStart: Boolean): String

    // --- Raw Service ---

    @JvmStatic external fun nativeCacheKeyForUrl(url: String): String

    // --- Lightweight Settings ---

    @JvmStatic external fun nativeGetSettingBool(settingsJson: String, key: String, defaultVal: Boolean): Boolean
    @JvmStatic external fun nativeSetSettingBool(settingsJson: String, key: String, value: Boolean): String
    @JvmStatic external fun nativeGetSettingString(settingsJson: String, key: String, defaultVal: String): String
    @JvmStatic external fun nativeSetSettingString(settingsJson: String, key: String, value: String): String

    // --- HTTP Client ---

    @JvmStatic external fun nativeParseUrl(url: String): String

    // --- Account Export ---

    @JvmStatic external fun nativeEncryptAccount(
        userId: String, token: String, refreshToken: String,
        homeServer: String, deviceId: String, deviceName: String,
        displayName: String, avatarUrl: String,
        includeCache: Boolean, passphrase: String
    ): String

    @JvmStatic external fun nativeDecryptAccount(encrypted: String, passphrase: String): String

    // --- Audio ---

    @JvmStatic external fun nativeFormatDuration(ms: Long): String
    @JvmStatic external fun nativeComputeProgress(posMs: Long, durMs: Long): Float
    @JvmStatic external fun nativeIsSupportedAudio(mimeType: String): Boolean

    // --- Media Filter ---

    @JvmStatic external fun nativeGetFileExtension(fileName: String, mimeType: String): String
    @JvmStatic external fun nativeIsValidMxcUri(uri: String): Boolean

    // --- Content Filter ---

    @JvmStatic external fun nativeKeywordFilterLoad(raw: String)
    @JvmStatic external fun nativeKeywordFilterCheck(text: String): String
    @JvmStatic external fun nativeKeywordFilterExport(): String
    @JvmStatic external fun nativeKeywordFilterCount(): Int
    @JvmStatic external fun nativeKeywordFilterClear()

    @JvmStatic external fun nativeShouldBlockImage(
        blockAll: Boolean, allowAvatars: Boolean, allowStickers: Boolean, allowEmoji: Boolean,
        mxcUrl: String, imageType: String
    ): Boolean

    // --- Network Stats ---

    @JvmStatic external fun nativeNetStatsStart(url: String, method: String): Int
    @JvmStatic external fun nativeNetStatsEnd(requestId: Int, statusCode: Int, bytesSent: Long, bytesReceived: Long, error: String)
    @JvmStatic external fun nativeNetStatsToJson(): String
    @JvmStatic external fun nativeNetStatsToText(): String
    @JvmStatic external fun nativeNetStatsClear()

    // --- Masquerade ---

    @JvmStatic external fun nativeIsValidMasqueradeName(name: String): Boolean
    @JvmStatic external fun nativeGetSuggestedMasqueradeNames(): String
    @JvmStatic external fun nativeIsValidIconAlias(alias: String): Boolean
    @JvmStatic external fun nativeBuildMasqueradeAlias(baseAlias: String, iconName: String): String

    // --- User Mask ---

    @JvmStatic external fun nativeUserMaskSet(mxid: String, displayName: String, avatarUrl: String, overrideMxid: String)
    @JvmStatic external fun nativeUserMaskRemove(mxid: String)
    @JvmStatic external fun nativeUserMaskResolveName(mxid: String, originalName: String): String
    @JvmStatic external fun nativeUserMaskResolveAvatar(mxid: String, originalUrl: String): String
    @JvmStatic external fun nativeUserMaskExportJson(): String
    @JvmStatic external fun nativeUserMaskImportJson(json: String)
    @JvmStatic external fun nativeIsValidMxid(mxid: String): Boolean
    @JvmStatic external fun nativeUserMaskClear()
    @JvmStatic external fun nativeUserMaskCount(): Int

    // --- Chunked Upload ---

    @JvmStatic external fun nativeUploaderGetChunkJson(index: Int): String
    @JvmStatic external fun nativeUploaderFail(error: String)
    @JvmStatic external fun nativeUploaderProgressJson(): String

    // --- Chat Features (Timezone + EXIF) ---

    @JvmStatic external fun nativeGetCommonTimezones(): String
    @JvmStatic external fun nativeFormatTimestampInTimezone(utcMs: Long, tzId: String): String
    @JvmStatic external fun nativeIsValidTimezoneId(tzId: String): Boolean
    @JvmStatic external fun nativeFileHasMetadata(mimeType: String): Boolean
    @JvmStatic external fun nativeGetStrippableMimeTypes(): String

    // --- Invitation Hide ---

    @JvmStatic external fun nativeInviteHide(roomId: String, roomName: String, inviterName: String, inviterMxid: String)
    @JvmStatic external fun nativeInviteUnhide(roomId: String)
    @JvmStatic external fun nativeInviteIsHidden(roomId: String): Boolean
    @JvmStatic external fun nativeInviteExportJson(): String
    @JvmStatic external fun nativeInviteImportJson(json: String)
    @JvmStatic external fun nativeInviteClear()
    @JvmStatic external fun nativeInviteCount(): Int

    // --- Thread Aggregator ---

    @JvmStatic external fun nativeThreadAdd(
        threadId: String, roomId: String, roomName: String,
        accountId: String, accountIndex: String,
        lastMsg: String, lastSender: String,
        lastTs: Long, replyCount: Int, unread: Boolean
    )
    @JvmStatic external fun nativeThreadGetAllJson(): String
    @JvmStatic external fun nativeThreadClear()
    @JvmStatic external fun nativeThreadCount(): Int
    @JvmStatic external fun nativeThreadRemoveRoom(roomId: String)

    // --- User Messages ---

    @JvmStatic external fun nativeFormatUserMessagePreview(roomName: String, body: String, msgType: String, maxLen: Int): String

    // --- Room Version ---

    @JvmStatic external fun nativeGetRoomVersionsJson(): String
    @JvmStatic external fun nativeIsValidRoomVersion(version: String): Boolean

    // --- Chat Preview ---

    @JvmStatic external fun nativeFormatShortTime(epochMs: Long): String
    @JvmStatic external fun nativeTruncateMessage(body: String, maxLen: Int): String

    // --- RAM Monitor ---

    @JvmStatic external fun nativeGetMemoryInfo(): String
    @JvmStatic external fun nativeFormatMemoryLabel(rssKb: Long): String

    // --- Cache Manager ---

    @JvmStatic external fun nativeCacheTrack(eventId: String, roomId: String, roomName: String, timestamp: Long, sizeBytes: Long, msgType: String, body: String)
    @JvmStatic external fun nativeCacheStatsJson(): String
    @JvmStatic external fun nativeCacheGetByRoom(roomId: String): String
    @JvmStatic external fun nativeCacheGetOlderThan(beforeTs: Long): String
    // --- Message Aggregator (All Messages) ---

    @JvmStatic external fun nativeMsgAggAdd(eventId: String, roomId: String, roomName: String, accountId: String, accountIndex: String, senderName: String, body: String, msgType: String, originServerTs: Long)
    @JvmStatic external fun nativeMsgAggGetAllJson(): String
    @JvmStatic external fun nativeMsgAggClear()
    @JvmStatic external fun nativeMsgAggCount(): Int

    // --- Room Info ---

    @JvmStatic external fun nativeFormatCreationDate(epochMs: Long): String
    @JvmStatic external fun nativeIsLikelyFullHistory(cached: Int, estimated: Int): Boolean

    // --- Deleted Archive ---

    @JvmStatic external fun nativeDeletedArchiveAdd(eventId: String, roomId: String, roomName: String, senderName: String, body: String, msgType: String, timestamp: String, originTs: Long, deletedBy: String)
    @JvmStatic external fun nativeDeletedArchiveExportJson(): String
    @JvmStatic external fun nativeDeletedArchiveClear()
    @JvmStatic external fun nativeDeletedArchiveCount(): Int

    // --- Search Index ---

    @JvmStatic external fun nativeSearchIndexMessage(eventId: String, roomId: String, roomName: String, senderName: String, body: String, timestamp: Long, isEncrypted: Boolean)
    @JvmStatic external fun nativeSearchQuery(query: String, limit: Int): String
    @JvmStatic external fun nativeSearchClear()
    @JvmStatic external fun nativeSearchIndexedCount(): Int

    // --- Module Loader ---

    @JvmStatic external fun nativeModuleScanDir(dirPath: String)
    @JvmStatic external fun nativeModuleEnable(name: String)
    @JvmStatic external fun nativeModuleIsEnabled(name: String): Boolean
    @JvmStatic external fun nativeModuleListJson(): String

    // --- Notification Keywords ---

    @JvmStatic external fun nativeNotifKeywordAdd(keyword: String, caseSensitive: Boolean)
    @JvmStatic external fun nativeNotifKeywordCheck(body: String): String
    @JvmStatic external fun nativeNotifKeywordExport(): String
    @JvmStatic external fun nativeNotifKeywordImport(json: String)
    @JvmStatic external fun nativeNotifKeywordClear()

    // --- Reaction Preview ---

    @JvmStatic external fun nativeFormatReactionPreview(reactorName: String, emoji: String, sourceBody: String, sourceSender: String): String

    // --- Room Mirror ---

    @JvmStatic external fun nativeMirrorAdd(srcRoomId: String, srcRoomName: String, mirRoomId: String, mirRoomName: String, enabled: Boolean, useDolls: Boolean)
    @JvmStatic external fun nativeMirrorRemove(srcRoomId: String)
    @JvmStatic external fun nativeMirrorIsActive(srcRoomId: String): Boolean
    @JvmStatic external fun nativeMirrorFormatMessage(senderName: String, senderMxid: String, roomName: String, body: String, msgType: String, ts: Long): String
    @JvmStatic external fun nativeMirrorGenerateDollMxid(originalMxid: String, targetServer: String): String
    @JvmStatic external fun nativeMirrorIsValidDoll(mxid: String): Boolean
    @JvmStatic external fun nativeMirrorExportJson(): String

    // --- Input Tools ---

    @JvmStatic external fun nativeSymbolAdd(symbol: String, label: String)
    @JvmStatic external fun nativeSymbolExport(): String
    @JvmStatic external fun nativeSymbolImport(json: String)

    @JvmStatic external fun nativeReplacementAddRule(pattern: String, replacement: String, exactMatch: Boolean)
    @JvmStatic external fun nativeReplacementApply(text: String): String
    @JvmStatic external fun nativeReplacementExport(): String

    // --- LLM ---

    @JvmStatic external fun nativeBuildLlmRequest(prompt: String, provider: Int, endpoint: String, token: String, model: String, systemPrompt: String, temp: Float, maxTokens: Int): String
    @JvmStatic external fun nativeBuildLlmHeaders(provider: Int, token: String): String
    @JvmStatic external fun nativeParseLlmResponse(body: String, statusCode: Int, provider: Int): String
    @JvmStatic external fun nativeFormatLlmBroadcast(prompt: String, response: String): String

    // --- Duplicate Names ---

    @JvmStatic external fun nativeFormatUserDisplayName(displayName: String, mxid: String, showMxid: Boolean): String

    // --- MXID Visibility ---

    @JvmStatic external fun nativeMxidVisibilityHide(mxid: String)
    @JvmStatic external fun nativeMxidVisibilityShow(mxid: String)
    @JvmStatic external fun nativeMxidVisibilityIsVisible(mxid: String): Boolean
    @JvmStatic external fun nativeMxidVisibilityExport(): String

    // --- Read Receipts ---

    @JvmStatic external fun nativeComputeReceiptDisplay(receiptsJson: String, maxVisible: Int): String

    // --- Room Analytics ---

    @JvmStatic external fun nativeExtractServerName(mxid: String): String

    // --- User Hide Timer ---

    @JvmStatic external fun nativeUserHideFor(userId: String, displayName: String, minutes: Int)
    @JvmStatic external fun nativeUserHideIsHidden(userId: String): Boolean
    @JvmStatic external fun nativeUserHideGetActive(): String

    // --- Message Queue ---

    @JvmStatic external fun nativeMsgQueueEnqueue(msgId: String, roomId: String, body: String, formattedBody: String, order: Int, maxRetries: Int)
    @JvmStatic external fun nativeMsgQueueSetOrder(msgId: String, order: Int)
    @JvmStatic external fun nativeMsgQueueMarkFailed(msgId: String, error: String)
    @JvmStatic external fun nativeMsgQueueMarkSent(msgId: String)
    @JvmStatic external fun nativeMsgQueuePendingCount(): Int
    @JvmStatic external fun nativeMsgQueueExport(): String

    // --- Image Crop ---

    @JvmStatic external fun nativeIsValidCrop(imgW: Int, imgH: Int, x: Int, y: Int, w: Int, h: Int): Boolean

    // --- Auto-Scroll ---

    @JvmStatic external fun nativeComputeScrollPlan(smooth: Boolean, durationMin: Int, totalLines: Int, lineHeightPx: Int): String

    // --- Language Detection ---

    @JvmStatic external fun nativeDetectLanguage(text: String, method: Int): String
    @JvmStatic external fun nativeGetLanguageLabel(code: String): String

    // --- Language Hide ---

    @JvmStatic external fun nativeLangHideAdd(langCode: String, roomId: String, userId: String, specificUser: Boolean, minutes: Int)
    @JvmStatic external fun nativeLangHideIsHidden(langCode: String, roomId: String, userId: String): Boolean

    // --- Chat Push Down ---

    @JvmStatic external fun nativeChatPushDown(roomId: String, minutes: Int)
    @JvmStatic external fun nativeChatIsPushedDown(roomId: String): Boolean
    @JvmStatic external fun nativeChatPushDownRestore(roomId: String)

    // --- Emoji Blacklist ---

    @JvmStatic external fun nativeEmojiBlacklistAdd(emoji: String)
    @JvmStatic external fun nativeEmojiBlacklistRemove(emoji: String)
    @JvmStatic external fun nativeEmojiBlacklistIsBlocked(emoji: String): Boolean
    @JvmStatic external fun nativeEmojiBlacklistExport(): String
    @JvmStatic external fun nativeEmojiBlacklistImport(json: String)

    // --- Avatar History ---

    @JvmStatic external fun nativeAvatarAddChange(mxcUrl: String, eventId: String, timestamp: Long)
    @JvmStatic external fun nativeAvatarExportJson(): String
    @JvmStatic external fun nativeAvatarClear()

    // --- Jump to Date with Time ---

    @JvmStatic external fun nativeParseJumpToDate(input: String): String

    // --- Room Matching ---

    @JvmStatic external fun nativeMatchRooms(query: String, roomsJson: String): String
    @JvmStatic external fun nativeIsRoomId(input: String): Boolean
    @JvmStatic external fun nativeIsRoomAlias(input: String): Boolean

    // --- Event Links ---

    @JvmStatic external fun nativeExtractEventLinks(body: String): String
    @JvmStatic external fun nativeFormatResolvedEvent(sender: String, body: String): String
    @JvmStatic external fun nativeIsEventId(text: String): Boolean

    // --- Timestamps ---

    @JvmStatic external fun nativeFormatTimestamp(epochMs: Long, includeSeconds: Boolean): String
    @JvmStatic external fun nativeFormatFullTimestamp(epochMs: Long): String

    // --- Lightweight Call ---

    @JvmStatic external fun nativeLightCallEnter(): String
    @JvmStatic external fun nativeLightCallExit(): String
    @JvmStatic external fun nativeLightCallAssessMemory(): String
    @JvmStatic external fun nativeShouldUseLightweightMode(): Boolean

    // --- Scheduled Edits ---

    @JvmStatic external fun nativeSchedEditSchedule(roomId: String, targetEventId: String, newContent: String, contentUrl: String, formattedContent: String, formattedUrl: String, scheduledAtMs: Long, recurring: Boolean): String
    @JvmStatic external fun nativeSchedEditCancel(editId: String)
    @JvmStatic external fun nativeSchedEditGetDue(): String
    @JvmStatic external fun nativeSchedEditMarkApplied(editId: String)
    @JvmStatic external fun nativeSchedEditMarkFailed(editId: String, error: String)
    @JvmStatic external fun nativeSchedEditExport(): String
    @JvmStatic external fun nativeSchedEditStats(): String

    // --- SVG Rendering ---

    @JvmStatic external fun nativeParseSvg(svgData: String): String
    @JvmStatic external fun nativeIsValidSvg(data: String): Boolean

    // --- Drawing Canvas ---

    @JvmStatic external fun nativeDrawMoveTo(x: Double, y: Double)
    @JvmStatic external fun nativeDrawLineTo(x: Double, y: Double)
    @JvmStatic external fun nativeDrawSetColor(argb: Int)
    @JvmStatic external fun nativeDrawSetWidth(w: Double)
    @JvmStatic external fun nativeDrawExportJson(): String
    @JvmStatic external fun nativeDrawToSvgPath(): String
    @JvmStatic external fun nativeDrawClear()

    // --- Profile Swiper ---

    @JvmStatic external fun nativeProfileSwiperSetProfiles(profilesJson: String)
    @JvmStatic external fun nativeProfileSwiperNext(): String
    @JvmStatic external fun nativeProfileSwiperPrev(): String

    // --- Rainbow Generator ---

    @JvmStatic external fun nativeGenerateRainbow(text: String): String

    // --- Text Formatting ---

    @JvmStatic external fun nativeFormatSpoiler(text: String): String
    @JvmStatic external fun nativeFormatEmote(sender: String, text: String): String
    @JvmStatic external fun nativeFormatShrug(text: String): String
    @JvmStatic external fun nativeFormatLenny(text: String): String
    @JvmStatic external fun nativeFormatTableFlip(text: String): String
    @JvmStatic external fun nativeFormatPlain(text: String): String
    @JvmStatic external fun nativeIsEmojiOnly(text: String): Boolean
    @JvmStatic external fun nativeTruncateText(text: String, maxLen: Int): String

    // --- URL Tools ---

    @JvmStatic external fun nativeExtractFirstUrl(text: String): String
    @JvmStatic external fun nativeUrlEncode(input: String): String
    @JvmStatic external fun nativeUrlDecode(input: String): String
    @JvmStatic external fun nativeBuildMatrixToUrl(roomId: String): String

    // --- Notification Priority ---

    @JvmStatic external fun nativeComputeNotifPriority(isDM: Boolean, isMention: Boolean, isRoomMention: Boolean, isKeyword: Boolean, isCall: Boolean, isBackground: Boolean, dnd: Boolean, favorite: Boolean): String
    @JvmStatic external fun nativeFormatNotifTitle(roomName: String, senderName: String, isDM: Boolean): String
    @JvmStatic external fun nativeFormatNotifBody(body: String, senderName: String, isDM: Boolean, showSender: Boolean): String
    @JvmStatic external fun nativeIsRoomMention(body: String): Boolean

    // --- Matrix Patterns ---

    @JvmStatic external fun nativeIsUserId(input: String): Boolean
    @JvmStatic external fun nativeParseMatrixToPermalink(url: String): String
    @JvmStatic external fun nativeIsValidEmail(input: String): Boolean

    // --- Desync Detector ---

    @JvmStatic external fun nativeDesyncTrackEvent(eventId: String, serverName: String, timestamp: Long)
    @JvmStatic external fun nativeDesyncCheck(roomId: String, currentServer: String): String

    // --- Latency Tracker ---

    @JvmStatic external fun nativeLatencyRecord(latencyMs: Double, server: String, endpoint: String, success: Boolean)
    @JvmStatic external fun nativeLatencyStats(): String
    @JvmStatic external fun nativeLatencyStatsText(): String

    // --- String Utils ---

    @JvmStatic external fun nativeSanitizeRoomName(input: String): String
    @JvmStatic external fun nativeWordCount(input: String): Int

    // --- Location Sharing ---

    @JvmStatic external fun nativeLocationFormatMessage(lat: Double, lon: Double, acc: Double, label: String): String
    @JvmStatic external fun nativeLocationFormatGeoJson(lat: Double, lon: Double, acc: Double): String
    @JvmStatic external fun nativeLocationDistance(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double

    // --- Color Utils ---

    @JvmStatic external fun nativeContrastRatio(fgR: Int, fgG: Int, fgB: Int, bgR: Int, bgG: Int, bgB: Int): Double
    @JvmStatic external fun nativeParseColor(input: String): String

    // --- E2EE Utils ---

    @JvmStatic external fun nativeGetTrustLabel(level: Int): String
    @JvmStatic external fun nativeGetTrustBadge(level: Int): String

    // --- Thumbnail ---

    @JvmStatic external fun nativeComputeThumbnail(srcW: Int, srcH: Int, maxW: Int, maxH: Int, upscale: Boolean, quality: Int): String
    @JvmStatic external fun nativeBuildThumbnailUrl(mxcUri: String, w: Int, h: Int, method: String, animated: Boolean): String

    // --- Waveform ---

    @JvmStatic external fun nativeSuggestBarCount(durationMs: Long): Int
    @JvmStatic external fun nativeComputeRmsVolume(samples: IntArray): Double

    // --- Session Timeout ---

    @JvmStatic external fun nativeShouldLock(lockMethod: Int, idleTimeoutMin: Int, maxSessionMin: Int, maxFailedPin: Int, lockOnBg: Boolean, lastActivityMs: Long, sessionStartMs: Long, failedAttempts: Int, isLocked: Boolean, isBackground: Boolean): Boolean
    @JvmStatic external fun nativeIsValidPin(pin: String, minLen: Int, maxLen: Int): Boolean

    // --- Password Validator ---

    @JvmStatic external fun nativeValidatePassword(password: String): String

    // --- Spellcheck ---

    @JvmStatic external fun nativeEditDistance(a: String, b: String): Int

    // --- Typing Indicator ---

    @JvmStatic external fun nativeFormatTypingText(namesJson: String): String

    // --- Hash Utils ---

    @JvmStatic external fun nativeSha256Hex(input: String): String
    @JvmStatic external fun nativeGenerateToken(bytes: Int): String

    // --- Room Stats ---

    @JvmStatic external fun nativeComputeMessagesPerDay(count: Int, firstTs: Long, lastTs: Long): Double

    // --- Mention Parser ---

    @JvmStatic external fun nativeBuildUserPill(userId: String, displayName: String): String
    @JvmStatic external fun nativeStripPills(html: String): String

    // --- Poll Utils ---


    // --- Reaction Utils ---

    @JvmStatic external fun nativeGetQuickReactions(): String

    // --- File Validator ---

    @JvmStatic external fun nativeFormatFileSize(bytes: Long): String
    @JvmStatic external fun nativeGetExtensionFromName(fileName: String): String

    // --- Date Utils ---

    @JvmStatic external fun nativeFormatChatTimestamp(epochMs: Long, includeSeconds: Boolean): String
    @JvmStatic external fun nativeFormatRelativeTime(epochMs: Long): String

    // --- Message Queue ---

    @JvmStatic external fun nativeTextSimilarity(a: String, b: String): Double

    // --- Pinned Events (Element Web parity) ---

    @JvmStatic external fun nativeParsePinnedEventIds(stateJson: String): String
    @JvmStatic external fun nativeBuildPinnedEventsContent(idsJson: String): String

    // --- Server Capabilities ---

    @JvmStatic external fun nativeParseServerCapabilities(wellKnownJson: String): String

    // --- Username Validator ---

    @JvmStatic external fun nativeValidateUsername(username: String): String

    // --- Emoji Analyzer ---

    @JvmStatic external fun nativeIsEmojiOnlyMessage(text: String): Boolean

    // --- Identity Utils ---

    @JvmStatic external fun nativeResolveMatrixId(input: String): String
    @JvmStatic external fun nativeGetInitials(name: String): String

    // --- Notification Analyzer ---

    @JvmStatic external fun nativeSuggestQuietHours(byHourJson: String): String

    // --- Sync Analyzer ---

    @JvmStatic external fun nativeFormatProgressBar(percent: Double, width: Int): String

    // --- User Rating ---

    @JvmStatic external fun nativeComputeStreak(timestampsJson: String): String

    // --- Event Timeline ---

    @JvmStatic external fun nativeFormatGroupLabel(timestampMs: Long): String

    // --- Room Directory ---

    @JvmStatic external fun nativeExtractServers(roomsJson: String): String

    // --- SSO Utils ---

    @JvmStatic external fun nativeValidateHomeserverUrl(input: String): String

    // --- Backup Utils ---

    @JvmStatic external fun nativeIsValidRecoveryKey(key: String): Boolean

    // --- Device Manager ---

    @JvmStatic external fun nativeFormatDeviceLastSeen(lastSeenMs: Long): String
    @JvmStatic external fun nativeIsDeviceInactive(lastSeenMs: Long): Boolean

    // --- Presence ---

    @JvmStatic external fun nativeFormatPresence(presence: Int): String
    @JvmStatic external fun nativeFormatPresenceLine(presence: Int, lastActiveAgoMs: Long, statusMsg: String): String

    // --- Room Permissions ---

    @JvmStatic external fun nativeGetSuggestedRole(plJson: String, userId: String): String

    // --- Room Summary ---

    @JvmStatic external fun nativeFormatLastMessagePreview(sender: String, body: String, encrypted: Boolean): String

    // --- Membership ---

    @JvmStatic external fun nativeFormatMembership(membershipStr: String): String

    // --- Event Validator ---

    @JvmStatic external fun nativeValidateEvent(eventId: String, eventType: String, senderId: String, contentJson: String, originTs: String, blockedUsersJson: String): String

    // --- Room Encryption ---

    @JvmStatic external fun nativeIsRoomEncrypted(stateJson: String): Boolean

    // --- Login Utils ---

    @JvmStatic external fun nativeBuildLoginBody(userId: String, password: String, deviceName: String, deviceId: String): String

    // --- Account Utils ---

    @JvmStatic external fun nativeValidatePasswordWithUsername(password: String, username: String): String

    // --- Connection Monitor ---

    @JvmStatic external fun nativeConnMonitorOnConnected()
    @JvmStatic external fun nativeConnMonitorOnDisconnected()
    @JvmStatic external fun nativeConnMonitorGetStatus(): String

    // --- Push Rules ---

    @JvmStatic external fun nativeParsePushCondition(conditionJson: String): String

    // --- Space Utils ---

    @JvmStatic external fun nativeBuildSpaceChildContent(suggested: Boolean, order: String, autoJoin: Boolean, canonical: Boolean): String

    // --- Event Relations ---


    // --- E2EE Decoration ---

    @JvmStatic external fun nativeComputeE2eeDecoration(encrypted: Boolean, verified: Boolean, crossSigned: Boolean, decryptError: Boolean, blacklisted: Boolean, beforeJoined: Boolean, errorReason: String): String

    // --- Room List ---

    @JvmStatic external fun nativeGetBadgeText(count: Int, highlights: Int): String

    // --- Media Utils ---

    @JvmStatic external fun nativeMimeToMsgType(mimeType: String): String
    @JvmStatic external fun nativeIsValidBlurhash(hash: String): Boolean

    // --- Notification Settings ---

    @JvmStatic external fun nativeFormatNotifMode(mode: Int): String

    // --- Invite Utils ---

    @JvmStatic external fun nativeBuildInviteBody(userId: String, reason: String): String

    // --- Verification ---

    @JvmStatic external fun nativeGetVerificationEmojis(): String

    // --- Session Manager ---

    @JvmStatic external fun nativeFormatSessionBadge(unread: Int, highlights: Int): String

    // --- Auth Utils ---

    @JvmStatic external fun nativeFormatRateLimitMessage(responseJson: String, httpStatus: Int): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeIsServerNotice(contentJson: String): Boolean

    // --- Event Encryption ---

    @JvmStatic external fun nativeParseEncryptedHeader(contentJson: String): String

    // --- Report Utils ---

    @JvmStatic external fun nativeGetReasonDescription(code: String): String

    // --- Secret Storage ---

    @JvmStatic external fun nativeExtractDefaultSecretKey(accountDataJson: String): String
    @JvmStatic external fun nativeHasCrossSigningSecrets(accountDataJson: String): Boolean

    // --- Report / Rageshake ---

    @JvmStatic external fun nativeIsOffensive(score: Int): Boolean
    @JvmStatic external fun nativeTruncateReportDescription(description: String, maxLen: Int): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeIsContentScannerAvailable(serverCapabilitiesJson: String): Boolean

    // --- Matrix Error ---


    // --- Call Content Builders ---

    @JvmStatic external fun nativeBuildCallInviteContent(callId: String, isVideo: Boolean, sdpOffer: String, lifetimeSec: Int): String
    @JvmStatic external fun nativeBuildCallAnswerContent(callId: String, sdpAnswer: String): String
    @JvmStatic external fun nativeGetCallState(eventContentJson: String): String

    // --- Room State ---

    @JvmStatic external fun nativeIsPublicRoom(stateContentJson: String): Boolean
    @JvmStatic external fun nativeIsInviteOnly(stateContentJson: String): Boolean
    @JvmStatic external fun nativeJoinRuleToString(stateContentJson: String): String
    @JvmStatic external fun nativeIsHistoryPubliclyVisible(stateContentJson: String): Boolean
    @JvmStatic external fun nativeHistoryVisibilityToString(stateContentJson: String): String
    @JvmStatic external fun nativeAreGuestsAllowed(stateContentJson: String): Boolean
    @JvmStatic external fun nativeIsRoomUpgraded(stateContentJson: String): Boolean

    // --- Matrix Pattern Validators ---

    @JvmStatic external fun nativeIsMxcUrl(url: String): Boolean
    @JvmStatic external fun nativeIsPhoneNumber(input: String): Boolean
    @JvmStatic external fun nativeExtractServerNameFromId(mxid: String): String
    @JvmStatic external fun nativeExtractUserNameFromId(mxid: String): String
    @JvmStatic external fun nativeCandidateAliasFromRoomName(roomName: String, domain: String, maxLength: Int): String

    // --- Widget List ---

    @JvmStatic external fun nativeListRoomWidgets(stateEventsJson: String): String

    // --- Session Rename ---

    @JvmStatic external fun nativeBuildSessionRenameBody(sessionId: String, newName: String): String

    // --- Permalink Detection ---

    @JvmStatic external fun nativeIsMatrixToPermalink(url: String): Boolean
    @JvmStatic external fun nativeIsAppPermalink(url: String): Boolean
    @JvmStatic external fun nativeIsValidOrderString(order: String): Boolean
    @JvmStatic external fun nativeIsGroupId(input: String): Boolean

    // --- Device Name ---

    @JvmStatic external fun nativeParseDeviceName(userAgent: String): String

    // --- Matrix ID Extraction ---

    @JvmStatic external fun nativeExtractMatrixIds(text: String): String

    // --- Login Flows ---

    @JvmStatic external fun nativeParseLoginFlowsList(apiResponseJson: String): String
    @JvmStatic external fun nativeBuildUserIdentifier(userId: String): String

    // --- Notification Mode ---

    @JvmStatic external fun nativeIsNotifModeDifferent(oldMode: String, newMode: String): Boolean
    @JvmStatic external fun nativeGetDefaultModeForRoom(isDirect: Boolean, isEncrypted: Boolean): String

    // --- Password Strength ---

    @JvmStatic external fun nativeMeetsMinimumRequirements(password: String): Boolean
    @JvmStatic external fun nativeCountCharClasses(password: String): Int
    @JvmStatic external fun nativeIsCommonPassword(password: String): Boolean

    // --- SSO Utilities ---

    @JvmStatic external fun nativeBuildSsoLoginUrl(baseUrl: String, redirectUrl: String): String
    @JvmStatic external fun nativeGetSsoProviderBrand(provider: String): String

    // --- Trust Label ---


    // --- MXC URL Utilities ---

    @JvmStatic external fun nativeIsMxcUri(url: String): Boolean
    @JvmStatic external fun nativeExtractMxcServerName(mxcUrl: String): String
    @JvmStatic external fun nativeExtractMxcMediaId(mxcUrl: String): String
    @JvmStatic external fun nativeBuildMxcUri(serverName: String, mediaId: String): String
    @JvmStatic external fun nativeResolveMxcDownloadUrl(mxcUrl: String, homeServerUrl: String): String
    @JvmStatic external fun nativeHasTextWithImage(contentJson: String): Boolean

    // --- MXC Thumbnail ---

    @JvmStatic external fun nativeResolveMxcThumbnailUrl(mxcUrl: String, homeServerUrl: String, width: Int, height: Int): String

    // --- Content Utilities ---

    @JvmStatic external fun nativeGetExtensionFromMimeType(mimetype: String): String
    @JvmStatic external fun nativeExtractUsefulTextFromReply(repliedBody: String): String
    @JvmStatic external fun nativeFormatSpoilerTextFromHtml(formattedBody: String): String
    @JvmStatic external fun nativeGetLatestEditEventId(editSummaryJson: String, originalEventId: String): String
    @JvmStatic external fun nativeGetEditedTargetEventId(contentJson: String): String

    // --- User Status ---

    @JvmStatic external fun nativeBuildUserStatusJson(status: String, emoji: String, nowMs: Long): String
    @JvmStatic external fun nativeGetPresenceStatusText(isOnline: Boolean, lastActiveMs: Long): String
    @JvmStatic external fun nativeGetStatusSuggestions(): String

    // --- Markdown Renderer ---


    // --- Megolm Decryptor ---

    @JvmStatic external fun nativeMegolmAddSession(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String): Boolean
    @JvmStatic external fun nativeMegolmDecrypt(roomId: String, senderKey: String, sessionId: String, ciphertext: String): String
    @JvmStatic external fun nativeMegolmSessionCount(): Int
    @JvmStatic external fun nativeMegolmClearRoom(roomId: String)

    // --- Olm Account & Session ---

    @JvmStatic external fun nativeOlmCreateAccount(userId: String, deviceId: String): Boolean
    @JvmStatic external fun nativeOlmGetIdentityKeys(): String
    @JvmStatic external fun nativeOlmGenerateOneTimeKeys(count: Int): String
    @JvmStatic external fun nativeOlmSignMessage(message: String): String
    @JvmStatic external fun nativeOlmCreateInboundSession(theirIdentityKey: String, preKeyMessage: String): String
    @JvmStatic external fun nativeOlmDecryptMessage(senderKey: String, sessionId: String, ciphertext: String): String
    @JvmStatic external fun nativeOlmPickleAccount(): String
    @JvmStatic external fun nativeOlmUnpickleAccount(pickled: String, userId: String, deviceId: String): Boolean

    // --- Event Signing ---

    @JvmStatic external fun nativeSignEvent(eventJson: String): String
    @JvmStatic external fun nativeVerifyEventSignature(eventJson: String, signKeyB64: String): Boolean

    // --- Device Verification ---

    @JvmStatic external fun nativeVerifyDeviceSignature(deviceKeysJson: String, userId: String, deviceId: String, signKeyB64: String, signatureB64: String): Boolean
    @JvmStatic external fun nativeComputeDeviceFingerprint(identityKeyBase64: String): String

    // --- SAS Emoji Verification ---

    @JvmStatic external fun nativeSasCreate(): String
    @JvmStatic external fun nativeSasSetTheirKey(theirPubkey: String): Boolean
    @JvmStatic external fun nativeSasGetEmojis(): String
    @JvmStatic external fun nativeSasCalculateMac(input: String, info: String): String
    @JvmStatic external fun nativeSasVerifyMac(theirMac: String, input: String, info: String): Boolean
    @JvmStatic external fun nativeSasDestroy()

    // --- JSON Parser ---

    @JvmStatic external fun nativeParseJsonStringValue(json: String, key: String): String

    // --- Federation Version ---

    @JvmStatic external fun nativeFederationVersionToJson(versionJson: String): String

    // --- Auth Models ---

    @JvmStatic external fun nativePresenceEnumToString(presence: Int): String
    @JvmStatic external fun nativeCredentialsToJson(credentialsJson: String): String

    // --- Call Models ---

    @JvmStatic external fun nativeSdpTypeToString(type: Int): String
    @JvmStatic external fun nativeEndCallReasonToString(reason: Int): String

    // --- Message Content ---

    @JvmStatic external fun nativeMessageTextToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageNoticeToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageEmoteToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageImageToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageVideoToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageAudioToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageFileToJson(contentJson: String): String

    // --- Crypto Models ---

    @JvmStatic external fun nativeDeviceInfoToJson(deviceJson: String): String

    // --- Offline Cache ---

    @JvmStatic external fun nativeCanFitInStorage(required: Long, available: Long, reserved: Long): Boolean
    @JvmStatic external fun nativeEstimateMessageCacheSize(messageCount: Int, avgBodySize: Int): Long

    // --- Sign Out Service ---

    @JvmStatic external fun nativeShouldIgnoreSignOutError(errorCode: String, httpCode: Int): Boolean
    @JvmStatic external fun nativeSignInAgainBodyToJson(paramsJson: String): String

    // --- Message Extras ---

    @JvmStatic external fun nativePollTypeToString(type: Int): String
    @JvmStatic external fun nativePollTypeFromString(type: String): Int

    // --- Terms Service ---

    @JvmStatic external fun nativeAcceptTermsBodyToJson(bodyJson: String): String

    // --- Live Draft ---

    @JvmStatic external fun nativeLiveDraftConfigToJson(configJson: String): String

    // --- Encrypted File ---

    @JvmStatic external fun nativeEncryptedFileKeyToJson(keyJson: String): String
    @JvmStatic external fun nativeIsValidJwkKey(keyJson: String): Boolean
    @JvmStatic external fun nativeExtractFileKey(keyJson: String): String
    @JvmStatic external fun nativeEncryptedFileInfoToJson(infoJson: String): String
    @JvmStatic external fun nativeIsValidEncryptedFile(infoJson: String): Boolean
    @JvmStatic external fun nativeExtractFileIv(infoJson: String): String

    // --- Crypto Algorithms ---

    @JvmStatic external fun nativeSha256(data: ByteArray): String
    @JvmStatic external fun nativeBase58Encode(data: ByteArray): String

    // --- TLS Bridge ---

    @JvmStatic external fun nativeTlsBridgeAvailable(): Boolean

    // --- Create Room ---

    @JvmStatic external fun nativeCreateRoomPresetToString(preset: Int): String

    // --- Widget Manager ---

    @JvmStatic external fun nativeWidgetMgrInit(roomId: String, userId: String, displayName: String, avatarUrl: String): Boolean
    @JvmStatic external fun nativeWidgetMgrSetSecurityPolicy(policyJson: String): Boolean
    @JvmStatic external fun nativeWidgetMgrLoadWidgets(stateEventsJson: String): String
    @JvmStatic external fun nativeWidgetMgrCreateWidget(widgetId: String, type: String, url: String, name: String, waitForIframeLoad: Boolean): String
    @JvmStatic external fun nativeWidgetMgrRemoveWidget(widgetId: String): String
    @JvmStatic external fun nativeWidgetMgrSetPinned(widgetId: String, pinned: Boolean): String
    @JvmStatic external fun nativeWidgetMgrResize(widgetId: String, width: Int, height: Int): String
    @JvmStatic external fun nativeWidgetMgrSetMinimized(widgetId: String, minimized: Boolean): String
    @JvmStatic external fun nativeWidgetMgrSetMaximized(widgetId: String, maximized: Boolean): String
    @JvmStatic external fun nativeWidgetMgrRequestCapability(widgetId: String, capability: Int): String
    @JvmStatic external fun nativeWidgetMgrApproveCapability(widgetId: String, capability: Int): String
    @JvmStatic external fun nativeWidgetMgrDenyCapability(widgetId: String, capability: Int): String
    @JvmStatic external fun nativeWidgetMgrGetUrl(widgetId: String): String
    @JvmStatic external fun nativeWidgetMgrBuildPostMessage(widgetId: String, action: String, data: String): String
    @JvmStatic external fun nativeWidgetMgrParsePostMessage(message: String): String
    @JvmStatic external fun nativeWidgetMgrSupportsPiP(widgetId: String): Boolean
    @JvmStatic external fun nativeWidgetMgrGetByType(type: String): String
    @JvmStatic external fun nativeWidgetMgrCount(): String
    @JvmStatic external fun nativeWidgetMgrBuildCsp(): String
    @JvmStatic external fun nativeApplyWidgetUrlTemplate(url: String, templateJson: String): String
    @JvmStatic external fun nativeValidateWidgetSecurity(url: String, policyJson: String): String
    @JvmStatic external fun nativeClassifyWidgetType(type: String): String
    @JvmStatic external fun nativeIsAutoApprovedCapability(capability: Int, widgetType: String): Boolean

    // --- Key Backup Manager ---

    @JvmStatic external fun nativeBackupExtractPrivateKey(recoveryKey: String): String
    @JvmStatic external fun nativeBackupGenerateRecoveryKey(curve25519Key: String): String
    @JvmStatic external fun nativeBackupParseVersion(json: String): String
    @JvmStatic external fun nativeBackupBuildCreateVersion(configJson: String): String
    @JvmStatic external fun nativeBackupBuildDelete(version: String): String
    @JvmStatic external fun nativeBackupExportSession(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String, firstMessageIndex: Long, isForwarded: Boolean, forwardedCount: Long): String
    @JvmStatic external fun nativeBackupEncryptSession(sessionJson: String, authData: String): String
    @JvmStatic external fun nativeBackupParseKeys(backupJson: String): String
    @JvmStatic external fun nativeBackupDecryptSession(sessionJson: String, backupKey: String, roomId: String): String
    @JvmStatic external fun nativeBackupDecryptAll(keysJson: String, authData: String, recoveryKey: String): String
    @JvmStatic external fun nativeBackupVerifyIntegrity(authData: String): Boolean
    @JvmStatic external fun nativeBackupVerifyRecoveryMatch(recoveryKey: String, authData: String): Boolean
    @JvmStatic external fun nativeBackupProgress(): String
    @JvmStatic external fun nativeBackupProgressJson(): String
    @JvmStatic external fun nativeBackupSetTotalKeys(count: Int)
    @JvmStatic external fun nativeBackupAdvanceUploaded()
    @JvmStatic external fun nativeBackupAdvanceDownloaded()
    @JvmStatic external fun nativeBackupAdvanceDecrypted()
    @JvmStatic external fun nativeBackupAdvanceImported()
    @JvmStatic external fun nativeBackupMarkComplete()
    @JvmStatic external fun nativeBackupReset()

    // --- Live Location ---

    @JvmStatic external fun nativeLiveLocationParseGeoUri(uri: String): String
    @JvmStatic external fun nativeLiveLocationFormatMessage(lat: Double, lon: Double, accuracy: Double, label: String): String
    @JvmStatic external fun nativeLiveLocationFormatGeoUri(lat: Double, lon: Double): String
    @JvmStatic external fun nativeLiveLocationDistance(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double
    @JvmStatic external fun nativeLiveLocationStartSession(roomId: String, userId: String, description: String, timeoutSec: Int, intervalSec: Int, autoStop: Boolean, autoStopMin: Int): String
    @JvmStatic external fun nativeLiveLocationStopSession(sessionId: String): String
    @JvmStatic external fun nativeLiveLocationIsDue(sessionId: String): Boolean
    @JvmStatic external fun nativeLiveLocationUpdate(sessionId: String, lat: Double, lon: Double, accuracy: Double): String
    @JvmStatic external fun nativeLiveLocationGetActive(userId: String): String
    @JvmStatic external fun nativeLiveLocationGetRoomSessions(roomId: String): String
    @JvmStatic external fun nativeLiveLocationHistory(sessionId: String): String
    @JvmStatic external fun nativeLiveLocationBuildMapUrl(roomId: String, configJson: String): String
    @JvmStatic external fun nativeLiveLocationCluster(coordsJson: String, radiusMeters: Double): String
    @JvmStatic external fun nativeLiveLocationWithinGeofence(lat: Double, lon: Double, centerLat: Double, centerLon: Double, radiusMeters: Double): Boolean

    // --- Call Manager ---

    @JvmStatic external fun nativeCallStartOutgoing(roomId: String, calleeId: String, calleeName: String, callType: Int, sdpOffer: String): String
    @JvmStatic external fun nativeCallHandleIncoming(callId: String, roomId: String, callerId: String, callerName: String, callType: Int, sdpOffer: String, lifetimeSec: Int): String
    @JvmStatic external fun nativeCallAnswer(callId: String, sdpAnswer: String): String
    @JvmStatic external fun nativeCallReject(callId: String): String
    @JvmStatic external fun nativeCallHangup(callId: String): String
    @JvmStatic external fun nativeCallGetActive(): String
    @JvmStatic external fun nativeCallGetIncoming(): String
    @JvmStatic external fun nativeCallGetRoomCalls(roomId: String): String
    @JvmStatic external fun nativeCallIsRoomInCall(roomId: String): Boolean
    @JvmStatic external fun nativeCallFormatDuration(seconds: Int): String
    @JvmStatic external fun nativeCallParseSdp(sdpText: String, type: String): String
    @JvmStatic external fun nativeCallSetMuted(callId: String, muted: Boolean)
    @JvmStatic external fun nativeCallSetVideo(callId: String, enabled: Boolean)
    @JvmStatic external fun nativeCallReset()

    // --- Thread Manager ---

    @JvmStatic external fun nativeThreadIsRoot(eventContent: String, eventId: String): Boolean
    @JvmStatic external fun nativeThreadExtractRoot(eventContent: String): String
    @JvmStatic external fun nativeThreadUpsert(threadJson: String)
    @JvmStatic external fun nativeThreadAddReply(threadId: String, senderId: String, senderName: String, body: String, timestampMs: Long)
    @JvmStatic external fun nativeThreadGetList(limit: Int, offset: Int): String
    @JvmStatic external fun nativeThreadSetUnread(threadId: String, count: Int, highlighted: Boolean)
    @JvmStatic external fun nativeThreadMarkRead(threadId: String, readPos: Long)
    @JvmStatic external fun nativeThreadGetUnreadState(threadId: String): String
    @JvmStatic external fun nativeThreadTotalUnread(): Int
    @JvmStatic external fun nativeThreadFormatCount(count: Int): String
    @JvmStatic external fun nativeThreadGetNotifications(): String
    @JvmStatic external fun nativeThreadReset()

    // --- Poll Manager ---

    @JvmStatic external fun nativePollBuildStart(question: String, optionsJson: String, kind: Int, maxSelections: Int, unstable: Boolean): String
    @JvmStatic external fun nativePollBuildResponse(pollId: String, selectionsJson: String, unstable: Boolean): String
    @JvmStatic external fun nativePollBuildEnd(pollId: String, reason: String, unstable: Boolean): String
    @JvmStatic external fun nativePollTally(pollJson: String, votesJson: String): String
    @JvmStatic external fun nativePollIsValidQuestion(question: String): Boolean

    // --- Space Graph ---

    @JvmStatic external fun nativeSpaceSetRoot(spaceId: String, name: String, topic: String, avatarUrl: String)
    @JvmStatic external fun nativeSpaceAddChildRaw(parentId: String, childId: String, suggested: Boolean)
    @JvmStatic external fun nativeSpaceAddChild(parentId: String, childJson: String)
    @JvmStatic external fun nativeSpaceSetMetadata(roomId: String, name: String, topic: String, avatarUrl: String, joinRule: String, isJoined: Boolean)
    @JvmStatic external fun nativeSpaceTraverse(mode: Int, maxDepth: Int): String
    @JvmStatic external fun nativeSpaceGetChildren(spaceId: String): String
    @JvmStatic external fun nativeSpaceGetParents(roomId: String): String
    @JvmStatic external fun nativeSpaceGetDepth(roomId: String): Int
    @JvmStatic external fun nativeSpaceIsInSpace(spaceId: String, roomId: String): Boolean
    @JvmStatic external fun nativeSpaceToTree(spaceId: String, maxDepth: Int): String
    @JvmStatic external fun nativeSpaceSearch(spaceId: String, query: String): String
    @JvmStatic external fun nativeSpaceReset()

    // --- Pin Manager ---

    @JvmStatic external fun nativePinEvent(roomId: String, eventId: String, pinnedBy: String, powerLevel: Int): String
    @JvmStatic external fun nativeUnpinEvent(roomId: String, eventId: String, removedBy: String, powerLevel: Int): String
    @JvmStatic external fun nativePinToggle(roomId: String, eventId: String, userId: String, powerLevel: Int): String
    @JvmStatic external fun nativePinLoadState(roomId: String, stateJson: String)
    @JvmStatic external fun nativePinGetEvents(roomId: String): String
    @JvmStatic external fun nativePinIsPinned(roomId: String, eventId: String): Boolean
    @JvmStatic external fun nativePinCount(roomId: String): Int
    @JvmStatic external fun nativePinCanManage(powerLevel: Int): Boolean
    @JvmStatic external fun nativePinReset()

    // --- Media Viewer ---

    @JvmStatic external fun nativeMediaViewerParse(contentJson: String): String
    @JvmStatic external fun nativeMediaViewerFormatSize(bytes: Long): String
    @JvmStatic external fun nativeMediaViewerFormatDuration(durationMs: Int): String
    @JvmStatic external fun nativeMediaViewerViewport(contentJson: String, viewportW: Int, viewportH: Int): String
    @JvmStatic external fun nativeMediaViewerThumbnailUrl(mxcUrl: String, homeServer: String, width: Int, height: Int): String
    @JvmStatic external fun nativeMediaViewerDownloadUrl(mxcUrl: String, homeServer: String): String
    @JvmStatic external fun nativeMediaViewerExifRotation(rawExif: Int): Int
    @JvmStatic external fun nativeMediaViewerCanThumbnail(mimeType: String): Boolean

    // --- OIDC/SSO Login ---

    @JvmStatic external fun nativeOidcParseMetadata(json: String): String
    @JvmStatic external fun nativeOidcBuildRegistration(configJson: String): String
    @JvmStatic external fun nativeOidcParseRegistration(json: String): String
    @JvmStatic external fun nativeOidcBuildAuthorization(metadataJson: String, registrationJson: String, configJson: String): String
    @JvmStatic external fun nativeOidcParseToken(json: String): String
    @JvmStatic external fun nativeOidcBuildRefresh(refreshToken: String, clientId: String): String
    @JvmStatic external fun nativeOidcParseWhoami(json: String): String
    @JvmStatic external fun nativeOidcParseWellKnown(json: String): String
    @JvmStatic external fun nativeOidcIsCallback(url: String): Boolean
    @JvmStatic external fun nativeOidcExtractCode(callbackUrl: String): String
    @JvmStatic external fun nativeOidcBuildPasswordLogin(userId: String, password: String, deviceId: String, deviceName: String): String

    // --- User Directory ---

    @JvmStatic external fun nativeUserDirBuildSearch(searchTerm: String, limit: Int): String
    @JvmStatic external fun nativeUserDirSearch(query: String, responseJson: String): String
    @JvmStatic external fun nativeUserDirBestName(displayName: String, userId: String): String
    @JvmStatic external fun nativeUserDirAvatarInit(displayName: String, userId: String): String
    @JvmStatic external fun nativeUserDirIsValidQuery(query: String): Boolean

    // --- Profiler ---

    @JvmStatic external fun nativeProfileStart()
    @JvmStatic external fun nativeProfileStop()
    @JvmStatic external fun nativeProfileReset()
    @JvmStatic external fun nativeProfileIsActive(): Boolean
    @JvmStatic external fun nativeProfileReport(): String
    @JvmStatic external fun nativeProfileReportText(): String
    @JvmStatic external fun nativeProfileGetSummary(name: String): String
    @JvmStatic external fun nativeProfileMemory(): String

    // --- Profiler: User Action Timing ---
    @JvmStatic external fun nativeProfileStartAction(actionName: String, isCold: Boolean): Int
    @JvmStatic external fun nativeProfileStopAction(actionIndex: Int): Long
    @JvmStatic external fun nativeProfileSetBudget(actionPattern: String, budgetNs: Long)
    @JvmStatic external fun nativeProfileActionReport(): String
    @JvmStatic external fun nativeProfileActionReportText(): String

    // --- Profiler: Real-Time Overlay ---
    @JvmStatic external fun nativeProfileOverlaySnapshot(): String
    @JvmStatic external fun nativeProfileOverlayText(): String

    // --- Device Manager Full ---

    @JvmStatic external fun nativeDeviceParseList(json: String): String
    @JvmStatic external fun nativeDeviceParseInfo(deviceId: String, json: String): String
    @JvmStatic external fun nativeDeviceParseCrypto(deviceId: String, userId: String, json: String): String
    @JvmStatic external fun nativeDeviceBuildRename(deviceId: String, newName: String): String
    @JvmStatic external fun nativeDeviceBuildDelete(deviceId: String, authType: String, authSession: String, password: String): String
    @JvmStatic external fun nativeDeviceFormatFingerprint(rawKey: String): String
    @JvmStatic external fun nativeDeviceGetTrustLabel(crossSigningVerified: Boolean, locallyVerified: Boolean): String
    @JvmStatic external fun nativeDeviceFormatLastSeen(timestampMs: Long): String
    @JvmStatic external fun nativeDeviceIsInactive(lastSeenTs: Long, inactivityDays: Int): Boolean
    @JvmStatic external fun nativeDeviceSatisfiesVersion(clientVersion: String, minRequired: String): Boolean

    // --- Room Directory ---

    @JvmStatic external fun nativeRoomDirBuildSearch(searchTerm: String, limit: Int, since: String): String
    @JvmStatic external fun nativeRoomDirParseResponse(json: String): String
    @JvmStatic external fun nativeRoomDirBuildVisibility(visibility: Int): String
    @JvmStatic external fun nativeRoomDirParseVisibility(json: String): String
    @JvmStatic external fun nativeRoomDirCheckAlias(aliasLocalPart: String, json: String): String
    @JvmStatic external fun nativeRoomDirFormatPreview(roomJson: String): String

    // --- Session Manager (multi-account) ---

    @JvmStatic external fun nativeSessionComputeId(userId: String, deviceId: String): String
    @JvmStatic external fun nativeSessionCreate(credentialsJson: String, configJson: String, loginType: Int): String
    @JvmStatic external fun nativeSessionOpen(sessionId: String): Boolean
    @JvmStatic external fun nativeSessionClose(sessionId: String): Boolean
    @JvmStatic external fun nativeSessionRemove(sessionId: String): Boolean
    @JvmStatic external fun nativeSessionSetActive(sessionId: String): Boolean
    @JvmStatic external fun nativeSessionGetActive(): String
    @JvmStatic external fun nativeSessionHasActive(): Boolean
    @JvmStatic external fun nativeSessionGetAll(): String
    @JvmStatic external fun nativeSessionCount(): Int

    // --- Server Notice Handler ---

    @JvmStatic external fun nativeServerNoticeParse(errorJson: String): String
    @JvmStatic external fun nativeServerNoticeFormatLimit(errorJson: String, mode: Int): String
    @JvmStatic external fun nativeServerNoticeGetDescription(errorCode: String): String
    @JvmStatic external fun nativeServerNoticeIsResourceLimit(errorCode: String): Boolean
    @JvmStatic external fun nativeServerNoticeIsRateLimit(errorCode: String): Boolean
    @JvmStatic external fun nativeServerNoticeIsConsent(errorCode: String): Boolean
    @JvmStatic external fun nativeServerNoticeFormatDowntime(retryAfterMs: Long): String
    @JvmStatic external fun nativeServerNoticeGetBanner(errorJson: String): String

    // --- Media Upload Manager ---

    @JvmStatic external fun nativeUploadParseResponse(json: String): String
    @JvmStatic external fun nativeUploadBuildContent(attachmentJson: String, mxcUrl: String): String
    @JvmStatic external fun nativeUploadIsSizeValid(fileSize: Long): Boolean
    @JvmStatic external fun nativeUploadFormatSizeWarning(fileSize: Long, maxSize: Long): String
    @JvmStatic external fun nativeUploadGetProgress(): String
    @JvmStatic external fun nativeUploadResetProgress(totalBytes: Long)
    @JvmStatic external fun nativeUploadSetMaxSize(maxBytes: Long)

    // --- Identity Server Manager ---

    @JvmStatic external fun nativeIdentityParse3pid(input: String): String
    @JvmStatic external fun nativeIdentityBuildBind(threePid: String): String
    @JvmStatic external fun nativeIdentityBuildLookup(pidsJson: String): String
    @JvmStatic external fun nativeIdentityParseLookup(json: String): String
    @JvmStatic external fun nativeIdentitySetServer(url: String): String
    @JvmStatic external fun nativeIdentityGetServer(): String

    // --- Event Relations ---

    @JvmStatic external fun nativeRelationParse(eventContent: String): String
    @JvmStatic external fun nativeRelationIsReply(eventContent: String): Boolean
    @JvmStatic external fun nativeRelationIsEdit(eventContent: String): Boolean
    @JvmStatic external fun nativeRelationIsReaction(eventContent: String): Boolean
    @JvmStatic external fun nativeRelationExtractThreadRoot(eventContent: String): String
    @JvmStatic external fun nativeRelationExtractReplySource(eventContent: String): String
    @JvmStatic external fun nativeRelationBuildReply(eventId: String): String
    @JvmStatic external fun nativeRelationBuildEdit(eventId: String): String
    @JvmStatic external fun nativeRelationBuildThread(eventId: String, replyToId: String): String
    @JvmStatic external fun nativeRelationBuildAnnotation(eventId: String, key: String): String

    // --- Cross-Signing ---

    @JvmStatic external fun nativeCrossSigningIsInit(): Boolean
    @JvmStatic external fun nativeCrossSigningCanSign(): Boolean
    @JvmStatic external fun nativeCrossSigningBuildKeys(userId: String, mskPublic: String, uskPublic: String, sskPublic: String): String
    @JvmStatic external fun nativeCrossSigningCheckSelf(): String
    @JvmStatic external fun nativeCrossSigningImportKeys(mskPrivate: String, uskPrivate: String, sskPrivate: String): String
    @JvmStatic external fun nativeCrossSigningTrustMaster()

    // --- Draft Manager ---

    @JvmStatic external fun nativeDraftSave(roomId: String, content: String, type: Int, linkedEventId: String)
    @JvmStatic external fun nativeDraftGet(roomId: String): String
    @JvmStatic external fun nativeDraftDelete(roomId: String)
    @JvmStatic external fun nativeDraftHasDraft(roomId: String): Boolean
    @JvmStatic external fun nativeDraftAutoSave(roomId: String, text: String): Boolean
    @JvmStatic external fun nativeDraftStripPrefix(text: String): String

    // --- Room History Visibility ---

    @JvmStatic external fun nativeRoomStateParseVisibility(contentJson: String): String
    @JvmStatic external fun nativeRoomStateParseJoinRules(contentJson: String): String
    @JvmStatic external fun nativeRoomStateShouldShare(contentJson: String): Boolean
    @JvmStatic external fun nativeRoomStateIsPublic(roomId: String): Boolean
    @JvmStatic external fun nativeRoomStateIsInviteOnly(roomId: String): Boolean
    @JvmStatic external fun nativeRoomStateSetVisibility(roomId: String, visibility: Int)
    @JvmStatic external fun nativeRoomStateSetJoinRule(roomId: String, joinRule: Int)

    // --- Terms/Consent Manager ---

    @JvmStatic external fun nativeTermsParse(json: String): String
    @JvmStatic external fun nativeTermsBuildAgree(urlsJson: String): String
    @JvmStatic external fun nativeTermsAreRequired(errorJson: String): Boolean
    @JvmStatic external fun nativeTermsGetPending(responseJson: String, agreedJson: String): String

    // --- Transparent Overlay ---

    @JvmStatic external fun nativeOverlaySetConfig(configJson: String)
    @JvmStatic external fun nativeOverlayTouchDown(x: Double, y: Double, pointerId: Int, timeNs: Long): Int
    @JvmStatic external fun nativeOverlayTouchMove(x: Double, y: Double, pointerId: Int, timeNs: Long): Int
    @JvmStatic external fun nativeOverlayTouchUp(pointerId: Int, timeNs: Long): Int
    @JvmStatic external fun nativeOverlayBack(timeNs: Long): Int
    @JvmStatic external fun nativeOverlayTick(timeNs: Long): Int
    @JvmStatic external fun nativeOverlayGetState(): String

    // --- Transparent Overlay: Safety Mode ---
    @JvmStatic external fun nativeOverlaySetSafetyMode(mode: Int)
    @JvmStatic external fun nativeOverlaySetSafetyPerms(permissionsJson: String)
    @JvmStatic external fun nativeOverlayIsTouchAllowed(action: Int): Boolean
    @JvmStatic external fun nativeOverlaySafetyToJson(): String

    // --- Message Composer ---

    @JvmStatic external fun nativeComposerSetText(text: String)
    @JvmStatic external fun nativeComposerGetState(): String
    @JvmStatic external fun nativeComposerEnterRegular()
    @JvmStatic external fun nativeComposerEnterEdit(eventId: String)
    @JvmStatic external fun nativeComposerEnterQuote(eventId: String)
    @JvmStatic external fun nativeComposerEnterReply(eventId: String)
    @JvmStatic external fun nativeComposerApplyBold(text: String, selStart: Int, selEnd: Int): String
    @JvmStatic external fun nativeComposerApplyItalic(text: String, selStart: Int, selEnd: Int): String
    @JvmStatic external fun nativeComposerBuildQuoted(quotedText: String, replyText: String, quotedSender: String): String
    @JvmStatic external fun nativeComposerAutoEmoji(text: String): String
    @JvmStatic external fun nativeComposerExtractMention(text: String, cursorPos: Int): String
    @JvmStatic external fun nativeComposerValidate(text: String, maxLength: Int): String

    // --- Text Undo (delete protection) ---

    @JvmStatic external fun nativeUndoSetConfig(configJson: String)
    @JvmStatic external fun nativeUndoCheckpoint(text: String, cursorPos: Int, description: String)
    @JvmStatic external fun nativeUndoOnSelectAll(text: String, cursorPos: Int)
    @JvmStatic external fun nativeUndoOnBeforePaste(currentText: String, cursorPos: Int, pastedText: String)
    @JvmStatic external fun nativeUndoDo(): String
    @JvmStatic external fun nativeUndoRedo(): String
    @JvmStatic external fun nativeUndoGetState(): String

    // --- Room Permissions ---

    @JvmStatic external fun nativePermParse(powerLevelsJson: String): String
    @JvmStatic external fun nativePermGetRole(userId: String, powerLevel: Int): String
    @JvmStatic external fun nativePermBuildContent(powerLevelsJson: String): String
    @JvmStatic external fun nativePermBuildKick(userId: String, reason: String): String
    @JvmStatic external fun nativePermBuildBan(userId: String, reason: String): String
    @JvmStatic external fun nativePermFormatChange(userId: String, oldPower: Int, newPower: Int): String

    // --- Offline Cache Manager ---

    @JvmStatic external fun nativeCacheRegisterRoom(roomJson: String)
    @JvmStatic external fun nativeCacheGetPlan(): String
    @JvmStatic external fun nativeCacheGetStats(): String
    @JvmStatic external fun nativeCacheGetPressure(availableBytes: Long, reservedBytes: Long): String
    @JvmStatic external fun nativeCacheEvictToFree(targetBytes: Long, availableBytes: Long, reservedBytes: Long): String
    @JvmStatic external fun nativeCacheRecordHit(roomId: String, bytes: Long)
    @JvmStatic external fun nativeCacheRecordMiss(roomId: String, bytes: Long)

    // --- Spoiler Manager ---

    @JvmStatic external fun nativeSpoilerBuildImage(body: String, mxcUrl: String, mimeType: String, width: Int, height: Int, sizeBytes: Long, reason: String): String
    @JvmStatic external fun nativeSpoilerBuildText(body: String, reason: String): String
    @JvmStatic external fun nativeSpoilerHasSpoiler(formattedBody: String): Boolean
    @JvmStatic external fun nativeSpoilerDetectType(formattedBody: String): String
    @JvmStatic external fun nativeSpoilerBuildContent(body: String, mxcUrl: String, msgType: String, reason: String): String

    // --- WebRTC Utils ---

    @JvmStatic external fun nativeFormatCallDuration(seconds: Int): String
    @JvmStatic external fun nativeIsCallEvent(eventType: String): Boolean

    // --- Message Retry ---

    @JvmStatic external fun nativeComputeRetryDelay(retryCount: Int, maxDelayMs: Int): Int
    @JvmStatic external fun nativeDecideRetry(retryCount: Int, errorCode: Int, retryAfterHeader: String): String
    @JvmStatic external fun nativeFormatMessageStatus(state: Int): String

    // --- Sync Utils ---


    // --- Event Display ---

    @JvmStatic external fun nativeClassifyEvent(eventType: String, msgType: String): Int

    // --- Permalink ---

    @JvmStatic external fun nativeBuildEventPermalink(roomId: String, eventId: String): String

    // --- Network Monitor ---

    @JvmStatic external fun nativeGetRecommendedMediaQuality(type: Int, connected: Boolean, metered: Boolean, signal: Int, latency: Double, loss: Double): String

    // --- Client Info ---

    @JvmStatic external fun nativeCompareSemver(a: String, b: String): Int

    // --- Keyshare ---

    @JvmStatic external fun nativeBuildKeyRequestBody(roomId: String, sessionId: String, senderKey: String, algorithm: String, requestId: String, deviceId: String): String

    // --- Display Name ---

    @JvmStatic external fun nativeUserIdToDisplayName(userId: String): String
    @JvmStatic external fun nativeUserIdToColor(userId: String): String

    // --- Message Location ---

    @JvmStatic external fun nativeEstimatePaginationRequests(missingEvents: Int, pageSize: Int): Int

    // --- Timeline Utils ---

    @JvmStatic external fun nativeShouldAutoScroll(isOwnMessage: Boolean): Boolean

    // --- Cross Signing ---


    // --- Edit History ---

    @JvmStatic external fun nativeGetEditBadgeText(editCount: Int): String

    // --- Read Marker / Unread Count ---
    // Ported from: TimelineViewModel.kt (read marker index math)
    //              ReadMarkers.kt (server-side read marker management)
    //              RoomSummary.kt (unread count display)

    /**
     * Compute the read marker position and unread statistics.
     * @return JSON with lastReadEventId, firstUnreadEventId, unreadCount,
     *         unreadMentions, hasUnread, readMarkerIndex
     */
    @JvmStatic external fun nativeComputeReadMarker(
        lastReadEventId: String,
        loadedEventIds: Array<String>,
        loadedSenders: Array<String>,
        isMention: BooleanArray,
        isHighlight: BooleanArray,
        myUserId: String
    ): String

    @JvmStatic external fun nativeAdvanceReadMarker(roomId: String, latestEventId: String): String
    @JvmStatic external fun nativeReadMarkerToJson(lastReadEventId: String, unreadCount: Int, unreadMentions: Int, unreadHighlights: Int, hasUnread: Boolean): String

    // --- Kotlin fallbacks for Read Marker ---

    fun computeReadMarkerFallback(
        lastReadEventId: String,
        loadedEventIds: Array<String>,
        loadedSenders: Array<String>,
        isMention: BooleanArray,
        isHighlight: BooleanArray,
        myUserId: String
    ): JSONObject {
        val result = JSONObject()
        result.put("lastReadEventId", lastReadEventId)
        result.put("firstUnreadEventId", "")
        result.put("unreadCount", 0)
        result.put("unreadMentions", 0)
        result.put("hasUnread", false)
        result.put("readMarkerIndex", -1)

        if (lastReadEventId.isEmpty() || loadedEventIds.isEmpty()) return result

        val markerIndex = loadedEventIds.indexOf(lastReadEventId)
        if (markerIndex < 0) return result

        result.put("readMarkerIndex", markerIndex)

        var unreadCount = loadedEventIds.size - markerIndex - 1
        var unreadMentions = 0
        for (i in markerIndex + 1 until loadedEventIds.size) {
            if (i < loadedSenders.size && loadedSenders[i] == myUserId) {
                unreadCount--
                continue
            }
            if (i < isMention.size && isMention[i]) unreadMentions++
        }

        result.put("unreadCount", maxOf(0, unreadCount))
        result.put("unreadMentions", unreadMentions)
        result.put("hasUnread", unreadCount > 0)
        if (markerIndex + 1 < loadedEventIds.size) {
            result.put("firstUnreadEventId", loadedEventIds[markerIndex + 1])
        }
        return result
    }

    fun shouldShowJumpToUnreadFallback(readMarkerJson: String): Boolean {
        return readMarkerJson.contains("\"hasUnread\": true") ||
               readMarkerJson.contains("\"hasUnread\":true")
    }

    fun formatUnreadJumpLabelFallback(unreadCount: Int, unreadMentions: Int): String {
        val sb = StringBuilder()
        sb.append(unreadCount)
        if (unreadCount == 1) sb.append(" new message")
        else sb.append(" new messages")
        if (unreadMentions > 0) sb.append(" ($unreadMentions mentions)")
        return sb.toString()
    }

    fun advanceReadMarkerFallback(roomId: String, latestEventId: String): String {
        return latestEventId
    }

    // --- Slash Commands ---
    // Ported from: SlashCommandParser.kt, SlashCommand.kt, Command.kt

    @JvmStatic external fun nativeParseSlashCommand(text: String): String
    @JvmStatic external fun nativeFormatSlashCommand(command: String, arguments: String, type: Int, senderDisplayName: String): String
    @JvmStatic external fun nativeIsKnownSlashCommand(text: String): Boolean
    @JvmStatic external fun nativeGetAvailableCommands(): String

    // --- Typing Monitor ---
    // Ported from: TypingUsersTracker.kt, TypingHelper.kt, TypingView.kt

    @JvmStatic external fun nativeUpdateTypingState(roomId: String, typingUserIds: Array<String>, displayNames: Array<String>, nowMs: Long): String
    @JvmStatic external fun nativeIsUserTyping(typingStateJson: String, userId: String, nowMs: Long): Boolean

    // --- URL Preview / OpenGraph ---
    // Ported from: UrlPreviewer.kt, EventHtmlRenderer.kt

    @JvmStatic external fun nativeParseUrlPreview(html: String, baseUrl: String): String
    @JvmStatic external fun nativeExtractHtmlTitle(html: String): String
    @JvmStatic external fun nativeResolveUrl(baseUrl: String, relative: String): String
    @JvmStatic external fun nativeStripHtmlTags(html: String): String

    // --- Power Levels / Room Permissions ---
    // Ported from: PowerLevelsContent.kt, RoomPermissions.kt, PowerLevelsHelper.kt

    @JvmStatic external fun nativeParsePowerLevels(json: String): String
    @JvmStatic external fun nativeComputeUserPermissions(powerLevelsJson: String, userId: String): String
    @JvmStatic external fun nativeGetUserRole(powerLevel: Int): String

    // --- Well-Known / Server Discovery ---
    // Ported from: WellKnown.kt, LoginServerUrlFormatter.kt

    @JvmStatic external fun nativeParseWellKnown(json: String): String
    @JvmStatic external fun nativeFormatServerUrl(input: String): String
    @JvmStatic external fun nativeIsValidHomeserverUrl(url: String): Boolean
    @JvmStatic external fun nativeNeedsWellKnownLookup(input: String): Boolean

    // --- Room Sort / Ordering ---
    // Ported from: RoomComparator.kt, RoomListViewModel.kt

    @JvmStatic external fun nativeSortRooms(roomsJson: String): String
    @JvmStatic external fun nativeGetRoomSortKey(lastEventTs: Long, notifCount: Int, highlightCount: Int, isDirect: Boolean, hasUnread: Boolean, tagStr: String, priority: Int): Int
    @JvmStatic external fun nativeGetRoomSectionName(tagStr: String, isDirect: Boolean): String

    // --- Key Backup / Recovery ---
    // Ported from: KeysBackup.kt, KeysBackupSetupSharedViewModel.kt

    @JvmStatic external fun nativeFormatRecoveryKey(raw: String): String
    @JvmStatic external fun nativeValidateRecoveryKey(key: String): Boolean
    @JvmStatic external fun nativeIsValidPassphrase(passphrase: String): Boolean
    @JvmStatic external fun nativeComputeRecoveryKey(curve25519Key: String): String
    @JvmStatic external fun nativeParseMatrixError(json: String): String
    @JvmStatic external fun nativeGetBackupAlgorithmDescription(algorithm: String): String
    @JvmStatic external fun nativeIsSupportedBackupAlgorithm(algorithm: String): Boolean
    @JvmStatic external fun nativeGetRecoveryKeyExample(): String
    @JvmStatic external fun nativeGetMinPassphraseLength(): Int

    // --- AI Agent Executor ---
    // Enables /agent <task> — LLM-driven chat automation like Claude Code

    @JvmStatic external fun nativeBuildAgentSystemPrompt(systemPrompt: String): String
    @JvmStatic external fun nativeProcessAgentIteration(stateJson: String, llmResponse: String): String
    @JvmStatic external fun nativeParseToolCalls(llmResponse: String): String
    @JvmStatic external fun nativeExtractTextAnswer(llmResponse: String): String

    // --- Push Condition Evaluator ---
    // Ported from: EventMatchCondition.kt, Glob.kt

    @JvmStatic external fun nativeEvaluatePushCondition(eventJson: String, key: String, pattern: String): Boolean
    @JvmStatic external fun nativeExtractJsonField(json: String, fieldPath: String): String

    // --- Thread Metadata ---
    // Computes header: title, starter avatar, message count

    @JvmStatic external fun nativeComputeThreadMeta(rootContent: String, replySenders: Array<String>, replyBodies: Array<String>, replyTimestamps: LongArray): String

    // --- Pending Message Editing ---
    // Edit messages before they're sent — no need to wait for server confirmation

    @JvmStatic external fun nativeCanEditPendingMessage(localId: String, state: Int): String

    // --- Sender Notification Filter ---
    // Per-room: choose which senders trigger notifications

    @JvmStatic external fun nativeShouldNotifyForSender(settingsJson: String, senderId: String, hasMention: Boolean, hasHighlight: Boolean): Boolean
    @JvmStatic external fun nativeToggleMuteSender(settingsJson: String, senderId: String, mute: Boolean): String

    // --- String Order / Fractional Indexing ---
    // Ported from: StringOrderUtils.kt, SpaceOrderUtils.kt

    @JvmStatic external fun nativeStringMidPoints(left: String, right: String, count: Int): String
    @JvmStatic external fun nativeStringAverage(left: String, right: String): String

    // --- Event Type Classifier ---
    // Ported from: EventType.kt (146L), MessageType.kt (52L)

    @JvmStatic external fun nativeRouteEventForProcessing(eventType: String, msgType: String): String
    @JvmStatic external fun nativeIsVerificationEvent(eventType: String): Boolean

    // --- Content Guard ---
    // Protects against emoji attacks and excessive media

    @JvmStatic external fun nativeCheckEmojiAttack(text: String, maxEmojis: Int, maxUnique: Int): String

    // --- Debug Tools ---

    @JvmStatic external fun nativeGetModuleCount(): String

    // --- User Status (like Element Web) ---

    @JvmStatic external fun nativeParseUserStatus(accountDataJson: String): String

    // --- Jump to Unread Time Label ---
    // Optional: shows \"Jump to unread (3 hours ago)\" instead of just \"Jump to unread\"

    @JvmStatic external fun nativeFormatJumpToUnreadLabel(readMarkerJson: String, nowMs: Long): String

    // --- Content Utils (MXC URLs, Message Types) ---
    // Ported from: ContentUrlResolver.kt, MessageContent.kt
    @JvmStatic external fun nativeParseMessageContent(contentJson: String): String

    // --- Room State Parsers ---
    // Ported from: RoomJoinRules.kt, RoomHistoryVisibility.kt, RoomGuestAccess.kt, RoomCreate.kt
    @JvmStatic external fun nativeParseRoomCreate(contentJson: String): String

    // --- Login Flow Parser ---
    // Ported from: LoginWizard.kt, LoginFlow.kt, LoginFlowTypes.kt

    @JvmStatic external fun nativeParseLoginFlows(json: String): String
    @JvmStatic external fun nativeGetLoginFlowDescription(type: String): String
    @JvmStatic external fun nativeGetSsoProviderIcon(providerId: String): String

    // --- Device Naming / User Agent ---
    // Ported from: ComputeUserAgentUseCase.kt

    @JvmStatic external fun nativeBuildUserAgent(appName: String, appVersion: String, manufacturer: String, model: String, androidVersion: String, buildId: String, flavor: String, sdkVersion: String): String
    @JvmStatic external fun nativeBuildDeviceDisplayName(appName: String, deviceModel: String): String
    @JvmStatic external fun nativeShortDeviceName(manufacturer: String, model: String): String

    // --- Sync Filter Builder ---
    // Ported from: SyncFilterParams.kt, SyncFilterBuilder.kt

    @JvmStatic external fun nativeGetDefaultSyncFilter(): String

    // --- Server Capabilities (updated from HomeServerCapabilities.kt) ---

    @JvmStatic external fun nativeIsDelegatedOidcEnabled(authenticationIssuer: String): Boolean

    // --- Room Display Name ---
    // Ported from: RoomDisplayNameResolver.kt
    @JvmStatic external fun nativeNormalizeRoomName(name: String): String

    // --- Notification Count Formatter ---
    // Ported from: RoomSummaryFormatter.kt

    @JvmStatic external fun nativeFormatBadgeText(totalCount: Int): String

    // --- Text Format Utilities ---

    @JvmStatic external fun nativeFormatCountToShortDecimal(value: Long): String
    @JvmStatic external fun nativeFormatDurationWithUnits(totalSeconds: Long, appendSeconds: Boolean): String
    @JvmStatic external fun nativeFormatUnreadJumpLabel(unreadCount: Int, unreadMentions: Int): String
    @JvmStatic external fun nativeShouldShowJumpToUnread(readMarkerJson: String): Boolean


    // --- Permalink Parser (updated) ---

    @JvmStatic external fun nativeParsePermalinkFull(url: String): String

    // --- Kotlin fallbacks for Slash Commands ---

    fun parseSlashCommandFallback(text: String): JSONObject {
        val result = JSONObject()
        result.put("isSlashCommand", false)
        result.put("command", "")
        result.put("arguments", "")
        result.put("type", -1)
        result.put("needsMatrixId", false)
        result.put("isMessage", false)
        result.put("isAdmin", false)
        if (text.isEmpty() || !text.startsWith("/")) return result

        val spaceIdx = text.indexOf(' ')
        val cmdPart = if (spaceIdx < 0) text else text.substring(0, spaceIdx)
        val args = if (spaceIdx >= 0) text.substring(spaceIdx + 1).trim() else ""
        val lowered = cmdPart.lowercase()

        val commands = mapOf(
            "/me" to 1, "/join" to 2, "/part" to 3, "/invite" to 4,
            "/kick" to 5, "/ban" to 6, "/unban" to 7, "/op" to 8,
            "/deop" to 9, "/nick" to 10, "/topic" to 11, "/roomname" to 12,
            "/notice" to 13, "/shrug" to 14, "/tableflip" to 15, "/unflip" to 16,
            "/lenny" to 17, "/rainbow" to 18, "/rainbowme" to 19, "/plain" to 20,
            "/spoiler" to 21, "/avatar" to 22, "/discardsession" to 23,
            "/clearscalartoken" to 24, "/markdown" to 25
        )
        val type = commands[lowered] ?: return result

        result.put("isSlashCommand", true)
        result.put("command", cmdPart)
        result.put("arguments", args)
        result.put("type", type)
        result.put("isMessage", type in listOf(1, 13, 14, 15, 16, 17, 18, 19, 20, 21, 25))
        result.put("isAdmin", type in listOf(5, 6, 7, 8, 9))
        return result
    }

    fun formatSlashCommandFallback(command: String, arguments: String, type: Int, sender: String): String {
        return when (type) {
            1, 19 -> "* $sender $arguments"
            13 -> arguments
            14 -> if (arguments.isNotEmpty()) "$arguments ¯\\_(ツ)_/¯" else "¯\\_(ツ)_/¯"
            15 -> "(╯°□°）╯︵ ┻━┻"
            16 -> "┬──┬ ノ( ゜-゜ノ)"
            17 -> "( ͡° ͜ʖ ͡°)"
            21 -> "||$arguments||"
            else -> arguments
        }
    }

    // --- Room Counter (multi-account room counting, join-order numbering, export) ---

    @JvmStatic external fun nativeCountRooms(roomsJson: String, accountCount: Int, uniqueOnly: Boolean, perAccountSplit: Boolean): String
    @JvmStatic external fun nativeAssignJoinOrder(roomsJson: String, accountCount: Int): String
    @JvmStatic external fun nativeSwapAccountOrder(accountsJson: String, posA: Int, posB: Int): String
    @JvmStatic external fun nativeIsDumpBetter(candidateEventCount: Int, candidateStartMs: Long, candidateEndMs: Long, baselineEventCount: Int, baselineStartMs: Long, baselineEndMs: Long, candidateHasGaps: Boolean, baselineHasGaps: Boolean): Boolean
    @JvmStatic external fun nativePrioritizeExportServers(serversJson: String): String

    // --- Web Search ---

    @JvmStatic external fun nativeBuildSearchUrl(engine: String, endpoint: String, apiKey: String, engineId: String, query: String, maxResults: Int): String
    @JvmStatic external fun nativeParseSearchResponse(engine: String, responseJson: String, query: String): String
    @JvmStatic external fun nativeParseWebCommand(commandArgs: String): String
    @JvmStatic external fun nativeFormatSearchResultsForAgent(responseJson: String): String

    // --- Native HTTP Client (TLS bridge) ---

    @JvmStatic external fun nativeInitTlsBridge(): Boolean

    @JvmStatic fun nativeTlsRequest(host: String, port: Int, request: String, timeoutMs: Int): String {
        return tlsRequestFallback(host, port, request, timeoutMs)
    }

    // --- Native API (homeserver config) ---

    @JvmStatic external fun nativeSetHomeserverUrl(url: String)
    @JvmStatic external fun nativeSetAccessToken(token: String)
    @JvmStatic external fun nativeApiSync(filter: String, since: String, timeout: Int): String
    @JvmStatic external fun nativeApiLogin(userId: String, password: String, deviceId: String): String
    @JvmStatic external fun nativeApiSendEvent(roomId: String, eventType: String, txnId: String, contentJson: String): String
    @JvmStatic external fun nativeApiJoinRoom(roomId: String, reason: String): String
    @JvmStatic external fun nativeApiLeaveRoom(roomId: String): String
    @JvmStatic external fun nativeApiGetProfile(userId: String): String
    @JvmStatic external fun nativeApiWhoAmI(): String
    @JvmStatic external fun nativeApiLogout(): Boolean
    @JvmStatic external fun nativeApiGetRoomMembers(roomId: String): String
    @JvmStatic external fun nativeApiInviteUser(roomId: String, userId: String, reason: String): String
    @JvmStatic external fun nativeApiAvailable(): Boolean

    // --- Extended API (room, profile, search, moderation) ---

    @JvmStatic external fun nativeApiGetRoomMessages(roomId: String, from: String, dir: String, limit: Int): String
    @JvmStatic external fun nativeApiCreateRoom(name: String, topic: String, isDirect: Boolean, invitees: String): String
    @JvmStatic external fun nativeApiSearch(query: String, roomId: String, limit: Int): String
    @JvmStatic external fun nativeApiKickUser(roomId: String, userId: String, reason: String): String
    @JvmStatic external fun nativeApiBanUser(roomId: String, userId: String, reason: String): String
    @JvmStatic external fun nativeApiUnbanUser(roomId: String, userId: String): String
    @JvmStatic external fun nativeApiRedactEvent(roomId: String, eventId: String, txnId: String): String
    @JvmStatic external fun nativeApiGetPushRules(): String
    @JvmStatic external fun nativeApiCreateFilter(userId: String, filterJson: String): String
    @JvmStatic external fun nativeApiGetDisplayName(userId: String): String
    @JvmStatic external fun nativeApiSetDisplayName(userId: String, displayName: String): String
    @JvmStatic external fun nativeApiGetVersions(): String
    @JvmStatic external fun nativeApiLogoutAll(): Boolean
    @JvmStatic external fun nativeApiPublicRooms(server: String, query: String, limit: Int): String

    // --- Display Name & Avatar Utilities ---


    // --- Permalink Builder ---

    @JvmStatic external fun nativeBuildRoomPermalink(roomId: String): String
    @JvmStatic external fun nativeBuildUserPermalink(userId: String): String

    // --- Media Utilities ---


    // --- Room Encryption ---


    // --- Event Display ---

    @JvmStatic external fun nativeShouldShowTimestamp(currentSender: String, currentTs: Long, previousTs: Long, showAll: Boolean): Boolean

    @JvmStatic external fun nativeGenerateDeviceName(model: String, osVersion: String): String

    // --- Account Validation ---

    @JvmStatic external fun nativeIsValidDisplayName(name: String, maxLen: Int): Boolean

    // --- Well-Known / Server Discovery ---

    @JvmStatic external fun nativeNeedsWellKnownDiscovery(homeserverUrl: String): Boolean

    // --- Polls ---

    @JvmStatic external fun nativeIsPollEnded(closeTimestampMs: Long): Boolean

    // --- Membership ---

    @JvmStatic external fun nativeCanReadMessages(membership: String): Boolean

    // --- Invites ---


    // --- Event Validation ---

    @JvmStatic external fun nativeIsBodyWithinLimits(body: String, maxLength: Int): Boolean

    // --- Widgets ---

    @JvmStatic external fun nativeIsJitsiWidget(type: String): Boolean
    @JvmStatic external fun nativeGetWidgetTypeName(type: String): String

    // --- WebRTC / Calls ---

    @JvmStatic external fun nativeIsCallExpired(createdAtMs: Long, timeoutSec: Int): Boolean

    // --- Notification Settings ---

    @JvmStatic external fun nativeParseNotifMode(action: String): String

    // --- Crypto / E2EE ---

    @JvmStatic external fun nativeIsValidDeviceKey(key: String): Boolean

    // --- Server Info ---

    @JvmStatic external fun nativeParseServerVersion(apiResponseJson: String): String

    // --- Time Formatting ---

    @JvmStatic external fun nativeFormatTimeAgoLabel(timestampMs: Long, nowMs: Long): String

    // --- Edit History ---

    @JvmStatic external fun nativeFormatEditSummary(originalBody: String, newBody: String): String

    // --- Cross-Signing ---

    @JvmStatic external fun nativeNeedsCrossSigningSetup(statusJson: String): Boolean
    @JvmStatic external fun nativeFormatCrossSigningStatus(statusJson: String): String

    // --- Event Type Display ---

    @JvmStatic external fun nativeGetEventTypeDescription(eventType: String, msgType: String): String
    @JvmStatic external fun nativeGetEventTypeIcon(eventType: String, msgType: String): String
    @JvmStatic external fun nativeIsContinuation(curSender: String, prevSender: String, curTs: Long, prevTs: Long): Boolean

    // --- Power Levels ---

    @JvmStatic external fun nativeParseRoomPowerLevels(stateContentJson: String): String
    @JvmStatic external fun nativeHasPower(plJson: String, userId: String, action: String): Boolean

    // --- SSO ---

    @JvmStatic external fun nativeIsSsoCallbackUrl(url: String): Boolean
    @JvmStatic external fun nativeExtractSsoProvider(idpId: String): String

    // --- Room Tombstone ---

    @JvmStatic external fun nativeParseRoomTombstoneContent(stateEventJson: String): String

    // --- Network Quality ---

    @JvmStatic external fun nativeClassifyNetworkQuality(signalStrength: Int, latencyMs: Double, lossRate: Double): String

    // --- Connection Monitor ---

    @JvmStatic external fun nativeFormatDowntime(downtimeMs: Long): String

    // --- Event Preview ---

    @JvmStatic external fun nativeFormatEventPreview(senderName: String, body: String, eventType: String, msgType: String, showSender: Boolean): String

    // --- Room Encryption ---

    @JvmStatic external fun nativeParseEncryptionConfig(stateContentJson: String): String
    @JvmStatic external fun nativeComputeEncryptionStatus(algorithm: String): String

    // --- Space Utilities ---

    @JvmStatic external fun nativeParseSpaceChildren(stateEventsJson: String): String

    // --- E2EE Decoration ---

    @JvmStatic external fun nativeGetE2eeIconName(state: String): String
    @JvmStatic external fun nativeGetE2eeColor(state: String): String

    // --- Backup Utilities ---

    @JvmStatic external fun nativeBuildCreateBackupBody(algorithm: String, authData: String): String
    @JvmStatic external fun nativeFormatBackupStats(infoJson: String): String
    @JvmStatic external fun nativeNeedsBackupAttention(infoJson: String): Boolean

    // --- Read Marker / Notifications ---

    @JvmStatic external fun nativeBuildRoomNotifSettingsBody(mode: String): String

    // --- Notifications / Badge ---


    // --- Presence ---

    @JvmStatic external fun nativeParsePresence(userId: String, apiResponseJson: String): String

    // --- Matrix Error ---


    // --- OpenID Token ---

    @JvmStatic external fun nativeParseOpenIdToken(json: String): String

    // --- Notification Counts ---

    @JvmStatic external fun nativeFormatCombinedNotificationCount(roomCount: Int, threadCount: Int): String
    @JvmStatic external fun nativeGetTotalUnreadCount(roomCount: Int, threadCount: Int): Int

    // --- Presence Indicator ---

    @JvmStatic external fun nativeGetPresenceIndicator(presence: String): String
    @JvmStatic external fun nativeIsPresenceStale(lastUpdatedMs: Long): Boolean
    @JvmStatic external fun nativeFormatStatusMessage(message: String, maxLen: Int): String

    // --- Space Utilities ---

    @JvmStatic external fun nativeBuildSpaceParentContent(parentSpaceId: String, canonical: Boolean): String

    // --- Widget Utilities ---

    @JvmStatic external fun nativeParseWidgetStateContent(stateContentJson: String, widgetId: String, roomId: String): String

    // --- Key Share Requests ---

    @JvmStatic external fun nativeShouldShareKey(algorithm: String, hasSession: Boolean, sessionVerified: Boolean, userTrusted: Boolean): Boolean

    // --- Recovery Key ---

    @JvmStatic external fun nativeExtractCurveKeyFromRecoveryKey(recoveryKey: String): String

    // --- Membership ---

    @JvmStatic external fun nativeIsActiveMember(membership: String): Boolean

    // --- WebRTC / Calls ---

    @JvmStatic external fun nativeBuildCallHangupContent(callId: String, reason: String): String
    @JvmStatic external fun nativeFormatCallNotification(callJson: String): String

    // --- Content Scanner / ToS ---

    @JvmStatic external fun nativeMustAcceptTos(responseJson: String): Boolean
    @JvmStatic external fun nativeBuildTosAcceptBody(version: String): String

    // --- Login Utilities ---

    @JvmStatic external fun nativeIsValidLoginCredentials(userId: String, password: String): Boolean
    @JvmStatic external fun nativeGenerateDeviceId(): String

    // --- Password Validator ---

    @JvmStatic external fun nativeComputePasswordStrength(password: String): Int
    @JvmStatic external fun nativeGetStrengthLabel(strength: Int): String
    @JvmStatic external fun nativeGeneratePasswordFeedback(password: String): String

    // --- Event Validator ---

    @JvmStatic external fun nativeIsValidEventId(eventId: String): Boolean
    @JvmStatic external fun nativeIsValidSenderId(senderId: String): Boolean
    @JvmStatic external fun nativeIsFileSizeWithinLimits(fileSize: Long, maxSizeBytes: Long): Boolean

    // --- Invite Utilities ---

    @JvmStatic external fun nativeIsInviteExpired(invitedAtMs: Long, maxAgeDays: Int): Boolean
    @JvmStatic external fun nativeBuildKnockBody(reason: String): String

    // --- Device Manager ---

    @JvmStatic external fun nativeFormatFingerprint(fingerprint: String): String

    // --- Version Compatibility ---

    @JvmStatic external fun nativeSatisfiesMinVersion(current: String, minimum: String): Boolean

    // --- Federation Version ---

    @JvmStatic external fun nativeParseFederationVersion(json: String): String

    // --- Report Utilities ---

    @JvmStatic external fun nativeIsValidReportReason(reason: String): Boolean

    // --- URL Preview ---

    @JvmStatic external fun nativeIsPreviewableUrl(url: String): Boolean
    @JvmStatic external fun nativeExtractUrls(text: String): String

    // --- Device Manager ---


    // --- Permalink Utilities ---

    @JvmStatic external fun nativeIsSameRoomPermalink(url1: String, url2: String): Boolean

    // --- Display Name (advanced) ---

    @JvmStatic external fun nativeGetBestDisplayName(displayName: String, userId: String): String
    @JvmStatic external fun nativeFormatMemberName(displayName: String, userId: String, powerLevel: Int, showBadge: Boolean): String

    // --- Identity Utilities ---

    @JvmStatic external fun nativeIsEmail(input: String): Boolean
    @JvmStatic external fun nativeIsMsisdn(input: String): Boolean
    @JvmStatic external fun nativeExtractAliasLocalpart(alias: String): String

    @JvmStatic external fun nativeIsImageUrl(url: String): Boolean

    // --- Permalink Parser ---

    @JvmStatic external fun nativeExtractRoomIdFromPermalink(url: String): String
    @JvmStatic external fun nativeExtractEventIdFromPermalink(url: String): String
    @JvmStatic external fun nativeExtractUserIdFromPermalink(url: String): String

    // --- URL Preview ---

    @JvmStatic external fun nativeTruncateDescription(text: String, maxLen: Int): String
    @JvmStatic external fun nativeExtractMetaDescription(html: String): String
    @JvmStatic external fun nativeUrlPreviewToJson(previewJson: String): String

    // --- Device Type ---

    @JvmStatic external fun nativeClassifyDeviceType(userAgent: String, clientName: String): String

    // --- Web Search ---

    @JvmStatic external fun nativeBuildSearxngUrl(endpoint: String, query: String, maxResults: Int): String
    @JvmStatic external fun nativeBuildDuckDuckGoUrl(query: String): String
    @JvmStatic external fun nativeBuildGoogleUrl(apiKey: String, engineId: String, query: String, maxResults: Int): String


    // --- Device Type ---


    // --- Knock Reason ---

    @JvmStatic external fun nativeFormatKnockReason(reason: String): String

    // --- Server Compatibility ---

    @JvmStatic external fun nativeIsServerCompatible(serverVersion: String, minRequired: String): Boolean

    // --- Device Fingerprint ---

    @JvmStatic external fun nativeExtractDeviceFingerprint(deviceId: String, keysJson: String): String

    // --- Event Distance ---

    @JvmStatic external fun nativeEventDistance(indexA: Int, indexB: Int): Int

    // --- Widget Validation ---

    @JvmStatic external fun nativeIsEtherpadWidget(type: String): Boolean
    @JvmStatic external fun nativeIsValidWidgetUrl(url: String): Boolean

    // --- Encryption Defaults ---

    @JvmStatic external fun nativeGetDefaultEncryptionAlgorithm(): String
    @JvmStatic external fun nativeRequiresDeviceVerification(algorithm: String): Boolean

    // --- Event Timestamp Validation ---

    @JvmStatic external fun nativeIsReasonableTimestamp(originServerTs: String, maxFutureMs: Long): Boolean

    // --- OIDC / MAS Authentication ---

    @JvmStatic external fun nativeDiscoverOidc(homeserverUrl: String): String
    @JvmStatic external fun nativeBuildOAuthUrl(clientId: String, redirectUri: String, state: String, codeChallenge: String, prompt: String): String
    @JvmStatic external fun nativeExchangeOidcCode(tokenEndpoint: String, clientId: String, redirectUri: String, code: String, codeVerifier: String): String
    @JvmStatic external fun nativeParseOAuthCallback(url: String, redirectUri: String): String
    @JvmStatic external fun nativeGenerateOAuthState(): String
    @JvmStatic external fun nativeGeneratePkce(): String

    // --- Native Sync Response Parser (bypass Moshi) ---
    // Controlled by Labs: SETTINGS_LABS_NATIVE_SYNC_PARSER

    @JvmStatic external fun nativeParseSyncResponse(json: String): String
    @JvmStatic external fun nativeGetNextBatch(json: String): String
    @JvmStatic external fun nativeParseSyncRoomsJson(json: String): String
    @JvmStatic external fun nativeParseEvent(json: String): String
    @JvmStatic external fun nativeParseTimeline(json: String): String
    @JvmStatic external fun nativeCountEventsInSync(json: String): Int
    @JvmStatic external fun nativeSyncResponseRoundtrip(json: String): String
    @JvmStatic external fun nativeExtractNextBatchLight(partialJson: String): String

    // --- Timeline Chunk (native pagination engine) ---

    @JvmStatic external fun nativeTimelineAddEvents(roomId: String, eventsJson: String, prevToken: String, nextToken: String, direction: Int): Int
    @JvmStatic external fun nativeTimelineGetEvents(roomId: String): String
    @JvmStatic external fun nativeTimelineGetEvent(eventId: String): String
    @JvmStatic external fun nativeTimelineClear(roomId: String)
    @JvmStatic external fun nativeTimelineGetReplies(eventId: String): String
    @JvmStatic external fun nativeTimelineGetLatestEdit(eventId: String): String
    @JvmStatic external fun nativeTimelineGetThreadEvents(rootEventId: String): String
    @JvmStatic external fun nativeTimelineChunkCount(roomId: String): Int
    @JvmStatic external fun nativeTimelineGetSnapshot(roomId: String, limit: Int, offset: Int): String
    @JvmStatic external fun nativeTimelineEventsAvailable(roomId: String, direction: Int): Int
    @JvmStatic external fun nativeTimelineAttachDb(roomId: String, dbKey: String): Boolean
    @JvmStatic external fun nativeTimelineAddSyncEvent(roomId: String, eventId: String, type: String, senderId: String, contentJson: String, originTs: Long, displayIndex: Int, stateKey: String, redacts: String, relType: String, relatesToId: String): Int

    // --- Event Utilities ---

    @JvmStatic external fun nativeFormatEventSummary(eventType: String, msgType: String, senderName: String, body: String, membership: String, displayName: String, isRedacted: Boolean, isEncrypted: Boolean): String
    @JvmStatic external fun nativeFormatTypingIndicator(namesJson: String, maxNames: Int): String
    @JvmStatic external fun nativeCalculateCapabilities(userLevel: Int, eventsDefault: Int, stateDefault: Int, inviteLvl: Int, kickLvl: Int, banLvl: Int, redactLvl: Int, notifyLvl: Int): String

    // --- Content Builder ---

    @JvmStatic external fun nativeBuildTextContent(msgType: String, body: String, formattedBody: String): String
    @JvmStatic external fun nativeBuildImageContent(body: String, mxcUrl: String, w: Int, h: Int, sz: Long, mime: String): String
    @JvmStatic external fun nativeBuildFileContent(body: String, mxcUrl: String, fileName: String, sz: Long, mime: String): String
    @JvmStatic external fun nativeBuildRoomStateContent(eventType: String, value1: String, value2: String): String

    // --- Media Utilities ---


    // --- Account Data ---

    @JvmStatic external fun nativeParseDirectMessages(json: String): String
    @JvmStatic external fun nativeParseIgnoredUsers(json: String): String
    @JvmStatic external fun nativeAddBreadcrumb(currentJson: String, roomId: String): String
    @JvmStatic external fun nativeIsValidUserId(userId: String): Boolean
    @JvmStatic external fun nativeServerNameFromMxid(mxid: String): String

    // --- Relation Builder ---

    @JvmStatic external fun nativeBuildReplyRelation(eventId: String): String
    @JvmStatic external fun nativeBuildThreadRelation(rootId: String, latestId: String, fallingBack: Boolean): String
    @JvmStatic external fun nativeBuildEditRelation(eventId: String): String
    @JvmStatic external fun nativeBuildReactionRelation(eventId: String, key: String): String
    @JvmStatic external fun nativeWrapWithRelation(contentJson: String, relationJson: String): String

    // --- Live Draft ---

    @JvmStatic external fun nativeShouldAutoDraft(text: String, threshold: Int): Boolean
    @JvmStatic external fun nativeBuildDraftMessage(prefix: String, text: String): String
    @JvmStatic external fun nativeFinalizeDraftMessage(fullDraftText: String, prefix: String): String

    // --- Pure Kotlin fallback implementations ---

    fun validateAndBuildFallback(
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): JSONObject {
        val result = JSONObject()

        if (!isEnabled) {
            return result.put("error", "/jumptodate is disabled. Enable it in Settings → Labs.")
        }

        // Validate date format
        val dateRegex = Regex("^(\\d{4})-(\\d{2})-(\\d{2})$")
        val match = dateRegex.matchEntire(dateString)
            ?: return result.put("error", "Invalid date format. Use YYYY-MM-DD.")

        val year = match.groupValues[1].toInt()
        val month = match.groupValues[2].toInt()
        val day = match.groupValues[3].toInt()

        if (month < 1 || month > 12) return result.put("error", "Invalid month. Must be 01-12.")
        if (day < 1 || day > 31) return result.put("error", "Invalid day. Must be 01-31.")

        // Days per month validation
        val daysInMonth = intArrayOf(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
        val isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)
        val maxDay = if (month == 2 && isLeapYear) 29 else daysInMonth[month - 1]

        if (day > maxDay) return result.put("error", "Invalid day for given month.")

        // Compute Unix timestamp (UTC, milliseconds) using Java's Calendar
        val cal = java.util.GregorianCalendar(java.util.TimeZone.getTimeZone("UTC"))
        cal.set(year, month - 1, day, 0, 0, 0)
        cal.set(java.util.Calendar.MILLISECOND, 0)
        val timestamp = cal.timeInMillis

        // Build MSC3030 URL
        val url = "$serverUrl/_matrix/client/unstable/org.matrix.msc3030/rooms/$roomId/timestamp_to_event?ts=$timestamp&dir=f"

        result.put("url", url)
        result.put("accessToken", accessToken)
        result.put("timestamp", timestamp)

        return result
    }

    fun parseResponseFallback(responseBody: String?, httpStatus: Int): JSONObject {
        val result = JSONObject()

        if (responseBody == null) {
            return result.put("error", "Empty response from server.")
                    .put("statusCode", httpStatus)
        }

        if (httpStatus != 200) {
            try {
                val json = JSONObject(responseBody)
                val errcode = json.optString("errcode", "")
                val error = json.optString("error", "")
                val msg = buildString {
                    append("Server returned $httpStatus")
                    if (errcode.isNotEmpty()) append(" ($errcode)")
                    if (error.isNotEmpty()) append(": $error")
                }
                result.put("error", msg)
                result.put("statusCode", httpStatus)
                return result
            } catch (e: Exception) {
                return result.put("error", "Server returned $httpStatus")
                        .put("statusCode", httpStatus)
            }
        }

        try {
            val json = JSONObject(responseBody)
            val eventId = json.optString("event_id", "")

            if (eventId.isEmpty()) {
                return result.put("error", "Response missing event_id field.")
                        .put("statusCode", httpStatus)
            }

            if (eventId == "~") {
                return result.put("error", "No events found after the given date.")
                        .put("statusCode", httpStatus)
            }

            result.put("eventId", eventId)
            result.put("statusCode", httpStatus)
            return result
        } catch (e: Exception) {
            return result.put("error", "Failed to parse server response.")
                    .put("statusCode", httpStatus)
        }
    }

    fun parseRelationFallback(eventJson: String, allowedTypes: String): JSONObject {
        val result = JSONObject()
        try {
            val json = JSONObject(eventJson)
            val content = json.optJSONObject("content") ?: json
            val relatesTo = content.optJSONObject("m.relates_to")
                ?: content.optJSONObject("relates_to") ?: return result.put("isRelation", false)

            val sourceEventId = relatesTo.optString("event_id", "")
            val relationType = relatesTo.optString("rel_type", "")

            if (sourceEventId.isEmpty()) return result.put("isRelation", false)

            val jumpableTypes = setOf("m.annotation", "m.reference", "m.replace", "m.thread")
            if (allowedTypes.isNotEmpty() && !allowedTypes.split(",").any { it == relationType }) {
                return result.put("isRelation", false)
            }
            if (!jumpableTypes.contains(relationType)) {
                return result.put("isRelation", false)
            }

            result.put("isRelation", true)
            result.put("sourceEventId", sourceEventId)
            result.put("relationType", relationType)
            return result
        } catch (e: Exception) {
            return result.put("isRelation", false)
        }
    }

    // --- Export fallback implementations ---

    fun formatEventHtmlFallback(
        senderName: String, timestamp: String, body: String,
        msgType: String, fileName: String, mediaSize: String,
        relationType: String, isContinuation: Boolean
    ): String = buildString {
        if (!isContinuation) {
            appendLine("<div class=\"mx_EventTile\">")
            appendLine("  <div class=\"mx_EventTile_info\">")
            append("    <span class=\"mx_EventTile_sender\">").append(escapeHtmlFn(senderName)).appendLine("</span>")
            if (timestamp.isNotEmpty())
                append("    <span class=\"mx_MessageTimestamp\">").append(timestamp).appendLine("</span>")
            appendLine("  </div>")
        } else {
            appendLine("<div class=\"mx_EventTile mx_EventTile_continuation\">")
        }
        appendLine("  <div class=\"mx_EventTile_body\">")
        if (msgType in listOf("m.image", "m.video", "m.file", "m.audio")) {
            appendLine("    <div class=\"mx_EventTile_attachment\">")
            append("      <span class=\"mx_Attachment_name\">").append(fileName.ifEmpty { msgType.substring(2) }).appendLine("</span>")
            if (mediaSize.isNotEmpty())
                append("      <span class=\"mx_Attachment_size\">").append(mediaSize).appendLine(" bytes</span>")
            appendLine("    </div>")
        }
        if (body.isNotEmpty()) {
            appendLine("    <div class=\"mx_EventTile_content\">")
            append("      ").appendLine(escapeHtmlFn(body))
            appendLine("    </div>")
        }
        appendLine("  </div>")
        appendLine("</div>")
    }

    fun formatEventPlainTextFallback(
        senderName: String, timestamp: String, body: String,
        msgType: String, fileName: String, relationType: String
    ): String = buildString {
        if (timestamp.isNotEmpty()) append(timestamp).append(" - ")
        append(senderName).append(": ")
        if (msgType in listOf("m.image", "m.video", "m.file", "m.audio")) {
            append("[").append(msgType.substring(2)).append(" attached")
            if (fileName.isNotEmpty()) append(": ").append(fileName)
            append("]")
        }
        if (body.isNotEmpty()) append(" ").append(body)
        if (relationType == "m.reference") append(" (in reply)")
        appendLine()
    }

    fun buildHtmlExportFallback(
        roomName: String, roomTopic: String, exportDate: String, eventHtmls: Array<String>
    ): String = buildString {
        appendLine("<!DOCTYPE html>")
        appendLine("<html lang=\"en\">")
        appendLine("<head>")
        appendLine("<meta charset=\"UTF-8\">")
        append("<title>").append(escapeHtmlFn(roomName)).appendLine(" — Chat Export</title>")
        appendLine("<style>body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;margin:0;padding:16px;background:#f5f5f5;}.mx_EventTile{background:#fff;border-radius:8px;padding:12px 16px;margin-bottom:8px;box-shadow:0 1px 2px rgba(0,0,0,0.05);}.mx_EventTile_continuation{margin-top:-4px;border-radius:0 0 8px 8px;}.mx_EventTile_sender{font-weight:600;color:#333;margin-right:8px;}.mx_MessageTimestamp{color:#999;font-size:0.85em;}.mx_EventTile_body{color:#222;line-height:1.5;}.mx_EventTile_attachment{background:#f0f0f0;border-radius:4px;padding:8px;margin-bottom:8px;}.mx_ExportHeader{background:#fff;border-radius:8px;padding:16px;margin-bottom:16px;box-shadow:0 1px 3px rgba(0,0,0,0.1);}.mx_ExportHeader h1{margin:0 0 8px;font-size:1.5em;}.mx_ExportHeader p{margin:4px 0;color:#666;font-size:0.9em;}.mx_EventTile_content{white-space:pre-wrap;word-wrap:break-word;}.mx_EventTile_reaction{font-style:italic;color:#666;}</style>")
        appendLine("</head>")
        appendLine("<body>")
        appendLine("<div class=\"mx_ExportHeader\">")
        append("  <h1>").append(escapeHtmlFn(roomName)).appendLine("</h1>")
        if (roomTopic.isNotEmpty()) append("  <p>").append(escapeHtmlFn(roomTopic)).appendLine("</p>")
        append("  <p>Exported: ").append(exportDate).appendLine("</p>")
        append("  <p>Total messages: ").append(eventHtmls.size).appendLine("</p>")
        appendLine("</div>")
        for (html in eventHtmls) append(html)
        appendLine("<hr><p style=\"color:#999;text-align:center;\">Exported with Progressive Chat</p>")
        appendLine("</body></html>")
    }

    private fun escapeHtmlFn(s: String): String = s
        .replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace("\"", "&quot;")

    // --- Kotlin fallback cache (in-memory HashMap) ---

    data class CachedEvent(
        val eventId: String = "",
        val senderId: String = "",
        val senderName: String = "",
        val timestamp: String = "",
        val body: String = "",
        val msgType: String = "",
        val eventType: String = "",
        val relationType: String = "",
        val sourceEventId: String = "",
        val sentByMe: Boolean = false
    )

    private val fallbackCache = mutableMapOf<String, CachedEvent>()
    private val fallbackRelationIndex = mutableMapOf<String, MutableList<String>>()

    fun cachePutFallback(
        eventId: String, senderId: String, senderName: String, timestamp: String,
        body: String, msgType: String, eventType: String,
        relationType: String, sourceEventId: String, sentByMe: Boolean
    ) {
        val event = CachedEvent(eventId, senderId, senderName, timestamp, body, msgType, eventType, relationType, sourceEventId, sentByMe)
        synchronized(fallbackCache) {
            val old = fallbackCache[eventId]
            if (old != null && old.sourceEventId.isNotEmpty()) {
                fallbackRelationIndex[old.sourceEventId]?.remove(eventId)
            }
            fallbackCache[eventId] = event
            if (sourceEventId.isNotEmpty()) {
                fallbackRelationIndex.getOrPut(sourceEventId) { mutableListOf() }.add(eventId)
            }
        }
    }

    fun cacheGetContextFallback(eventId: String): JSONObject {
        val result = JSONObject()
        synchronized(fallbackCache) {
            val e = fallbackCache[eventId] ?: return result.put("cached", false)
            result.put("cached", true)
            result.put("eventId", e.eventId)
            result.put("senderId", e.senderId)
            result.put("senderName", e.senderName)
            result.put("timestamp", e.timestamp)
            result.put("body", e.body)
            result.put("msgType", e.msgType)
            result.put("eventType", e.eventType)
            result.put("sentByMe", e.sentByMe)
            result.put("hasFailed", false)
        }
        return result
    }

    fun cacheClearFallback() {
        synchronized(fallbackCache) {
            fallbackCache.clear()
            fallbackRelationIndex.clear()
        }
    }

    fun cacheSizeFallback(): Int = synchronized(fallbackCache) { fallbackCache.size }

    // --- Translation fallback ---

    fun buildTranslateRequestFallback(
        text: String, sourceLang: String, targetLang: String,
        apiEndpoint: String, apiToken: String, model: String
    ): JSONObject {
        val body = JSONObject()
        body.put("model", model)
        val messages = org.json.JSONArray()
        val systemMsg = JSONObject().apply {
            put("role", "system")
            var prompt = "You are a translator. Translate the following text"
            if (sourceLang.isNotEmpty()) prompt += " from $sourceLang"
            prompt += " to $targetLang. Output ONLY the translation, nothing else."
            put("content", prompt)
        }
        messages.put(systemMsg)
        messages.put(JSONObject().apply {
            put("role", "user")
            put("content", text)
        })
        body.put("messages", messages)
        body.put("temperature", 0.1)
        return body
    }

    fun parseTranslateResponseFallback(responseBody: String?, httpStatus: Int): JSONObject {
        val result = JSONObject()
        if (responseBody == null) return result.put("success", false).put("error", "Empty response")
        if (httpStatus != 200) {
            try {
                val json = JSONObject(responseBody)
                val error = json.optJSONObject("error")
                val msg = error?.optString("message", "Server returned $httpStatus") ?: "Server returned $httpStatus"
                return result.put("success", false).put("error", msg).put("statusCode", httpStatus)
            } catch (e: Exception) {
                return result.put("success", false).put("error", "Server returned $httpStatus").put("statusCode", httpStatus)
            }
        }
        try {
            val json = JSONObject(responseBody)
            val choices = json.optJSONArray("choices")
            if (choices == null || choices.length() == 0)
                return result.put("success", false).put("error", "No choices in response")
            val message = choices.getJSONObject(0).optJSONObject("message")
            if (message == null)
                return result.put("success", false).put("error", "No message in response")
            val content = message.optString("content", "")
            if (content.isEmpty())
                return result.put("success", false).put("error", "No content in response")
            result.put("success", true)
            result.put("translatedText", content)
            return result
        } catch (e: Exception) {
            return result.put("success", false).put("error", "Failed to parse response")
        }
    }

    // --- Room Counter fallbacks ---

    @JvmStatic fun countRoomsFallback(roomsJson: String, accountCount: Int, uniqueOnly: Boolean, perAccountSplit: Boolean): String {
        try {
            val rooms = JSONArray(roomsJson)
            val counts = IntArray(accountCount)
            for (i in 0 until rooms.length()) {
                val r = rooms.getJSONObject(i)
                val acc = r.optInt("accountIndex", 0)
                if (acc in 0 until accountCount) counts[acc]++
            }
            val total = counts.sum()
            if (perAccountSplit) return counts.joinToString("+")
            return "($total)"
        } catch (e: Exception) { return "(0)" }
    }

    @JvmStatic fun assignJoinOrderFallback(roomsJson: String, accountCount: Int): String {
        try {
            val rooms = JSONArray(roomsJson)
            val counters = IntArray(accountCount)
            var global = 0
            for (i in 0 until rooms.length()) {
                val r = rooms.getJSONObject(i)
                val acc = r.optInt("accountIndex", 0)
                if (acc in 0 until accountCount) counters[acc]++
                r.put("joinOrder", if (acc in 0 until accountCount) counters[acc] else 0)
                global++
                r.put("globalJoinOrder", global)
            }
            return rooms.toString()
        } catch (e: Exception) { return "[]" }
    }

    @JvmStatic fun swapAccountOrderFallback(accountsJson: String, posA: Int, posB: Int): String {
        try {
            val arr = JSONArray(accountsJson)
            if (posA in 0 until arr.length() && posB in 0 until arr.length()) {
                val tmp = arr.getJSONObject(posA).optInt("orderIndex", 0)
                arr.getJSONObject(posA).put("orderIndex", arr.getJSONObject(posB).optInt("orderIndex", 0))
                arr.getJSONObject(posB).put("orderIndex", tmp)
            }
            return arr.toString()
        } catch (e: Exception) { return accountsJson }
    }

    @JvmStatic fun isDumpBetterFallback(candidateEventCount: Int, candidateStartMs: Long, candidateEndMs: Long, baselineEventCount: Int, baselineStartMs: Long, baselineEndMs: Long, candidateHasGaps: Boolean, baselineHasGaps: Boolean): Boolean {
        if (candidateHasGaps && !baselineHasGaps) return false
        if (!candidateHasGaps || baselineHasGaps) {
            if (candidateEventCount > baselineEventCount) return true
            if (candidateEventCount == baselineEventCount && candidateStartMs <= baselineStartMs && candidateEndMs >= baselineEndMs) return true
        }
        return false
    }

    @JvmStatic fun prioritizeServersFallback(serversJson: String): String {
        try {
            val servers = JSONArray(serversJson)
            val active = mutableListOf<JSONObject>()
            for (i in 0 until servers.length()) {
                val s = servers.getJSONObject(i)
                if (!s.optBoolean("excluded", false)) active.add(s)
            }
            active.sortBy { it.optInt("priority", 0) }
            return JSONArray(active).toString()
        } catch (e: Exception) { return serversJson }
    }

    // --- Web Search fallbacks ---

    @JvmStatic fun buildSearchUrlFallback(engine: String, endpoint: String, apiKey: String, engineId: String, query: String, maxResults: Int): String {
        val q = query.replace(" ", "+")
        return when (engine) {
            "searxng" -> "${endpoint}search?q=${q}&format=json&pageno=1"
            "ddg" -> "https://api.duckduckgo.com/?q=${q}&format=json&no_html=1"
            "google" -> "https://customsearch.googleapis.com/customsearch/v1?key=${apiKey}&cx=${engineId}&q=${q}&num=${maxResults}"
            else -> ""
        }
    }

    @JvmStatic fun parseSearchResponseFallback(engine: String, json: String, query: String): String {
        try {
            val r = JSONObject(json)
            val result = JSONObject()
            result.put("query", query)
            val results = JSONArray()
            if (engine == "searxng") {
                val arr = r.optJSONArray("results") ?: return result.put("results", results).toString()
                for (i in 0 until arr.length()) {
                    val item = arr.getJSONObject(i)
                    val ri = JSONObject()
                    ri.put("title", item.optString("title"))
                    ri.put("url", item.optString("url"))
                    ri.put("snippet", item.optString("content"))
                    ri.put("source", item.optString("engine"))
                    results.put(ri)
                }
            } else if (engine == "google") {
                val arr = r.optJSONArray("items") ?: return result.put("results", results).toString()
                for (i in 0 until arr.length()) {
                    val item = arr.getJSONObject(i)
                    val ri = JSONObject()
                    ri.put("title", item.optString("title"))
                    ri.put("url", item.optString("link"))
                    ri.put("snippet", item.optString("snippet"))
                    ri.put("source", "google")
                    results.put(ri)
                }
            } else if (engine == "ddg") {
                val abstract = r.optString("Abstract")
                if (abstract.isNotEmpty()) {
                    val ri = JSONObject()
                    ri.put("title", r.optString("Heading", "Instant Answer"))
                    ri.put("url", r.optString("AbstractURL"))
                    ri.put("snippet", abstract)
                    ri.put("source", "duckduckgo")
                    results.put(ri)
                }
            }
            result.put("results", results)
            result.put("success", results.length() > 0)
            return result.toString()
        } catch (e: Exception) {
            return """{"query":"$query","results":[],"success":false,"error":"${e.message}"}"""
        }
    }

    @JvmStatic fun parseWebCommandFallback(args: String): String {
        val parts = args.trim().split(Regex("\\s+"), limit = 2)
        if (parts.isEmpty() || parts[0].isEmpty()) return """{"isValid":false}"""
        val engineWords = mapOf("ddg" to "ddg", "duckduckgo" to "ddg", "google" to "google", "g" to "google", "searxng" to "searxng", "sx" to "searxng")
        val engine = if (parts.size >= 2 && parts[0] in engineWords) engineWords[parts[0]] ?: "" else ""
        val query = if (engine.isNotEmpty() && parts.size >= 2) parts[1] else args.trim()
        return """{"engine":"$engine","query":"$query","isValid":${query.isNotEmpty()}}"""
    }

    @JvmStatic fun formatSearchForAgentFallback(responseJson: String): String {
        try {
            val r = JSONObject(responseJson)
            val sb = StringBuilder()
            sb.append("Web results for '${r.optString("query")}':\n")
            val results = r.optJSONArray("results") ?: return sb.toString()
            for (i in 0 until results.length()) {
                val item = results.getJSONObject(i)
                sb.append("${i+1}. ${item.optString("title")}\n")
                sb.append("   ${item.optString("url")}\n")
                val snippet = item.optString("snippet")
                if (snippet.isNotEmpty()) sb.append("   $snippet\n")
            }
            return sb.toString()
        } catch (e: Exception) { return "" }
    }

    // --- Live Draft fallbacks ---

    @JvmStatic fun shouldAutoDraftFallback(text: String, threshold: Int): Boolean {
        if (text.length < threshold) return false
        return text.contains(" ") || text.contains("\t") || text.contains("\n")
    }

    @JvmStatic fun buildDraftMessageFallback(prefix: String, text: String): String = "$prefix$text"

    @JvmStatic fun finalizeDraftFallback(full: String, prefix: String): String {
        return if (full.startsWith(prefix)) full.removePrefix(prefix) else full
    }

    // --- TLS Request Fallback (called from C++ via JNI) ---

    @JvmStatic fun tlsRequestFallback(host: String, port: Int, request: String, timeoutMs: Int): String {
        try {
            val socket = javax.net.ssl.SSLSocketFactory.getDefault().createSocket(host, port) as javax.net.ssl.SSLSocket
            socket.soTimeout = timeoutMs
            socket.startHandshake()

            val out = socket.outputStream
            out.write(request.toByteArray(Charsets.UTF_8))
            out.flush()

            val input = socket.inputStream
            val response = input.bufferedReader().readText()
            socket.close()
            return response
        } catch (e: Exception) {
            return ""
        }
    }

    // --- Native HTTP client initialization ---

    @JvmStatic fun initTlsBridge(): Boolean {
        return try {
            System.loadLibrary("progressive_native")
            true
        } catch (e: UnsatisfiedLinkError) {
            false
        }
    }

    // --- OIDC fallbacks ---

    @JvmStatic fun discoverOidcFallback(homeserverUrl: String): String {
        return """{"supportsOidc":false,"supportsPassword":true,"errorMessage":"Native OIDC not loaded"}"""
    }

    @JvmStatic fun buildOAuthUrlFallback(clientId: String, redirectUri: String, state: String, codeChallenge: String, prompt: String): String {
        return ""
    }

    @JvmStatic fun exchangeOidcCodeFallback(tokenEndpoint: String, clientId: String, redirectUri: String, code: String, codeVerifier: String): String {
        return """{"success":false,"errorMessage":"Native OIDC not loaded"}"""
    }

    @JvmStatic fun parseOAuthCallbackFallback(url: String, redirectUri: String): String {
        return if (url.contains("error=access_denied")) """{"action":"go_back"}"""
               else if (url.contains("code=")) """{"action":"success","fullUrl":"$url"}"""
               else """{"action":"none"}"""
    }

    @JvmStatic fun generateOAuthStateFallback(): String {
        return java.util.UUID.randomUUID().toString().replace("-", "")
    }

    @JvmStatic fun generatePkceFallback(): String {
        val verifier = java.util.Base64.getUrlEncoder().withoutPadding()
            .encodeToString(java.util.UUID.randomUUID().toString().toByteArray())
            .take(64)
        return """{"codeVerifier":"$verifier","codeChallenge":"$verifier"}"""
    }

    // --- Timeline Chunk fallbacks ---
    @JvmStatic fun timelineAddEventsFallback(roomId: String, eventsJson: String, prevToken: String, nextToken: String, direction: Int): Int = 0
    @JvmStatic fun timelineGetEventsFallback(roomId: String): String = "[]"
    @JvmStatic fun timelineGetEventFallback(eventId: String): String = "{}"
    @JvmStatic fun timelineClearFallback(roomId: String) {}
    @JvmStatic fun timelineGetRepliesFallback(eventId: String): String = "[]"
    @JvmStatic fun timelineGetLatestEditFallback(eventId: String): String = eventId
    @JvmStatic fun timelineGetThreadEventsFallback(rootEventId: String): String = "[]"
    @JvmStatic fun timelineChunkCountFallback(roomId: String): Int = 0
    @JvmStatic fun timelineGetSnapshotFallback(roomId: String, limit: Int, offset: Int): String = "[]"
    @JvmStatic fun timelineEventsAvailableFallback(roomId: String, direction: Int): Int = 0
    @JvmStatic fun timelineAttachDbFallback(roomId: String, dbKey: String): Boolean = false
    @JvmStatic fun timelineAddSyncEventFallback(roomId: String, eventId: String, type: String, senderId: String, contentJson: String, originTs: Long, displayIndex: Int, stateKey: String, redacts: String, relType: String, relatesToId: String): Int = -1

    // --- Event Utils fallbacks ---
    @JvmStatic fun formatEventSummaryFallback(eventType: String, msgType: String, senderName: String, body: String, membership: String, displayName: String, isRedacted: Boolean, isEncrypted: Boolean): String {
        if (isRedacted) return "Message removed"
        return if (senderName.isNotEmpty()) "$senderName: $body" else body
    }
    @JvmStatic fun formatTypingIndicatorFallback(namesJson: String, maxNames: Int): String {
        val names = try { JSONArray(namesJson) } catch (e: Exception) { return "" }
        if (names.length() == 0) return ""
        if (names.length() == 1) return "${names.getString(0)} is typing..."
        return "${names.getString(0)} and ${names.length()-1} other(s) are typing..."
    }
    @JvmStatic fun calculateCapabilitiesFallback(userLevel: Int, eventsDefault: Int, stateDefault: Int, inviteLvl: Int, kickLvl: Int, banLvl: Int, redactLvl: Int, notifyLvl: Int): String {
        return """{"isOwner":${userLevel >= 100},"isModerator":${userLevel >= 50},"canSendMessages":${userLevel >= eventsDefault},"canInvite":${userLevel >= inviteLvl},"canKick":${userLevel >= kickLvl},"canBan":${userLevel >= banLvl}}"""
    }

    // --- Content Builder fallbacks ---
    @JvmStatic fun buildTextContentFallback(msgType: String, body: String, formattedBody: String): String {
        val fb = if (formattedBody.isNotEmpty()) ""","format":"org.matrix.custom.html","formatted_body":"$formattedBody"""" else ""
        return """{"msgtype":"$msgType","body":"$body"$fb}"""
    }
    @JvmStatic fun buildImageContentFallback(body: String, mxcUrl: String, w: Int, h: Int, sz: Long, mime: String): String {
        return """{"msgtype":"m.image","body":"$body","url":"$mxcUrl","info":{"w":$w,"h":$h,"size":$sz,"mimetype":"$mime"}}"""
    }
    @JvmStatic fun buildFileContentFallback(body: String, mxcUrl: String, fileName: String, sz: Long, mime: String): String {
        return """{"msgtype":"m.file","body":"$body","url":"$mxcUrl","filename":"$fileName","info":{"size":$sz,"mimetype":"$mime"}}"""
    }
    @JvmStatic fun buildRoomStateContentFallback(eventType: String, value1: String, value2: String): String {
        return when (eventType) {
            "m.room.name" -> """{"name":"$value1"}"""
            "m.room.topic" -> """{"topic":"$value1"}"""
            "m.room.member" -> """{"membership":"$value1","reason":"$value2"}"""
            else -> """{"value":"$value1"}"""
        }
    }

    // --- Media Utils fallbacks ---
    @JvmStatic fun calculateThumbnailSizeFallback(origW: Int, origH: Int, maxW: Int, maxH: Int): String = """{"width":$maxW,"height":$maxH}"""
    @JvmStatic fun formatFileSizeFallback(bytes: Long): String {
        if (bytes < 1024) return "$bytes B"
        val units = arrayOf("B", "KB", "MB", "GB")
        var size = bytes.toDouble(); var u = 0
        while (size >= 1024 && u < 3) { size /= 1024; u++ }
        return "%.1f %s".format(size, units[u])
    }
    @JvmStatic fun formatDurationFallback(ms: Long): String {
        val s = (ms / 1000).toInt()
        return if (s >= 3600) "%d:%02d:%02d".format(s/3600, s%3600/60, s%60)
               else "%d:%02d".format(s/60, s%60)
    }
    @JvmStatic fun sanitizeFilenameFallback(name: String, maxLen: Int): String = name.take(maxLen)

    // --- Account Data fallbacks ---
    @JvmStatic fun parseDirectMessagesFallback(json: String): String = json
    @JvmStatic fun parseIgnoredUsersFallback(json: String): String = "[]"
    @JvmStatic fun addBreadcrumbFallback(currentJson: String, roomId: String): String {
        return """{"recent_rooms":["$roomId"]}"""
    }
    @JvmStatic fun isValidUserIdFallback(userId: String): Boolean = userId.startsWith("@") && userId.contains(":")
    @JvmStatic fun serverNameFromMxidFallback(mxid: String): String = mxid.substringAfter(":")

    // --- Relation Builder fallbacks ---
    @JvmStatic fun buildReplyRelationFallback(eventId: String): String = """{"m.in_reply_to":{"event_id":"$eventId"}}"""
    @JvmStatic fun buildThreadRelationFallback(rootId: String, latestId: String, fallingBack: Boolean): String =
        """{"rel_type":"m.thread","event_id":"$rootId","m.in_reply_to":{"event_id":"$latestId"},"is_falling_back":$fallingBack}"""
    @JvmStatic fun buildEditRelationFallback(eventId: String): String = """{"rel_type":"m.replace","event_id":"$eventId"}"""
    @JvmStatic fun buildReactionRelationFallback(eventId: String, key: String): String = """{"rel_type":"m.annotation","event_id":"$eventId","key":"$key"}"""
    @JvmStatic fun wrapWithRelationFallback(contentJson: String, relationJson: String): String {
        return contentJson.dropLast(1) + ",\"m.relates_to\":" + relationJson + "}"
    }

    // --- Native Sync Response Parser fallbacks ---
    @JvmStatic fun nativeParseSyncResponseFallback(json: String): String =
        """{"next_batch":"","account_data_count":0,"presence_count":0,"to_device_count":0,"rooms_join_count":0,"rooms_invite_count":0,"rooms_leave_count":0,"device_lists_changed":0}"""
    @JvmStatic fun nativeGetNextBatchFallback(json: String): String = ""
    @JvmStatic fun nativeParseSyncRoomsJsonFallback(json: String): String = "[]"
    @JvmStatic fun nativeParseEventFallback(json: String): String = "{}"
    @JvmStatic fun nativeParseTimelineFallback(json: String): String =
        """{"events_count":0,"limited":false,"prev_batch":""}"""
    @JvmStatic fun nativeCountEventsInSyncFallback(json: String): Int = 0
    @JvmStatic fun nativeSyncResponseRoundtripFallback(json: String): String = json
    @JvmStatic fun nativeExtractNextBatchLightFallback(partialJson: String): String = ""

    // --- Native API fallbacks ---
    @JvmStatic fun nativeSetHomeserverUrlFallback(url: String) {}
    @JvmStatic fun nativeSetAccessTokenFallback(token: String) {}
    @JvmStatic fun nativeApiLoginFallback(userId: String, password: String, deviceId: String): String =
        """{"success":false,"error":"Native library not loaded"}"""
    @JvmStatic fun nativeApiSyncFallback(filter: String, since: String, timeout: Int): String =
        """{"next_batch":"","rooms_joined":0,"rooms_invited":0,"rooms_left":0,"events_total_timeline":0}"""
    @JvmStatic fun nativeApiSendEventFallback(roomId: String, eventType: String, txnId: String, contentJson: String): String = ""
    @JvmStatic fun nativeApiJoinRoomFallback(roomId: String, reason: String): String = ""
    @JvmStatic fun nativeApiLeaveRoomFallback(roomId: String): String = ""
    @JvmStatic fun nativeApiGetProfileFallback(userId: String): String = "{}"
    @JvmStatic fun nativeApiWhoAmIFallback(): String = """{"user_id":""}"""
    @JvmStatic fun nativeApiLogoutFallback(): Boolean = false
    @JvmStatic fun nativeApiGetRoomMembersFallback(roomId: String): String = """{"chunk":[]}"""
    @JvmStatic fun nativeApiInviteUserFallback(roomId: String, userId: String, reason: String): String = ""
    @JvmStatic fun nativeApiAvailableFallback(): Boolean = false

    @JvmStatic fun nativeApiGetRoomMessagesFallback(roomId: String, from: String, dir: String, limit: Int): String = """{"chunk":[]}"""
    @JvmStatic fun nativeApiCreateRoomFallback(name: String, topic: String, isDirect: Boolean, invitees: String): String = ""
    @JvmStatic fun nativeApiSearchFallback(query: String, roomId: String, limit: Int): String = """{"search_categories":{}}"""
    @JvmStatic fun nativeApiKickUserFallback(roomId: String, userId: String, reason: String): String = ""
    @JvmStatic fun nativeApiBanUserFallback(roomId: String, userId: String, reason: String): String = ""
    @JvmStatic fun nativeApiUnbanUserFallback(roomId: String, userId: String): String = ""
    @JvmStatic fun nativeApiRedactEventFallback(roomId: String, eventId: String, txnId: String): String = ""
    @JvmStatic fun nativeApiGetPushRulesFallback(): String = """{}"""
    @JvmStatic fun nativeApiCreateFilterFallback(userId: String, filterJson: String): String = ""
    @JvmStatic fun nativeApiGetDisplayNameFallback(userId: String): String = ""
    @JvmStatic fun nativeApiSetDisplayNameFallback(userId: String, displayName: String): String = ""
    @JvmStatic fun nativeApiGetVersionsFallback(): String = """{"versions":[]}"""
    @JvmStatic fun nativeApiLogoutAllFallback(): Boolean = false
    @JvmStatic fun nativeApiPublicRoomsFallback(server: String, query: String, limit: Int): String = """{"chunk":[]}"""

    // --- Display Name & Avatar fallbacks ---
    @JvmStatic fun nativeUserIdToDisplayNameFallback(userId: String, capitalize: Boolean): String {
        val name = userId.removePrefix("@").substringBefore(":")
        return if (capitalize) name.replaceFirstChar { it.uppercase() } else name
    }
    @JvmStatic fun nativeGetInitialsFallback(name: String, maxChars: Int): String = name.take(maxChars).uppercase()

    // --- Permalink fallbacks ---
    @JvmStatic fun nativeBuildEventPermalinkFallback(roomId: String, eventId: String): String = "https://matrix.to/#/$roomId/$eventId"
    @JvmStatic fun nativeBuildRoomPermalinkFallback(roomId: String): String = "https://matrix.to/#/$roomId"
    @JvmStatic fun nativeBuildUserPermalinkFallback(userId: String): String = "https://matrix.to/#/$userId"

    // --- Media fallbacks ---
    @JvmStatic fun nativeFormatFileSizeFallback(bytes: Long): String {
        if (bytes < 1024) return "$bytes B"
        val kb = bytes / 1024.0
        if (kb < 1024) return "%.1f KB".format(kb)
        return "%.1f MB".format(kb / 1024)
    }
    @JvmStatic fun nativeMimeToMsgTypeFallback(mimeType: String): String = when {
        mimeType.startsWith("image/") -> "m.image"
        mimeType.startsWith("video/") -> "m.video"
        mimeType.startsWith("audio/") -> "m.audio"
        else -> "m.file"
    }

    // --- Key Backup fallbacks ---
    @JvmStatic fun nativeFormatRecoveryKeyFallback(raw: String): String = raw.chunked(4).joinToString(" ")
    @JvmStatic fun nativeValidateRecoveryKeyFallback(key: String): Boolean = key.length == 59 && key.all { it in '0'..'9' || it in 'A'..'Z' || it in 'a'..'z' }
    @JvmStatic fun nativeGetBackupAlgorithmDescriptionFallback(algorithm: String): String = when(algorithm) {
        "m.megolm_backup.v1.curve25519-aes-sha2" -> "Encrypted backup (Megolm key)"
        else -> algorithm
    }
    @JvmStatic fun nativeIsSupportedBackupAlgorithmFallback(algorithm: String): Boolean = algorithm.startsWith("m.megolm_backup")
    @JvmStatic fun nativeGetRecoveryKeyExampleFallback(): String = "EsTj 4fGz 8hWq ... (example)"
    @JvmStatic fun nativeIsValidPassphraseFallback(passphrase: String): Boolean = passphrase.length >= 8
    @JvmStatic fun nativeGetMinPassphraseLengthFallback(): Int = 8

    // --- Room Encryption fallback ---
    @JvmStatic fun nativeIsRoomEncryptedFallback(stateContentJson: String): Boolean = stateContentJson.contains("m.megolm") || stateContentJson.contains("encrypted")

    // --- Event Display fallback ---
    @JvmStatic fun nativeShouldShowTimestampFallback(currentSender: String, currentTs: Long, previousTs: Long, showAll: Boolean): Boolean {
        if (showAll) return true
        return (currentTs - previousTs) > 300_000 // 5 minutes
    }

    // --- User ID fallback ---
    @JvmStatic fun nativeIsValidUserIdFallback(userId: String): Boolean = userId.startsWith("@") && userId.contains(":")

    // --- Date & Time fallbacks ---
    @JvmStatic fun nativeFormatDurationFallback(durationMs: Long): String {
        val sec = durationMs / 1000
        if (sec < 60) return "${sec}s"
        val min = sec / 60
        if (min < 60) return "${min}m ${sec % 60}s"
        return "${min / 60}h ${min % 60}m"
    }
    @JvmStatic fun nativeFormatPresenceFallback(presence: String, lastActiveMs: Long): String = when(presence) {
        "online" -> "Online"
        "unavailable" -> "Idle"
        else -> "Offline"
    }
    @JvmStatic fun nativeBuildDeviceDisplayNameFallback(appName: String, deviceModel: String): String = "$appName ($deviceModel)"
    @JvmStatic fun nativeGenerateDeviceNameFallback(model: String, osVersion: String): String = "Progressive Chat ($model, Android $osVersion)"
    @JvmStatic fun nativeIsValidDisplayNameFallback(name: String, maxLen: Int): Boolean = name.isNotEmpty() && name.length <= maxLen

    // --- Well-Known fallbacks ---
    @JvmStatic fun nativeParseWellKnownFallback(responseJson: String): String = """{"homeserver_url":"","identity_server":"","valid":false}"""
    @JvmStatic fun nativeNeedsWellKnownDiscoveryFallback(homeserverUrl: String): Boolean = !homeserverUrl.startsWith("https://matrix.")

    // --- Polls fallback ---
    @JvmStatic fun nativeIsPollEndedFallback(closeTimestampMs: Long): Boolean = closeTimestampMs > 0 && closeTimestampMs < System.currentTimeMillis()

    // --- Membership fallback ---
    @JvmStatic fun nativeCanReadMessagesFallback(membership: String): Boolean = membership == "join" || membership == "invite"

    // --- Invites fallback ---
    @JvmStatic fun nativeBuildInviteBodyFallback(userId: String, reason: String): String {
        val r = if (reason.isNotEmpty()) ""","reason":"$reason"""" else ""
        return """{"user_id":"$userId"$r}"""
    }

    // --- Event Validation fallback ---
    @JvmStatic fun nativeIsBodyWithinLimitsFallback(body: String, maxLength: Int): Boolean = body.length <= maxLength

    // --- Widgets fallback ---
    @JvmStatic fun nativeIsJitsiWidgetFallback(type: String): Boolean = type == "jitsi" || type == "m.jitsi"
    @JvmStatic fun nativeGetWidgetTypeNameFallback(type: String): String = when(type) {
        "jitsi", "m.jitsi" -> "Jitsi Meet"
        "etherpad", "m.etherpad" -> "Etherpad"
        else -> type
    }

    // --- WebRTC / Calls fallbacks ---
    @JvmStatic fun nativeIsCallExpiredFallback(createdAtMs: Long, timeoutSec: Int): Boolean =
        (System.currentTimeMillis() - createdAtMs) > timeoutSec * 1000
    @JvmStatic fun nativeFormatCallDurationFallback(seconds: Int): String {
        val m = seconds / 60; val s = seconds % 60
        return if (m > 0) "${m}m ${s}s" else "${s}s"
    }

    // --- Notification Settings fallbacks ---
    @JvmStatic fun nativeFormatNotifModeFallback(mode: String): String = when(mode) {
        "all" -> "All messages"
        "mentions" -> "Mentions only"
        "none" -> "Muted"
        else -> "Default"
    }
    @JvmStatic fun nativeParseNotifModeFallback(action: String): String = when {
        action.contains("notify") -> "all"
        action.contains("dont_notify") -> "none"
        else -> "mentions"
    }

    // --- Crypto fallback ---
    @JvmStatic fun nativeIsValidDeviceKeyFallback(key: String): Boolean =
        key.length in 43..44 && key.all { it in 'A'..'Z' || it in 'a'..'z' || it in '0'..'9' || it == '+' || it == '/' || it == '=' }

    // --- Server Info fallback ---
    @JvmStatic fun nativeParseServerVersionFallback(apiResponseJson: String): String = "0.0.0"

    // --- Time fallback ---
    @JvmStatic fun nativeFormatTimeAgoLabelFallback(timestampMs: Long, nowMs: Long): String {
        val diff = (nowMs - timestampMs) / 1000
        return when {
            diff < 60 -> "just now"
            diff < 3600 -> "${diff / 60}m ago"
            diff < 86400 -> "${diff / 3600}h ago"
            else -> "${diff / 86400}d ago"
        }
    }

    // --- Edit History fallbacks ---
    @JvmStatic fun nativeFormatEditSummaryFallback(originalBody: String, newBody: String): String = newBody
    @JvmStatic fun nativeGetEditBadgeTextFallback(editCount: Int): String = if (editCount > 0) "Edited ($editCount)" else ""

    // --- Cross-Signing fallbacks ---
    @JvmStatic fun nativeNeedsCrossSigningSetupFallback(statusJson: String): Boolean =
        !statusJson.contains("\"master_key_ok\":true") || !statusJson.contains("\"self_signing_key_ok\":true")
    @JvmStatic fun nativeFormatCrossSigningStatusFallback(statusJson: String): String {
        val masterOk = statusJson.contains("\"master_ok\":true") || statusJson.contains("\"master_key_ok\":true")
        val selfOk = statusJson.contains("\"self_signing_ok\":true") || statusJson.contains("\"self_signing_key_ok\":true")
        val userOk = statusJson.contains("\"user_signing_ok\":true")
        return when {
            masterOk && selfOk && userOk -> "Verified"
            masterOk && selfOk -> "Self-verified"
            masterOk -> "Not verified"
            else -> "Setup needed"
        }
    }

    // --- Native SQLite DB fallbacks ---
    @JvmStatic fun nativeSqliteDbOpenFallback(dbPath: String, key: String): Boolean =
        false // Fallback: no-op, use Realm

    @JvmStatic fun nativeSqliteDbCloseFallback(key: String) {}
    @JvmStatic fun nativeSqliteDbInsertEventFallback(key: String, eventId: String, roomId: String,
        type: String, senderId: String, contentJson: String, originTs: Long,
        ageTs: Long, displayIndex: Int): Boolean = false

    @JvmStatic fun nativeSqliteDbInsertEventRelFallback(key: String, eventId: String, roomId: String,
        type: String, senderId: String, contentJson: String, originTs: Long,
        ageTs: Long, displayIndex: Int, stateKey: String, redacts: String,
        relType: String, relatesToId: String): Boolean = false

    @JvmStatic fun nativeSqliteDbQueryEventsFallback(key: String, roomId: String,
        limit: Int, offset: Int, ascending: Boolean): String = "[]"

    @JvmStatic fun nativeSqliteDbQueryEventFallback(key: String, eventId: String): String = "{}"
    @JvmStatic fun nativeSqliteDbDeleteEventFallback(key: String, eventId: String) {}
    @JvmStatic fun nativeSqliteDbCountEventsFallback(key: String, roomId: String): Int = 0
    @JvmStatic fun nativeSqliteDbMaxDisplayIndexFallback(key: String, roomId: String): Int = 0

    @JvmStatic fun nativeSqliteDbUpsertRoomFallback(key: String, roomId: String,
        displayName: String, avatarUrl: String, topic: String, membership: String,
        notifCount: Int, highlightCount: Int, lastActivityMs: Long,
        isDirect: Boolean, isSpace: Boolean, isFavourite: Boolean, isEncrypted: Boolean): Boolean = false

    @JvmStatic fun nativeSqliteDbQueryRoomsFallback(key: String): String = "[]"
    @JvmStatic fun nativeSqliteDbBeginTransactionFallback(key: String) {}
    @JvmStatic fun nativeSqliteDbCommitTransactionFallback(key: String) {}
    @JvmStatic fun nativeSqliteDbSchemaVersionFallback(key: String): Int = 0

    // --- Event Type / Power Level fallbacks ---
    @JvmStatic fun nativeGetEventTypeDescriptionFallback(eventType: String, msgType: String): String = when(msgType) {
        "m.image" -> "Image"; "m.video" -> "Video"; "m.audio" -> "Audio"; "m.file" -> "File"
        "m.emote" -> "* ${eventType}"; "m.notice" -> "Notice"; else -> "Message"
    }
    @JvmStatic fun nativeGetEventTypeIconFallback(eventType: String, msgType: String): String = when(msgType) {
        "m.image" -> "📷"; "m.video" -> "🎥"; "m.audio" -> "🎵"; "m.file" -> "📎"
        "m.emote" -> "💬"; else -> "💬"
    }
    @JvmStatic fun nativeIsContinuationFallback(curSender: String, prevSender: String, curTs: Long, prevTs: Long): Boolean =
        curSender == prevSender && (curTs - prevTs) < 300_000
    @JvmStatic fun nativeParseRoomPowerLevelsFallback(stateContentJson: String): String =
        """{"users_default":0,"events_default":0,"state_default":50,"ban":50,"kick":50,"redact":50,"invite":50}"""
    @JvmStatic fun nativeHasPowerFallback(plJson: String, userId: String, action: String): Boolean = true

    // --- SSO fallbacks ---
    @JvmStatic fun nativeIsSsoCallbackUrlFallback(url: String): Boolean = url.contains("loginToken")
    @JvmStatic fun nativeExtractSsoProviderFallback(idpId: String): String = when(idpId) {
        "google" -> "Google"; "github" -> "GitHub"; "facebook" -> "Facebook"
        "apple" -> "Apple"; "gitlab" -> "GitLab"; else -> idpId
    }

    // --- Room Tombstone fallback ---
    @JvmStatic fun nativeParseRoomTombstoneContentFallback(stateEventJson: String): String = "{}"

    // --- Network Quality fallback ---
    @JvmStatic fun nativeClassifyNetworkQualityFallback(signalStrength: Int, latencyMs: Double, lossRate: Double): String =
        when {
            lossRate > 10.0 || latencyMs > 500 -> "Poor"
            lossRate > 2.0 || latencyMs > 200 -> "Fair"
            else -> "Good"
        }

    // --- Connection Monitor fallback ---
    @JvmStatic fun nativeFormatDowntimeFallback(downtimeMs: Long): String {
        val s = downtimeMs / 1000
        return when { s < 60 -> "${s}s"; s < 3600 -> "${s/60}m ${s%60}s"; else -> "${s/3600}h ${(s%3600)/60}m" }
    }

    // --- Event Preview fallback ---
    @JvmStatic fun nativeFormatEventPreviewFallback(senderName: String, body: String, eventType: String, msgType: String, showSender: Boolean): String {
        val prefix = if (showSender) "$senderName: " else ""
        return prefix + body.take(120).replace("\n", " ")
    }

    // --- Room Encryption fallbacks ---
    @JvmStatic fun nativeParseEncryptionConfigFallback(stateContentJson: String): String = "{}"
    @JvmStatic fun nativeComputeEncryptionStatusFallback(algorithm: String): String = when {
        algorithm.isEmpty() -> "Not encrypted"
        algorithm.contains("megolm") -> "Encrypted (Megolm)"
        else -> "Encrypted"
    }

    // --- Space Utilities fallback ---
    @JvmStatic fun nativeParseSpaceChildrenFallback(stateEventsJson: String): String = "[]"

    // --- E2EE Decoration fallbacks ---
    @JvmStatic fun nativeGetE2eeIconNameFallback(state: String): String = when {
        state.contains("verified") -> "ic_shield_verified"
        state.contains("warning") -> "ic_shield_warning"
        else -> "ic_shield_black"
    }
    @JvmStatic fun nativeGetE2eeColorFallback(state: String): String = when {
        state.contains("verified") -> "#4CAF50"
        state.contains("warning") -> "#FF9800"
        else -> "#F44336"
    }

    // --- Backup fallbacks ---
    @JvmStatic fun nativeBuildCreateBackupBodyFallback(algorithm: String, authData: String): String =
        """{"algorithm":"$algorithm","auth_data":$authData}"""
    @JvmStatic fun nativeFormatBackupStatsFallback(infoJson: String): String = "Backup stats"
    @JvmStatic fun nativeNeedsBackupAttentionFallback(infoJson: String): Boolean {
        val total = Regex("\"total_keys\":(\\d+)").find(infoJson)?.groupValues?.get(1)?.toIntOrNull() ?: 0
        val backed = Regex("\"backed_up_keys\":(\\d+)").find(infoJson)?.groupValues?.get(1)?.toIntOrNull() ?: 0
        return total > 0 && backed < total
    }

    // --- Room Notif Settings fallback ---
    @JvmStatic fun nativeBuildRoomNotifSettingsBodyFallback(mode: String): String = """{"actions":["$mode"]}"""

    // --- Notifications / Badge fallback ---
    @JvmStatic fun nativeFormatBadgeTextFallback(totalCount: Int): String =
        if (totalCount > 99) "99+" else if (totalCount > 0) "$totalCount" else ""

    // --- Presence fallback ---
    @JvmStatic fun nativeParsePresenceFallback(userId: String, apiResponseJson: String): String =
        """{"user_id":"$userId","presence":"offline","last_active_ago_ms":0}"""

    // --- Matrix Error fallback ---
    @JvmStatic fun nativeGetRetryAfterMsFallback(_errorJson: String): Long = 3000L

    // --- OpenID Token fallback ---
    @JvmStatic fun nativeParseOpenIdTokenFallback(json: String): String = "{}"

    // --- Notification Counts fallbacks ---
    @JvmStatic fun nativeFormatCombinedNotificationCountFallback(roomCount: Int, threadCount: Int): String {
        val total = roomCount + threadCount
        return if (total > 99) "99+" else if (total > 0) "$total" else ""
    }
    @JvmStatic fun nativeGetTotalUnreadCountFallback(roomCount: Int, threadCount: Int): Int = roomCount + threadCount

    // --- Presence Indicator fallbacks ---
    @JvmStatic fun nativeGetPresenceIndicatorFallback(presence: String): String = when(presence) {
        "online" -> "🟢"; "unavailable" -> "🟡"; else -> "⚫"
    }
    @JvmStatic fun nativeIsPresenceStaleFallback(lastUpdatedMs: Long): Boolean =
        (System.currentTimeMillis() - lastUpdatedMs) > 300_000
    @JvmStatic fun nativeFormatStatusMessageFallback(message: String, maxLen: Int): String =
        if (message.length <= maxLen) message else message.take(maxLen) + "…"

    // --- Space fallbacks ---
    @JvmStatic fun nativeBuildSpaceChildContentFallback(suggested: Boolean, order: String, autoJoin: Boolean, canonical: Boolean): String =
        """{"suggested":$suggested,"order":"$order","auto_join":$autoJoin}"""
    @JvmStatic fun nativeBuildSpaceParentContentFallback(parentSpaceId: String, canonical: Boolean): String =
        """{"via":[""],"canonical":$canonical}"""

    // --- Widget fallback ---
    @JvmStatic fun nativeParseWidgetStateContentFallback(stateContentJson: String, widgetId: String, roomId: String): String =
        """{"widget_id":"$widgetId","type":"","name":"","url":""}"""

    // --- Key Share fallback ---
    @JvmStatic fun nativeShouldShareKeyFallback(algorithm: String, hasSession: Boolean, sessionVerified: Boolean, userTrusted: Boolean): Boolean =
        hasSession && sessionVerified

    // --- Recovery Key fallbacks ---
    @JvmStatic fun nativeComputeRecoveryKeyFallback(curve25519Key: String): String = curve25519Key
    @JvmStatic fun nativeExtractCurveKeyFromRecoveryKeyFallback(recoveryKey: String): String = recoveryKey.replace(" ", "")

    // --- Membership fallbacks ---
    @JvmStatic fun nativeFormatMembershipFallback(membership: String): String = when(membership) {
        "join" -> "Joined"; "invite" -> "Invited"; "knock" -> "Knocked"; "ban" -> "Banned"; else -> "Left"
    }
    @JvmStatic fun nativeIsActiveMemberFallback(membership: String): Boolean = membership == "join" || membership == "invite"

    // --- Calls fallbacks ---
    @JvmStatic fun nativeBuildCallHangupContentFallback(callId: String, reason: String): String =
        """{"call_id":"$callId","reason":"$reason"}"""
    @JvmStatic fun nativeFormatCallNotificationFallback(callJson: String): String = "Incoming call"

    // --- Content Scanner / ToS fallbacks ---
    @JvmStatic fun nativeIsServerNoticeFallback(eventContentJson: String): Boolean = eventContentJson.contains("\"server_notice\"")
    @JvmStatic fun nativeMustAcceptTosFallback(responseJson: String): Boolean = responseJson.contains("\"m.consent\"")
    @JvmStatic fun nativeBuildTosAcceptBodyFallback(version: String): String = """{"version":"$version"}"""

    // --- Login Utilities fallbacks ---
    @JvmStatic fun nativeIsValidLoginCredentialsFallback(userId: String, password: String): Boolean =
        userId.isNotEmpty() && password.isNotEmpty()
    @JvmStatic fun nativeGenerateDeviceIdFallback(): String =
        "PC" + java.util.UUID.randomUUID().toString().take(8).uppercase()

    // --- Password Validator fallbacks ---
    @JvmStatic fun nativeValidatePasswordFallback(password: String): String {
        val len = password.length
        val hasUpper = password.any { it.isUpperCase() }
        val hasLower = password.any { it.isLowerCase() }
        val hasDigit = password.any { it.isDigit() }
        val valid = len >= 8 && hasUpper && hasLower && hasDigit
        val strength = (len * 5 + (if (hasUpper) 20 else 0) + (if (hasLower) 20 else 0) + (if (hasDigit) 20 else 0)).coerceAtMost(100)
        val label = when { strength >= 80 -> "Strong"; strength >= 60 -> "Good"; strength >= 40 -> "Fair"; else -> "Weak" }
        return """{"valid":$valid,"strength":$strength,"strength_label":"$label","feedback":""}"""
    }
    @JvmStatic fun nativeComputePasswordStrengthFallback(password: String): Int {
        val len = password.length.coerceAtMost(20)
        val classes = listOf(password.any { it.isUpperCase() }, password.any { it.isLowerCase() }, password.any { it.isDigit() }).count { it }
        return (len * 4 + classes * 15).coerceAtMost(100)
    }
    @JvmStatic fun nativeGetStrengthLabelFallback(strength: Int): String = when {
        strength >= 80 -> "Strong"; strength >= 60 -> "Good"; strength >= 40 -> "Fair"; else -> "Weak"
    }
    @JvmStatic fun nativeGeneratePasswordFeedbackFallback(password: String): String = when {
        password.length < 8 -> "Use at least 8 characters"
        !password.any { it.isUpperCase() } -> "Add an uppercase letter"
        !password.any { it.isDigit() } -> "Add a digit"
        else -> ""
    }

    // --- Event Validator fallbacks ---
    @JvmStatic fun nativeIsValidEventIdFallback(eventId: String): Boolean = eventId.startsWith("\$") && eventId.length > 10
    @JvmStatic fun nativeIsValidSenderIdFallback(senderId: String): Boolean = senderId.startsWith("@") && senderId.contains(":")
    @JvmStatic fun nativeIsFileSizeWithinLimitsFallback(fileSize: Long, maxSizeBytes: Long): Boolean = fileSize <= maxSizeBytes

    // --- Invite fallbacks ---
    @JvmStatic fun nativeIsInviteExpiredFallback(invitedAtMs: Long, maxAgeDays: Int): Boolean =
        (System.currentTimeMillis() - invitedAtMs) > maxAgeDays * 86400_000L
    @JvmStatic fun nativeBuildKnockBodyFallback(reason: String): String =
        """{"reason":"$reason"}"""

    // --- Device fallbacks ---
    @JvmStatic fun nativeFormatFingerprintFallback(fingerprint: String): String =
        fingerprint.chunked(4).joinToString(" ")
    @JvmStatic fun nativeIsDeviceInactiveFallback(lastSeenMs: Long): Boolean =
        lastSeenMs > 0 && (System.currentTimeMillis() - lastSeenMs) > 90L * 86400_000

    // --- Version fallback ---
    @JvmStatic fun nativeSatisfiesMinVersionFallback(current: String, minimum: String): Boolean =
        nativeCompareSemverFallback(current, minimum) >= 0

    // --- Federation fallback ---
    @JvmStatic fun nativeParseFederationVersionFallback(json: String): String = """{"name":"","version":""}"""

    // --- Report fallbacks ---
    @JvmStatic fun nativeIsValidReportReasonFallback(reason: String): Boolean = reason.isNotEmpty()
    @JvmStatic fun nativeGetReasonDescriptionFallback(code: String): String = when(code) {
        "m.spam" -> "Spam"; "m.violence" -> "Violence"; "m.hate" -> "Hate speech"
        "m.harassment" -> "Harassment"; "m.suicide" -> "Self-harm"; "m.child" -> "Child exploitation"
        else -> code
    }

    // --- Secret Storage fallbacks ---
    @JvmStatic fun nativeExtractDefaultSecretKeyFallback(accountDataJson: String): String = ""
    @JvmStatic fun nativeHasCrossSigningSecretsFallback(accountDataJson: String): Boolean =
        accountDataJson.contains("\"m.cross_signing")

    // --- Report fallbacks ---
    @JvmStatic fun nativeIsOffensiveFallback(score: Int): Boolean = score < -50
    @JvmStatic fun nativeTruncateReportDescriptionFallback(description: String, maxLen: Int): String =
        if (description.length <= maxLen) description else description.take(maxLen) + "..."

    // --- Content Scanner fallback ---
    @JvmStatic fun nativeIsContentScannerAvailableFallback(serverCapabilitiesJson: String): Boolean = false

    // --- Matrix Error fallbacks ---
    @JvmStatic fun nativeIsPasswordErrorFallback(errorCode: String): Boolean = errorCode == "M_WEAK_PASSWORD"
    @JvmStatic fun nativeGetAllErrorCodesFallback(): String = """["M_UNKNOWN","M_FORBIDDEN","M_BAD_JSON","M_NOT_JSON","M_NOT_FOUND","M_LIMIT_EXCEEDED","M_UNKNOWN_TOKEN","M_MISSING_TOKEN","M_WEAK_PASSWORD","M_INVALID_USERNAME","M_USER_IN_USE","M_EXCLUSIVE","M_THREEPID_IN_USE","M_THREEPID_NOT_FOUND","M_SERVER_NOT_TRUSTED","M_CONSENT_NOT_GIVEN","M_UNSUPPORTED_ROOM_VERSION","M_INCOMPATIBLE_ROOM_VERSION","M_CANNOT_LEAVE_SERVER_NOTICE_ROOM","M_RESOURCE_LIMIT_EXCEEDED"]"""

    // --- Call Content fallbacks ---
    @JvmStatic fun nativeBuildCallInviteContentFallback(callId: String, isVideo: Boolean, sdpOffer: String, lifetimeSec: Int): String =
        """{"call_id":"$callId","offer":{"type":"offer","sdp":"$sdpOffer"},"version":1,"lifetime":$lifetimeSec}"""
    @JvmStatic fun nativeBuildCallAnswerContentFallback(callId: String, sdpAnswer: String): String =
        """{"call_id":"$callId","answer":{"type":"answer","sdp":"$sdpAnswer"},"version":1}"""
    @JvmStatic fun nativeGetCallStateFallback(eventContentJson: String): String = "unknown"

    // --- Room State fallbacks ---
    @JvmStatic fun nativeIsPublicRoomFallback(stateContentJson: String): Boolean =
        stateContentJson.contains("\"join_rule\":\"public\"")
    @JvmStatic fun nativeIsInviteOnlyFallback(stateContentJson: String): Boolean =
        stateContentJson.contains("\"join_rule\":\"invite\"")
    @JvmStatic fun nativeJoinRuleToStringFallback(stateContentJson: String): String =
        Regex("\"join_rule\":\"(\\w+)\"").find(stateContentJson)?.groupValues?.get(1) ?: "unknown"
    @JvmStatic fun nativeIsHistoryPubliclyVisibleFallback(stateContentJson: String): Boolean =
        stateContentJson.contains("\"world_readable\"")
    @JvmStatic fun nativeHistoryVisibilityToStringFallback(stateContentJson: String): String =
        Regex("\"history_visibility\":\"(\\w+)\"").find(stateContentJson)?.groupValues?.get(1) ?: "unknown"
    @JvmStatic fun nativeAreGuestsAllowedFallback(stateContentJson: String): Boolean =
        stateContentJson.contains("\"guest_access\":\"can_join\"")
    @JvmStatic fun nativeIsRoomUpgradedFallback(stateContentJson: String): Boolean =
        stateContentJson.contains("\"replacement_room\"")

    // --- Matrix Pattern fallbacks ---
    @JvmStatic fun nativeIsUserIdFallback(input: String): Boolean = input.startsWith("@") && input.contains(":") && input.length > 3
    @JvmStatic fun nativeIsRoomIdFallback(input: String): Boolean = input.startsWith("!") && input.contains(":") && input.length > 3
    @JvmStatic fun nativeIsRoomAliasFallback(input: String): Boolean = input.startsWith("#") && input.contains(":") && input.length > 3
    @JvmStatic fun nativeIsEventIdFallback(input: String): Boolean = input.startsWith("\$") && input.length > 10
    @JvmStatic fun nativeIsMxcUrlFallback(url: String): Boolean = url.startsWith("mxc://")
    @JvmStatic fun nativeIsPhoneNumberFallback(input: String): Boolean = input.startsWith("+") && input.drop(1).all { it.isDigit() }
    @JvmStatic fun nativeIsValidEmailFallback(input: String): Boolean = input.contains("@") && input.contains(".")
    @JvmStatic fun nativeExtractServerNameFromIdFallback(mxid: String): String = mxid.substringAfter(":")
    @JvmStatic fun nativeExtractUserNameFromIdFallback(mxid: String): String = mxid.removePrefix("@").substringBefore(":")
    @JvmStatic fun nativeCandidateAliasFromRoomNameFallback(roomName: String, domain: String, maxLength: Int): String =
        "#${roomName.lowercase().replace(" ", "_").take(maxLength)}:$domain"

    // --- Widget fallback ---
    @JvmStatic fun nativeListRoomWidgetsFallback(stateEventsJson: String): String = "[]"

    // --- Session Rename fallback ---
    @JvmStatic fun nativeBuildSessionRenameBodyFallback(sessionId: String, newName: String): String =
        """{"session_id":"$sessionId","display_name":"$newName"}"""

    // --- Permalink fallbacks ---
    @JvmStatic fun nativeIsMatrixToPermalinkFallback(url: String): Boolean =
        url.contains("matrix.to/#/") || url.contains("matrix.to/")
    @JvmStatic fun nativeIsAppPermalinkFallback(url: String): Boolean =
        url.contains("#/") && (url.startsWith("https://matrix.to") || url.startsWith("https://app.element.io"))
    @JvmStatic fun nativeIsValidOrderStringFallback(order: String): Boolean =
        order.isNotEmpty() && order.length < 50 && order.all { it in 0x20.toChar()..0x7E.toChar() }
    @JvmStatic fun nativeIsGroupIdFallback(input: String): Boolean = input.startsWith("+") && input.contains(":") && input.length > 3

    // --- Device Name fallback ---
    @JvmStatic fun nativeParseDeviceNameFallback(userAgent: String): String = userAgent

    // --- Matrix ID fallbacks ---
    @JvmStatic fun nativeExtractMatrixIdsFallback(text: String): String {
        val users = Regex("@[^\\s:]+:[^\\s]+").findAll(text).map { it.value }.toList()
        val rooms = Regex("![^\\s:]+:[^\\s]+").findAll(text).map { it.value }.toList()
        return """{"user_ids":${users.joinToString(","){ "\"$it\"" }},"room_ids":${rooms.joinToString(","){ "\"$it\"" }},"room_aliases":[],"event_ids":[]}"""
    }
    @JvmStatic fun nativeParseMatrixToPermalinkFallback(url: String): String =
        """{"type":"","user_id":"","room_id":"","event_id":"","valid":false}"""

    // --- Login fallbacks ---
    @JvmStatic fun nativeParseLoginFlowsListFallback(apiResponseJson: String): String = """[{"type":"m.login.password","description":"Password","supported":true}]"""
    @JvmStatic fun nativeBuildUserIdentifierFallback(userId: String): String =
        """{"type":"m.id.user","user":"$userId"}"""

    // --- Notification Mode fallbacks ---
    @JvmStatic fun nativeIsNotifModeDifferentFallback(oldMode: String, newMode: String): Boolean = oldMode != newMode
    @JvmStatic fun nativeGetDefaultModeForRoomFallback(isDirect: Boolean, isEncrypted: Boolean): String =
        if (isDirect) "mentions" else "all"

    // --- Password fallbacks ---
    @JvmStatic fun nativeMeetsMinimumRequirementsFallback(password: String): Boolean =
        password.length >= 8 && password.any { it.isUpperCase() } && password.any { it.isLowerCase() } && password.any { it.isDigit() }
    @JvmStatic fun nativeCountCharClassesFallback(password: String): Int =
        listOf(password.any { it.isUpperCase() }, password.any { it.isLowerCase() }, password.any { it.isDigit() }, password.any { !it.isLetterOrDigit() }).count { it }
    @JvmStatic fun nativeIsCommonPasswordFallback(password: String): Boolean =
        password == "password" || password == "12345678" || password == "qwerty123"

    // --- SSO fallbacks ---
    @JvmStatic fun nativeBuildSsoLoginUrlFallback(baseUrl: String, redirectUrl: String): String =
        "$baseUrl/sso/redirect?redirectUrl=$redirectUrl"
    @JvmStatic fun nativeGetSsoProviderBrandFallback(provider: String): String = when(provider) {
        "google" -> "Google"; "github" -> "GitHub"; "facebook" -> "Facebook"
        "apple" -> "Apple"; "gitlab" -> "GitLab"; else -> provider
    }

    // --- Trust Label fallback ---
    @JvmStatic fun nativeGetTrustLabelFallback(level: String): String = when(level) {
        "verified" -> "Verified"; "warning" -> "Warning"; "blacklisted" -> "Blocked"; else -> "Unknown"
    }

    // --- MXC fallbacks ---
    @JvmStatic fun nativeIsMxcUriFallback(url: String): Boolean = url.startsWith("mxc://")
    @JvmStatic fun nativeExtractMxcServerNameFallback(mxcUrl: String): String =
        mxcUrl.removePrefix("mxc://").substringBefore("/")
    @JvmStatic fun nativeExtractMxcMediaIdFallback(mxcUrl: String): String =
        mxcUrl.substringAfterLast("/")
    @JvmStatic fun nativeBuildMxcUriFallback(serverName: String, mediaId: String): String =
        "mxc://$serverName/$mediaId"
    @JvmStatic fun nativeResolveMxcDownloadUrlFallback(mxcUrl: String, homeServerUrl: String): String =
        "${homeServerUrl.trimEnd('/')}/_matrix/media/v3/download/${mxcUrl.removePrefix("mxc://")}"
    @JvmStatic fun nativeHasTextWithImageFallback(contentJson: String): Boolean =
        contentJson.contains("\"msgtype\":\"m.image\"")

    // --- MXC Thumbnail fallback ---
    @JvmStatic fun nativeResolveMxcThumbnailUrlFallback(mxcUrl: String, homeServerUrl: String, width: Int, height: Int): String =
        "${homeServerUrl.trimEnd('/')}/_matrix/media/v3/thumbnail/${mxcUrl.removePrefix("mxc://")}?width=$width&height=$height&method=scale"

    // --- Content Utilities fallbacks ---
    @JvmStatic fun nativeGetExtensionFromMimeTypeFallback(mimetype: String): String = when {
        mimetype.contains("jpeg") || mimetype.contains("jpg") -> ".jpg"
        mimetype.contains("png") -> ".png"; mimetype.contains("gif") -> ".gif"
        mimetype.contains("webp") -> ".webp"; mimetype.contains("mp4") -> ".mp4"
        else -> ""
    }
    @JvmStatic fun nativeExtractUsefulTextFromReplyFallback(repliedBody: String): String =
        repliedBody.lines().dropWhile { it.startsWith(">") || it.isBlank() }.joinToString("\n")
    @JvmStatic fun nativeFormatSpoilerTextFromHtmlFallback(formattedBody: String): String =
        formattedBody.replace(Regex("<span[^>]*>"), "").replace("</span>", "")
    @JvmStatic fun nativeGetLatestEditEventIdFallback(editSummaryJson: String, originalEventId: String): String = originalEventId
    @JvmStatic fun nativeGetEditedTargetEventIdFallback(contentJson: String): String = ""

    // --- User Status fallbacks ---
    @JvmStatic fun nativeBuildUserStatusJsonFallback(status: String, emoji: String, nowMs: Long): String =
        """{"status":"$status","emoji":"$emoji","timestamp_ms":$nowMs}"""
    @JvmStatic fun nativeGetPresenceStatusTextFallback(isOnline: Boolean, lastActiveMs: Long): String =
        if (isOnline) "Online" else "Offline"
    @JvmStatic fun nativeGetStatusSuggestionsFallback(): String =
        """["In a meeting","On vacation","Working remotely","Available","Busy","Be right back"]"""

    // --- Markdown fallbacks ---
    @JvmStatic fun nativeMarkdownToHtmlFallback(markdown: String, enableTables: Boolean, enableLinks: Boolean, enableCode: Boolean, enableScroll: Boolean): String =
        markdown // Fallback: return raw markdown (real rendering via WebView/Android)
    @JvmStatic fun nativeParseMarkdownTableFallback(tableBlock: String, withScroll: Boolean): String = tableBlock

    // --- Event Relation fallbacks ---
    @JvmStatic fun nativeIsReplyFallback(contentJson: String): Boolean =
        contentJson.contains("\"m.in_reply_to\"")
    @JvmStatic fun nativeIsEditFallback(contentJson: String): Boolean =
        contentJson.contains("\"m.replace\"")
    @JvmStatic fun nativeIsReactionFallback(contentJson: String): Boolean =
        contentJson.contains("\"m.annotation\"")
    @JvmStatic fun nativeIsThreadRootFallback(contentJson: String): Boolean =
        contentJson.contains("\"m.thread\"")
    @JvmStatic fun nativeExtractThreadRootFallback(contentJson: String): String =
        Regex("\"m\\.thread\".*?\"event_id\":\"(\\\$[^\"]+)\"").find(contentJson)?.groupValues?.get(1) ?: ""
    @JvmStatic fun nativeExtractReplySourceFallback(contentJson: String): String =
        Regex("\"m\\.in_reply_to\".*?\"event_id\":\"(\\\$[^\"]+)\"").find(contentJson)?.groupValues?.get(1) ?: ""
    @JvmStatic fun nativeExtractEditSourceFallback(contentJson: String): String =
        Regex("\"m\\.replace\".*?\"event_id\":\"(\\\$[^\"]+)\"").find(contentJson)?.groupValues?.get(1) ?: ""
    @JvmStatic fun nativeBuildReplyRelationWithThreadFallback(eventId: String, threadRoot: String): String =
        """{"m.in_reply_to":{"event_id":"$eventId"},"m.thread":{"event_id":"$threadRoot"}}"""

    // --- Room Content fallbacks ---
    @JvmStatic fun nativeParseRoomNameContentFallback(contentJson: String): String {
        val name = Regex("\"name\":\"([^\"]+)\"").find(contentJson)?.groupValues?.get(1) ?: ""
        return """{"name":"$name"}"""
    }
    @JvmStatic fun nativeParseRoomTopicContentFallback(contentJson: String): String {
        val topic = Regex("\"topic\":\"([^\"]+)\"").find(contentJson)?.groupValues?.get(1) ?: ""
        return """{"topic":"$topic"}"""
    }
    @JvmStatic fun nativeParseRoomAvatarContentFallback(contentJson: String): String {
        val url = Regex("\"url\":\"(mxc://[^\"]+)\"").find(contentJson)?.groupValues?.get(1) ?: ""
        return """{"avatar_url":"$url"}"""
    }

    // --- Connection Monitor fallback ---
    @JvmStatic fun nativeGetBannerColorFallback(downtimeMs: Long): String = when {
        downtimeMs > 300_000 -> "#F44336"; downtimeMs > 60_000 -> "#FF9800"; else -> "#4CAF50"
    }

    // --- Content Guard fallbacks ---
    @JvmStatic fun nativeCountEmojisFallback(text: String): Int = 0 // requires Unicode analysis
    @JvmStatic fun nativeCountUniqueEmojisFallback(text: String): Int = 0
    @JvmStatic fun nativeFormatMediaCollapseLabelFallback(count: Int): String = "$count media items"
    @JvmStatic fun nativeIsEmojiCodePointFallback(codepoint: Int): Boolean = codepoint >= 0x1F300

    // --- MIME fallback ---
    @JvmStatic fun nativeNormalizeMimeTypeFallback(mimeType: String): String =
        if (mimeType == "image/jpg") "image/jpeg" else mimeType

    // --- Room State fallbacks ---
    @JvmStatic fun nativeParseJoinRulesFallback(contentJson: String): String {
        val rule = Regex("\"join_rule\":\"(\\w+)\"").find(contentJson)?.groupValues?.get(1) ?: "unknown"
        return """{"rule":"$rule"}"""
    }
    @JvmStatic fun nativeParseHistoryVisibilityFallback(contentJson: String): String {
        val vis = Regex("\"history_visibility\":\"(\\w+)\"").find(contentJson)?.groupValues?.get(1) ?: "unknown"
        return """{"visibility":"$vis"}"""
    }
    @JvmStatic fun nativeParseGuestAccessFallback(contentJson: String): String {
        val acc = Regex("\"guest_access\":\"(\\w+)\"").find(contentJson)?.groupValues?.get(1) ?: "unknown"
        return """{"access":"$acc"}"""
    }

    // --- Push Rules fallbacks ---
    @JvmStatic fun nativeIsKnownPushRuleKindFallback(kind: String): Boolean =
        kind in listOf("override", "underride", "sender", "room", "content")
    @JvmStatic fun nativeGetRuleKindDescriptionFallback(kind: String, enabled: Boolean): String =
        "${kind.replaceFirstChar { it.uppercase() }} rules${if (enabled) "" else " (disabled)"}"
    @JvmStatic fun nativeIsMsc3061SharedKeyFallback(roomKeyContentJson: String): Boolean =
        roomKeyContentJson.contains("\"shared\"")
    @JvmStatic fun nativeFormatMsc3061StatusFallback(isShared: Boolean, visibilitySetting: String): String =
        if (isShared) "Shared ($visibilitySetting)" else "Not shared"
    @JvmStatic fun nativeCanShareHistoryFallback(roomVisibility: String): Boolean =
        roomVisibility in listOf("shared", "world_readable")

    // --- Poll fallbacks ---
    @JvmStatic fun nativeGeneratePollOptionIdFallback(): String =
        java.util.UUID.randomUUID().toString().take(8)

    // --- Identity fallbacks ---
    @JvmStatic fun nativeDisambiguateNameFallback(displayName: String, mxid: String): String =
        if (displayName.isNotEmpty()) displayName else mxid.removePrefix("@").substringBefore(":")
    @JvmStatic fun nativeGetIdentityInitialsFallback(displayName: String): String =
        displayName.take(2).uppercase()
    @JvmStatic fun nativeIsCanonicalAliasFallback(alias: String, expectedRoomId: String): Boolean =
        alias.startsWith("#") && alias.contains(":$expectedRoomId")
    @JvmStatic fun nativeSuggestAliasesFallback(roomName: String): String {
        val clean = roomName.lowercase().replace(" ", "-").take(20)
        return """["#$clean"]"""
    }

    // --- Presence/Backup/CrossSigning fallbacks ---
    @JvmStatic fun nativeParsePresenceInfoFallback(userId: String, apiResponseJson: String): String = "unknown"
    @JvmStatic fun nativeParseBackupInfoFallback(apiResponseJson: String): String = "No backup"
    @JvmStatic fun nativeParseCrossSigningStatusFallback(accountDataJson: String, userId: String): String = "Not set up"
    @JvmStatic fun nativeParseKeyBackupVersionFallback(json: String): String = "{}"

    // --- Device list fallback ---
    @JvmStatic fun nativeParseDeviceListFallback(apiResponseJson: String, currentDeviceId: String): String =
        """{"total_devices":0,"verified_devices":0,"unverified_devices":0}"""

    // --- Permissions fallback ---
    @JvmStatic fun nativeComputePermissionsFallback(powerLevelsJson: String, myUserId: String): String =
        """{"can_send_messages":true,"can_send_images":true,"can_ban":false,"can_kick":false}"""

    // --- Tombstone/Scanner/Notice fallbacks ---
    @JvmStatic fun nativeParseTombstoneFallback(contentJson: String): String = "{}"
    @JvmStatic fun nativeParseScanResultFallback(apiResponseJson: String): String = "Clean"
    @JvmStatic fun nativeParseServerNoticeFallback(eventContentJson: String, eventId: String): String = "Notice"

    // --- Member/Public Room fallbacks ---
    @JvmStatic fun nativeParseMemberListFallback(roomId: String, apiResponseJson: String, isTruncated: Boolean): String =
        """{"room_id":"$roomId","total":0,"joined":0,"invited":0,"banned":0,"truncated":false,"members":[]}"""
    @JvmStatic fun nativeParsePublicRoomFallback(json: String): String =
        """{"room_id":"","name":"","topic":"","members":0,"world_readable":false,"guest_can_join":false}"""

    // --- Event Relation fallback ---
    @JvmStatic fun nativeParseEventRelationFallback(contentJson: String): String =
        """{"rel_type":"","event_id":"","key":""}"""

    // --- Public Rooms / Thread fallbacks ---
    @JvmStatic fun nativeParsePublicRoomsResponseFallback(json: String): String =
        """{"total":0,"next_batch":"","rooms":[]}"""
    @JvmStatic fun nativeComputeThreadSummaryFallback(rootEventId: String, eventsJson: String): String =
        """{"root_event_id":"$rootEventId","reply_count":0}"""

    // --- Relation Description fallback ---
    @JvmStatic fun nativeFormatRelationDescriptionFallback(relType: String, eventId: String, key: String): String =
        when(relType) {
            "m.replace" -> "edited"; "m.annotation" -> "reacted $key"
            "m.in_reply_to" -> "replied"; "m.thread" -> "threaded"; else -> relType
        }

    // --- Content Scanner fallback ---
    @JvmStatic fun nativeBuildScanRequestBodyFallback(mxcUri: String): String =
        """{"mxc_uri":"$mxcUri"}"""

    // --- Event Content fallback ---
    @JvmStatic fun nativeParseEventContentFallback(eventType: String, contentJson: String): String {
        val msgType = Regex("\"msgtype\":\"(\\w+)\"").find(contentJson)?.groupValues?.get(1) ?: ""
        return """{"event_type":"$eventType","msg_type":"$msgType","body":"","file_size":0,"duration_ms":0}"""
    }

    // --- Canonical JSON fallback ---
    @JvmStatic fun nativeCanonicalizeJsonFallback(json: String): String = json

    // --- Uploader fallbacks ---
    @JvmStatic fun nativeUploaderSetChunkSizeMbFallback(mb: Int) {}
    @JvmStatic fun nativeUploaderComputeChunksFallback(fileSize: Long): Int = 0
    @JvmStatic fun nativeUploaderGetChunkInfoFallback(index: Int): String = "{}"
    @JvmStatic fun nativeUploaderContentRangeFallback(index: Int): String = ""
    @JvmStatic fun nativeUploaderAdvanceFallback() {}
    @JvmStatic fun nativeUploaderCancelFallback() {}
    @JvmStatic fun nativeUploaderResetFallback() {}
    @JvmStatic fun nativeUploaderProgressFallback(): String = """{"uploaded":0,"total":0,"chunks":0,"done":false,"progress":0}"""
    @JvmStatic fun nativeSuggestChunkSizeMbFallback(fileSize: Long): Int =
        when { fileSize < 100_000_000 -> 10; fileSize < 1_000_000_000 -> 20; else -> 50 }

    // --- Thread List fallback ---
    @JvmStatic fun nativeBuildThreadListJsonFallback(eventsJson: String): String = "[]"

    // --- Thread Unread fallback ---
    @JvmStatic fun nativeComputeThreadUnreadCountFallback(eventIdsJson: String, readReceiptId: String, highlightIdsJson: String): String =
        """{"total":0,"unread":0,"highlight":0,"has_unread":false}"""

    // --- Sync Filter fallback ---
    @JvmStatic fun nativeBuildSyncFilterFallback(includeThreads: Boolean, includePresence: Boolean, timelineLimit: Int, lazyLoadMembers: Boolean): String =
        """{"room":{"timeline":{"limit":$timelineLimit}},"presence":{"include":$includePresence}}"""

    // --- Read Receipts fallbacks ---
    @JvmStatic fun nativeFormatReceiptAccessibilityFallback(receiptsJson: String, overflowCount: Int): String = "Read"
    @JvmStatic fun nativeFormatOverflowLabelFallback(count: Int): String = if (count > 0) "+$count" else ""

    // --- Space Hierarchy fallback ---
    @JvmStatic fun nativeSearchSpaceChildrenFallback(childrenJson: String, query: String): String = "[]"

    // --- 3PID/Presence fallbacks ---
    @JvmStatic fun nativeParseThreePidFallback(input: String): String {
        val medium = if (input.contains("@")) "email" else "msisdn"
        return """{"medium":"$medium","address":"$input"}"""
    }
    @JvmStatic fun nativeFormatPresenceAggregationFallback(userNamesJson: String, maxNames: Int): String = "Unknown"

    // --- Reaction/Poll fallbacks ---
    @JvmStatic fun nativeFormatReactionAggregationFallback(key: String, count: Int, reactorsJson: String): String = "$key $count"
    @JvmStatic fun nativeTrackPollResponseFallback(optionId: String, userId: String): String =
        """{"option_id":"$optionId","user_id":"$userId","recorded":true}"""

    // --- Audio fallbacks ---
    @JvmStatic fun nativeIsSupportedAudioTypeFallback(mimeType: String): Boolean =
        mimeType.startsWith("audio/") || mimeType == "application/ogg"
    @JvmStatic fun nativeFormatPositionInfoFallback(positionMs: Long, durationMs: Long): String =
        "${positionMs / 1000}s / ${durationMs / 1000}s"

    // --- Direct Message fallback ---
    @JvmStatic fun nativeParseDirectMessageMapFallback(json: String): String = "{}"

    // --- Edit History fallbacks ---
    @JvmStatic fun nativeGetEditCountBadgeFallback(editCount: Int): String =
        if (editCount > 1) "Edited $editCount times" else if (editCount == 1) "Edited" else ""
    @JvmStatic fun nativeComputeEditDiffSummaryFallback(oldBody: String, newBody: String): String {
        val added = (newBody.length - oldBody.length).coerceAtLeast(0)
        val removed = (oldBody.length - newBody.length).coerceAtLeast(0)
        return "+$added/-$removed chars"
    }

    // --- Notification State fallback ---
    @JvmStatic fun nativeComputeNotificationStateFallback(roomJson: String): String =
        """{"level":"none","count":0,"badge_text":"","show_badge":false}"""

    // --- Room List Search fallback ---
    @JvmStatic fun nativeSearchRoomListFallback(roomsJson: String, query: String): String = "[]"

    // --- Event Classifier fallback ---
    @JvmStatic fun nativeIsStateEventFallback(eventType: String): Boolean =
        eventType.startsWith("m.room.") && eventType != "m.room.message" && eventType != "m.room.encrypted"

    // --- Poll Results fallback ---
    @JvmStatic fun nativeComputePollResultsFallback(pollJson: String): String =
        """{"question":"","total_votes":0,"is_ended":false,"winner":"","options":[]}"""

    // --- Location Sharing fallbacks ---
    @JvmStatic fun nativeLocationStartSessionFallback(sessionId: String, roomId: String, userId: String, intervalSec: Int): String = sessionId
    @JvmStatic fun nativeLocationStopSessionFallback(sessionId: String) {}
    @JvmStatic fun nativeLocationIsDueFallback(sessionId: String): Boolean = false
    @JvmStatic fun nativeLocationExportJsonFallback(): String = "[]"

    // --- AI Agent fallbacks ---
    @JvmStatic fun nativeAgentHasToolCallsFallback(llmResponse: String): Boolean = llmResponse.contains("\"tool_calls\"")
    @JvmStatic fun nativeAgentExtractTextAnswerFallback(llmResponse: String): String = llmResponse
    @JvmStatic fun nativeAgentGetToolsSchemaFallback(): String = "[]"

    // --- Notification Formatting fallbacks ---
    @JvmStatic fun nativeFormatThreadNotificationCountFallback(threadCount: Int, highlightCount: Int): String =
        if (threadCount > 99) "99+" else if (threadCount > 0) "$threadCount" else ""
    @JvmStatic fun nativeFormatUnreadCounterFallback(count: Int): String =
        if (count > 99) "99+" else if (count > 0) "$count" else ""

    // --- Push Evaluator fallback ---
    @JvmStatic fun nativeEvaluatePushNotificationFallback(eventJson: String, rulesJson: String, myDisplayName: String, myUserId: String): String =
        """{"notify":true,"highlight":false,"noisy":true,"rule":""}"""

    // --- Room Upgrade fallback ---
    @JvmStatic fun nativeProcessRoomUpgradeFallback(tombstoneEventJson: String): String =
        """{"is_upgrade":false,"successor":"","notice":""}"""

    // --- Redaction fallback ---
    @JvmStatic fun nativeFormatRedactionNoticeFallback(reason: String, redactedBySelf: Boolean, isStateEvent: Boolean): String =
        if (isStateEvent) "This event is no longer available"
        else if (redactedBySelf) if (reason.isEmpty()) "You removed this message" else "You removed this message: $reason"
        else if (reason.isEmpty()) "Message removed" else "Message removed: $reason"

    // --- Key Backup fallback ---
    @JvmStatic fun nativeValidateAndFormatRecoveryKeyFallback(rawKey: String): String {
        val clean = rawKey.replace(" ", "")
        return if (clean.length < 40) """{"valid":false,"formatted":"","error":"Invalid key"}"""
        else """{"valid":true,"formatted":"${clean.chunked(4).joinToString(" ")}","error":""}"""
    }

    // --- Member/Call/Edit fallbacks ---
    @JvmStatic fun nativeFormatMemberNoticeFallback(membership: String, prevMembership: String, senderId: String, senderName: String, targetId: String, targetName: String, reason: String, isDirect: Boolean, sentBySelf: Boolean): String {
        val target = if (sentBySelf && senderId == targetId) "You" else targetName
        val room = if (isDirect) "chat" else "room"
        return when(membership) {
            "join" -> "$target joined the $room"
            "invite" -> "$senderName invited $target" + if (reason.isNotEmpty()) ": $reason" else ""
            "ban" -> "$senderName banned $target" + if (reason.isNotEmpty()) ": $reason" else ""
            "leave" -> if (senderId == targetId) "$target left the $room" else "$senderName kicked $target" + if (reason.isNotEmpty()) ": $reason" else ""
            "knock" -> "$target requested to join" + if (reason.isNotEmpty()) ": $reason" else ""
            else -> "$target $membership"
        }
    }
    @JvmStatic fun nativeFormatCallNoticeFallback(eventType: String, isVideo: Boolean, senderName: String, sentBySelf: Boolean): String {
        val who = if (sentBySelf) "You" else senderName
        return when(eventType) {
            "m.call.invite" -> "$who placed a ${if (isVideo) "video" else "voice"} call"
            "m.call.answer" -> "$who answered the call"
            "m.call.hangup" -> "$who ended the call"
            "m.call.reject" -> "$who declined the call"
            else -> "$who $eventType"
        }
    }
    @JvmStatic fun nativeAnnotateEditedFallback(body: String, isEdited: Boolean): String =
        if (isEdited) "$body (edited)" else body

    // --- Room State notice fallbacks ---
    @JvmStatic fun nativeFormatRoomNameNoticeFallback(senderName: String, newName: String, sentBySelf: Boolean): String {
        val who = if (sentBySelf) "You" else senderName
        return if (newName.isEmpty()) "$who removed the room name" else "$who changed the room name to $newName"
    }
    @JvmStatic fun nativeFormatRoomTopicNoticeFallback(senderName: String, newTopic: String, sentBySelf: Boolean): String {
        val who = if (sentBySelf) "You" else senderName
        return if (newTopic.isEmpty()) "$who removed the topic" else "$who changed the topic to: $newTopic"
    }
    @JvmStatic fun nativeFormatRoomAvatarNoticeFallback(senderName: String, isRemoved: Boolean, sentBySelf: Boolean): String {
        val who = if (sentBySelf) "You" else senderName
        return if (isRemoved) "$who removed the room avatar" else "$who changed the room avatar"
    }
    @JvmStatic fun nativeFormatRoomCreateNoticeFallback(senderName: String, predecessorRoomId: String, isDirect: Boolean, sentBySelf: Boolean): String {
        val who = if (sentBySelf) "You" else senderName
        val type = if (isDirect) "chat" else "room"
        return if (predecessorRoomId.isNotEmpty()) "$who upgraded the $type" else "$who created the $type"
    }
    @JvmStatic fun nativeFormatRoomTombstoneNoticeFallback(senderName: String, replacementRoom: String, sentBySelf: Boolean): String {
        val who = if (sentBySelf) "You" else senderName
        return if (replacementRoom.isNotEmpty()) "$who upgraded the room to $replacementRoom" else "This room has been replaced"
    }
    @JvmStatic fun nativeFormatRoomEncryptionNoticeFallback(senderName: String, isEnabled: Boolean, sentBySelf: Boolean): String {
        val who = if (sentBySelf) "You" else senderName
        return if (isEnabled) "$who enabled encryption" else "$who disabled encryption"
    }

    // --- Power Level Diff fallback ---
    @JvmStatic fun nativeFormatPowerLevelDiffFallback(senderName: String, oldLevelsJson: String, newLevelsJson: String, userNamesJson: String, sentBySelf: Boolean): String {
        val who = if (sentBySelf) "You" else senderName
        return "$who changed power levels"
    }

    // --- Poll fallback ---
    @JvmStatic fun nativeIsValidPollQuestionFallback(question: String): Boolean =
        question.isNotEmpty() && question.length < 1000

    // --- Uploads fallbacks ---
    @JvmStatic fun nativeIsStickerEventFallback(eventType: String): Boolean = eventType == "m.sticker"
    @JvmStatic fun nativeHasAttachmentUrlFallback(decryptedContentJson: String): Boolean =
        decryptedContentJson.contains("\"url\":\"mxc://")
    @JvmStatic fun nativeCreateUploadsFilterJsonFallback(numberOfEvents: Int): String =
        """{"types":["m.room.message"],"limit":$numberOfEvents}"""

    // --- Matrix Error fallbacks ---
    @JvmStatic fun nativeGetErrorDescriptionFallback(errorCode: String): String =
        errorCode.replace("M_", "").replace("_", " ").lowercase().replaceFirstChar { it.uppercase() }
    @JvmStatic fun nativeFormatImageNotificationFallback(sender: String): String =
        "$sender sent an image"
    @JvmStatic fun nativeFormatFileNotificationFallback(sender: String, fileName: String): String =
        if (fileName.isNotEmpty()) "$sender sent a file: $fileName" else "$sender sent a file"
    @JvmStatic fun nativeFormatVideoNotificationFallback(sender: String): String =
        "$sender sent a video"
    @JvmStatic fun nativeFormatAudioNotificationFallback(sender: String, isVoice: Boolean): String =
        if (isVoice) "$sender sent a voice message" else "$sender sent an audio file"
    @JvmStatic fun nativeFormatInviteNotificationFallback(inviter: String, roomName: String): String =
        if (roomName.isNotEmpty()) "$inviter invited you to $roomName" else "$inviter invited you"
    @JvmStatic fun nativeFormatRoomNotificationFallback(count: Int, roomName: String): String =
        if (roomName.isNotEmpty()) "$count messages in $roomName" else "$count messages"
    @JvmStatic fun nativeFormatStickerNotificationFallback(sender: String): String =
        "$sender sent a sticker"
    @JvmStatic fun nativeFormatLocationNotificationFallback(sender: String): String =
        "$sender shared their location"
    @JvmStatic fun nativeFormatPollNotificationFallback(sender: String, isStart: Boolean): String =
        if (isStart) "$sender started a poll" else "$sender ended a poll"

    // --- Raw Service fallback ---
    @JvmStatic fun nativeCacheKeyForUrlFallback(url: String): String =
        url.replace("https://", "").replace("/", "_").take(200)

    // --- Lightweight Settings fallbacks ---
    @JvmStatic fun nativeGetSettingBoolFallback(settingsJson: String, key: String, defaultVal: Boolean): Boolean = defaultVal
    @JvmStatic fun nativeSetSettingBoolFallback(settingsJson: String, key: String, value: Boolean): String {
        val kv = """"$key":$value"""
        return if (settingsJson.indexOf('{') >= 0) settingsJson.replaceFirst("{", "{$kv,")
        else """{$kv}"""
    }
    @JvmStatic fun nativeGetSettingStringFallback(settingsJson: String, key: String, defaultVal: String): String =
        Regex(""""$key":"([^"]*)"""").find(settingsJson)?.groupValues?.getOrNull(1) ?: defaultVal
    @JvmStatic fun nativeSetSettingStringFallback(settingsJson: String, key: String, value: String): String {
        val kv = """"$key":"$value""""
        if (Regex(""""$key":""").containsMatchIn(settingsJson))
            return Regex(""""$key":"[^"]*"""").replace(settingsJson, """"$key":"$value"""")
        return if (settingsJson.indexOf('{') >= 0) settingsJson.replaceFirst("{", "{$kv,")
        else """{$kv}"""
    }

    // --- HTTP Client fallback ---
    @JvmStatic fun nativeParseUrlFallback(url: String): String {
        val m = Regex("""^(https?)://([^/:]+)(?::(\d+))?(/.*)?""").find(url)
        return if (m != null) {
            val (scheme, host, port, path) = m.destructured
            """{"scheme":"$scheme","host":"$host","port":${port.toIntOrNull() ?: (if (scheme == "https") 443 else 80)},"path":"${path ?: "/"}","valid":true}"""
        } else """{"scheme":"","host":"","port":0,"path":"","valid":false}"""
    }

    // --- Megolm fallbacks ---
    @JvmStatic fun nativeMegolmAddSessionFallback(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String): Boolean = false
    @JvmStatic fun nativeMegolmDecryptFallback(roomId: String, senderKey: String, sessionId: String, ciphertext: String): String = ""
    @JvmStatic fun nativeMegolmSessionCountFallback(): Int = 0
    @JvmStatic fun nativeMegolmClearRoomFallback(roomId: String) {}

    // --- Olm Account fallbacks ---
    @JvmStatic fun nativeOlmCreateAccountFallback(userId: String, deviceId: String): Boolean = false
    @JvmStatic fun nativeOlmGetIdentityKeysFallback(): String = "{}"
    @JvmStatic fun nativeOlmGenerateOneTimeKeysFallback(count: Int): String = "{}"
    @JvmStatic fun nativeOlmSignMessageFallback(message: String): String = ""
    @JvmStatic fun nativeOlmCreateInboundSessionFallback(theirIdentityKey: String, preKeyMessage: String): String = ""
    @JvmStatic fun nativeOlmDecryptMessageFallback(senderKey: String, sessionId: String, ciphertext: String): String = ""
    @JvmStatic fun nativeOlmPickleAccountFallback(): String = ""
    @JvmStatic fun nativeOlmUnpickleAccountFallback(pickled: String, userId: String, deviceId: String): Boolean = false

    // --- Event Signing fallbacks ---
    @JvmStatic fun nativeSignEventFallback(eventJson: String): String = eventJson
    @JvmStatic fun nativeVerifyEventSignatureFallback(eventJson: String, signKeyB64: String): Boolean = false

    // --- Device Verification fallbacks ---
    @JvmStatic fun nativeVerifyDeviceSignatureFallback(deviceKeysJson: String, userId: String, deviceId: String, signKeyB64: String, signatureB64: String): Boolean = false
    @JvmStatic fun nativeComputeDeviceFingerprintFallback(identityKeyBase64: String): String =
        identityKeyBase64.take(20).chunked(4).joinToString(" ") { it.uppercase() }

    // --- SAS fallbacks ---
    @JvmStatic fun nativeSasCreateFallback(): String = ""
    @JvmStatic fun nativeSasSetTheirKeyFallback(theirPubkey: String): Boolean = false
    @JvmStatic fun nativeSasGetEmojisFallback(): String = "[]"
    @JvmStatic fun nativeSasCalculateMacFallback(input: String, info: String): String = ""
    @JvmStatic fun nativeSasVerifyMacFallback(theirMac: String, input: String, info: String): Boolean = false
    @JvmStatic fun nativeSasDestroyFallback() {}

    // --- JSON Parser fallback ---
    @JvmStatic fun nativeParseJsonStringValueFallback(json: String, key: String): String =
        Regex(""""$key":"([^"]*)"""").find(json)?.groupValues?.getOrNull(1) ?: ""

    // --- Federation Version fallback ---
    @JvmStatic fun nativeFederationVersionToJsonFallback(versionJson: String): String = versionJson

    // --- Auth Models fallbacks ---
    @JvmStatic fun nativePresenceEnumToStringFallback(presence: Int): String = when(presence) {
        0 -> "offline"; 1 -> "online"; 2 -> "unavailable"; else -> "offline" }
    @JvmStatic fun nativeCredentialsToJsonFallback(credsJson: String): String = credsJson

    // --- Call Models fallbacks ---
    @JvmStatic fun nativeSdpTypeToStringFallback(type: Int): String = when(type) {
        0 -> "offer"; 1 -> "answer"; else -> "offer" }
    @JvmStatic fun nativeEndCallReasonToStringFallback(reason: Int): String = when(reason) {
        0 -> "user_hung_up"; 1 -> "invite_timeout"; 2 -> "ice_failed"; 3 -> "unknown_error"; else -> "unknown" }

    // --- Message Content fallbacks ---
    @JvmStatic fun nativeMessageTextToJsonFallback(contentJson: String): String = contentJson
    @JvmStatic fun nativeMessageNoticeToJsonFallback(contentJson: String): String = contentJson
    @JvmStatic fun nativeMessageEmoteToJsonFallback(contentJson: String): String = contentJson
    @JvmStatic fun nativeMessageImageToJsonFallback(contentJson: String): String = contentJson
    @JvmStatic fun nativeMessageVideoToJsonFallback(contentJson: String): String = contentJson
    @JvmStatic fun nativeMessageAudioToJsonFallback(contentJson: String): String = contentJson
    @JvmStatic fun nativeMessageFileToJsonFallback(contentJson: String): String = contentJson

    // --- Crypto Models fallback ---
    @JvmStatic fun nativeDeviceInfoToJsonFallback(deviceJson: String): String = deviceJson

    // --- Offline Cache fallbacks ---
    @JvmStatic fun nativeCanFitInStorageFallback(required: Long, available: Long, reserved: Long): Boolean =
        required <= (available - reserved)
    @JvmStatic fun nativeEstimateMessageCacheSizeFallback(messageCount: Int, avgBodySize: Int): Long =
        messageCount.toLong() * avgBodySize.toLong()

    // --- Sign Out Service fallbacks ---
    @JvmStatic fun nativeShouldIgnoreSignOutErrorFallback(errorCode: String, httpCode: Int): Boolean =
        httpCode == 401 || errorCode == "M_UNKNOWN_TOKEN"
    @JvmStatic fun nativeSignInAgainBodyToJsonFallback(paramsJson: String): String = paramsJson

    // --- Message Extras fallbacks ---
    @JvmStatic fun nativePollTypeToStringFallback(type: Int): String = when(type) {
        0 -> "org.matrix.msc3381.poll.disclosed"; 1 -> "m.poll.disclosed"
        2 -> "org.matrix.msc3381.poll.undisclosed"; 3 -> "m.poll.undisclosed"
        else -> "m.poll.disclosed" }
    @JvmStatic fun nativePollTypeFromStringFallback(type: String): Int = when(type) {
        "m.poll.disclosed" -> 1; "m.poll.undisclosed" -> 3
        "org.matrix.msc3381.poll.disclosed" -> 0; "org.matrix.msc3381.poll.undisclosed" -> 2
        else -> 1 }

    // --- Terms Service fallback ---
    @JvmStatic fun nativeAcceptTermsBodyToJsonFallback(bodyJson: String): String = bodyJson

    // --- Live Draft fallback ---
    @JvmStatic fun nativeLiveDraftConfigToJsonFallback(configJson: String): String = configJson

    // --- Encrypted File fallbacks ---
    @JvmStatic fun nativeEncryptedFileKeyToJsonFallback(keyJson: String): String = keyJson
    @JvmStatic fun nativeIsValidJwkKeyFallback(keyJson: String): Boolean =
        "\"alg\":\"A256CTR\"" in keyJson && "\"k\":" in keyJson
    @JvmStatic fun nativeExtractFileKeyFallback(keyJson: String): String =
        Regex(""""k":"([^"]+)"""").find(keyJson)?.groupValues?.getOrNull(1) ?: ""
    @JvmStatic fun nativeEncryptedFileInfoToJsonFallback(infoJson: String): String = infoJson
    @JvmStatic fun nativeIsValidEncryptedFileFallback(infoJson: String): Boolean =
        "\"url\":\"mxc://" in infoJson && "\"iv\":\"" in infoJson
    @JvmStatic fun nativeExtractFileIvFallback(infoJson: String): String =
        Regex(""""iv":"([^"]+)"""").find(infoJson)?.groupValues?.getOrNull(1) ?: ""

    // --- Crypto Algorithms fallbacks ---
    @JvmStatic fun nativeSha256Fallback(data: ByteArray): String {
        val md = java.security.MessageDigest.getInstance("SHA-256")
        return md.digest(data).joinToString("") { "%02x".format(it) }
    }
    @JvmStatic fun nativeBase58EncodeFallback(data: ByteArray): String {
        val alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
        val sb = StringBuilder()
        var num = java.math.BigInteger(1, data)
        val zero = java.math.BigInteger.ZERO
        while (num > zero) {
            val divrem = num.divideAndRemainder(java.math.BigInteger.valueOf(58))
            sb.insert(0, alphabet[divrem[1].toInt()])
            num = divrem[0]
        }
        for (b in data) if (b.toInt() == 0) sb.insert(0, '1') else break
        return sb.toString()
    }

    // --- TLS Bridge fallback ---
    @JvmStatic fun nativeTlsBridgeAvailableFallback(): Boolean = false

    // --- Create Room fallback ---
    @JvmStatic fun nativeCreateRoomPresetToStringFallback(preset: Int): String = when(preset) {
        0 -> "private_chat"; 1 -> "public_chat"; 2 -> "trusted_private_chat"
        else -> "private_chat" }

    // --- Widget Manager fallbacks ---
    private var _widgetMgrWidgets = mutableListOf<Map<String, Any>>()

    @JvmStatic fun nativeWidgetMgrInitFallback(roomId: String, userId: String, displayName: String, avatarUrl: String): Boolean = true
    @JvmStatic fun nativeWidgetMgrSetSecurityPolicyFallback(policyJson: String): Boolean = true
    @JvmStatic fun nativeWidgetMgrLoadWidgetsFallback(stateEventsJson: String): String = "[]"
    @JvmStatic fun nativeWidgetMgrCreateWidgetFallback(widgetId: String, type: String, url: String, name: String, waitForIframeLoad: Boolean): String {
        _widgetMgrWidgets.add(mapOf("widgetId" to widgetId, "type" to type, "url" to url, "name" to name))
        return """{"id":"$widgetId","type":"$type","url":"$url","name":"${name.ifEmpty{widgetId}}"}"""
    }
    @JvmStatic fun nativeWidgetMgrRemoveWidgetFallback(widgetId: String): String {
        _widgetMgrWidgets.removeAll { it["widgetId"] == widgetId }
        return """{"state_key":"$widgetId"}"""
    }
    @JvmStatic fun nativeWidgetMgrSetPinnedFallback(widgetId: String, pinned: Boolean): String =
        """{"widgetId":"$widgetId","pinned":$pinned}"""
    @JvmStatic fun nativeWidgetMgrResizeFallback(widgetId: String, width: Int, height: Int): String =
        """{"widgetId":"$widgetId","width":$width,"height":$height}"""
    @JvmStatic fun nativeWidgetMgrSetMinimizedFallback(widgetId: String, minimized: Boolean): String =
        """{"widgetId":"$widgetId","minimized":$minimized}"""
    @JvmStatic fun nativeWidgetMgrSetMaximizedFallback(widgetId: String, maximized: Boolean): String =
        """{"widgetId":"$widgetId","maximized":$maximized}"""
    @JvmStatic fun nativeWidgetMgrRequestCapabilityFallback(widgetId: String, capability: Int): String =
        """{"widgetId":"$widgetId","capability":$capability,"approved":true,"reason":"auto-approved (fallback)"}"""
    @JvmStatic fun nativeWidgetMgrApproveCapabilityFallback(widgetId: String, capability: Int): String =
        """{"approved":true}"""
    @JvmStatic fun nativeWidgetMgrDenyCapabilityFallback(widgetId: String, capability: Int): String =
        """{"denied":true}"""
    @JvmStatic fun nativeWidgetMgrGetUrlFallback(widgetId: String): String = ""
    @JvmStatic fun nativeWidgetMgrBuildPostMessageFallback(widgetId: String, action: String, data: String): String =
        """{"api":"fromWidget","widgetId":"$widgetId","action":"$action","data":${data.ifEmpty{"{}"}}}"""
    @JvmStatic fun nativeWidgetMgrParsePostMessageFallback(message: String): String {
        val action = Regex(""""action":"([^"]+)"""").find(message)?.groupValues?.getOrNull(1) ?: ""
        val wid = Regex(""""widgetId":"([^"]+)"""").find(message)?.groupValues?.getOrNull(1) ?: ""
        return """{"api":"","action":"$action","widgetId":"$wid","data":{}}"""
    }
    @JvmStatic fun nativeWidgetMgrSupportsPiPFallback(widgetId: String): Boolean = false
    @JvmStatic fun nativeWidgetMgrGetByTypeFallback(type: String): String = "[]"
    @JvmStatic fun nativeWidgetMgrCountFallback(): String = "0"
    @JvmStatic fun nativeWidgetMgrBuildCspFallback(): String = ""
    @JvmStatic fun nativeApplyWidgetUrlTemplateFallback(url: String, templateJson: String): String = url
    @JvmStatic fun nativeValidateWidgetSecurityFallback(url: String, policyJson: String): String =
        if (url.startsWith("https://")) """{"valid":true,"reason":""}""" else """{"valid":false,"reason":"URL must use https://"}"""
    @JvmStatic fun nativeClassifyWidgetTypeFallback(type: String): String = when(type) {
        "m.jitsi","jitsi" -> "Video Conference"
        "m.etherpad","etherpad" -> "Collaborative Document"
        "m.custom" -> "Custom Widget"
        "m.stickerpicker" -> "Sticker Picker"
        "m.calculator" -> "Calculator"
        "m.youtube" -> "YouTube"
        "m.spotify" -> "Spotify"
        "m.whiteboard" -> "Whiteboard"
        else -> type }
    @JvmStatic fun nativeIsAutoApprovedCapabilityFallback(capability: Int, widgetType: String): Boolean =
        widgetType in listOf("m.jitsi","jitsi","m.stickerpicker")

    // --- Key Backup Manager fallbacks ---
    @JvmStatic fun nativeBackupExtractPrivateKeyFallback(recoveryKey: String): String = ""
    @JvmStatic fun nativeBackupGenerateRecoveryKeyFallback(curve25519Key: String): String {
        // Simple ASCII-based mock recovery key
        return curve25519Key.take(43).chunked(4).joinToString(" ")
    }
    @JvmStatic fun nativeBackupParseVersionFallback(json: String): String {
        val ver = Regex(""""version":"([^"]+)"""").find(json)?.groupValues?.getOrNull(1) ?: "0"
        val algo = Regex(""""algorithm":"([^"]+)"""").find(json)?.groupValues?.getOrNull(1) ?: ""
        val cnt = Regex(""""count":(\d+)""").find(json)?.groupValues?.getOrNull(1) ?: "0"
        return """{"version":"$ver","algorithm":"$algo","count":$cnt,"valid":true,"etag":""}"""
    }
    @JvmStatic fun nativeBackupBuildCreateVersionFallback(configJson: String): String = """{"algorithm":"m.megolm_backup.v1.curve25519-aes-sha2","auth_data":{},"version":"1"}"""
    @JvmStatic fun nativeBackupBuildDeleteFallback(version: String): String = "{}"
    @JvmStatic fun nativeBackupExportSessionFallback(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String, firstMessageIndex: Long, isForwarded: Boolean, forwardedCount: Long): String =
        """{"room_id":"$roomId","session_id":"$sessionId","sender_key":"$senderKey","first_index":$firstMessageIndex}"""
    @JvmStatic fun nativeBackupEncryptSessionFallback(sessionJson: String, authData: String): String = sessionJson
    @JvmStatic fun nativeBackupParseKeysFallback(backupJson: String): String = "[]"
    @JvmStatic fun nativeBackupDecryptSessionFallback(sessionJson: String, backupKey: String, roomId: String): String {
        val sid = Regex(""""session_id":"([^"]+)"""").find(sessionJson)?.groupValues?.getOrNull(1) ?: ""
        return """{"session_id":"$sid","sender_key":"","session_key":"","decrypted":false,"error":"fallback — native lib not loaded"}"""
    }
    @JvmStatic fun nativeBackupDecryptAllFallback(keysJson: String, authData: String, recoveryKey: String): String = "[]"
    @JvmStatic fun nativeBackupVerifyIntegrityFallback(authData: String): Boolean = authData.contains("public_key")
    @JvmStatic fun nativeBackupVerifyRecoveryMatchFallback(recoveryKey: String, authData: String): Boolean = recoveryKey.isNotEmpty()
    @JvmStatic fun nativeBackupProgressFallback(): String = "0"
    @JvmStatic fun nativeBackupProgressJsonFallback(): String = """{"total_keys":0,"uploaded":0,"failed":0,"downloaded":0,"decrypted":0,"imported":0,"is_running":false,"is_complete":false,"started_at":0,"last_update":0}"""
    @JvmStatic fun nativeBackupSetTotalKeysFallback(count: Int) {}
    @JvmStatic fun nativeBackupAdvanceUploadedFallback() {}
    @JvmStatic fun nativeBackupAdvanceDownloadedFallback() {}
    @JvmStatic fun nativeBackupAdvanceDecryptedFallback() {}
    @JvmStatic fun nativeBackupAdvanceImportedFallback() {}
    @JvmStatic fun nativeBackupMarkCompleteFallback() {}
    @JvmStatic fun nativeBackupResetFallback() {}

    // --- Live Location fallbacks ---
    @JvmStatic fun nativeLiveLocationParseGeoUriFallback(uri: String): String {
        val m = Regex("""geo:(-?\d+\.?\d*),(-?\d+\.?\d*)(?:;u=(\d+))?""").find(uri)
        return if (m != null) """{"lat":${m.groupValues[1]},"lon":${m.groupValues[2]},"uncertainty":${m.groupValues.getOrNull(3)?:"0"},"valid":true,"crs":"wgs84","label":""}"""
        else """{"lat":0,"lon":0,"valid":false,"crs":"","label":""}"""
    }
    @JvmStatic fun nativeLiveLocationFormatMessageFallback(lat: Double, lon: Double, accuracy: Double, label: String): String =
        "Location: $lat,$lon" + if (label.isNotEmpty()) " ($label)" else "" + if (accuracy > 0) " (±${accuracy.toInt()}m)" else ""
    @JvmStatic fun nativeLiveLocationFormatGeoUriFallback(lat: Double, lon: Double): String =
        "geo:$lat,$lon"
    @JvmStatic fun nativeLiveLocationDistanceFallback(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double {
        val R = 6371000.0
        val dLat = Math.toRadians(lat2 - lat1)
        val dLon = Math.toRadians(lon2 - lon1)
        val a = Math.sin(dLat/2)*Math.sin(dLat/2) + Math.cos(Math.toRadians(lat1))*Math.cos(Math.toRadians(lat2))*Math.sin(dLon/2)*Math.sin(dLon/2)
        return 2*R*Math.atan2(Math.sqrt(a), Math.sqrt(1-a))
    }
    @JvmStatic fun nativeLiveLocationStartSessionFallback(roomId: String, userId: String, description: String, timeoutSec: Int, intervalSec: Int, autoStop: Boolean, autoStopMin: Int): String =
        """{"description":"${description.ifEmpty{"$userId's location"}}","timeout":$timeoutSec,"live":true}"""
    @JvmStatic fun nativeLiveLocationStopSessionFallback(sessionId: String): String = """{"live":false,"timeout":0}"""
    @JvmStatic fun nativeLiveLocationIsDueFallback(sessionId: String): Boolean = false
    @JvmStatic fun nativeLiveLocationUpdateFallback(sessionId: String, lat: Double, lon: Double, accuracy: Double): String =
        """{"org.matrix.msc3488.location":"geo:$lat,$lon${if (accuracy > 0) ";u=${accuracy.toInt()}" else ""}"}"""
    @JvmStatic fun nativeLiveLocationGetActiveFallback(userId: String): String = "[]"
    @JvmStatic fun nativeLiveLocationGetRoomSessionsFallback(roomId: String): String = "[]"
    @JvmStatic fun nativeLiveLocationHistoryFallback(sessionId: String): String = "[]"
    @JvmStatic fun nativeLiveLocationBuildMapUrlFallback(roomId: String, configJson: String): String = ""
    @JvmStatic fun nativeLiveLocationClusterFallback(coordsJson: String, radiusMeters: Double): String = "[]"
    @JvmStatic fun nativeLiveLocationWithinGeofenceFallback(lat: Double, lon: Double, centerLat: Double, centerLon: Double, radiusMeters: Double): Boolean =
        nativeLiveLocationDistanceFallback(lat, lon, centerLat, centerLon) <= radiusMeters

    // --- Call Manager fallbacks ---
    @JvmStatic fun nativeCallStartOutgoingFallback(roomId: String, calleeId: String, _calleeName: String, _callType: Int, sdpOffer: String): String =
        """{"call_id":"call_1","offer":{"type":"offer","sdp":"$sdpOffer"},"version":1}"""
    @JvmStatic fun nativeCallHandleIncomingFallback(callId: String, roomId: String, callerId: String, callerName: String, callType: Int, sdpOffer: String, lifetimeSec: Int): String = callId
    @JvmStatic fun nativeCallAnswerFallback(callId: String, sdpAnswer: String): String =
        """{"call_id":"$callId","answer":{"type":"answer","sdp":"$sdpAnswer"},"version":1}"""
    @JvmStatic fun nativeCallRejectFallback(callId: String): String = """{"call_id":"$callId","reason":"rejected","version":1}"""
    @JvmStatic fun nativeCallHangupFallback(callId: String): String = """{"call_id":"$callId","reason":"user_hung_up","version":1}"""
    @JvmStatic fun nativeCallGetActiveFallback(): String = "{}"
    @JvmStatic fun nativeCallGetIncomingFallback(): String = "{}"
    @JvmStatic fun nativeCallGetRoomCallsFallback(roomId: String): String = "[]"
    @JvmStatic fun nativeCallIsRoomInCallFallback(roomId: String): Boolean = false
    @JvmStatic fun nativeCallFormatDurationFallback(seconds: Int): String {
        val h = seconds / 3600; val m = (seconds % 3600) / 60; val s = seconds % 60
        return if (h > 0) "${h}:${m.toString().padStart(2,'0')}:${s.toString().padStart(2,'0')}"
        else "${m.toString().padStart(2,'0')}:${s.toString().padStart(2,'0')}"
    }
    @JvmStatic fun nativeCallParseSdpFallback(sdpText: String, type: String): String = """{"type":"$type","sdp":"","valid":false}"""
    @JvmStatic fun nativeCallSetMutedFallback(callId: String, muted: Boolean) {}
    @JvmStatic fun nativeCallSetVideoFallback(callId: String, enabled: Boolean) {}
    @JvmStatic fun nativeCallResetFallback() {}

    // --- Thread Manager fallbacks ---
    @JvmStatic fun nativeThreadIsRootFallback(eventContent: String, eventId: String): Boolean =
        """"rel_type":"m.thread"""" in eventContent
    @JvmStatic fun nativeThreadExtractRootFallback(eventContent: String): String =
        Regex(""""event_id":"(\$[^"]+)"""").find(eventContent)?.groupValues?.getOrNull(1) ?: ""
    @JvmStatic fun nativeThreadUpsertFallback(threadJson: String) {}
    @JvmStatic fun nativeThreadAddReplyFallback(threadId: String, senderId: String, senderName: String, body: String, timestampMs: Long) {}
    @JvmStatic fun nativeThreadGetListFallback(limit: Int, offset: Int): String =
        """{"threads":[],"total_count":0,"unread_count":0,"highlighted_count":0,"has_more":false}"""
    @JvmStatic fun nativeThreadSetUnreadFallback(threadId: String, count: Int, highlighted: Boolean) {}
    @JvmStatic fun nativeThreadMarkReadFallback(threadId: String, readPos: Long) {}
    @JvmStatic fun nativeThreadGetUnreadStateFallback(threadId: String): String =
        """{"thread_id":"$threadId","unread_count":0,"highlighted":false,"read_receipt_pos":0}"""
    @JvmStatic fun nativeThreadTotalUnreadFallback(): Int = 0
    @JvmStatic fun nativeThreadFormatCountFallback(count: Int): String = if (count > 99) "99+" else count.toString()
    @JvmStatic fun nativeThreadGetNotificationsFallback(): String = "[]"
    @JvmStatic fun nativeThreadResetFallback() {}

    // --- Poll Manager fallbacks ---
    @JvmStatic fun nativePollBuildStartFallback(question: String, optionsJson: String, kind: Int, maxSelections: Int, unstable: Boolean): String {
        val prefix = if (unstable) "org.matrix.msc3381.poll" else "m.poll"
        val opts = optionsJson.removeSurrounding("[", "]").split("\",\"").map { it.trim('"') }.mapIndexed { i, t ->
            """{"id":"${('A'+i).toChar()}","$prefix.org_text":"$t"}"""
        }.joinToString(",")
        return """{"$prefix.start":{},"$prefix.kind":"${if (kind==0) "disclosed" else "undisclosed"}", "$prefix.max_selections":$maxSelections, "$prefix.question":{"body":"$question","msgtype":"m.text"}, "$prefix.answers":[$opts]}"""
    }
    @JvmStatic fun nativePollBuildResponseFallback(pollId: String, selectionsJson: String, unstable: Boolean): String {
        val prefix = if (unstable) "org.matrix.msc3381.poll" else "m.poll"
        return """{"$prefix.response":{},"$prefix.org_selections":$selectionsJson}"""
    }
    @JvmStatic fun nativePollBuildEndFallback(pollId: String, reason: String, unstable: Boolean): String = """{"m.poll.end":{}}"""
    @JvmStatic fun nativePollTallyFallback(pollJson: String, votesJson: String): String =
        """{"question":"","total_votes":0,"is_closed":false,"winner_idx":-1,"plain_text":"","html":"","winner":""}"""
    @JvmStatic fun nativePollIsValidQuestionFallback(question: String): Boolean = question.isNotEmpty() && question.length <= 340

    // --- Space Graph fallbacks ---
    @JvmStatic fun nativeSpaceSetRootFallback(spaceId: String, name: String, topic: String, avatarUrl: String) {}
    @JvmStatic fun nativeSpaceAddChildRawFallback(parentId: String, childId: String, suggested: Boolean) {}
    @JvmStatic fun nativeSpaceAddChildFallback(parentId: String, childJson: String) {}
    @JvmStatic fun nativeSpaceSetMetadataFallback(roomId: String, name: String, topic: String, avatarUrl: String, joinRule: String, isJoined: Boolean) {}
    @JvmStatic fun nativeSpaceTraverseFallback(mode: Int, maxDepth: Int): String = """{"root_id":"","root_name":"","total_nodes":0,"total_spaces":0,"total_rooms":0,"max_depth":0,"nodes":[]}"""
    @JvmStatic fun nativeSpaceGetChildrenFallback(spaceId: String): String = "[]"
    @JvmStatic fun nativeSpaceGetParentsFallback(roomId: String): String = "[]"
    @JvmStatic fun nativeSpaceGetDepthFallback(roomId: String): Int = 0
    @JvmStatic fun nativeSpaceIsInSpaceFallback(spaceId: String, roomId: String): Boolean = false
    @JvmStatic fun nativeSpaceToTreeFallback(spaceId: String, maxDepth: Int): String = "{}"
    @JvmStatic fun nativeSpaceSearchFallback(spaceId: String, query: String): String = "[]"
    @JvmStatic fun nativeSpaceResetFallback() {}

    // --- Pin Manager fallbacks ---
    @JvmStatic fun nativePinEventFallback(roomId: String, eventId: String, pinnedBy: String, powerLevel: Int): String {
        if (powerLevel < 50) return """{"error":"Power level $powerLevel insufficient (need 50)"}"""
        return """{"pinned":["$eventId"]}"""
    }
    @JvmStatic fun nativeUnpinEventFallback(roomId: String, eventId: String, removedBy: String, powerLevel: Int): String {
        if (powerLevel < 50) return """{"error":"Power level $powerLevel insufficient (need 50)"}"""
        return """{"pinned":[]}"""
    }
    @JvmStatic fun nativePinToggleFallback(roomId: String, eventId: String, userId: String, powerLevel: Int): String =
        nativePinEventFallback(roomId, eventId, userId, powerLevel)
    @JvmStatic fun nativePinLoadStateFallback(roomId: String, stateJson: String) {}
    @JvmStatic fun nativePinGetEventsFallback(roomId: String): String = """{"room_id":"$roomId","pinned_count":0,"events":[]}"""
    @JvmStatic fun nativePinIsPinnedFallback(roomId: String, eventId: String): Boolean = false
    @JvmStatic fun nativePinCountFallback(roomId: String): Int = 0
    @JvmStatic fun nativePinCanManageFallback(powerLevel: Int): Boolean = powerLevel >= 50
    @JvmStatic fun nativePinResetFallback() {}

    // --- Media Viewer fallbacks ---
    @JvmStatic fun nativeMediaViewerParseFallback(contentJson: String): String {
        val url = Regex(""""url":"([^"]+)"""").find(contentJson)?.groupValues?.getOrNull(1) ?: ""
        val mime = Regex(""""mimetype":"([^"]+)"""").find(contentJson)?.groupValues?.getOrNull(1) ?: ""
        return """{"mxc":"$url","mime":"$mime","type":"Image","width":0,"height":0,"display_w":0,"display_h":0,"size":0,"size_fmt":"0 B","duration_ms":0,"duration_fmt":"0:00","exif_rotation":0,"has_thumb":false,"file_name":"","thumb_url":""}"""
    }
    @JvmStatic fun nativeMediaViewerFormatSizeFallback(bytes: Long): String = when {
        bytes < 1024 -> "$bytes B"
        bytes < 1048576 -> "${"%.1f".format(bytes/1024.0)} KB"
        bytes < 1073741824 -> "${"%.1f".format(bytes/1048576.0)} MB"
        else -> "${"%.2f".format(bytes/1073741824.0)} GB"
    }
    @JvmStatic fun nativeMediaViewerFormatDurationFallback(durationMs: Int): String {
        val s = durationMs / 1000; val m = s / 60; val h = m / 60
        return if (h > 0) "${h}:${(m%60).toString().padStart(2,'0')}:${(s%60).toString().padStart(2,'0')}"
        else "${m}:${(s%60).toString().padStart(2,'0')}"
    }
    @JvmStatic fun nativeMediaViewerViewportFallback(contentJson: String, viewportW: Int, viewportH: Int): String =
        """{"scale":1.0,"min_scale":0.5,"max_scale":5.0,"offset_x":0,"offset_y":0,"media_w":0,"media_h":0}"""
    @JvmStatic fun nativeMediaViewerThumbnailUrlFallback(mxcUrl: String, homeServer: String, width: Int, height: Int): String {
        val server = Regex("""mxc://([^/]+)/(.+)""").find(mxcUrl)?.destructured?.let { (s, id) -> "$s/$id" } ?: return ""
        return "${homeServer.trimEnd('/')}/_matrix/media/r0/thumbnail/$server?width=$width&height=$height&method=scale"
    }
    @JvmStatic fun nativeMediaViewerDownloadUrlFallback(mxcUrl: String, homeServer: String): String {
        val server = Regex("""mxc://([^/]+)/(.+)""").find(mxcUrl)?.destructured?.let { (s, id) -> "$s/$id" } ?: return ""
        return "${homeServer.trimEnd('/')}/_matrix/media/r0/download/$server"
    }
    @JvmStatic fun nativeMediaViewerExifRotationFallback(rawExif: Int): Int = when(rawExif) {
        3 -> 180; 6 -> 90; 8 -> 270; else -> 0 }
    @JvmStatic fun nativeMediaViewerCanThumbnailFallback(mimeType: String): Boolean =
        mimeType.startsWith("image/") || mimeType.startsWith("video/")

    // --- OIDC/SSO Login fallbacks ---
    @JvmStatic fun nativeOidcParseMetadataFallback(json: String): String =
        """{"issuer":"","auth_endpoint":"","token_endpoint":"","userinfo_endpoint":"","reg_endpoint":"","supports_registration":false,"valid":false}"""
    @JvmStatic fun nativeOidcBuildRegistrationFallback(configJson: String): String =
        """{"client_name":"Progressive Chat","redirect_uris":["chat.progressive.app:/"],"application_type":"native","grant_types":["authorization_code","refresh_token"],"response_types":["code"],"token_endpoint_auth_method":"none"}"""
    @JvmStatic fun nativeOidcParseRegistrationFallback(json: String): String =
        """{"client_id":"","client_secret":"","valid":false}"""
    @JvmStatic fun nativeOidcBuildAuthorizationFallback(metadataJson: String, registrationJson: String, configJson: String): String =
        """{"url":"https://auth.example.org/authorize","state":"state123","nonce":"nonce456","code_verifier":"verifier","code_challenge":"challenge","valid":false}"""
    @JvmStatic fun nativeOidcParseTokenFallback(json: String): String {
        val at = Regex(""""access_token":"([^"]+)"""").find(json)?.groupValues?.getOrNull(1) ?: ""
        return """{"access_token":"$at","refresh_token":"","expires_in":3600,"success":${at.isNotEmpty()}}"""
    }
    @JvmStatic fun nativeOidcBuildRefreshFallback(refreshToken: String, clientId: String): String =
        "grant_type=refresh_token&refresh_token=$refreshToken&client_id=$clientId"
    @JvmStatic fun nativeOidcParseWhoamiFallback(json: String): String =
        """{"user_id":"","device_id":"","valid":false}"""
    @JvmStatic fun nativeOidcParseWellKnownFallback(json: String): String =
        """{"base_url":"","oidc_issuer":"","supports_oidc":false,"supports_password":true,"requires_oidc":false}"""
    @JvmStatic fun nativeOidcIsCallbackFallback(url: String): Boolean =
        url.contains("code=") || url.contains("login/sso/redirect")
    @JvmStatic fun nativeOidcExtractCodeFallback(callbackUrl: String): String =
        Regex("""[?&]code=([^&]+)""").find(callbackUrl)?.groupValues?.getOrNull(1) ?: ""
    @JvmStatic fun nativeOidcBuildPasswordLoginFallback(userId: String, password: String, deviceId: String, deviceName: String): String =
        """{"type":"m.login.password","identifier":{"type":"m.id.user","user":"$userId"},"password":"$password"}"""

    // --- User Directory fallbacks ---
    @JvmStatic fun nativeUserDirBuildSearchFallback(searchTerm: String, limit: Int): String =
        """{"search_term":"$searchTerm","limit":$limit}"""
    @JvmStatic fun nativeUserDirSearchFallback(query: String, responseJson: String): String = responseJson
    @JvmStatic fun nativeUserDirBestNameFallback(displayName: String, userId: String): String =
        displayName.ifEmpty { userId.removePrefix("@").substringBefore(":") }
    @JvmStatic fun nativeUserDirAvatarInitFallback(displayName: String, userId: String): String {
        val name = displayName.ifEmpty { userId.removePrefix("@").substringBefore(":") }
        return if (name.isNotEmpty()) name.take(1).uppercase() else "?"
    }
    @JvmStatic fun nativeUserDirIsValidQueryFallback(query: String): Boolean = query.length in 2..256

    // --- Profiler fallbacks ---
    @JvmStatic fun nativeProfileStartFallback() {}
    @JvmStatic fun nativeProfileStopFallback() {}
    @JvmStatic fun nativeProfileResetFallback() {}
    @JvmStatic fun nativeProfileIsActiveFallback(): Boolean = false
    @JvmStatic fun nativeProfileReportFallback(): String = """{"total_time_ns":0,"total_time_fmt":"0ns","is_profiling":false,"entries":[],"snapshots":[]}"""
    @JvmStatic fun nativeProfileReportTextFallback(): String = "Profiler not available (native lib not loaded)"
    @JvmStatic fun nativeProfileGetSummaryFallback(name: String): String = """{"name":"$name","calls":0,"total_ns":0,"avg_ns":0,"min_ns":0,"max_ns":0}"""
    @JvmStatic fun nativeProfileMemoryFallback(): String = """{"bytes":0,"alloc_count":0,"dealloc_count":0,"ts":0}"""

    @JvmStatic fun nativeProfileStartActionFallback(actionName: String, isCold: Boolean): Int {
        val start = System.nanoTime()
        _actionTimers[actionName] = start
        return 0
    }
    @JvmStatic fun nativeProfileStopActionFallback(actionIndex: Int): Long {
        return 0L
    }
    @JvmStatic fun nativeProfileSetBudgetFallback(actionPattern: String, budgetNs: Long) {}
    @JvmStatic fun nativeProfileActionReportFallback(): String =
        """{"actions":[],"frame_fps":0,"frame_count":0}"""
    @JvmStatic fun nativeProfileActionReportTextFallback(): String = "Action profiler not available (native lib not loaded)"

    @JvmStatic fun nativeProfileOverlaySnapshotFallback(): String =
        """{"fps":0.0,"fps_color":"#F44336","memory_bytes":0,"memory_mb":0.0,"active_actions":0,"violations":0,"violation_list":[],"hot_actions":[],"drops":0,"total_frames":0}"""
    @JvmStatic fun nativeProfileOverlayTextFallback(): String = "Profiler N/A"

    private val _actionTimers = mutableMapOf<String, Long>()

    // --- Device Manager Full fallbacks ---
    @JvmStatic fun nativeDeviceParseListFallback(json: String): String = "[]"
    @JvmStatic fun nativeDeviceParseInfoFallback(deviceId: String, json: String): String =
        """{"device_id":"$deviceId","user_id":"","display_name":"","last_seen_ts":0,"last_seen":"Never","last_seen_ip":"","user_agent":"","is_inactive":true}"""
    @JvmStatic fun nativeDeviceParseCryptoFallback(deviceId: String, userId: String, json: String): String =
        """{"device_id":"$deviceId","user_id":"$userId","display_name":"","fingerprint":"","identity_key":"","trust_label":"Not verified","is_verified":false,"is_blocked":false,"algorithms":[]}"""
    @JvmStatic fun nativeDeviceBuildRenameFallback(deviceId: String, newName: String): String =
        """{"display_name":"$newName"}"""
    @JvmStatic fun nativeDeviceBuildDeleteFallback(deviceId: String, authType: String, authSession: String, password: String): String =
        """{"devices":["$deviceId"],"auth":{"type":"$authType","session":"$authSession"}}"""
    @JvmStatic fun nativeDeviceFormatFingerprintFallback(rawKey: String): String =
        rawKey.uppercase().chunked(4).joinToString(" ")
    @JvmStatic fun nativeDeviceGetTrustLabelFallback(crossSigningVerified: Boolean, locallyVerified: Boolean): String =
        if (crossSigningVerified) "Verified" else if (locallyVerified) "Verified (local)" else "Not verified"
    @JvmStatic fun nativeDeviceFormatLastSeenFallback(timestampMs: Long): String {
        if (timestampMs <= 0) return "Never"
        val diff = System.currentTimeMillis() - timestampMs
        return when {
            diff < 60000 -> "Just now"
            diff < 3600000 -> "${diff/60000}m ago"
            diff < 86400000 -> "${diff/3600000}h ago"
            diff < 2592000000 -> "${diff/86400000}d ago"
            else -> "${diff/2592000000}mo ago"
        }
    }
    @JvmStatic fun nativeDeviceIsInactiveFallback(lastSeenTs: Long, inactivityDays: Int): Boolean {
        if (lastSeenTs <= 0) return true
        return (System.currentTimeMillis() - lastSeenTs) > inactivityDays * 86400000L
    }
    @JvmStatic fun nativeDeviceSatisfiesVersionFallback(clientVersion: String, minRequired: String): Boolean =
        clientVersion >= minRequired

    // --- Room Directory fallbacks ---
    @JvmStatic fun nativeRoomDirBuildSearchFallback(searchTerm: String, limit: Int, since: String): String {
        val json = StringBuilder("""{"limit":$limit""")
        if (since.isNotEmpty()) json.append(""","since":"$since"""")
        if (searchTerm.isNotEmpty()) json.append(""","filter":{"generic_search_term":"$searchTerm"}""")
        json.append("}")
        return json.toString()
    }
    @JvmStatic fun nativeRoomDirParseResponseFallback(json: String): String =
        """{"rooms":[],"next_batch":"","has_more":false,"total_estimate":0,"loaded_count":0}"""
    @JvmStatic fun nativeRoomDirBuildVisibilityFallback(visibility: Int): String =
        if (visibility == 1) """{"visibility":"public"}""" else """{"visibility":"private"}"""
    @JvmStatic fun nativeRoomDirParseVisibilityFallback(json: String): String =
        if (json.contains("public")) "public" else "private"
    @JvmStatic fun nativeRoomDirCheckAliasFallback(aliasLocalPart: String, json: String): String =
        """{"alias":"#$aliasLocalPart","available":false}"""
    @JvmStatic fun nativeRoomDirFormatPreviewFallback(roomJson: String): String {
        val name = Regex(""""name":"([^"]+)"""").find(roomJson)?.groupValues?.getOrNull(1) ?: ""
        val topic = Regex(""""topic":"([^"]+)"""").find(roomJson)?.groupValues?.getOrNull(1) ?: ""
        val members = Regex(""""num_members":(\d+)""").find(roomJson)?.groupValues?.getOrNull(1) ?: "0"
        return "$name — $topic ($members members)"
    }

    // --- Session Manager fallbacks ---
    @JvmStatic fun nativeSessionComputeIdFallback(userId: String, deviceId: String): String =
        java.security.MessageDigest.getInstance("MD5").digest("$userId|$deviceId".toByteArray()).joinToString("") { "%02x".format(it) }
    @JvmStatic fun nativeSessionCreateFallback(credentialsJson: String, configJson: String, loginType: Int): String {
        val userId = Regex(""""user_id":"([^"]+)"""").find(credentialsJson)?.groupValues?.getOrNull(1) ?: ""
        return """{"session_id":"session_1","user_id":"$userId","device_id":"","display_name":"","avatar_url":"","home_server":"","state":"created","is_active":false,"is_syncing":false,"is_openable":true,"login_type":"password","created_at":${System.currentTimeMillis()},"last_sync":0,"push_rule":""}"""
    }
    @JvmStatic fun nativeSessionOpenFallback(sessionId: String): Boolean = true
    @JvmStatic fun nativeSessionCloseFallback(sessionId: String): Boolean = true
    @JvmStatic fun nativeSessionRemoveFallback(sessionId: String): Boolean = true
    @JvmStatic fun nativeSessionSetActiveFallback(sessionId: String): Boolean = true
    @JvmStatic fun nativeSessionGetActiveFallback(): String = "{}"
    @JvmStatic fun nativeSessionHasActiveFallback(): Boolean = false
    @JvmStatic fun nativeSessionGetAllFallback(): String = "[]"
    // --- Server Notice Handler fallbacks ---
    @JvmStatic fun nativeServerNoticeParseFallback(errorJson: String): String {
        val code = Regex(""""errcode":"([^"]+)"""").find(errorJson)?.groupValues?.getOrNull(1) ?: ""
        val msg = Regex(""""error":"([^"]+)"""").find(errorJson)?.groupValues?.getOrNull(1) ?: ""
        val admin = Regex(""""admin_contact":"([^"]+)"""").find(errorJson)?.groupValues?.getOrNull(1) ?: ""
        return """{"type":"$code","body":"$msg","is_resource_limit":$code=="M_RESOURCE_LIMIT_EXCEEDED","is_consent":$code=="M_CONSENT_NOT_GIVEN","is_rate_limit":$code=="M_LIMIT_EXCEEDED","limit_type":"unknown","admin_contact":"$admin","consent_uri":"","retry_after_ms":0,"banner_color":"#2196F3"}"""
    }
    @JvmStatic fun nativeServerNoticeFormatLimitFallback(_errorJson: String, mode: Int): String =
        if (mode == 1) "This homeserver has exceeded a resource limit. Please contact your service administrator."
        else "This homeserver is approaching a resource limit."
    @JvmStatic fun nativeServerNoticeGetDescriptionFallback(errorCode: String): String = when(errorCode) {
        "M_FORBIDDEN" -> "Access forbidden"; "M_UNKNOWN_TOKEN" -> "Unknown access token"
        "M_LIMIT_EXCEEDED" -> "Rate limit exceeded"; "M_RESOURCE_LIMIT_EXCEEDED" -> "Server resource limit exceeded"
        "M_CONSENT_NOT_GIVEN" -> "Consent not given"; "M_USER_DEACTIVATED" -> "User account deactivated"
        "M_NOT_FOUND" -> "Not found"; "M_BAD_JSON" -> "Bad JSON format"; "M_UNAUTHORIZED" -> "Not authorized"
        else -> "Error: $errorCode" }
    @JvmStatic fun nativeServerNoticeIsResourceLimitFallback(errorCode: String): Boolean = errorCode == "M_RESOURCE_LIMIT_EXCEEDED"
    @JvmStatic fun nativeServerNoticeIsRateLimitFallback(errorCode: String): Boolean = errorCode == "M_LIMIT_EXCEEDED"
    @JvmStatic fun nativeServerNoticeIsConsentFallback(errorCode: String): Boolean = errorCode == "M_CONSENT_NOT_GIVEN"
    @JvmStatic fun nativeServerNoticeFormatDowntimeFallback(retryAfterMs: Long): String {
        val s = retryAfterMs / 1000
        return when { s < 60 -> "${s}s"; s < 3600 -> "${s/60}m"; s < 86400 -> "${s/3600}h"; else -> "${s/86400}d" }
    }
    @JvmStatic fun nativeServerNoticeGetBannerFallback(errorJson: String): String =
        if ("M_RESOURCE_LIMIT_EXCEEDED" in errorJson) "#FF4444" else "#2196F3"

    // --- Media Upload Manager fallbacks ---
    @JvmStatic fun nativeUploadParseResponseFallback(json: String): String {
        val uri = Regex(""""content_uri":"([^"]+)"""").find(json)?.groupValues?.getOrNull(1) ?: ""
        return """{"content_uri":"$uri","success":${uri.isNotEmpty()}}"""
    }
    @JvmStatic fun nativeUploadBuildContentFallback(attachmentJson: String, mxcUrl: String): String {
        val body = Regex(""""name":"([^"]+)"""").find(attachmentJson)?.groupValues?.getOrNull(1) ?: ""
        val mime = Regex(""""mime_type":"([^"]+)"""").find(attachmentJson)?.groupValues?.getOrNull(1) ?: ""
        return """{"msgtype":"m.file","body":"$body","url":"$mxcUrl","info":{"size":0,"mimetype":"$mime"}}"""
    }
    @JvmStatic fun nativeUploadIsSizeValidFallback(fileSize: Long): Boolean = fileSize <= 104857600
    @JvmStatic fun nativeUploadFormatSizeWarningFallback(fileSize: Long, maxSize: Long): String =
        "File too large: ${fileSize/1048576.0} MB exceeds ${maxSize/1048576.0} MB limit"
    @JvmStatic fun nativeUploadGetProgressFallback(): String = """{"total_bytes":0,"uploaded_bytes":0,"percent":0.0,"is_complete":false,"content_uri":""}"""
    @JvmStatic fun nativeUploadResetProgressFallback(totalBytes: Long) {}
    @JvmStatic fun nativeUploadSetMaxSizeFallback(maxBytes: Long) {}

    // --- Identity Server Manager fallbacks ---
    @JvmStatic fun nativeIdentityParse3pidFallback(input: String): String {
        val medium = if (input.contains("@")) "email" else "msisdn"
        return """{"medium":"$medium","value":"$input","valid":true}"""
    }
    @JvmStatic fun nativeIdentityBuildBindFallback(threePid: String): String =
        """{"medium":"email","address":"$threePid"}"""
    @JvmStatic fun nativeIdentityBuildLookupFallback(pidsJson: String): String =
        """{"threepids":[["email","$pidsJson"]]}"""
    @JvmStatic fun nativeIdentityParseLookupFallback(json: String): String = "[]"
    @JvmStatic fun nativeIdentitySetServerFallback(url: String): String {
        val final = if (url.startsWith("http")) url else "https://$url"
        return """{"url":"$final"}"""
    }
    @JvmStatic fun nativeIdentityGetServerFallback(): String = ""

    // --- Event Relations fallbacks ---
    @JvmStatic fun nativeRelationParseFallback(eventContent: String): String {
        val type = Regex(""""rel_type":"([^"]+)"""").find(eventContent)?.groupValues?.getOrNull(1) ?: ""
        val eid = Regex(""""event_id":"([^"]+)"""").find(eventContent)?.groupValues?.getOrNull(1) ?: ""
        return """{"type":"$type","rel_type":"$type","event_id":"$eid","is_reply":false,"is_edit":$type=="m.replace","is_thread":$type=="m.thread","is_annotation":$type=="m.annotation"}"""
    }
    @JvmStatic fun nativeRelationIsReplyFallback(eventContent: String): Boolean =
        """m.in_reply_to""" in eventContent
    @JvmStatic fun nativeRelationIsEditFallback(eventContent: String): Boolean =
        """"rel_type":"m.replace"""" in eventContent
    @JvmStatic fun nativeRelationIsReactionFallback(eventContent: String): Boolean =
        """"rel_type":"m.annotation"""" in eventContent
    @JvmStatic fun nativeRelationExtractThreadRootFallback(eventContent: String): String =
        Regex(""""rel_type":"m\.thread".*?"event_id":"(\$[^"]+)"""").find(eventContent)?.groupValues?.getOrNull(1) ?: ""
    @JvmStatic fun nativeRelationExtractReplySourceFallback(eventContent: String): String =
        Regex(""""m\.in_reply_to":\{"event_id":"(\$[^"]+)"""").find(eventContent)?.groupValues?.getOrNull(1) ?: ""
    @JvmStatic fun nativeRelationBuildReplyFallback(eventId: String): String =
        """{"m.in_reply_to":{"event_id":"$eventId"}}"""
    @JvmStatic fun nativeRelationBuildEditFallback(eventId: String): String =
        """{"m.relates_to":{"rel_type":"m.replace","event_id":"$eventId"}}"""
    @JvmStatic fun nativeRelationBuildThreadFallback(eventId: String, replyToId: String): String =
        """{"m.relates_to":{"rel_type":"m.thread","event_id":"$eventId"}}"""
    @JvmStatic fun nativeRelationBuildAnnotationFallback(eventId: String, key: String): String =
        """{"m.relates_to":{"rel_type":"m.annotation","event_id":"$eventId","key":"$key"}}"""

    // --- Cross-Signing fallbacks ---
    @JvmStatic fun nativeCrossSigningIsInitFallback(): Boolean = false
    @JvmStatic fun nativeCrossSigningCanSignFallback(): Boolean = false
    @JvmStatic fun nativeCrossSigningBuildKeysFallback(userId: String, mskPublic: String, uskPublic: String, sskPublic: String): String =
        """{"user_id":"$userId","is_trusted":false,"was_trusted_once":false,"key_count":3,"has_msk":true,"has_usk":true,"has_ssk":true}"""
    @JvmStatic fun nativeCrossSigningCheckSelfFallback(): String =
        """{"is_trusted":false,"cross_signing_verified":false,"locally_verified":false}"""
    @JvmStatic fun nativeCrossSigningImportKeysFallback(mskPrivate: String, uskPrivate: String, sskPrivate: String): String =
        """{"is_trusted":true,"cross_signing_verified":true,"locally_verified":true}"""
    @JvmStatic fun nativeCrossSigningTrustMasterFallback() {}

    // --- Draft Manager fallbacks ---
    @JvmStatic fun nativeDraftSaveFallback(roomId: String, content: String, type: Int, linkedEventId: String) {}
    @JvmStatic fun nativeDraftGetFallback(roomId: String): String = "{}"
    @JvmStatic fun nativeDraftDeleteFallback(roomId: String) {}
    @JvmStatic fun nativeDraftHasDraftFallback(roomId: String): Boolean = false
    @JvmStatic fun nativeDraftAutoSaveFallback(roomId: String, text: String): Boolean = false
    @JvmStatic fun nativeDraftStripPrefixFallback(text: String): String = text.removePrefix("draft: ")

    // --- Room History Visibility fallbacks ---
    @JvmStatic fun nativeRoomStateParseVisibilityFallback(contentJson: String): String =
        if ("world_readable" in contentJson) "world_readable"
        else if ("invited" in contentJson) "invited"
        else if ("joined" in contentJson) "joined"
        else "shared"
    @JvmStatic fun nativeRoomStateParseJoinRulesFallback(contentJson: String): String =
        if ("public" in contentJson) "public" else "invite"
    @JvmStatic fun nativeRoomStateShouldShareFallback(contentJson: String): Boolean =
        "world_readable" in contentJson || "shared" in contentJson
    @JvmStatic fun nativeRoomStateIsPublicFallback(roomId: String): Boolean = false
    @JvmStatic fun nativeRoomStateIsInviteOnlyFallback(roomId: String): Boolean = true
    @JvmStatic fun nativeRoomStateSetVisibilityFallback(roomId: String, visibility: Int) {}
    @JvmStatic fun nativeRoomStateSetJoinRuleFallback(roomId: String, joinRule: Int) {}

    // --- Terms/Consent fallbacks ---
    @JvmStatic fun nativeTermsParseFallback(json: String): String =
        """{"has_policies":false,"count":0,"policies":[]}"""
    @JvmStatic fun nativeTermsBuildAgreeFallback(urlsJson: String): String =
        """{"user_accepts":$urlsJson}"""
    @JvmStatic fun nativeTermsAreRequiredFallback(errorJson: String): Boolean =
        "M_TERMS_NOT_SIGNED" in errorJson || "M_CONSENT_NOT_GIVEN" in errorJson
    @JvmStatic fun nativeTermsGetPendingFallback(responseJson: String, agreedJson: String): String = "[]"

    // --- Transparent Overlay fallbacks ---
    @JvmStatic fun nativeOverlaySetConfigFallback(configJson: String) {}
    @JvmStatic fun nativeOverlayTouchDownFallback(x: Double, y: Double, pointerId: Int, timeNs: Long): Int = 0
    @JvmStatic fun nativeOverlayTouchMoveFallback(x: Double, y: Double, pointerId: Int, timeNs: Long): Int = 0
    @JvmStatic fun nativeOverlayTouchUpFallback(pointerId: Int, timeNs: Long): Int = 0
    @JvmStatic fun nativeOverlayBackFallback(timeNs: Long): Int = 0
    @JvmStatic fun nativeOverlayTickFallback(timeNs: Long): Int = 0
    @JvmStatic fun nativeOverlayGetStateFallback(): String =
        """{"gesture":0,"bg_is_fg":false,"one_finger_armed":false,"two_finger_waiting":false,"fg_until_ms":0,"pointers":0}"""

    @JvmStatic fun nativeOverlaySetSafetyModeFallback(mode: Int) {}
    @JvmStatic fun nativeOverlaySetSafetyPermsFallback(permissionsJson: String) {}
    @JvmStatic fun nativeOverlayIsTouchAllowedFallback(action: Int): Boolean = true
    @JvmStatic fun nativeOverlaySafetyToJsonFallback(): String =
        """{"mode":0,"mode_label":"Full access","allow_tap":true,"allow_scroll":true,"allow_long_press":true,"allow_text_input":true,"allow_navigation":true,"show_sensitive":true,"allowed":["Tap","Scroll","Long press","Text input","Navigation","Media control"]}"""

    // --- Message Composer fallbacks ---
    @JvmStatic fun nativeComposerSetTextFallback(text: String) {}
    @JvmStatic fun nativeComposerGetStateFallback(): String =
        """{"send_mode":0,"send_mode_name":"regular","text":"","linked_event_id":"","can_send":"allowed","is_fullscreen":false,"text_length":0}"""
    @JvmStatic fun nativeComposerEnterRegularFallback() {}
    @JvmStatic fun nativeComposerEnterEditFallback(eventId: String) {}
    @JvmStatic fun nativeComposerEnterQuoteFallback(eventId: String) {}
    @JvmStatic fun nativeComposerEnterReplyFallback(eventId: String) {}
    @JvmStatic fun nativeComposerApplyBoldFallback(text: String, selStart: Int, selEnd: Int): String =
        text.substring(0, selStart) + "**" + text.substring(selStart, selEnd) + "**" + text.substring(selEnd)
    @JvmStatic fun nativeComposerApplyItalicFallback(text: String, selStart: Int, selEnd: Int): String =
        text.substring(0, selStart) + "*" + text.substring(selStart, selEnd) + "*" + text.substring(selEnd)
    @JvmStatic fun nativeComposerBuildQuotedFallback(quotedText: String, replyText: String, quotedSender: String): String =
        "> " + quotedText.replace("\n", "\n> ") + "\n\n" + replyText
    @JvmStatic fun nativeComposerAutoEmojiFallback(text: String): String = text
    @JvmStatic fun nativeComposerExtractMentionFallback(text: String, cursorPos: Int): String = ""
    @JvmStatic fun nativeComposerValidateFallback(text: String, maxLength: Int): String {
        val isEmpty = text.isBlank(); val tooLong = text.length > maxLength
        return """{"valid":${!isEmpty && !tooLong},"isEmpty":$isEmpty,"is_too_long":$tooLong,"length":${text.length},"max_length":$maxLength,"error":"${if (isEmpty) "Message is empty" else if (tooLong) "Too long" else ""}"}"""
    }

    // --- Text Undo fallbacks ---
    @JvmStatic fun nativeUndoSetConfigFallback(configJson: String) {}
    @JvmStatic fun nativeUndoCheckpointFallback(text: String, cursorPos: Int, description: String) {}
    @JvmStatic fun nativeUndoOnSelectAllFallback(text: String, cursorPos: Int) {}
    @JvmStatic fun nativeUndoOnBeforePasteFallback(currentText: String, cursorPos: Int, pastedText: String) {}
    @JvmStatic fun nativeUndoDoFallback(): String =
        """{"text":"","cursor":0,"can_undo":false,"can_redo":false,"description":""}"""
    @JvmStatic fun nativeUndoRedoFallback(): String =
        """{"text":"","cursor":0,"can_undo":false,"can_redo":false,"description":""}"""
    @JvmStatic fun nativeUndoGetStateFallback(): String =
        """{"can_undo":false,"can_redo":false,"undo_count":0,"redo_count":0,"next_undo":"","next_redo":"","enabled":false}"""

    // --- Room Permissions fallbacks ---
    @JvmStatic fun nativePermParseFallback(powerLevelsJson: String): String =
        """{"ban":50,"kick":50,"invite":0,"redact":50,"events_default":0,"state_default":50,"users_default":0,"user_count":0,"event_count":0,"room_notification_level":50}"""
    @JvmStatic fun nativePermGetRoleFallback(userId: String, powerLevel: Int): String {
        val role = when { powerLevel >= 150 -> "Super Admin"; powerLevel >= 100 -> "Admin"; powerLevel >= 50 -> "Moderator"; else -> "User" }
        return """{"user_id":"$userId","power":$powerLevel,"role":"$role","can_ban":${powerLevel>=50},"can_kick":${powerLevel>=50},"can_invite":${powerLevel>=0},"can_redact":${powerLevel>=50},"can_send_msg":${powerLevel>=0},"can_send_state":${powerLevel>=50},"can_notify_room":${powerLevel>=50},"can_change_pl":${powerLevel>=100},"description":"Power $powerLevel ($role)"}"""
    }
    @JvmStatic fun nativePermBuildContentFallback(powerLevelsJson: String): String =
        """{"ban":50,"kick":50,"invite":0,"redact":50,"events_default":0,"users_default":0,"state_default":50,"users":{}}"""
    @JvmStatic fun nativePermBuildKickFallback(userId: String, reason: String): String =
        """{"user_id":"$userId","reason":"$reason"}"""
    @JvmStatic fun nativePermBuildBanFallback(userId: String, reason: String): String =
        """{"user_id":"$userId","reason":"$reason"}"""
    @JvmStatic fun nativePermFormatChangeFallback(userId: String, oldPower: Int, newPower: Int): String {
        val oldRole = when { oldPower >= 100 -> "Admin"; oldPower >= 50 -> "Moderator"; else -> "User" }
        val newRole = when { newPower >= 100 -> "Admin"; newPower >= 50 -> "Moderator"; else -> "User" }
        return "$userId changed from $oldRole ($oldPower) to $newRole ($newPower)"
    }

    // --- Offline Cache fallbacks ---
    @JvmStatic fun nativeCacheRegisterRoomFallback(roomJson: String) {}
    @JvmStatic fun nativeCacheGetPlanFallback(): String =
        """{"totalBudget":0,"totalAllocated":0,"roomsCached":0,"roomsSkipped":0,"roomPlans":[],"estimatedTimeMs":0}"""
    @JvmStatic fun nativeCacheGetStatsFallback(): String =
        """{"total_cached_mb":0.0,"total_messages":0,"total_media":0,"rooms_cached":0,"hits":0,"misses":0,"hit_rate":0.0,"bandwidth_saved_mb":0.0,"evictions":0,"evicted_mb":0.0}"""
    @JvmStatic fun nativeCacheGetPressureFallback(availableBytes: Long, reservedBytes: Long): String {
        val free = availableBytes - reservedBytes
        return """{"level":0,"label":"plenty of space"}""" // Simplified
    }
    @JvmStatic fun nativeCacheEvictToFreeFallback(targetBytes: Long, availableBytes: Long, reservedBytes: Long): String = "[]"
    @JvmStatic fun nativeCacheRecordHitFallback(roomId: String, bytes: Long) {}
    @JvmStatic fun nativeCacheRecordMissFallback(roomId: String, bytes: Long) {}

    // --- Spoiler fallbacks ---
    @JvmStatic fun nativeSpoilerBuildImageFallback(body: String, mxcUrl: String, mimeType: String, width: Int, height: Int, sizeBytes: Long, reason: String): String {
        val reasonAttr = if (reason.isNotEmpty()) " data-mx-spoiler-reason=\"$reason\"" else ""
        val img = "<img src=\"$mxcUrl\" alt=\"$body\" title=\"$body\" />"
        return """{"plain_body":"$body","formatted_body":"<span data-mx-spoiler$reasonAttr>$img</span>","type":"image","has_spoiler":true}"""
    }
    @JvmStatic fun nativeSpoilerBuildTextFallback(body: String, reason: String): String {
        val reasonAttr = if (reason.isNotEmpty()) " data-mx-spoiler-reason=\"$reason\"" else ""
        return """{"plain_body":"||$body||","formatted_body":"<span data-mx-spoiler$reasonAttr>$body</span>","type":"text","has_spoiler":true}"""
    }
    @JvmStatic fun nativeSpoilerHasSpoilerFallback(formattedBody: String): Boolean = "data-mx-spoiler" in formattedBody
    @JvmStatic fun nativeSpoilerDetectTypeFallback(formattedBody: String): String =
        when { "<img" in formattedBody -> "image"; "<video" in formattedBody -> "video"; else -> "text" }
    @JvmStatic fun nativeSpoilerBuildContentFallback(body: String, mxcUrl: String, msgType: String, reason: String): String =
        """{"msgtype":"$msgType","body":"$body","url":"$mxcUrl","m.spoiler":true}"""

    @JvmStatic fun nativeSessionCountFallback(): Int = 0

    // --- URL Preview fallbacks ---
    @JvmStatic fun nativeIsPreviewableUrlFallback(url: String): Boolean = url.startsWith("http")
    @JvmStatic fun nativeExtractUrlsFallback(text: String): String {
        val regex = Regex("https?://[^\\s]+")
        return regex.findAll(text).joinToString(",", "[", "]") { "\"${it.value}\"" }
    }

    // --- Device Manager fallback ---
    @JvmStatic fun nativeFormatDeviceLastSeenFallback(lastSeenMs: Long): String {
        if (lastSeenMs <= 0) return "Never"
        val diff = (System.currentTimeMillis() - lastSeenMs) / 1000
        return when {
            diff < 60 -> "Active now"
            diff < 3600 -> "${diff / 60}m ago"
            diff < 86400 -> "${diff / 3600}h ago"
            else -> "${diff / 86400}d ago"
        }
    }

    // --- Permalink fallback ---
    @JvmStatic fun nativeIsSameRoomPermalinkFallback(url1: String, url2: String): Boolean {
        val id1 = Regex("#/([!@#][^/?]+)").find(url1)?.groupValues?.get(1) ?: ""
        val id2 = Regex("#/([!@#][^/?]+)").find(url2)?.groupValues?.get(1) ?: ""
        return id1 == id2 && id1.isNotEmpty()
    }

    // --- Display Name fallbacks ---
    @JvmStatic fun nativeGetBestDisplayNameFallback(displayName: String, userId: String): String =
        displayName.ifEmpty { userId.removePrefix("@").substringBefore(":") }
    @JvmStatic fun nativeFormatMemberNameFallback(displayName: String, userId: String, powerLevel: Int, showBadge: Boolean): String {
        val name = displayName.ifEmpty { userId.removePrefix("@").substringBefore(":") }
        return if (showBadge && powerLevel >= 50) "$name ⭐" else name
    }

    // --- Identity fallbacks ---
    @JvmStatic fun nativeIsEmailFallback(input: String): Boolean = input.contains("@") && input.contains(".")
    @JvmStatic fun nativeIsMsisdnFallback(input: String): Boolean = input.startsWith("+") && input.drop(1).all { it.isDigit() }
    @JvmStatic fun nativeExtractAliasLocalpartFallback(alias: String): String = alias.removePrefix("#").substringBefore(":")

    // --- Link Preview fallback ---
    @JvmStatic fun nativeIsImageUrlFallback(url: String): Boolean =
        url.endsWith(".jpg") || url.endsWith(".png") || url.endsWith(".gif") || url.endsWith(".webp")

    // --- Permalink Parser fallbacks ---
    @JvmStatic fun nativeExtractRoomIdFromPermalinkFallback(url: String): String =
        Regex("[!#][^:/?#]+").find(url)?.value ?: ""
    @JvmStatic fun nativeExtractEventIdFromPermalinkFallback(url: String): String =
        Regex("\\\$[^:/?#]+").find(url)?.value ?: ""
    @JvmStatic fun nativeExtractUserIdFromPermalinkFallback(url: String): String =
        Regex("@[^:/?#]+").find(url)?.value ?: ""

    // --- URL Preview fallbacks ---
    @JvmStatic fun nativeStripHtmlTagsFallback(html: String): String = html.replace(Regex("<[^>]*>"), " ").replace(Regex("\\s+"), " ").trim()
    @JvmStatic fun nativeTruncateDescriptionFallback(text: String, maxLen: Int): String =
        if (text.length <= maxLen) text else text.take(maxLen).substringBeforeLast(" ") + "…"

    @JvmStatic fun nativeExtractHtmlTitleFallback(html: String): String =
        Regex("""<title[^>]*>([^<]+)</title>""", RegexOption.IGNORE_CASE).find(html)?.groupValues?.getOrNull(1) ?: ""
    @JvmStatic fun nativeExtractMetaDescriptionFallback(html: String): String =
        Regex("""<meta[^>]+name=["']description["'][^>]+content=["']([^"']+)""", RegexOption.IGNORE_CASE).find(html)?.groupValues?.getOrNull(1) ?: ""
    @JvmStatic fun nativeResolveUrlFallback(baseUrl: String, relative: String): String {
        if (relative.startsWith("http")) return relative
        if (relative.startsWith("//")) return "https:" + relative
        val base = baseUrl.substringBeforeLast("/")
        return if (relative.startsWith("/")) baseUrl.substringBefore("/", baseUrl.substringAfter("://")) + relative
        else "$base/$relative"
    }
    @JvmStatic fun nativeUrlPreviewToJsonFallback(previewJson: String): String = previewJson

    // --- Device Type fallback ---
    @JvmStatic fun nativeClassifyDeviceTypeFallback(userAgent: String, clientName: String): String = when {
        userAgent.contains("Mobile") || userAgent.contains("Android") -> "Mobile"
        userAgent.contains("Tablet") -> "Tablet"
        else -> "Desktop"
    }

    // --- Web Search fallback ---
    @JvmStatic fun nativeBuildSearxngUrlFallback(endpoint: String, query: String, maxResults: Int): String =
        "${endpoint.trimEnd('/')}/search?q=${java.net.URLEncoder.encode(query, "UTF-8")}&format=json"
    @JvmStatic fun nativeBuildDuckDuckGoUrlFallback(query: String): String =
        "https://api.duckduckgo.com/?q=${java.net.URLEncoder.encode(query, "UTF-8")}&format=json"
    @JvmStatic fun nativeBuildGoogleUrlFallback(apiKey: String, engineId: String, query: String, maxResults: Int): String =
        "https://customsearch.googleapis.com/customsearch/v1?key=$apiKey&cx=$engineId&q=${java.net.URLEncoder.encode(query, "UTF-8")}&num=$maxResults"

    // --- Version Comparison fallback ---
    @JvmStatic fun nativeCompareSemverFallback(a: String, b: String): Int {
        val ap = a.split(".").map { it.toIntOrNull() ?: 0 }
        val bp = b.split(".").map { it.toIntOrNull() ?: 0 }
        for (i in 0 until maxOf(ap.size, bp.size)) {
            val av = ap.getOrElse(i) { 0 }; val bv = bp.getOrElse(i) { 0 }
            if (av != bv) return av.compareTo(bv)
        }
        return 0
    }

    // --- Knock / Server / Fingerprint fallbacks ---
    @JvmStatic fun nativeFormatKnockReasonFallback(reason: String): String = reason.ifEmpty { "No reason provided" }

    @JvmStatic fun nativeIsServerCompatibleFallback(serverVersion: String, minRequired: String): Boolean {
        val r = nativeCompareSemverFallback(serverVersion, minRequired)
        return r >= 0
    }
    @JvmStatic fun nativeExtractDeviceFingerprintFallback(deviceId: String, keysJson: String): String = deviceId.take(10)

    // --- Event Distance fallback ---
    @JvmStatic fun nativeEventDistanceFallback(indexA: Int, indexB: Int): Int = kotlin.math.abs(indexA - indexB)

    // --- Widget Validation fallbacks ---
    @JvmStatic fun nativeIsEtherpadWidgetFallback(type: String): Boolean = type == "m.etherpad" || type == "etherpad"
    @JvmStatic fun nativeIsValidWidgetUrlFallback(url: String): Boolean = url.startsWith("https://")

    // --- Encryption Defaults fallbacks ---
    @JvmStatic fun nativeGetDefaultEncryptionAlgorithmFallback(): String = "m.megolm.v1.aes-sha2"
    @JvmStatic fun nativeRequiresDeviceVerificationFallback(algorithm: String): Boolean =
        !algorithm.isEmpty() && algorithm != "m.olm.v1.curve25519-aes-sha2"

    // --- Timestamp fallback ---
    @JvmStatic fun nativeIsReasonableTimestampFallback(originServerTs: String, maxFutureMs: Long): Boolean {
        val ts = originServerTs.toLongOrNull() ?: return true
        return (System.currentTimeMillis() - ts) < maxFutureMs
    }

}
