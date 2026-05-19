# Progressive Chat — Native C++ JNI API Reference

## Overview
- **Language:** C++20, NDK 21.3, CMake 3.22.1
- **JNI surface:** ~400 functions across `chat.progressive.app.native.ProgressiveNative`
- **Library:** `libprogressive_native.so`
- **Modules:** 199 C++ files in `vector/src/main/cpp/`

All functions have Kotlin `external fun` declarations and `*Fallback` implementations in `ProgressiveNative.kt`.

---

## Labs Flags (all off by default)

| Flag | Key | Getter |
|------|-----|--------|
| Native HTTP | `SETTINGS_LABS_NATIVE_HTTP_KEY` | `isNativeHttpEnabled()` |
| Native Timeline | `SETTINGS_LABS_NATIVE_TIMELINE_KEY` | `isNativeTimelineEnabled()` |
| Native DB | `SETTINGS_LABS_NATIVE_DB_KEY` | `isNativeDbEnabled()` |
| Native Sync Parser | `SETTINGS_LABS_NATIVE_SYNC_PARSER_KEY` | `isNativeSyncParserEnabled()` |
| Native Crypto | `SETTINGS_LABS_NATIVE_CRYPTO_KEY` | `isNativeCryptoEnabled()` |
| Native Markdown | `SETTINGS_LABS_NATIVE_MARKDOWN_KEY` | `isNativeMarkdownEnabled()` |

---

## API Reference

### Matrix API (25 endpoints)
| Function | Signature | Returns |
|----------|-----------|---------|
| `nativeApiLogin` | `(userId, password, deviceId)` | Credentials JSON |
| `nativeApiSync` | `(filter, since, timeout)` | SyncResponse summary |
| `nativeApiSendEvent` | `(roomId, eventType, txnId, contentJson)` | Response body |
| `nativeApiJoinRoom` | `(roomId, reason)` | Response body |
| `nativeApiLeaveRoom` | `(roomId)` | Response body |
| `nativeApiGetProfile` | `(userId)` | Profile JSON |
| `nativeApiWhoAmI` | `()` | User info JSON |
| `nativeApiLogout` | `()` | Boolean |
| `nativeApiGetRoomMembers` | `(roomId)` | Members JSON |
| `nativeApiInviteUser` | `(roomId, userId, reason)` | Response body |
| `nativeApiGetRoomMessages` | `(roomId, from, dir, limit)` | Messages JSON |
| `nativeApiCreateRoom` | `(name, topic, isDirect, invitees)` | Response body |
| `nativeApiSearch` | `(query, roomId, limit)` | Results JSON |
| `nativeApiKickUser` | `(roomId, userId, reason)` | Response body |
| `nativeApiBanUser` | `(roomId, userId, reason)` | Response body |
| `nativeApiUnbanUser` | `(roomId, userId)` | Response body |
| `nativeApiRedactEvent` | `(roomId, eventId, txnId)` | Response body |
| `nativeApiGetPushRules` | `()` | Rules JSON |
| `nativeApiCreateFilter` | `(userId, filterJson)` | Filter ID |
| `nativeApiGetDisplayName` | `(userId)` | Display name |
| `nativeApiSetDisplayName` | `(userId, displayName)` | Response body |
| `nativeApiGetVersions` | `()` | Versions JSON |
| `nativeApiLogoutAll` | `()` | Boolean |
| `nativeApiPublicRooms` | `(server, query, limit)` | Rooms JSON |
| `nativeSetHomeserverUrl` | `(url)` | void |
| `nativeSetAccessToken` | `(token)` | void |
| `nativeApiAvailable` | `()` | Boolean |

### Sync Parser
| Function | Signature | Description |
|----------|-----------|-------------|
| `nativeParseSyncResponse` | `(json)` | Full sync summary |
| `nativeGetNextBatch` | `(json)` | Extract next_batch token |
| `nativeParseSyncRoomsJson` | `(json)` | Room list from sync |
| `nativeParseEvent` | `(json)` | Single event parse |
| `nativeParseTimeline` | `(json)` | Timeline section parse |
| `nativeCountEventsInSync` | `(json)` | Total event count |
| `nativeSyncResponseRoundtrip` | `(json)` | Parse→serialize validation |
| `nativeExtractNextBatchLight` | `(partialJson)` | Fast 64KB next_batch |

