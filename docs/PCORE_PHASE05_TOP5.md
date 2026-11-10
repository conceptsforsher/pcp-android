# Phase 0.5 — Conversion spec for the top-5 room-seam files

Rule: every SDK call below becomes a suspend call on `PcoreBridge`
(see port-pcore). The ViewModel keeps NO SDK import after its phase.

| # | File | score | sdk | getRoom | roomService |
|---|---|---|---|---|---|
| 1 | `chat/pcp/app/features/home/room/detail/TimelineViewModel.kt` | 75 | 45 | 2 | 12 |
| 2 | `chat/pcp/app/features/home/room/detail/composer/MessageComposerViewModel.kt` | 45 | 27 | 2 | 6 |
| 3 | `chat/pcp/app/features/home/HomeDetailViewModel.kt` | 43 | 11 | 0 | 16 |
| 4 | `chat/pcp/app/features/home/room/list/home/HomeRoomListViewModel.kt` | 42 | 20 | 2 | 8 |
| 5 | `chat/pcp/app/features/home/room/detail/timeline/factory/MessageItemFactory.kt` | 41 | 30 | 1 | 4 |

## Per-file call inventory

### 1. chat/pcp/app/features/home/room/detail/TimelineViewModel.kt

- L80: import …`MatrixPatterns`
- L81: import …`isMxcUrl`
- L82: import …`orFalse`
- L83: import …`tryOrNull`
- L84: import …`QueryStringValue`
- L85: import …`RawService`
- L86: import …`Session`
- L87: import …`MXCryptoError`
- L88: import …`EVerificationState`
- L89: import …`EventType`
- L90: import …`RelationType`
- L91: import …`WithHeldCode`
- L92: import …`isAttachmentMessage`
- L93: import …`isTextMessage`
- L94: import …`toContent`
- L95: import …`toModel`
- L96: import …`FileService`
- L97: import …`getRoom`
- L98: import …`Room`
- L99: import …`getStateEvent`
- L100: import …`getTimelineEvent`
- L101: import …`UpdateLiveLocationShareResult`
- L102: import …`ChangeMembershipState`
- L103: import …`roomMemberQueryParams`
- L104: import …`LocalRoomCreationState`
- L105: import …`Membership`
- L106: import …`RoomMemberSummary`
- L107: import …`RoomSummary`
- L108: import …`RoomLocalEcho`
- L109: import …`MessageContent`
- L110: import …`MessageWithAttachmentContent`
- L111: import …`getFileUrl`
- L112: import …`RelationDefaultContent`
- L113: import …`RoomTombstoneContent`
- L114: import …`ReadService`
- L115: import …`Timeline`
- L116: import …`TimelineEvent`
- L117: import …`isLiveLocation`
- L118: import …`SyncRequestState`
- L119: import …`ThreadNotificationBadgeState`
- L120: import …`ThreadNotificationState`
- L121: import …`WidgetType`
- L122: import …`toOptional`
- L123: import …`flow`
- L124: import …`unwrap`
- L158: `private val room = session.getRoom(initialState.roomId)`
- L232: `tryOrNull { session.roomService().onRoomDisplayed(initialState.roomId) }`
- L797: `session.roomService().getRoomSummary(roomId)`
- L817: `val isRoomJoined = session.getRoom(roomId)?.roomSummary()?.membership == Membership.JOIN`
- L832: `session.roomService().joinRoom(roomId, viaServers = viaServers)`
- L903: `room.sendService().redactEvent(event.root, action.reason, listOf(RelationType.REFERENCE))`
- L906: `room.sendService().redactEvent(event.root, action.reason)`
- L986: `session.roomService().leaveRoom(initialState.roomId)`
- L997: `session.roomService().joinRoom(initialState.roomId)`
- L1469: `session.roomService().getRoomMember(inviterId, summary.roomId)`

### 2. chat/pcp/app/features/home/room/detail/composer/MessageComposerViewModel.kt

- L55: import …`QueryStringValue`
- L56: import …`Session`
- L57: import …`ContentAttachmentData`
- L58: import …`EventType`
- L59: import …`getRootThreadEventId`
- L60: import …`isThread`
- L61: import …`toContent`
- L62: import …`toModel`
- L63: import …`getRoom`
- L64: import …`getRoomSummary`
- L65: import …`Room`
- L66: import …`getStateEvent`
- L67: import …`getTimelineEvent`
- L68: import …`PowerLevelsContent`
- L69: import …`RoomAvatarContent`
- L70: import …`RoomEncryptionAlgorithm`
- L71: import …`RoomMemberContent`
- L72: import …`MessageContentWithFormattedBody`
- L73: import …`MessageType`
- L74: import …`shouldRenderInThread`
- L75: import …`UserDraft`
- L76: import …`getRelationContent`
- L77: import …`getTextEditableContent`
- L78: import …`CreateSpaceParams`
- L79: import …`Optional`
- L80: import …`flow`
- L81: import …`unwrap`
- L98: `private val room = session.getRoom(initialState.roomId)`
- L258: `room.sendService().sendTextMessage(action.text, autoMarkdown = action.autoMarkdown)`
- L286: `room.sendService().sendTextMessage(parsedCommand.message, autoMarkdown = false)`
- L365: `room.sendService().sendTextMessage(`
- L534: `session.roomService().leaveRoom(parsedCommand.roomId)`
- L706: `room.sendService().sendTextMessage(defaultMessage, MessageType.MSGTYPE_EMOTE)`
- L708: `room.sendService().sendTextMessage(sendChatEffect.message, sendChatEffect.chatEffect.toMessageType()`
- L715: `session.roomService().joinRoom(command.roomAlias, command.reason, emptyList())`
- L794: `?.let { session.getRoom(it) }`
- L797: `session.roomService().leaveRoom(it.roomId)`
- L897: `} ?: room.sendService().sendTextMessage(sequence)`

