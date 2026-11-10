package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

data class TelegramFullUser(
    val id: Long,
    val firstName: String,
    val lastName: String,
    val username: String,
    val phoneNumber: String,
    val bio: String?,
    val profilePhoto: TelegramProfilePhoto?,
    val emojiStatus: TelegramEmojiStatus?,
    val premiumStatus: TelegramPremiumStatus?,
    val isVerified: Boolean,
    val isSupport: Boolean,
    val isBot: Boolean,
    val isScam: Boolean,
    val isFake: Boolean,
    val isContact: Boolean,
    val isMutualContact: Boolean,
    val isCloseFriend: Boolean,
    val restrictionReason: String?,
    val haveAccess: Boolean,
    val type: String,
    val languageCode: String,
    val addedToAttachmentMenu: Boolean
)

data class TelegramProfilePhoto(
    val id: Long,
    val isPersonal: Boolean,
    val hasAnimation: Boolean,
    val addedDate: Long,
    val sizes: List<TelegramPhoto.PhotoSize>,
    val minithumbnail: ByteArray?
)

data class TelegramProfilePhotos(
    val totalCount: Int,
    val photos: List<TelegramProfilePhoto>
)

data class TelegramEmojiStatus(
    val customEmojiId: Long,
    val expirationDate: Long
)

data class TelegramPremiumStatus(
    val isPremium: Boolean,
    val giftedById: Long,
    val expiresIn: Long,
    val features: List<PremiumFeature>,
    val subscriptionType: String,
    val canUpgrade: Boolean
)

data class PremiumFeature(
    val name: String,
    val description: String,
    val isFeatured: Boolean
)

data class TelegramSessionInfo(
    val id: Long,
    val isCurrent: Boolean,
    val isPasswordPending: Boolean,
    val apiId: Int,
    val applicationName: String,
    val applicationVersion: String,
    val isOfficialApplication: Boolean,
    val deviceModel: String,
    val platform: String,
    val systemVersion: String,
    val logInDate: Long,
    val lastActiveDate: Long,
    val ip: String,
    val country: String,
    val region: String
)

data class TelegramSessions(
    val sessions: List<TelegramSessionInfo>,
    val ttlDays: Int
)

data class TelegramConnectedWebsite(
    val id: Long,
    val domainName: String,
    val botUserId: Long,
    val browser: String,
    val platform: String,
    val logInDate: Long,
    val lastActiveDate: Long,
    val ip: String,
    val location: String
)

data class TelegramConnectedWebsites(
    val websites: List<TelegramConnectedWebsite>
)

data class TelegramWallpaper(
    val id: Long,
    val isDefault: Boolean,
    val isDark: Boolean,
    val isBlurred: Boolean,
    val isPattern: Boolean,
    val isMoving: Boolean,
    val settings: TelegramWallpaperSettings?,
    val color: Int,
    val tintColor: Int,
    val gradients: List<Int>,
    val document: TelegramDocument?,
    val creatorUserId: Long
)

data class TelegramWallpaperSettings(
    val isBlurred: Boolean,
    val isMoving: Boolean,
    val backgroundColor: Int,
    val secondBackgroundColor: Int,
    val thirdBackgroundColor: Int,
    val fourthBackgroundColor: Int,
    val intensity: Int,
    val rotation: Int
)

data class TelegramChatBackground(
    val backgrounds: List<TelegramWallpaper>,
    val selectedId: Long
)

data class TelegramChatThemeInfo(
    val name: String,
    val emoji: String,
    val id: Long,
    val isDefault: Boolean,
    val isDark: Boolean
)

data class TelegramNotificationSettings(
    val useDefaultMuteFor: Boolean,
    val muteFor: Int,
    val useDefaultSound: Boolean,
    val soundId: Long,
    val useDefaultShowPreview: Boolean,
    val showPreview: Boolean,
    val useDefaultDisablePinnedMessageNotifications: Boolean,
    val disablePinnedMessageNotifications: Boolean,
    val useDefaultDisableMentionNotifications: Boolean,
    val disableMentionNotifications: Boolean
)

data class TelegramChatPosition(
    val list: String,
    val order: Long,
    val isPinned: Boolean,
    val source: String?
)

data class TelegramPermissions(
    val canSendBasicMessages: Boolean,
    val canSendAudios: Boolean,
    val canSendDocuments: Boolean,
    val canSendPhotos: Boolean,
    val canSendVideos: Boolean,
    val canSendVideoNotes: Boolean,
    val canSendVoiceNotes: Boolean,
    val canSendPolls: Boolean,
    val canSendOtherMessages: Boolean,
    val canAddWebPagePreviews: Boolean,
    val canChangeInfo: Boolean,
    val canInviteUsers: Boolean,
    val canPinMessages: Boolean,
    val canManageTopics: Boolean
)

data class TelegramChatMember(
    val userId: Long,
    val inviterUserId: Long,
    val joinedDate: Long,
    val status: String,
    val isAnonymous: Boolean,
    val canBeEdited: Boolean,
    val permissions: TelegramPermissions?,
    val restrictedUntilDate: Long,
    val bannedUntilDate: Long,
    val memberCount: Int
)

data class TelegramChatAdministrators(
    val administrators: List<TelegramChatMember>
)

data class TelegramSharedMedia(
    val totalCount: Int,
    val messages: List<TelegramRichMessage>,
    val nextOffset: String
)