### SQLite Database (SqliteDB)
| Function | Signature | Description |
|----------|-----------|-------------|
| `nativeSqliteDbOpen` | `(dbPath, key)` | Open/create database |
| `nativeSqliteDbClose` | `(key)` | Close database |
| `nativeSqliteDbInsertEvent` | `(key, eventId, roomId, type, senderId, contentJson, originTs, ageTs, displayIndex)` | Insert event |
| `nativeSqliteDbInsertEventRel` | `(+ stateKey, redacts, relType, relatesToId)` | Insert with relations |
| `nativeSqliteDbQueryEvents` | `(key, roomId, limit, offset, ascending)` | Query events |
| `nativeSqliteDbQueryEvent` | `(key, eventId)` | Find one event |
| `nativeSqliteDbDeleteEvent` | `(key, eventId)` | Delete event |
| `nativeSqliteDbCountEvents` | `(key, roomId)` | Count events |
| `nativeSqliteDbMaxDisplayIndex` | `(key, roomId)` | Max display index |
| `nativeSqliteDbUpsertRoom` | `(key, roomId, name, avatar, topic, membership, notifCount, highlightCount, lastActivityMs, isDirect, isSpace, isFav, isEncrypted)` | Upsert room summary |
| `nativeSqliteDbQueryRooms` | `(key)` | All rooms JSON |
| `nativeSqliteDbBeginTransaction` | `(key)` | Begin TX |
| `nativeSqliteDbCommitTransaction` | `(key)` | Commit TX |
| `nativeSqliteDbSchemaVersion` | `(key)` | Schema version |

### Timeline Engine
| Function | Signature | Description |
|----------|-----------|-------------|
| `nativeTimelineAddEvents` | `(roomId, eventsJson, prevToken, nextToken, dir)` | Add paginated chunk |
| `nativeTimelineGetEvents` | `(roomId)` | All events in order |
| `nativeTimelineGetEvent` | `(eventId)` | Find event |
| `nativeTimelineClear` | `(roomId)` | Clear timeline |
| `nativeTimelineGetReplies` | `(eventId)` | Reply list |
| `nativeTimelineGetLatestEdit` | `(eventId)` | Latest edit |
| `nativeTimelineGetThreadEvents` | `(rootEventId)` | Thread events |
| `nativeTimelineChunkCount` | `(roomId)` | Chunk count |
| `nativeTimelineGetSnapshot` | `(roomId, limit, offset)` | Paginated snapshot |
| `nativeTimelineEventsAvailable` | `(roomId, direction)` | Events available |
| `nativeTimelineAttachDb` | `(roomId, dbKey)` | Attach SQLite |
| `nativeTimelineAddSyncEvent` | `(roomId, eventId, type, senderId, contentJson, originTs, displayIndex, stateKey, redacts, relType, relatesToId)` | Store /sync event |

### OIDC / MAS Authentication
| Function | Signature |
|----------|-----------|
| `nativeDiscoverOidc` | `(homeserverUrl)` |
| `nativeBuildOAuthUrl` | `(clientId, redirectUri, state, codeChallenge, prompt)` |
| `nativeExchangeOidcCode` | `(tokenEndpoint, clientId, redirectUri, code, codeVerifier)` |
| `nativeParseOAuthCallback` | `(url, redirectUri)` |
| `nativeGenerateOAuthState` | `()` |
| `nativeGeneratePkce` | `()` |

### E2EE Crypto

#### Olm Account
| Function | Signature | Description |
|----------|-----------|-------------|
| `nativeOlmCreateAccount` | `(userId, deviceId)` | Create device identity |
| `nativeOlmGetIdentityKeys` | `()` | Ed25519 + Curve25519 keys |
| `nativeOlmGenerateOneTimeKeys` | `(count)` | Generate OTKs JSON |
| `nativeOlmSignMessage` | `(message)` | Ed25519 signature |
| `nativeOlmCreateInboundSession` | `(theirIdentityKey, preKeyMessage)` | Inbound Olm session |
| `nativeOlmDecryptMessage` | `(senderKey, sessionId, ciphertext)` | Decrypt Olm message |
| `nativeOlmPickleAccount` | `()` | Serialize account |
| `nativeOlmUnpickleAccount` | `(pickled, userId, deviceId)` | Restore account |