### 3. chat/pcp/app/features/home/HomeDetailViewModel.kt

- L38: import …`RoomCategoryFilter`
- L39: import …`toActiveSpaceOrNoFilter`
- L40: import …`toActiveSpaceOrOrphanRooms`
- L41: import …`Session`
- L42: import …`NewSessionListener`
- L43: import …`RoomSortOrder`
- L44: import …`Membership`
- L45: import …`roomSummaryQueryParams`
- L46: import …`SyncRequestState`
- L47: import …`toMatrixItem`
- L48: import …`flow`
- L85: `session.roomService().refreshJoinedRoomSummaryPreviews(roomId)`
- L186: `val roomIds = session.roomService().getRoomSummaries(`
- L194: `session.roomService().markAllAsRead(roomIds)`
- L228: `session.roomService().getPagedRoomSummariesLive(`
- L241: `dmInvites = session.roomService().getRoomSummaries(`
- L249: `roomsInvite = session.roomService().getRoomSummaries(`
- L258: `val dmRooms = session.roomService().getNotificationCountForRooms(`
- L266: `val otherRooms = session.roomService().getNotificationCountForRooms(`

### 4. chat/pcp/app/features/home/room/list/home/HomeRoomListViewModel.kt

- L42: import …`orFalse`
- L43: import …`QueryStringValue`
- L44: import …`RoomCategoryFilter`
- L45: import …`RoomTagQueryFilter`
- L46: import …`toActiveSpaceOrNoFilter`
- L47: import …`Session`
- L48: import …`getRoom`
- L49: import …`getUserOrDefault`
- L50: import …`RoomSortOrder`
- L51: import …`RoomSummaryQueryParams`
- L52: import …`UpdatableLivePageResult`
- L53: import …`Membership`
- L54: import …`RoomSummary`
- L55: import …`RoomLocalEcho`
- L56: import …`RoomTag`
- L57: import …`roomSummaryQueryParams`
- L58: import …`Optional`
- L59: import …`toMatrixItem`
- L60: import …`toOption`
- L61: import …`flow`
- L94: `session.roomService().getFilteredPagedRoomSummariesLive(`
- L346: `val value = runCatching { session.roomService().leaveRoom(action.roomId) }`
- L354: `val room = session.getRoom(action.roomId)`
- L367: `session.getRoom(action.roomId)?.let { room ->`
- L391: `val localRoomIds = session.roomService()`
- L396: `session.roomService().deleteLocalRoom(it)`

### 5. chat/pcp/app/features/home/room/detail/timeline/factory/MessageItemFactory.kt

- L79: import …`isMxcUrl`
- L80: import …`Session`
- L81: import …`toElementToDecrypt`
- L82: import …`RelationType`
- L83: import …`EncryptedEventContent`
- L84: import …`isThread`
- L85: import …`toModel`
- L86: import …`getTimelineEvent`
- L87: import …`MessageAudioContent`
- L88: import …`MessageBeaconInfoContent`
- L89: import …`MessageContent`
- L90: import …`MessageContentWithFormattedBody`
- L91: import …`MessageEmoteContent`
- L92: import …`MessageEndPollContent`
- L93: import …`MessageFileContent`
- L94: import …`MessageImageInfoContent`
- L95: import …`MessageLocationContent`
- L96: import …`MessageNoticeContent`
- L97: import …`MessagePollContent`
- L98: import …`MessageTextContent`
- L99: import …`MessageType`
- L100: import …`MessageVerificationRequestContent`
- L101: import …`MessageVideoContent`
- L102: import …`asMessageAudioEvent`
- L103: import …`getFileUrl`
- L104: import …`getThumbnailUrl`
- L105: import …`ReplyToContent`
- L106: import …`getRelationContent`
- L107: import …`LightweightSettingsStorage`
- L108: import …`MimeTypes`
- L277: `session.roomService().getRoom(roomId)?.getTimelineEvent(pollStartEventId)`
- L427: `session.roomService().getRoomMember(messageContent.toUserId, roomId)?.displayName`