#### Megolm Decryptor
| Function | Signature | Description |
|----------|-----------|-------------|
| `nativeMegolmAddSession` | `(roomId, senderKey, sessionId, sessionKeyBase64)` | Import room key |
| `nativeMegolmDecrypt` | `(roomId, senderKey, sessionId, ciphertext)` | Decrypt Megolm message |
| `nativeMegolmSessionCount` | `()` | Session count |
| `nativeMegolmClearRoom` | `(roomId)` | Clear room sessions |

#### SAS Verification
| Function | Signature | Description |
|----------|-----------|-------------|
| `nativeSasCreate` | `()` | Start SAS, return pubkey |
| `nativeSasSetTheirKey` | `(theirPubkey)` | Set other party key |
| `nativeSasGetEmojis` | `()` | 6 emojis JSON |
| `nativeSasCalculateMac` | `(input, info)` | Compute MAC |
| `nativeSasVerifyMac` | `(theirMac, input, info)` | Verify MAC |
| `nativeSasDestroy` | `()` | Clean up |

#### Device Verification
| Function | Signature | Description |
|----------|-----------|-------------|
| `nativeVerifyDeviceSignature` | `(deviceKeysJson, userId, deviceId, signKeyB64, signatureB64)` | Ed25519 verify |
| `nativeComputeDeviceFingerprint` | `(identityKeyBase64)` | 8-word fingerprint |

### Event Relations
| Function | Signature | Description |
|----------|-----------|-------------|
| `nativeIsReply` | `(contentJson)` | Check for m.in_reply_to |
| `nativeIsEdit` | `(contentJson)` | Check for m.replace |
| `nativeIsReaction` | `(contentJson)` | Check for m.annotation |
| `nativeIsThreadRoot` | `(contentJson)` | Check for m.thread |
| `nativeExtractThreadRoot` | `(contentJson)` | Extract root event ID |
| `nativeExtractReplySource` | `(contentJson)` | Extract replied-to ID |
| `nativeExtractEditSource` | `(contentJson)` | Extract replaced event ID |
| `nativeBuildReplyRelationWithThread` | `(eventId, threadRoot)` | Build relation JSON |
| `nativeParseEventRelation` | `(contentJson)` | Full relation parse |
| `nativeFormatRelationDescription` | `(relType, eventId, key)` | Human-readable description |
| `nativeComputeThreadSummary` | `(rootEventId, eventsJson)` | Thread summary |
| `nativeBuildThreadListJson` | `(eventsJson)` | Thread list from events |

### Event Renderer (Notices)
| Function | Signature |
|----------|-----------|
| `nativeFormatMemberNotice` | `(membership, prevMembership, senderId, senderName, targetId, targetName, reason, isDirect, sentBySelf)` |
| `nativeFormatCallNotice` | `(eventType, isVideo, senderName, sentBySelf)` |
| `nativeAnnotateEdited` | `(body, isEdited)` |
| `nativeFormatRoomNameNotice` | `(senderName, newName, sentBySelf)` |
| `nativeFormatRoomTopicNotice` | `(senderName, newTopic, sentBySelf)` |
| `nativeFormatRoomAvatarNotice` | `(senderName, isRemoved, sentBySelf)` |
| `nativeFormatRoomCreateNotice` | `(senderName, predecessorRoomId, isDirect, sentBySelf)` |
| `nativeFormatRoomTombstoneNotice` | `(senderName, replacementRoom, sentBySelf)` |
| `nativeFormatRoomEncryptionNotice` | `(senderName, isEnabled, sentBySelf)` |

### Room State
| Function | Signature |
|----------|-----------|
| `nativeIsPublicRoom` | `(stateContentJson)` |
| `nativeIsInviteOnly` | `(stateContentJson)` |
| `nativeJoinRuleToString` | `(stateContentJson)` |
| `nativeIsHistoryPubliclyVisible` | `(stateContentJson)` |
| `nativeHistoryVisibilityToString` | `(stateContentJson)` |
| `nativeAreGuestsAllowed` | `(stateContentJson)` |
| `nativeIsRoomUpgraded` | `(stateContentJson)` |
| `nativeParseJoinRules` | `(contentJson)` |
| `nativeParseHistoryVisibility` | `(contentJson)` |
| `nativeParseGuestAccess` | `(contentJson)` |
| `nativeParseRoomNameContent` | `(contentJson)` |
| `nativeParseRoomTopicContent` | `(contentJson)` |
| `nativeParseRoomAvatarContent` | `(contentJson)` |
| `nativeParseTombstone` | `(contentJson)` |
| `nativeComputePermissions` | `(powerLevelsJson, myUserId)` |

### Room List / Space / Widget
| Function | Signature |
|----------|-----------|
| `nativeParseSpaceChildren` | `(stateEventsJson)` |
| `nativeBuildSpaceChildContent` | `(suggested, order, autoJoin, canonical)` |
| `nativeBuildSpaceParentContent` | `(parentSpaceId, canonical)` |
| `nativeListRoomWidgets` | `(stateEventsJson)` |
| `nativeParseWidgetStateContent` | `(stateContentJson, widgetId, roomId)` |
| `nativeIsJitsiWidget` | `(type)` |
| `nativeIsEtherpadWidget` | `(type)` |
| `nativeIsValidWidgetUrl` | `(url)` |
| `nativeGetWidgetTypeName` | `(type)` |

### Media Utilities
| Function | Signature |
|----------|-----------|
| `nativeFormatFileSize` | `(bytes)` |
| `nativeMimeToMsgType` | `(mimeType)` |
| `nativeIsMxcUri` | `(url)` |
| `nativeExtractMxcServerName` | `(mxcUrl)` |
| `nativeExtractMxcMediaId` | `(mxcUrl)` |
| `nativeBuildMxcUri` | `(serverName, mediaId)` |
| `nativeResolveMxcDownloadUrl` | `(mxcUrl, homeServerUrl)` |
| `nativeResolveMxcThumbnailUrl` | `(mxcUrl, homeServerUrl, width, height)` |
| `nativeNormalizeMimeType` | `(mimeType)` |
| `nativeGetExtensionFromMimeType` | `(mimetype)` |
| `nativeCalculateThumbnailSize` | `(origW, origH, maxW, maxH)` |
| `nativeHasTextWithImage` | `(contentJson)` |

### Chunked Uploader
| Function | Signature |
|----------|-----------|
| `nativeUploaderSetChunkSizeMb` | `(mb)` |
| `nativeUploaderComputeChunks` | `(fileSize)` |
| `nativeUploaderGetChunkInfo` | `(index)` |
| `nativeUploaderContentRange` | `(index)` |
| `nativeUploaderAdvance` | `()` |
| `nativeUploaderCancel` | `()` |
| `nativeUploaderReset` | `()` |
| `nativeUploaderProgress` | `()` |
| `nativeSuggestChunkSizeMb` | `(fileSize)` |

### Matrix ID / Pattern Validators
| Function | Signature |
|----------|-----------|
| `nativeIsUserId` | `(input)` |
| `nativeIsRoomId` | `(input)` |
| `nativeIsRoomAlias` | `(input)` |
| `nativeIsEventId` | `(input)` |
| `nativeIsMxcUrl` | `(url)` |
| `nativeIsValidEmail` | `(input)` |
| `nativeIsPhoneNumber` | `(input)` |
| `nativeIsGroupId` | `(input)` |
| `nativeExtractServerNameFromId` | `(mxid)` |
| `nativeExtractUserNameFromId` | `(mxid)` |
| `nativeCandidateAliasFromRoomName` | `(roomName, domain, maxLength)` |
| `nativeExtractMatrixIds` | `(text)` |

### Notification / Push
| Function | Signature |
|----------|-----------|
| `nativeFormatBadgeText` | `(totalCount)` |
| `nativeFormatCombinedNotificationCount` | `(roomCount, threadCount)` |
| `nativeGetTotalUnreadCount` | `(roomCount, threadCount)` |
| `nativeIsNotifModeDifferent` | `(oldMode, newMode)` |
| `nativeGetDefaultModeForRoom` | `(isDirect, isEncrypted)` |
| `nativeBuildRoomNotifSettingsBody` | `(mode)` |
| `nativeFormatNotifMode` | `(mode)` |
| `nativeParseNotifMode` | `(action)` |
| `nativeParseUnifiedPushMessage` | `(json)` |

### Password Validator
| Function | Signature |
|----------|-----------|
| `nativeValidatePassword` | `(password)` |
| `nativeComputePasswordStrength` | `(password)` |
| `nativeMeetsMinimumRequirements` | `(password)` |
| `nativeCountCharClasses` | `(password)` |
| `nativeIsCommonPassword` | `(password)` |
| `nativeGetStrengthLabel` | `(strength)` |
| `nativeGeneratePasswordFeedback` | `(password)` |

### Display Names / Identity
| Function | Signature |
|----------|-----------|
| `nativeUserIdToDisplayName` | `(userId, capitalize)` |
| `nativeGetInitials` | `(name, maxChars)` |
| `nativeGetBestDisplayName` | `(displayName, userId)` |
| `nativeFormatMemberName` | `(displayName, userId, powerLevel, showBadge)` |
| `nativeDisambiguateName` | `(displayName, mxid)` |
| `nativeGetIdentityInitials` | `(displayName)` |
| `nativeIsEmail` | `(input)` |
| `nativeIsMsisdn` | `(input)` |
| `nativeExtractAliasLocalpart` | `(alias)` |
| `nativeIsCanonicalAlias` | `(alias, expectedRoomId)` |
| `nativeSuggestAliases` | `(roomName)` |

### Permalink
| Function | Signature |
|----------|-----------|
| `nativeBuildEventPermalink` | `(roomId, eventId)` |
| `nativeBuildRoomPermalink` | `(roomId)` |
| `nativeBuildUserPermalink` | `(userId)` |
| `nativeExtractRoomIdFromPermalink` | `(url)` |
| `nativeExtractEventIdFromPermalink` | `(url)` |
| `nativeExtractUserIdFromPermalink` | `(url)` |
| `nativeIsSameRoomPermalink` | `(url1, url2)` |
| `nativeIsMatrixToPermalink` | `(url)` |
| `nativeIsAppPermalink` | `(url)` |
| `nativeParseMatrixToPermalink` | `(url)` |

### Markdown Renderer
| Function | Signature |
|----------|-----------|
| `nativeMarkdownToHtml` | `(markdown, enableTables, enableLinks, enableCode, enableScroll)` |
| `nativeParseMarkdownTable` | `(tableBlock, withScroll)` |

### Misc Utilities
| Category | Functions |
|----------|-----------|
| Date/Time | `nativeFormatDuration`, `nativeFormatTimeAgoLabel` |
| Presence | `nativeFormatPresence`, `nativeParsePresence`, `nativeGetPresenceIndicator`, `nativeIsPresenceStale`, `nativeGetPresenceStatusText`, `nativeFormatStatusMessage` |
| Device | `nativeBuildDeviceDisplayName`, `nativeGenerateDeviceName`, `nativeFormatDeviceLastSeen`, `nativeIsDeviceInactive`, `nativeFormatFingerprint`, `nativeGetBannerColor`, `nativeFormatDowntime` |
| Content | `nativeExtractUsefulTextFromReply`, `nativeFormatSpoilerTextFromHtml`, `nativeStripHtmlTags`, `nativeTruncateDescription`, `nativeGetLatestEditEventId`, `nativeGetEditedTargetEventId` |
| Key Backup | `nativeFormatRecoveryKey`, `nativeValidateRecoveryKey`, `nativeComputeRecoveryKey`, `nativeExtractCurveKeyFromRecoveryKey`, `nativeGetRecoveryKeyExample`, `nativeIsValidPassphrase`, `nativeGetMinPassphraseLength`, `nativeGetBackupAlgorithmDescription`, `nativeIsSupportedBackupAlgorithm`, `nativeParseKeyBackupVersion` |
| Report | `nativeIsValidReportReason`, `nativeGetReasonDescription`, `nativeIsOffensive`, `nativeTruncateReportDescription` |
| SSO | `nativeIsSsoCallbackUrl`, `nativeExtractSsoProvider`, `nativeBuildSsoLoginUrl`, `nativeGetSsoProviderBrand` |
| Login | `nativeIsValidLoginCredentials`, `nativeGenerateDeviceId`, `nativeParseLoginFlowsList`, `nativeBuildUserIdentifier` |
| Read Marker | `nativeAdvanceReadMarker`, `nativeShouldShowJumpToUnread`, `nativeFormatUnreadJumpLabel`, `nativeReadMarkerToJson` |
| Session | `nativeBuildSessionRenameBody` |
| Secret Storage | `nativeExtractDefaultSecretKey`, `nativeHasCrossSigningSecrets` |
| Version | `nativeCompareSemver`, `nativeSatisfiesMinVersion`, `nativeParseServerVersion`, `nativeIsServerCompatible`, `nativeParseFederationVersion` |
| Poll | `nativeIsPollEnded`, `nativeIsValidPollQuestion`, `nativeGeneratePollOptionId` |
| Invite | `nativeIsInviteExpired`, `nativeBuildKnockBody`, `nativeFormatKnockReason`, `nativeBuildInviteBody` |
| Push Rules | `nativeIsKnownPushRuleKind`, `nativeGetRuleKindDescription`, `nativeIsMsc3061SharedKey`, `nativeFormatMsc3061Status`, `nativeCanShareHistory` |
| Content Scanner | `nativeIsServerNotice`, `nativeMustAcceptTos`, `nativeBuildTosAcceptBody`, `nativeIsContentScannerAvailable`, `nativeParseScanResult`, `nativeParseServerNotice`, `nativeBuildScanRequestBody` |
| Canonical JSON | `nativeCanonicalizeJson` |
| Edit History | `nativeFormatEditSummary`, `nativeGetEditBadgeText` |
| Cross-Signing | `nativeNeedsCrossSigningSetup`, `nativeFormatCrossSigningStatus`, `nativeParseCrossSigningStatus` |
| E2EE Decoration | `nativeGetE2eeIconName`, `nativeGetE2eeColor`, `nativeGetTrustLabel` |
| Backup | `nativeBuildCreateBackupBody`, `nativeFormatBackupStats`, `nativeNeedsBackupAttention`, `nativeParseBackupInfo` |
| User Status | `nativeBuildUserStatusJson`, `nativeGetStatusSuggestions` |
| Calls | `nativeIsCallExpired`, `nativeFormatCallDuration`, `nativeBuildCallInviteContent`, `nativeBuildCallAnswerContent`, `nativeBuildCallHangupContent`, `nativeGetCallState` |
| Matrix Error | `nativeIsPasswordError`, `nativeGetAllErrorCodes`, `nativeGetRetryAfterMs`, `nativeParseMatrixErrorJson` |
| URL Preview | `nativeIsPreviewableUrl`, `nativeIsImageUrl`, `nativeExtractUrls` |
| Content Guard | `nativeCountEmojis`, `nativeCountUniqueEmojis`, `nativeFormatMediaCollapseLabel`, `nativeIsEmojiCodePoint` |
| Room Counter | `nativeCountRooms` |
| Room Uploads | `nativeIsStickerEvent`, `nativeHasAttachmentUrl`, `nativeCreateUploadsFilterJson` |
| Public Rooms | `nativeParsePublicRoom`, `nativeParsePublicRoomsResponse` |
| Member List | `nativeParseMemberList` |
| Event Content | `nativeParseEventContent` |
| Encrypted File | `nativeParseEncryptedFileKey` |
| OpenID Token | `nativeParseOpenIdToken` |

---

## Internals

### JNI Bridge (`jni_bridge.cpp`)
- **Lines:** ~3400
- **JNI functions:** ~400
- **Singleton state:** `g_eventDb`, `g_sqliteDbs`, `g_olmAccount`, `g_olmSessionMgr`, `g_megolmManager`, `g_sas`
- **Macro:** `JNI_FUNC(ret, name)` → `Java_chat_progressive_app_native_ProgressiveNative_##name`

### C++ Module Count
- **199 .cpp files** in `src/`
- **~200 .hpp headers** in `include/progressive/`

### Tests
- **85 tests** covering 38 modules
- Framework: `tests/test_framework.hpp`
- Executable: `test_progressive` (CMake)
