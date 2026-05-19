/*
 * Copyright 2018-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */
package im.vector.app.features.settings

import android.content.Context
import android.content.SharedPreferences
import android.media.RingtoneManager
import android.net.Uri
import android.provider.MediaStore
import androidx.annotation.BoolRes
import androidx.core.content.edit
import com.squareup.seismic.ShakeDetector
import im.vector.app.core.di.DefaultPreferences
import im.vector.app.core.resources.BuildMeta
import im.vector.app.core.resources.StringProvider
import im.vector.app.features.VectorFeatures
import im.vector.app.features.home.ShortcutsHandler
import im.vector.app.features.homeserver.ServerUrlsRepository
import im.vector.app.features.themes.ThemeUtils
import im.vector.lib.core.utils.timer.Clock
import im.vector.lib.strings.CommonStrings
import org.matrix.android.sdk.api.extensions.tryOrNull
import timber.log.Timber
import javax.inject.Inject

class VectorPreferences @Inject constructor(
        private val context: Context,
        private val clock: Clock,
        private val buildMeta: BuildMeta,
        private val vectorFeatures: VectorFeatures,
        @DefaultPreferences
        private val defaultPrefs: SharedPreferences,
        private val stringProvider: StringProvider,
) {

    companion object {
        const val SETTINGS_HELP_PREFERENCE_KEY = "SETTINGS_HELP_PREFERENCE_KEY"
        const val SETTINGS_CHANGE_PASSWORD_PREFERENCE_KEY = "SETTINGS_CHANGE_PASSWORD_PREFERENCE_KEY"
        const val SETTINGS_VERSION_PREFERENCE_KEY = "SETTINGS_VERSION_PREFERENCE_KEY"
        const val SETTINGS_NATIVE_CORE_VERSION_KEY = "SETTINGS_NATIVE_CORE_VERSION_KEY"
        const val SETTINGS_SDK_VERSION_PREFERENCE_KEY = "SETTINGS_SDK_VERSION_PREFERENCE_KEY"
        const val SETTINGS_CRYPTO_VERSION_PREFERENCE_KEY = "SETTINGS_CRYPTO_VERSION_PREFERENCE_KEY"
        const val SETTINGS_DO_LEGACY_INIT_SYNC = "SETTINGS_DO_LEGACY_INIT_SYNC"
        const val SETTINGS_DO_OPTIMIZED_INIT_SYNC = "SETTINGS_DO_OPTIMIZED_INIT_SYNC"
        const val SETTINGS_LOGGED_IN_PREFERENCE_KEY = "SETTINGS_LOGGED_IN_PREFERENCE_KEY"
        const val SETTINGS_HOME_SERVER_PREFERENCE_KEY = "SETTINGS_HOME_SERVER_PREFERENCE_KEY"
        const val SETTINGS_IDENTITY_SERVER_PREFERENCE_KEY = "SETTINGS_IDENTITY_SERVER_PREFERENCE_KEY"
        const val SETTINGS_DISCOVERY_PREFERENCE_KEY = "SETTINGS_DISCOVERY_PREFERENCE_KEY"
        const val SETTINGS_EMAILS_AND_PHONE_NUMBERS_PREFERENCE_KEY = "SETTINGS_EMAILS_AND_PHONE_NUMBERS_PREFERENCE_KEY"
        const val SETTINGS_EXTERNAL_ACCOUNT_MANAGEMENT_KEY = "SETTINGS_EXTERNAL_ACCOUNT_MANAGEMENT_KEY"

        const val SETTINGS_CLEAR_CACHE_PREFERENCE_KEY = "SETTINGS_CLEAR_CACHE_PREFERENCE_KEY"
        const val SETTINGS_CLEAR_MEDIA_CACHE_PREFERENCE_KEY = "SETTINGS_CLEAR_MEDIA_CACHE_PREFERENCE_KEY"
        const val SETTINGS_USER_SETTINGS_PREFERENCE_KEY = "SETTINGS_USER_SETTINGS_PREFERENCE_KEY"
        const val SETTINGS_CONTACT_PREFERENCE_KEYS = "SETTINGS_CONTACT_PREFERENCE_KEYS"
        const val SETTINGS_NOTIFICATIONS_TARGETS_PREFERENCE_KEY = "SETTINGS_NOTIFICATIONS_TARGETS_PREFERENCE_KEY"
        const val SETTINGS_NOTIFICATIONS_TARGET_DIVIDER_PREFERENCE_KEY = "SETTINGS_NOTIFICATIONS_TARGET_DIVIDER_PREFERENCE_KEY"
        const val SETTINGS_FDROID_BACKGROUND_SYNC_MODE = "SETTINGS_FDROID_BACKGROUND_SYNC_MODE"
        const val SETTINGS_BACKGROUND_SYNC_PREFERENCE_KEY = "SETTINGS_BACKGROUND_SYNC_PREFERENCE_KEY"
        const val SETTINGS_BACKGROUND_SYNC_DIVIDER_PREFERENCE_KEY = "SETTINGS_BACKGROUND_SYNC_DIVIDER_PREFERENCE_KEY"
        const val SETTINGS_LABS_PREFERENCE_KEY = "SETTINGS_LABS_PREFERENCE_KEY"
        const val SETTINGS_LABS_NEW_APP_LAYOUT_KEY = "SETTINGS_LABS_NEW_APP_LAYOUT_KEY"
        const val SETTINGS_LABS_DEFERRED_DM_KEY = "SETTINGS_LABS_DEFERRED_DM_KEY"
        const val SETTINGS_LABS_RICH_TEXT_EDITOR_KEY = "SETTINGS_LABS_RICH_TEXT_EDITOR_KEY"
        const val SETTINGS_LABS_NEW_SESSION_MANAGER_KEY = "SETTINGS_LABS_NEW_SESSION_MANAGER_KEY"
        const val SETTINGS_LABS_CLIENT_INFO_RECORDING_KEY = "SETTINGS_LABS_CLIENT_INFO_RECORDING_KEY"
        const val SETTINGS_LABS_VOICE_BROADCAST_KEY = "SETTINGS_LABS_VOICE_BROADCAST_KEY"
        const val SETTINGS_LABS_JUMP_TO_DATE_KEY = "SETTINGS_LABS_JUMP_TO_DATE_KEY"
        const val SETTINGS_LABS_JUMP_TO_SOURCE_KEY = "SETTINGS_LABS_JUMP_TO_SOURCE_KEY"
        const val SETTINGS_LABS_JUMP_TO_SOURCE_REACTIONS = "SETTINGS_LABS_JUMP_TO_SOURCE_REACTIONS"
        const val SETTINGS_LABS_JUMP_TO_SOURCE_REPLIES = "SETTINGS_LABS_JUMP_TO_SOURCE_REPLIES"
        const val SETTINGS_LABS_JUMP_TO_SOURCE_EDITS = "SETTINGS_LABS_JUMP_TO_SOURCE_EDITS"
        const val SETTINGS_LABS_CHAT_EXPORT_KEY = "SETTINGS_LABS_CHAT_EXPORT_KEY"
        const val SETTINGS_LABS_SKIP_AVATAR_CROP_KEY = "SETTINGS_LABS_SKIP_AVATAR_CROP_KEY"
        const val SETTINGS_LABS_TRANSLATE_KEY = "SETTINGS_LABS_TRANSLATE_KEY"
        const val SETTINGS_TRANSLATE_API_ENDPOINT = "SETTINGS_TRANSLATE_API_ENDPOINT"
        const val SETTINGS_TRANSLATE_API_TOKEN = "SETTINGS_TRANSLATE_API_TOKEN"
        const val SETTINGS_TRANSLATE_TARGET_LANGUAGE = "SETTINGS_TRANSLATE_TARGET_LANGUAGE"
        const val SETTINGS_LABS_TOR_KEY = "SETTINGS_LABS_TOR_KEY"
        const val SETTINGS_LABS_I2P_KEY = "SETTINGS_LABS_I2P_KEY"
        const val SETTINGS_LABS_YGGDRASIL_KEY = "SETTINGS_LABS_YGGDRASIL_KEY"
        const val SETTINGS_LABS_ACCOUNT_EXPORT_KEY = "SETTINGS_LABS_ACCOUNT_EXPORT_KEY"
        const val SETTINGS_LABS_AUDIO_STATUSBAR_KEY = "SETTINGS_LABS_AUDIO_STATUSBAR_KEY"
        const val SETTINGS_LABS_MEDIA_FILTER_KEY = "SETTINGS_LABS_MEDIA_FILTER_KEY"
        const val SETTINGS_LABS_EXPIRED_FILES_KEY = "SETTINGS_LABS_EXPIRED_FILES_KEY"
        const val SETTINGS_LABS_SEND_ORIGINAL_SIZE_KEY = "SETTINGS_LABS_SEND_ORIGINAL_SIZE_KEY"
        const val SETTINGS_LABS_SKIP_PREVIEW_KEY = "SETTINGS_LABS_SKIP_PREVIEW_KEY"
        const val SETTINGS_LABS_BLOCK_REMOTE_IMAGES_KEY = "SETTINGS_LABS_BLOCK_REMOTE_IMAGES_KEY"
        const val SETTINGS_LABS_KEYWORD_FILTER_KEY = "SETTINGS_LABS_KEYWORD_FILTER_KEY"
        const val SETTINGS_LABS_NETWORK_STATS_KEY = "SETTINGS_LABS_NETWORK_STATS_KEY"
        const val SETTINGS_LABS_MASQUERADE_KEY = "SETTINGS_LABS_MASQUERADE_KEY"
        const val SETTINGS_LABS_USER_MASK_KEY = "SETTINGS_LABS_USER_MASK_KEY"
        const val SETTINGS_LABS_CHUNKED_UPLOAD_KEY = "SETTINGS_LABS_CHUNKED_UPLOAD_KEY"
        const val SETTINGS_LABS_CUSTOM_TIMEZONE_KEY = "SETTINGS_LABS_CUSTOM_TIMEZONE_KEY"
        const val SETTINGS_LABS_STRIP_EXIF_KEY = "SETTINGS_LABS_STRIP_EXIF_KEY"
        const val SETTINGS_LABS_HIDE_INVITATION_KEY = "SETTINGS_LABS_HIDE_INVITATION_KEY"
        const val SETTINGS_LABS_ALL_THREADS_KEY = "SETTINGS_LABS_ALL_THREADS_KEY"
        const val SETTINGS_LABS_USER_MESSAGES_KEY = "SETTINGS_LABS_USER_MESSAGES_KEY"
        const val SETTINGS_LABS_ROOM_VERSION_KEY = "SETTINGS_LABS_ROOM_VERSION_KEY"
        const val SETTINGS_LABS_EXPANDED_PREVIEW_KEY = "SETTINGS_LABS_EXPANDED_PREVIEW_KEY"
        const val SETTINGS_LABS_RAM_STATUSBAR_KEY = "SETTINGS_LABS_RAM_STATUSBAR_KEY"
        const val SETTINGS_LABS_CACHE_MANAGER_KEY = "SETTINGS_LABS_CACHE_MANAGER_KEY"
        const val SETTINGS_LABS_ALL_MESSAGES_KEY = "SETTINGS_LABS_ALL_MESSAGES_KEY"
        const val SETTINGS_LABS_ROOM_INFO_KEY = "SETTINGS_LABS_ROOM_INFO_KEY"
        const val SETTINGS_LABS_AUTO_TRANSLATE_KEY = "SETTINGS_LABS_AUTO_TRANSLATE_KEY"
        const val SETTINGS_LABS_DELETED_ARCHIVE_KEY = "SETTINGS_LABS_DELETED_ARCHIVE_KEY"
        const val SETTINGS_LABS_E2E_SEARCH_KEY = "SETTINGS_LABS_E2E_SEARCH_KEY"
        const val SETTINGS_LABS_MODULE_UPDATES_KEY = "SETTINGS_LABS_MODULE_UPDATES_KEY"
        const val SETTINGS_LABS_NOTIF_KEYWORDS_KEY = "SETTINGS_LABS_NOTIF_KEYWORDS_KEY"
        const val SETTINGS_LABS_REACTION_PREVIEW_KEY = "SETTINGS_LABS_REACTION_PREVIEW_KEY"
        const val SETTINGS_LABS_ROOM_MIRROR_KEY = "SETTINGS_LABS_ROOM_MIRROR_KEY"
        const val SETTINGS_LABS_SYMBOL_BAR_KEY = "SETTINGS_LABS_SYMBOL_BAR_KEY"
        const val SETTINGS_LABS_AUTO_REPLACE_KEY = "SETTINGS_LABS_AUTO_REPLACE_KEY"
        const val SETTINGS_LABS_LLM_SLASH_KEY = "SETTINGS_LABS_LLM_SLASH_KEY"
        const val SETTINGS_LABS_EXTENDED_VIA_PARAMS_KEY = "SETTINGS_LABS_EXTENDED_VIA_PARAMS_KEY"
        const val SETTINGS_LABS_VIA_PARAM_COUNT_KEY = "SETTINGS_LABS_VIA_PARAM_COUNT_KEY"
        const val SETTINGS_LABS_INCLUDE_HISTORICAL_SERVERS_KEY = "SETTINGS_LABS_INCLUDE_HISTORICAL_SERVERS_KEY"
        const val SETTINGS_LABS_EDIT_STACKING_KEY = "SETTINGS_LABS_EDIT_STACKING_KEY"
        const val SETTINGS_LABS_SHOW_EDIT_COUNT_KEY = "SETTINGS_LABS_SHOW_EDIT_COUNT_KEY"
        const val SETTINGS_LABS_EMOJI_ATTACK_PROTECTION_KEY = "SETTINGS_LABS_EMOJI_ATTACK_PROTECTION_KEY"
        const val SETTINGS_LABS_EMOJI_MAX_COUNT_KEY = "SETTINGS_LABS_EMOJI_MAX_COUNT_KEY"
        const val SETTINGS_LABS_MEDIA_COLLAPSE_THRESHOLD_KEY = "SETTINGS_LABS_MEDIA_COLLAPSE_THRESHOLD_KEY"
        const val SETTINGS_LABS_OFFLINE_CACHE_KEY = "SETTINGS_LABS_OFFLINE_CACHE_KEY"
        const val SETTINGS_LABS_DUPLICATE_NAMES_KEY = "SETTINGS_LABS_DUPLICATE_NAMES_KEY"
        const val SETTINGS_LABS_READ_RECEIPTS_KEY = "SETTINGS_LABS_READ_RECEIPTS_KEY"
        const val SETTINGS_READ_RECEIPTS_MAX_VISIBLE = "SETTINGS_READ_RECEIPTS_MAX_VISIBLE"
        const val SETTINGS_LABS_ROOM_ANALYTICS_KEY = "SETTINGS_LABS_ROOM_ANALYTICS_KEY"
        const val SETTINGS_LABS_USER_HIDE_KEY = "SETTINGS_LABS_USER_HIDE_KEY"
        const val SETTINGS_LABS_MODERATION_KEY = "SETTINGS_LABS_MODERATION_KEY"
        const val SETTINGS_LABS_AUTO_SCROLL_KEY = "SETTINGS_LABS_AUTO_SCROLL_KEY"
        const val SETTINGS_LABS_MSG_QUEUE_KEY = "SETTINGS_LABS_MSG_QUEUE_KEY"
        const val SETTINGS_LABS_LOSSLESS_CROP_KEY = "SETTINGS_LABS_LOSSLESS_CROP_KEY"
        const val SETTINGS_LABS_LANG_DETECT_KEY = "SETTINGS_LABS_LANG_DETECT_KEY"
        const val SETTINGS_LABS_CHAT_PUSHDOWN_KEY = "SETTINGS_LABS_CHAT_PUSHDOWN_KEY"
        const val SETTINGS_LABS_EMOJI_BLACKLIST_KEY = "SETTINGS_LABS_EMOJI_BLACKLIST_KEY"
        const val SETTINGS_LABS_AVATAR_HISTORY_KEY = "SETTINGS_LABS_AVATAR_HISTORY_KEY"
        const val SETTINGS_LABS_JUMPTOROOM_KEY = "SETTINGS_LABS_JUMPTOROOM_KEY"
        const val SETTINGS_LABS_EVENT_LINK_RESOLVE_KEY = "SETTINGS_LABS_EVENT_LINK_RESOLVE_KEY"
        const val SETTINGS_LABS_SECONDS_TIMESTAMPS_KEY = "SETTINGS_LABS_SECONDS_TIMESTAMPS_KEY"
        const val SETTINGS_LABS_LIGHTWEIGHT_CALL_KEY = "SETTINGS_LABS_LIGHTWEIGHT_CALL_KEY"
        const val SETTINGS_LABS_SCHEDULED_EDITS_KEY = "SETTINGS_LABS_SCHEDULED_EDITS_KEY"
        const val SETTINGS_LABS_SVG_DRAW_KEY = "SETTINGS_LABS_SVG_DRAW_KEY"
        const val SETTINGS_LABS_PROFILE_SWIPER_KEY = "SETTINGS_LABS_PROFILE_SWIPER_KEY"
        const val SETTINGS_LABS_DESYNC_DETECTOR_KEY = "SETTINGS_LABS_DESYNC_DETECTOR_KEY"
        const val SETTINGS_DESYNC_INTERVAL_MINUTES = "SETTINGS_DESYNC_INTERVAL_MINUTES"
        const val SETTINGS_LABS_LOCATION_SHARING_KEY = "SETTINGS_LABS_LOCATION_SHARING_KEY"
        const val SETTINGS_LABS_ROOM_COUNT_KEY = "SETTINGS_LABS_ROOM_COUNT_KEY"
        const val SETTINGS_LABS_ROOM_COUNT_SPLIT_KEY = "SETTINGS_LABS_ROOM_COUNT_SPLIT_KEY"
        const val SETTINGS_LABS_ROOM_COUNT_UNIQUE_KEY = "SETTINGS_LABS_ROOM_COUNT_UNIQUE_KEY"
        const val SETTINGS_LABS_ROOM_NUMBERING_KEY = "SETTINGS_LABS_ROOM_NUMBERING_KEY"
        const val SETTINGS_LABS_MULTI_SERVER_EXPORT_KEY = "SETTINGS_LABS_MULTI_SERVER_EXPORT_KEY"
        const val SETTINGS_LABS_NATIVE_HTTP_KEY = "SETTINGS_LABS_NATIVE_HTTP_KEY"
        const val SETTINGS_LABS_NATIVE_TIMELINE_KEY = "SETTINGS_LABS_NATIVE_TIMELINE_KEY"
        const val SETTINGS_LABS_NATIVE_DB_KEY = "SETTINGS_LABS_NATIVE_DB_KEY"
        const val SETTINGS_LABS_NATIVE_SYNC_PARSER_KEY = "SETTINGS_LABS_NATIVE_SYNC_PARSER_KEY"
        const val SETTINGS_LABS_NATIVE_CRYPTO_KEY = "SETTINGS_LABS_NATIVE_CRYPTO_KEY"
        const val SETTINGS_LABS_NATIVE_MARKDOWN_KEY = "SETTINGS_LABS_NATIVE_MARKDOWN_KEY"
        const val SETTINGS_LABS_NATIVE_WIDGET_MANAGER_KEY = "SETTINGS_LABS_NATIVE_WIDGET_MANAGER_KEY"
        const val SETTINGS_LABS_TRANSPARENT_OVERLAY_KEY = "SETTINGS_LABS_TRANSPARENT_OVERLAY_KEY"
        const val SETTINGS_LABS_TEXT_UNDO_KEY = "SETTINGS_LABS_TEXT_UNDO_KEY"
        const val SETTINGS_LABS_SPOILER_KEY = "SETTINGS_LABS_SPOILER_KEY"
        const val SETTINGS_LABS_FULL_AVATAR_KEY = "SETTINGS_LABS_FULL_AVATAR_KEY"
        const val SETTINGS_LABS_AVATAR_ORIGINAL_RATIO = "SETTINGS_LABS_AVATAR_ORIGINAL_RATIO"
        const val SETTINGS_LABS_WEB_SEARCH_KEY = "SETTINGS_LABS_WEB_SEARCH_KEY"
        const val SETTINGS_LABS_AGENT_WEB_ACCESS_KEY = "SETTINGS_LABS_AGENT_WEB_ACCESS_KEY"
        const val SETTINGS_NOTIFICATION_ICON_STYLE = "SETTINGS_NOTIFICATION_ICON_STYLE"
        const val SETTINGS_NOTIFICATION_SHOW_COUNT = "SETTINGS_NOTIFICATION_SHOW_COUNT"
        const val SETTINGS_LABS_LIVE_DRAFT_KEY = "SETTINGS_LABS_LIVE_DRAFT_KEY"
        const val SETTINGS_LIVE_DRAFT_CHAR_THRESHOLD = "SETTINGS_LIVE_DRAFT_CHAR_THRESHOLD"
        const val SETTINGS_LIVE_DRAFT_UPDATE_INTERVAL_MS = "SETTINGS_LIVE_DRAFT_UPDATE_INTERVAL_MS"
        const val SETTINGS_CRYPTOGRAPHY_PREFERENCE_KEY = "SETTINGS_CRYPTOGRAPHY_PREFERENCE_KEY"
        const val SETTINGS_CRYPTOGRAPHY_DIVIDER_PREFERENCE_KEY = "SETTINGS_CRYPTOGRAPHY_DIVIDER_PREFERENCE_KEY"
        const val SETTINGS_CRYPTOGRAPHY_MANAGE_PREFERENCE_KEY = "SETTINGS_CRYPTOGRAPHY_MANAGE_PREFERENCE_KEY"
        const val SETTINGS_CRYPTOGRAPHY_MANAGE_DIVIDER_PREFERENCE_KEY = "SETTINGS_CRYPTOGRAPHY_MANAGE_DIVIDER_PREFERENCE_KEY"
        const val SETTINGS_ROOM_SETTINGS_LABS_END_TO_END_PREFERENCE_KEY = "SETTINGS_ROOM_SETTINGS_LABS_END_TO_END_PREFERENCE_KEY"
        const val SETTINGS_ROOM_SETTINGS_LABS_END_TO_END_IS_ACTIVE_PREFERENCE_KEY = "SETTINGS_ROOM_SETTINGS_LABS_END_TO_END_IS_ACTIVE_PREFERENCE_KEY"
        const val SETTINGS_ENCRYPTION_CROSS_SIGNING_PREFERENCE_KEY = "SETTINGS_ENCRYPTION_CROSS_SIGNING_PREFERENCE_KEY"
        const val SETTINGS_ENCRYPTION_EXPORT_E2E_ROOM_KEYS_PREFERENCE_KEY = "SETTINGS_ENCRYPTION_EXPORT_E2E_ROOM_KEYS_PREFERENCE_KEY"
        const val SETTINGS_ENCRYPTION_IMPORT_E2E_ROOM_KEYS_PREFERENCE_KEY = "SETTINGS_ENCRYPTION_IMPORT_E2E_ROOM_KEYS_PREFERENCE_KEY"
        const val SETTINGS_ENCRYPTION_NEVER_SENT_TO_PREFERENCE_KEY = "SETTINGS_ENCRYPTION_NEVER_SENT_TO_PREFERENCE_KEY"
        const val SETTINGS_SHOW_DEVICES_LIST_PREFERENCE_KEY = "SETTINGS_SHOW_DEVICES_LIST_PREFERENCE_KEY"
        const val SETTINGS_SHOW_DEVICES_LIST_V2_PREFERENCE_KEY = "SETTINGS_SHOW_DEVICES_LIST_V2_PREFERENCE_KEY"
        const val SETTINGS_ALLOW_INTEGRATIONS_KEY = "SETTINGS_ALLOW_INTEGRATIONS_KEY"
        const val SETTINGS_INTEGRATION_MANAGER_UI_URL_KEY = "SETTINGS_INTEGRATION_MANAGER_UI_URL_KEY"
        const val SETTINGS_SECURE_MESSAGE_RECOVERY_PREFERENCE_KEY = "SETTINGS_SECURE_MESSAGE_RECOVERY_PREFERENCE_KEY"
        const val SETTINGS_PERSISTED_SPACE_BACKSTACK = "SETTINGS_PERSISTED_SPACE_BACKSTACK"
        const val SETTINGS_SECURITY_INCOGNITO_KEYBOARD_PREFERENCE_KEY = "SETTINGS_SECURITY_INCOGNITO_KEYBOARD_PREFERENCE_KEY"

        const val SETTINGS_CRYPTOGRAPHY_HS_ADMIN_DISABLED_E2E_DEFAULT = "SETTINGS_CRYPTOGRAPHY_HS_ADMIN_DISABLED_E2E_DEFAULT"
//        const val SETTINGS_SECURE_BACKUP_RESET_PREFERENCE_KEY = "SETTINGS_SECURE_BACKUP_RESET_PREFERENCE_KEY"

        // user
        const val SETTINGS_PROFILE_PICTURE_PREFERENCE_KEY = "SETTINGS_PROFILE_PICTURE_PREFERENCE_KEY"

        // contacts
        const val SETTINGS_CONTACTS_PHONEBOOK_COUNTRY_PREFERENCE_KEY = "SETTINGS_CONTACTS_PHONEBOOK_COUNTRY_PREFERENCE_KEY"

        // interface
        const val SETTINGS_INTERFACE_LANGUAGE_PREFERENCE_KEY = "SETTINGS_INTERFACE_LANGUAGE_PREFERENCE_KEY"
        const val SETTINGS_INTERFACE_TEXT_SIZE_KEY = "SETTINGS_INTERFACE_TEXT_SIZE_KEY"
        const val SETTINGS_INTERFACE_BUBBLE_KEY = "SETTINGS_INTERFACE_BUBBLE_KEY"
        const val SETTINGS_SHOW_URL_PREVIEW_KEY = "SETTINGS_SHOW_URL_PREVIEW_KEY"
        private const val SETTINGS_SEND_TYPING_NOTIF_KEY = "SETTINGS_SEND_TYPING_NOTIF_KEY"
        private const val SETTINGS_ENABLE_MARKDOWN_KEY = "SETTINGS_ENABLE_MARKDOWN_KEY"
        private const val SETTINGS_ENABLE_RICH_TEXT_FORMATTING_KEY = "SETTINGS_ENABLE_RICH_TEXT_FORMATTING_KEY"
        private const val SETTINGS_ALWAYS_SHOW_TIMESTAMPS_KEY = "SETTINGS_ALWAYS_SHOW_TIMESTAMPS_KEY"
        private const val SETTINGS_12_24_TIMESTAMPS_KEY = "SETTINGS_12_24_TIMESTAMPS_KEY"
        private const val SETTINGS_SHOW_READ_RECEIPTS_KEY = "SETTINGS_SHOW_READ_RECEIPTS_KEY"
        private const val SETTINGS_SHOW_REDACTED_KEY = "SETTINGS_SHOW_REDACTED_KEY"
        private const val SETTINGS_SHOW_ROOM_MEMBER_STATE_EVENTS_KEY = "SETTINGS_SHOW_ROOM_MEMBER_STATE_EVENTS_KEY"
        private const val SETTINGS_SHOW_JOIN_LEAVE_MESSAGES_KEY = "SETTINGS_SHOW_JOIN_LEAVE_MESSAGES_KEY"
        private const val SETTINGS_SHOW_AVATAR_DISPLAY_NAME_CHANGES_MESSAGES_KEY = "SETTINGS_SHOW_AVATAR_DISPLAY_NAME_CHANGES_MESSAGES_KEY"
        private const val SETTINGS_VIBRATE_ON_MENTION_KEY = "SETTINGS_VIBRATE_ON_MENTION_KEY"
        private const val SETTINGS_SEND_MESSAGE_WITH_ENTER = "SETTINGS_SEND_MESSAGE_WITH_ENTER"
        private const val SETTINGS_ENABLE_CHAT_EFFECTS = "SETTINGS_ENABLE_CHAT_EFFECTS"
        private const val SETTINGS_SHOW_EMOJI_KEYBOARD = "SETTINGS_SHOW_EMOJI_KEYBOARD"
        private const val SETTINGS_LABS_ENABLE_LATEX_MATHS = "SETTINGS_LABS_ENABLE_LATEX_MATHS"
        const val SETTINGS_PRESENCE_USER_ALWAYS_APPEARS_OFFLINE = "SETTINGS_PRESENCE_USER_ALWAYS_APPEARS_OFFLINE"
        const val SETTINGS_AUTOPLAY_ANIMATED_IMAGES = "SETTINGS_AUTOPLAY_ANIMATED_IMAGES"
        private const val SETTINGS_ENABLE_DIRECT_SHARE = "SETTINGS_ENABLE_DIRECT_SHARE"

        // Room directory
        private const val SETTINGS_ROOM_DIRECTORY_SHOW_ALL_PUBLIC_ROOMS = "SETTINGS_ROOM_DIRECTORY_SHOW_ALL_PUBLIC_ROOMS"

        // Help
        private const val SETTINGS_SHOULD_SHOW_HELP_ON_ROOM_LIST_KEY = "SETTINGS_SHOULD_SHOW_HELP_ON_ROOM_LIST_KEY"

        // home
        private const val SETTINGS_PIN_UNREAD_MESSAGES_PREFERENCE_KEY = "SETTINGS_PIN_UNREAD_MESSAGES_PREFERENCE_KEY"
        private const val SETTINGS_PIN_MISSED_NOTIFICATIONS_PREFERENCE_KEY = "SETTINGS_PIN_MISSED_NOTIFICATIONS_PREFERENCE_KEY"

        // notifications
        const val SETTINGS_ENABLE_ALL_NOTIF_PREFERENCE_KEY = "SETTINGS_ENABLE_ALL_NOTIF_PREFERENCE_KEY"
        const val SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY = "SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY"
        const val SETTINGS_EMAIL_NOTIFICATION_CATEGORY_PREFERENCE_KEY = "SETTINGS_EMAIL_NOTIFICATION_CATEGORY_PREFERENCE_KEY"

        //    public static final String SETTINGS_TURN_SCREEN_ON_PREFERENCE_KEY = "SETTINGS_TURN_SCREEN_ON_PREFERENCE_KEY";
        const val SETTINGS_SYSTEM_CALL_NOTIFICATION_PREFERENCE_KEY = "SETTINGS_SYSTEM_CALL_NOTIFICATION_PREFERENCE_KEY"
        const val SETTINGS_SYSTEM_NOISY_NOTIFICATION_PREFERENCE_KEY = "SETTINGS_SYSTEM_NOISY_NOTIFICATION_PREFERENCE_KEY"
        const val SETTINGS_SYSTEM_SILENT_NOTIFICATION_PREFERENCE_KEY = "SETTINGS_SYSTEM_SILENT_NOTIFICATION_PREFERENCE_KEY"
        const val SETTINGS_NOTIFICATION_RINGTONE_PREFERENCE_KEY = "SETTINGS_NOTIFICATION_RINGTONE_PREFERENCE_KEY"
        const val SETTINGS_NOTIFICATION_RINGTONE_SELECTION_PREFERENCE_KEY = "SETTINGS_NOTIFICATION_RINGTONE_SELECTION_PREFERENCE_KEY"

        // media
        private const val SETTINGS_DEFAULT_MEDIA_COMPRESSION_KEY = "SETTINGS_DEFAULT_MEDIA_COMPRESSION_KEY"
        private const val SETTINGS_DEFAULT_MEDIA_SOURCE_KEY = "SETTINGS_DEFAULT_MEDIA_SOURCE_KEY"
        private const val SETTINGS_PREVIEW_MEDIA_BEFORE_SENDING_KEY = "SETTINGS_PREVIEW_MEDIA_BEFORE_SENDING_KEY"
        private const val SETTINGS_PLAY_SHUTTER_SOUND_KEY = "SETTINGS_PLAY_SHUTTER_SOUND_KEY"

        // background sync
        const val SETTINGS_START_ON_BOOT_PREFERENCE_KEY = "SETTINGS_START_ON_BOOT_PREFERENCE_KEY"
        const val SETTINGS_ENABLE_BACKGROUND_SYNC_PREFERENCE_KEY = "SETTINGS_ENABLE_BACKGROUND_SYNC_PREFERENCE_KEY"
        const val SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY = "SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY"
        const val SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY = "SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY"

        // notification method
        const val SETTINGS_NOTIFICATION_METHOD_KEY = "SETTINGS_NOTIFICATION_METHOD_KEY"

        // Calls
        const val SETTINGS_CALL_PREVENT_ACCIDENTAL_CALL_KEY = "SETTINGS_CALL_PREVENT_ACCIDENTAL_CALL_KEY"
        const val SETTINGS_CALL_RINGTONE_USE_RIOT_PREFERENCE_KEY = "SETTINGS_CALL_RINGTONE_USE_RIOT_PREFERENCE_KEY"
        const val SETTINGS_CALL_RINGTONE_URI_PREFERENCE_KEY = "SETTINGS_CALL_RINGTONE_URI_PREFERENCE_KEY"

        // labs
        const val SETTINGS_LAZY_LOADING_PREFERENCE_KEY = "SETTINGS_LAZY_LOADING_PREFERENCE_KEY"
        const val SETTINGS_USER_REFUSED_LAZY_LOADING_PREFERENCE_KEY = "SETTINGS_USER_REFUSED_LAZY_LOADING_PREFERENCE_KEY"
        const val SETTINGS_DATA_SAVE_MODE_PREFERENCE_KEY = "SETTINGS_DATA_SAVE_MODE_PREFERENCE_KEY"
        private const val SETTINGS_USE_JITSI_CONF_PREFERENCE_KEY = "SETTINGS_USE_JITSI_CONF_PREFERENCE_KEY"
        private const val SETTINGS_USE_NATIVE_CAMERA_PREFERENCE_KEY = "SETTINGS_USE_NATIVE_CAMERA_PREFERENCE_KEY"
        private const val SETTINGS_ENABLE_SEND_VOICE_FEATURE_PREFERENCE_KEY = "SETTINGS_ENABLE_SEND_VOICE_FEATURE_PREFERENCE_KEY"

        const val SETTINGS_LABS_ALLOW_EXTENDED_LOGS = "SETTINGS_LABS_ALLOW_EXTENDED_LOGS"
        const val SETTINGS_LABS_AUTO_REPORT_UISI = "SETTINGS_LABS_AUTO_REPORT_UISI"
        const val SETTINGS_PREF_SPACE_SHOW_ALL_ROOM_IN_HOME = "SETTINGS_PREF_SPACE_SHOW_ALL_ROOM_IN_HOME"

        /**
         * This is not preference, but category on preferences screen which contains [SETTINGS_PREF_SPACE_SHOW_ALL_ROOM_IN_HOME].
         * Needed to show/hide this category, depending on visibility of [SETTINGS_PREF_SPACE_SHOW_ALL_ROOM_IN_HOME]. */
        const val SETTINGS_PREF_SPACE_CATEGORY = "SETTINGS_PREF_SPACE_CATEGORY"

        private const val SETTINGS_DEVELOPER_MODE_PREFERENCE_KEY = "SETTINGS_DEVELOPER_MODE_PREFERENCE_KEY"
        const val SETTINGS_DEVELOPER_MODE_KEY_REQUEST_AUDIT_KEY = "SETTINGS_DEVELOPER_MODE_KEY_REQUEST_AUDIT_KEY"
        private const val SETTINGS_LABS_SHOW_HIDDEN_EVENTS_PREFERENCE_KEY = "SETTINGS_LABS_SHOW_HIDDEN_EVENTS_PREFERENCE_KEY"
        private const val SETTINGS_LABS_ENABLE_SWIPE_TO_REPLY = "SETTINGS_LABS_ENABLE_SWIPE_TO_REPLY"
        private const val SETTINGS_DEVELOPER_MODE_FAIL_FAST_PREFERENCE_KEY = "SETTINGS_DEVELOPER_MODE_FAIL_FAST_PREFERENCE_KEY"
        private const val SETTINGS_DEVELOPER_MODE_SHOW_INFO_ON_SCREEN_KEY = "SETTINGS_DEVELOPER_MODE_SHOW_INFO_ON_SCREEN_KEY"
        private const val SETTINGS_ENABLE_MEMORY_LEAK_ANALYSIS_KEY = "SETTINGS_ENABLE_MEMORY_LEAK_ANALYSIS_KEY"

        const val SETTINGS_LABS_MSC3061_SHARE_KEYS_HISTORY = "SETTINGS_LABS_MSC3061_SHARE_KEYS_HISTORY"

        // SETTINGS_LABS_HIDE_TECHNICAL_E2E_ERRORS
        private const val SETTINGS_LABS_SHOW_COMPLETE_HISTORY_IN_ENCRYPTED_ROOM = "SETTINGS_LABS_SHOW_COMPLETE_HISTORY_IN_ENCRYPTED_ROOM"
        const val SETTINGS_LABS_UNREAD_NOTIFICATIONS_AS_TAB = "SETTINGS_LABS_UNREAD_NOTIFICATIONS_AS_TAB"

        // Rageshake
        const val SETTINGS_USE_RAGE_SHAKE_KEY = "SETTINGS_USE_RAGE_SHAKE_KEY"
        const val SETTINGS_RAGE_SHAKE_DETECTION_THRESHOLD_KEY = "SETTINGS_RAGE_SHAKE_DETECTION_THRESHOLD_KEY"

        // Security
        const val SETTINGS_SECURITY_USE_FLAG_SECURE = "SETTINGS_SECURITY_USE_FLAG_SECURE"
        const val SETTINGS_SECURITY_USE_PIN_CODE_FLAG = "SETTINGS_SECURITY_USE_PIN_CODE_FLAG"
        const val SETTINGS_SECURITY_CHANGE_PIN_CODE_FLAG = "SETTINGS_SECURITY_CHANGE_PIN_CODE_FLAG"
        const val SETTINGS_SECURITY_USE_BIOMETRICS_FLAG = "SETTINGS_SECURITY_USE_BIOMETRICS_FLAG"
        private const val SETTINGS_SECURITY_USE_GRACE_PERIOD_FLAG = "SETTINGS_SECURITY_USE_GRACE_PERIOD_FLAG"
        const val SETTINGS_SECURITY_USE_COMPLETE_NOTIFICATIONS_FLAG = "SETTINGS_SECURITY_USE_COMPLETE_NOTIFICATIONS_FLAG"

        // New Session Manager
        const val SETTINGS_SESSION_MANAGER_SHOW_IP_ADDRESS = "SETTINGS_SESSION_MANAGER_SHOW_IP_ADDRESS"

        // other
        const val SETTINGS_MEDIA_SAVING_PERIOD_KEY = "SETTINGS_MEDIA_SAVING_PERIOD_KEY"
        private const val SETTINGS_MEDIA_SAVING_PERIOD_SELECTED_KEY = "SETTINGS_MEDIA_SAVING_PERIOD_SELECTED_KEY"
        private const val DID_ASK_TO_IGNORE_BATTERY_OPTIMIZATIONS_KEY = "DID_ASK_TO_IGNORE_BATTERY_OPTIMIZATIONS_KEY"
        private const val DID_MIGRATE_TO_NOTIFICATION_REWORK = "DID_MIGRATE_TO_NOTIFICATION_REWORK"
        private const val DID_ASK_TO_USE_ANALYTICS_TRACKING_KEY = "DID_ASK_TO_USE_ANALYTICS_TRACKING_KEY"
        private const val SETTINGS_DISPLAY_ALL_EVENTS_KEY = "SETTINGS_DISPLAY_ALL_EVENTS_KEY"

        private const val DID_ASK_TO_ENABLE_SESSION_PUSH = "DID_ASK_TO_ENABLE_SESSION_PUSH"

        private const val MEDIA_SAVING_3_DAYS = 0
        private const val MEDIA_SAVING_1_WEEK = 1
        private const val MEDIA_SAVING_1_MONTH = 2
        private const val MEDIA_SAVING_FOREVER = 3

        private const val TAKE_PHOTO_VIDEO_MODE = "TAKE_PHOTO_VIDEO_MODE"

        private const val SETTINGS_LABS_ENABLE_LIVE_LOCATION = "SETTINGS_LABS_ENABLE_LIVE_LOCATION"

        private const val SETTINGS_LABS_ENABLE_ELEMENT_CALL_PERMISSION_SHORTCUTS = "SETTINGS_LABS_ENABLE_ELEMENT_CALL_PERMISSION_SHORTCUTS"

        // This key will be used to identify clients with the old thread support enabled io.element.thread
        const val SETTINGS_LABS_ENABLE_THREAD_MESSAGES_OLD_CLIENTS = "SETTINGS_LABS_ENABLE_THREAD_MESSAGES"

        // This key will be used to identify clients with the new thread support enabled m.thread
        const val SETTINGS_LABS_ENABLE_THREAD_MESSAGES = "SETTINGS_LABS_ENABLE_THREAD_MESSAGES_FINAL"
        const val SETTINGS_LABS_THREAD_MESSAGES_CHANGED_BY_USER = "SETTINGS_LABS_THREAD_MESSAGES_CHANGED_BY_USER"
        const val SETTINGS_THREAD_MESSAGES_SYNCED = "SETTINGS_THREAD_MESSAGES_SYNCED"

        // This key will be used to enable user for displaying live user info or not.
        const val SETTINGS_TIMELINE_SHOW_LIVE_SENDER_INFO = "SETTINGS_TIMELINE_SHOW_LIVE_SENDER_INFO"

        const val SETTINGS_UNVERIFIED_SESSIONS_ALERT_LAST_SHOWN_MILLIS = "SETTINGS_UNVERIFIED_SESSIONS_ALERT_LAST_SHOWN_MILLIS_"
        const val SETTINGS_NEW_LOGIN_ALERT_SHOWN_FOR_DEVICE = "SETTINGS_NEW_LOGIN_ALERT_SHOWN_FOR_DEVICE_"

        // Possible values for TAKE_PHOTO_VIDEO_MODE
        const val TAKE_PHOTO_VIDEO_MODE_ALWAYS_ASK = 0
        const val TAKE_PHOTO_VIDEO_MODE_PHOTO = 1
        const val TAKE_PHOTO_VIDEO_MODE_VIDEO = 2

        const val HAD_EXISTING_LEGACY_DATA = "HAD_EXISTING_LEGACY_DATA"
        const val IS_ON_RUST_CRYPTO = "IS_ON_RUST_CRYPTO"
        // Background sync modes

        // some preferences keys must be kept after a logout
        private val mKeysToKeepAfterLogout = listOf(
                SETTINGS_DEFAULT_MEDIA_COMPRESSION_KEY,
                SETTINGS_DEFAULT_MEDIA_SOURCE_KEY,
                SETTINGS_PLAY_SHUTTER_SOUND_KEY,

                SETTINGS_SEND_TYPING_NOTIF_KEY,
                SETTINGS_ALWAYS_SHOW_TIMESTAMPS_KEY,
                SETTINGS_12_24_TIMESTAMPS_KEY,
                SETTINGS_SHOW_READ_RECEIPTS_KEY,
                SETTINGS_SHOW_ROOM_MEMBER_STATE_EVENTS_KEY,
                SETTINGS_SHOW_JOIN_LEAVE_MESSAGES_KEY,
                SETTINGS_SHOW_AVATAR_DISPLAY_NAME_CHANGES_MESSAGES_KEY,
                SETTINGS_MEDIA_SAVING_PERIOD_KEY,
                SETTINGS_MEDIA_SAVING_PERIOD_SELECTED_KEY,
                SETTINGS_PREVIEW_MEDIA_BEFORE_SENDING_KEY,
                SETTINGS_SEND_MESSAGE_WITH_ENTER,
                SETTINGS_SHOW_EMOJI_KEYBOARD,

                SETTINGS_PIN_UNREAD_MESSAGES_PREFERENCE_KEY,
                SETTINGS_PIN_MISSED_NOTIFICATIONS_PREFERENCE_KEY,
                // Do not keep SETTINGS_LAZY_LOADING_PREFERENCE_KEY because the user may log in on a server which does not support lazy loading
                SETTINGS_DATA_SAVE_MODE_PREFERENCE_KEY,
                SETTINGS_START_ON_BOOT_PREFERENCE_KEY,
                SETTINGS_INTERFACE_TEXT_SIZE_KEY,
                SETTINGS_USE_JITSI_CONF_PREFERENCE_KEY,
                SETTINGS_NOTIFICATION_RINGTONE_PREFERENCE_KEY,
                SETTINGS_NOTIFICATION_RINGTONE_SELECTION_PREFERENCE_KEY,

                SETTINGS_ROOM_SETTINGS_LABS_END_TO_END_PREFERENCE_KEY,
                SETTINGS_CONTACTS_PHONEBOOK_COUNTRY_PREFERENCE_KEY,
                SETTINGS_INTERFACE_LANGUAGE_PREFERENCE_KEY,
                SETTINGS_BACKGROUND_SYNC_PREFERENCE_KEY,
                SETTINGS_ENABLE_BACKGROUND_SYNC_PREFERENCE_KEY,
                SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY,
                SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY,

                SETTINGS_DEVELOPER_MODE_PREFERENCE_KEY,
                SETTINGS_LABS_SHOW_HIDDEN_EVENTS_PREFERENCE_KEY,
                SETTINGS_LABS_ALLOW_EXTENDED_LOGS,
                SETTINGS_DEVELOPER_MODE_FAIL_FAST_PREFERENCE_KEY,
                SETTINGS_ENABLE_MEMORY_LEAK_ANALYSIS_KEY,

                SETTINGS_USE_RAGE_SHAKE_KEY,
                SETTINGS_SECURITY_USE_FLAG_SECURE,
                SETTINGS_SECURITY_INCOGNITO_KEYBOARD_PREFERENCE_KEY,

                ShortcutsHandler.SHARED_PREF_KEY,
        )
    }

    /**
     * Allow subscribing and unsubscribing to configuration changes. This is
     * particularly useful when you need to be notified of a configuration change
     * in a background service, e.g. for the P2P demos.
     */
    fun subscribeToChanges(listener: SharedPreferences.OnSharedPreferenceChangeListener) {
        defaultPrefs.registerOnSharedPreferenceChangeListener(listener)
    }

    fun unsubscribeToChanges(listener: SharedPreferences.OnSharedPreferenceChangeListener) {
        defaultPrefs.unregisterOnSharedPreferenceChangeListener(listener)
    }

    /**
     * Clear the preferences.
     */
    fun clearPreferences() {
        val keysToKeep = HashSet(mKeysToKeepAfterLogout)

        // homeserver urls
        keysToKeep.add(ServerUrlsRepository.HOME_SERVER_URL_PREF)
        keysToKeep.add(ServerUrlsRepository.IDENTITY_SERVER_URL_PREF)

        // theme
        keysToKeep.add(ThemeUtils.APPLICATION_THEME_KEY)

        // get all the existing keys
        val keys = defaultPrefs.all.keys

        // remove the one to keep
        keys.removeAll(keysToKeep)

        defaultPrefs.edit {
            for (key in keys) {
                remove(key)
            }
        }
    }

    private fun getDefault(@BoolRes resId: Int) = context.resources.getBoolean(resId)

    fun areNotificationEnabledForDevice(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY, true)
    }

    fun setNotificationEnabledForDevice(enabled: Boolean) {
        defaultPrefs.edit {
            putBoolean(SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY, enabled)
        }
    }

    fun developerMode(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_DEVELOPER_MODE_PREFERENCE_KEY, false)
    }

    fun developerShowDebugInfo(): Boolean {
        return developerMode() && defaultPrefs.getBoolean(SETTINGS_DEVELOPER_MODE_SHOW_INFO_ON_SCREEN_KEY, false)
    }

    fun shouldShowHiddenEvents(): Boolean {
        return developerMode() && defaultPrefs.getBoolean(SETTINGS_LABS_SHOW_HIDDEN_EVENTS_PREFERENCE_KEY, false)
    }

    fun swipeToReplyIsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ENABLE_SWIPE_TO_REPLY, true)
    }

    fun labShowCompleteHistoryInEncryptedRoom(): Boolean {
        return developerMode() && defaultPrefs.getBoolean(SETTINGS_LABS_SHOW_COMPLETE_HISTORY_IN_ENCRYPTED_ROOM, false)
    }

    fun labAllowedExtendedLogging(): Boolean {
        return developerMode() && defaultPrefs.getBoolean(SETTINGS_LABS_ALLOW_EXTENDED_LOGS, false)
    }

    fun labAddNotificationTab(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_UNREAD_NOTIFICATIONS_AS_TAB, false)
    }

    fun latexMathsIsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ENABLE_LATEX_MATHS, false)
    }

    fun failFast(): Boolean {
        return buildMeta.isDebug || (developerMode() && defaultPrefs.getBoolean(SETTINGS_DEVELOPER_MODE_FAIL_FAST_PREFERENCE_KEY, false))
    }

    fun enableMemoryLeakAnalysis(isEnabled: Boolean) {
        defaultPrefs.edit(commit = true) {
            putBoolean(SETTINGS_ENABLE_MEMORY_LEAK_ANALYSIS_KEY, isEnabled)
        }
    }

    fun isMemoryLeakAnalysisEnabled(): Boolean {
        return buildMeta.isDebug && defaultPrefs.getBoolean(SETTINGS_ENABLE_MEMORY_LEAK_ANALYSIS_KEY, false)
    }

    fun didAskUserToEnableSessionPush(): Boolean {
        return defaultPrefs.getBoolean(DID_ASK_TO_ENABLE_SESSION_PUSH, false)
    }

    fun setDidAskUserToEnableSessionPush() {
        defaultPrefs.edit {
            putBoolean(DID_ASK_TO_ENABLE_SESSION_PUSH, true)
        }
    }

    /**
     * Tells if we have already asked the user to disable battery optimisations on android >= M devices.
     *
     * @return true if it was already requested
     */
    fun didAskUserToIgnoreBatteryOptimizations(): Boolean {
        return defaultPrefs.getBoolean(DID_ASK_TO_IGNORE_BATTERY_OPTIMIZATIONS_KEY, false)
    }

    /**
     * Mark as requested the question to disable battery optimisations.
     */
    fun setDidAskUserToIgnoreBatteryOptimizations() {
        defaultPrefs.edit {
            putBoolean(DID_ASK_TO_IGNORE_BATTERY_OPTIMIZATIONS_KEY, true)
        }
    }

    fun didMigrateToNotificationRework(): Boolean {
        return defaultPrefs.getBoolean(DID_MIGRATE_TO_NOTIFICATION_REWORK, false)
    }

    fun setDidMigrateToNotificationRework() {
        defaultPrefs.edit {
            putBoolean(DID_MIGRATE_TO_NOTIFICATION_REWORK, true)
        }
    }

    /**
     * Tells if the timestamp must be displayed in 12h format.
     *
     * @return true if the time must be displayed in 12h format
     */
    fun displayTimeIn12hFormat(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_12_24_TIMESTAMPS_KEY, false)
    }

    /**
     * Tells if the join and leave membership events should be shown in the messages list.
     *
     * @return true if the join and leave membership events should be shown in the messages list
     */
    fun showJoinLeaveMessages(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SHOW_JOIN_LEAVE_MESSAGES_KEY, true)
    }

    /**
     * Tells if the avatar and display name events should be shown in the messages list.
     *
     * @return true true if the avatar and display name events should be shown in the messages list.
     */
    fun showAvatarDisplayNameChangeMessages(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SHOW_AVATAR_DISPLAY_NAME_CHANGES_MESSAGES_KEY, true)
    }

    /**
     * Tells the native camera to take a photo or record a video.
     *
     * @return true to use the native camera app to record video or take photo.
     */
    fun useNativeCamera(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_USE_NATIVE_CAMERA_PREFERENCE_KEY, false)
    }

    /**
     * Tells if the send voice feature is enabled.
     *
     * @return true if the send voice feature is enabled.
     */
    fun isSendVoiceFeatureEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_ENABLE_SEND_VOICE_FEATURE_PREFERENCE_KEY, false)
    }

    /**
     * Show all rooms in room directory.
     */
    fun showAllPublicRooms(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_ROOM_DIRECTORY_SHOW_ALL_PUBLIC_ROOMS, false)
    }

    /**
     * Tells which compression level to use by default.
     *
     * @return the selected compression level
     */
    fun getSelectedDefaultMediaCompressionLevel(): Int {
        return Integer.parseInt(defaultPrefs.getString(SETTINGS_DEFAULT_MEDIA_COMPRESSION_KEY, "0")!!)
    }

    /**
     * Tells which media source to use by default.
     *
     * @return the selected media source
     */
    fun getSelectedDefaultMediaSource(): Int {
        return Integer.parseInt(defaultPrefs.getString(SETTINGS_DEFAULT_MEDIA_SOURCE_KEY, "0")!!)
    }

    /**
     * Tells whether to use shutter sound.
     *
     * @return true if shutter sound should play
     */
    fun useShutterSound(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_PLAY_SHUTTER_SOUND_KEY, true)
    }

    /**
     * Update the notification ringtone.
     *
     * @param uri the new notification ringtone, or null for no RingTone
     */
    fun setNotificationRingTone(uri: Uri?) {
        defaultPrefs.edit {
            var value = ""

            if (null != uri) {
                value = uri.toString()

                if (value.startsWith("file://")) {
                    // it should never happen
                    // else android.os.FileUriExposedException will be triggered.
                    // see https://github.com/element-hq/riot-android/issues/1725
                    return
                }
            }

            putString(SETTINGS_NOTIFICATION_RINGTONE_PREFERENCE_KEY, value)
        }
    }

    /**
     * Provides the selected notification ring tone.
     *
     * @return the selected ring tone or null for no RingTone
     */
    fun getNotificationRingTone(): Uri? {
        val url = defaultPrefs.getString(SETTINGS_NOTIFICATION_RINGTONE_PREFERENCE_KEY, null)

        // the user selects "None"
        if (url == "") {
            return null
        }

        var uri: Uri? = null

        // https://github.com/element-hq/riot-android/issues/1725
        if (null != url && !url.startsWith("file://")) {
            try {
                uri = Uri.parse(url)
            } catch (e: Exception) {
                Timber.e(e, "## getNotificationRingTone() : Uri.parse failed")
            }
        }

        if (null == uri) {
            uri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION)
        }

        Timber.v("## getNotificationRingTone() returns $uri")
        return uri
    }

    /**
     * Provide the notification ringtone filename.
     *
     * @return the filename or null if "None" is selected
     */
    fun getNotificationRingToneName(): String? {
        val toneUri = getNotificationRingTone() ?: return null

        try {
            val proj = arrayOf(MediaStore.Audio.Media.DISPLAY_NAME)
            return context.contentResolver.query(toneUri, proj, null, null, null)?.use {
                val columnIndex = it.getColumnIndexOrThrow(MediaStore.Audio.Media.DISPLAY_NAME)
                it.moveToFirst()
                it.getString(columnIndex)
            }
        } catch (e: Exception) {
            Timber.e(e, "## getNotificationRingToneName() failed")
        }

        return null
    }

    /**
     * Enable or disable the lazy loading.
     *
     * @param newValue true to enable lazy loading, false to disable it
     */
    fun setUseLazyLoading(newValue: Boolean) {
        defaultPrefs.edit {
            putBoolean(SETTINGS_LAZY_LOADING_PREFERENCE_KEY, newValue)
        }
    }

    /**
     * Tells if the lazy loading is enabled.
     *
     * @return true if the lazy loading of room members is enabled
     */
    fun useLazyLoading(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LAZY_LOADING_PREFERENCE_KEY, false)
    }

    /**
     * User explicitly refuses the lazy loading.
     */
    fun setUserRefuseLazyLoading() {
        defaultPrefs.edit {
            putBoolean(SETTINGS_USER_REFUSED_LAZY_LOADING_PREFERENCE_KEY, true)
        }
    }

    /**
     * Tells if the user has explicitly refused the lazy loading.
     *
     * @return true if the user has explicitly refuse the lazy loading of room members
     */
    fun hasUserRefusedLazyLoading(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_USER_REFUSED_LAZY_LOADING_PREFERENCE_KEY, false)
    }

    /**
     * Tells if the data save mode is enabled.
     *
     * @return true if the data save mode is enabled
     */
    fun useDataSaveMode(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_DATA_SAVE_MODE_PREFERENCE_KEY, false)
    }

    /**
     * Tells if the conf calls must be done with Jitsi.
     *
     * @return true if the conference call must be done with jitsi.
     */
    fun useJitsiConfCall(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_USE_JITSI_CONF_PREFERENCE_KEY, true)
    }

    /**
     * Tells if the application is started on boot.
     *
     * @return true if the application must be started on boot
     */
    fun autoStartOnBoot(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_START_ON_BOOT_PREFERENCE_KEY, true)
    }

    /**
     * Tells if the application is started on boot.
     *
     * @param value true to start the application on boot
     */
    fun setAutoStartOnBoot(value: Boolean) {
        defaultPrefs.edit {
            putBoolean(SETTINGS_START_ON_BOOT_PREFERENCE_KEY, value)
        }
    }

    /**
     * Provides the selected saving period.
     *
     * @return the selected period
     */
    fun getSelectedMediasSavingPeriod(): Int {
        return defaultPrefs.getInt(SETTINGS_MEDIA_SAVING_PERIOD_SELECTED_KEY, MEDIA_SAVING_1_WEEK)
    }

    /**
     * Updates the selected saving period.
     *
     * @param index the selected period index
     */
    fun setSelectedMediasSavingPeriod(index: Int) {
        defaultPrefs.edit {
            putInt(SETTINGS_MEDIA_SAVING_PERIOD_SELECTED_KEY, index)
        }
    }

    /**
     * Provides the minimum last access time to keep a media file.
     *
     * @return the min last access time (in seconds)
     */
    fun getMinMediasLastAccessTime(): Long {
        return when (getSelectedMediasSavingPeriod()) {
            MEDIA_SAVING_3_DAYS -> clock.epochMillis() / 1000 - 3 * 24 * 60 * 60
            MEDIA_SAVING_1_WEEK -> clock.epochMillis() / 1000 - 7 * 24 * 60 * 60
            MEDIA_SAVING_1_MONTH -> clock.epochMillis() / 1000 - 30 * 24 * 60 * 60
            MEDIA_SAVING_FOREVER -> 0
            else -> 0
        }
    }

    /**
     * Provides the selected saving period.
     *
     * @return the selected period
     */
    fun getSelectedMediasSavingPeriodString(): String {
        return when (getSelectedMediasSavingPeriod()) {
            MEDIA_SAVING_3_DAYS -> stringProvider.getString(CommonStrings.media_saving_period_3_days)
            MEDIA_SAVING_1_WEEK -> stringProvider.getString(CommonStrings.media_saving_period_1_week)
            MEDIA_SAVING_1_MONTH -> stringProvider.getString(CommonStrings.media_saving_period_1_month)
            MEDIA_SAVING_FOREVER -> stringProvider.getString(CommonStrings.media_saving_period_forever)
            else -> "?"
        }
    }

    /**
     * Fix some migration issues.
     */
    fun fixMigrationIssues() {
        // Nothing to do for the moment
    }

    /**
     * Tells if the markdown is enabled.
     *
     * @return true if the markdown is enabled
     */
    fun isMarkdownEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_ENABLE_MARKDOWN_KEY, false)
    }

    /**
     * Update the markdown enable status.
     *
     * @param isEnabled true to enable the markdown
     */
    fun setMarkdownEnabled(isEnabled: Boolean) {
        defaultPrefs.edit {
            putBoolean(SETTINGS_ENABLE_MARKDOWN_KEY, isEnabled)
        }
    }

    /**
     * Tells if text formatting is enabled within the rich text editor.
     *
     * @return true if the text formatting is enabled
     */
    fun isTextFormattingEnabled(): Boolean =
            defaultPrefs.getBoolean(SETTINGS_ENABLE_RICH_TEXT_FORMATTING_KEY, true)

    /**
     * Update whether text formatting is enabled within the rich text editor.
     *
     * @param isEnabled true to enable the text formatting
     */
    fun setTextFormattingEnabled(isEnabled: Boolean) =
            defaultPrefs.edit {
                putBoolean(SETTINGS_ENABLE_RICH_TEXT_FORMATTING_KEY, isEnabled)
            }

    /**
     * Tells if a confirmation dialog should be displayed before staring a call.
     */
    fun preventAccidentalCall(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_CALL_PREVENT_ACCIDENTAL_CALL_KEY, false)
    }

    /**
     * Tells if the read receipts should be shown.
     *
     * @return true if the read receipts should be shown
     */
    fun showReadReceipts(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SHOW_READ_RECEIPTS_KEY, true)
    }

    /**
     * Tells if the redacted message should be shown.
     *
     * @return true if the redacted should be shown
     */
    fun showRedactedMessages(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SHOW_REDACTED_KEY, true)
    }

    /**
     * Tells if the help on room list should be shown.
     *
     * @return true if the help on room list should be shown
     */
    fun shouldShowLongClickOnRoomHelp(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SHOULD_SHOW_HELP_ON_ROOM_LIST_KEY, true)
    }

    /**
     * Prevent help on room list to be shown again.
     */
    fun neverShowLongClickOnRoomHelpAgain() {
        defaultPrefs.edit {
            putBoolean(SETTINGS_SHOULD_SHOW_HELP_ON_ROOM_LIST_KEY, false)
        }
    }

    /**
     * Tells if the message timestamps must be always shown.
     *
     * @return true if the message timestamps must be always shown
     */
    fun alwaysShowTimeStamps(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_ALWAYS_SHOW_TIMESTAMPS_KEY, false)
    }

    /**
     * Tells if animated image attachments should automatically play their animation in the timeline.
     *
     * @return true if animated image attachments should automatically play their animation in the timeline
     */
    fun autoplayAnimatedImages(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_AUTOPLAY_ANIMATED_IMAGES, false)
    }

    /**
     * Tells if the typing notifications should be sent.
     *
     * @return true to send the typing notifs
     */
    fun sendTypingNotifs(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SEND_TYPING_NOTIF_KEY, true)
    }

    /**
     * Tells of the missing notifications rooms must be displayed at left (home screen).
     *
     * @return true to move the missed notifications to the left side
     */
    fun pinMissedNotifications(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_PIN_MISSED_NOTIFICATIONS_PREFERENCE_KEY, true)
    }

    /**
     * Tells of the unread rooms must be displayed at left (home screen).
     *
     * @return true to move the unread room to the left side
     */
    fun pinUnreadMessages(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_PIN_UNREAD_MESSAGES_PREFERENCE_KEY, true)
    }

    /**
     * Tells if the phone must vibrate when mentioning.
     *
     * @return true
     */
    fun vibrateWhenMentioning(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_VIBRATE_ON_MENTION_KEY, false)
    }

    /**
     * Tells if a dialog has been displayed to ask to use the analytics tracking (piwik, matomo, etc.).
     *
     * @return true if a dialog has been displayed to ask to use the analytics tracking
     */
    fun didAskToUseAnalytics(): Boolean {
        return defaultPrefs.getBoolean(DID_ASK_TO_USE_ANALYTICS_TRACKING_KEY, false)
    }

    /**
     * To call if the user has been asked for analytics tracking.
     *
     */
    fun setDidAskToUseAnalytics() {
        defaultPrefs.edit {
            putBoolean(DID_ASK_TO_USE_ANALYTICS_TRACKING_KEY, true)
        }
    }

    /**
     * Tells if the user wants to see URL previews in the timeline.
     *
     * @return true if the user wants to see URL previews in the timeline
     */
    fun showUrlPreviews(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SHOW_URL_PREVIEW_KEY, true)
    }

    /**
     * Tells if media should be previewed before sending.
     *
     * @return true to preview media
     */
    fun previewMediaWhenSending(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_PREVIEW_MEDIA_BEFORE_SENDING_KEY, false)
    }

    /**
     * Tells if message should be send by pressing enter on the soft keyboard.
     *
     * @return true to send message with enter
     */
    fun sendMessageWithEnter(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SEND_MESSAGE_WITH_ENTER, false)
    }

    /**
     * Tells if the emoji keyboard button should be visible or not.
     *
     * @return true to show emoji keyboard button.
     */
    fun showEmojiKeyboard(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SHOW_EMOJI_KEYBOARD, true)
    }

    /**
     * Tells if the timeline messages should be shown in a bubble or not.
     *
     * @return true to show timeline message in bubble.
     */
    fun useMessageBubblesLayout(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_INTERFACE_BUBBLE_KEY, getDefault(im.vector.app.config.R.bool.settings_interface_bubble_default))
    }

    /**
     * Tells if user should always appear offline or not.
     *
     * @return true if user should always appear offline
     */
    fun userAlwaysAppearsOffline(): Boolean {
        return defaultPrefs.getBoolean(
                SETTINGS_PRESENCE_USER_ALWAYS_APPEARS_OFFLINE,
                getDefault(im.vector.app.config.R.bool.settings_presence_user_always_appears_offline_default)
        )
    }

    /**
     * Update the rage shake enabled status.
     *
     * @param isEnabled true to enable rage shake.
     */
    fun setRageshakeEnabled(isEnabled: Boolean) {
        defaultPrefs.edit {
            putBoolean(SETTINGS_USE_RAGE_SHAKE_KEY, isEnabled)
        }
    }

    /**
     * Tells if the rage shake is used.
     *
     * @return true if the rage shake is used
     */
    fun useRageshake(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_USE_RAGE_SHAKE_KEY, true)
    }

    /**
     * Get the rage shake sensitivity.
     */
    fun getRageshakeSensitivity(): Int {
        return defaultPrefs.getInt(SETTINGS_RAGE_SHAKE_DETECTION_THRESHOLD_KEY, ShakeDetector.SENSITIVITY_MEDIUM)
    }

    /**
     * Tells if all the events must be displayed ie even the redacted events.
     *
     * @return true to display all the events even the redacted ones.
     */
    fun displayAllEvents(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_DISPLAY_ALL_EVENTS_KEY, false)
    }

    /**
     * The user does not allow screenshots of the application.
     */
    fun useFlagSecure(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SECURITY_USE_FLAG_SECURE, false)
    }

    /** Whether the keyboard should disable personalized learning. */
    fun useIncognitoKeyboard(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SECURITY_INCOGNITO_KEYBOARD_PREFERENCE_KEY, false)
    }

    /**
     * The user enable protecting app access with pin code.
     * Currently we use the pin code store to know if the pin is enabled, so this is not used
     */
    fun useFlagPinCode(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SECURITY_USE_PIN_CODE_FLAG, false)
    }

    fun setUseBiometricToUnlock(value: Boolean) {
        defaultPrefs.edit { putBoolean(SETTINGS_SECURITY_USE_BIOMETRICS_FLAG, value) }
    }

    fun useBiometricsToUnlock(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SECURITY_USE_BIOMETRICS_FLAG, true)
    }

    fun useGracePeriod(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_SECURITY_USE_GRACE_PERIOD_FLAG, true)
    }

    fun chatEffectsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_ENABLE_CHAT_EFFECTS, true)
    }

    fun directShareEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_ENABLE_DIRECT_SHARE, true)
    }

    /**
     * Return true if Pin code is disabled, or if user set the settings to see full notification content.
     */
    fun useCompleteNotificationFormat(): Boolean {
        return !useFlagPinCode() ||
                defaultPrefs.getBoolean(SETTINGS_SECURITY_USE_COMPLETE_NOTIFICATIONS_FLAG, true)
    }

    fun backgroundSyncTimeOut(): Int {
        return tryOrNull {
            // The xml pref is saved as a string so use getString and parse
            defaultPrefs.getString(SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY, null)?.toInt()
        } ?: BackgroundSyncMode.DEFAULT_SYNC_TIMEOUT_SECONDS
    }

    fun setBackgroundSyncTimeout(timeInSecond: Int) {
        defaultPrefs
                .edit()
                .putString(SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY, timeInSecond.toString())
                .apply()
    }

    fun backgroundSyncDelay(): Int {
        return tryOrNull {
            // The xml pref is saved as a string so use getString and parse
            defaultPrefs.getString(SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY, null)?.toInt()
        } ?: BackgroundSyncMode.DEFAULT_SYNC_DELAY_SECONDS
    }

    fun setBackgroundSyncDelay(timeInSecond: Int) {
        defaultPrefs
                .edit()
                .putString(SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY, timeInSecond.toString())
                .apply()
    }

    fun isBackgroundSyncEnabled(): Boolean {
        return getFdroidSyncBackgroundMode() != BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_DISABLED
    }

    fun setFdroidSyncBackgroundMode(mode: BackgroundSyncMode) {
        defaultPrefs
                .edit()
                .putString(SETTINGS_FDROID_BACKGROUND_SYNC_MODE, mode.name)
                .apply()
    }

    fun getFdroidSyncBackgroundMode(): BackgroundSyncMode {
        return try {
            val strPref = defaultPrefs
                    .getString(SETTINGS_FDROID_BACKGROUND_SYNC_MODE, BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_BATTERY.name)
            BackgroundSyncMode.values().firstOrNull { it.name == strPref } ?: BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_BATTERY
        } catch (e: Throwable) {
            BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_BATTERY
        }
    }

    fun labsAutoReportUISI(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_AUTO_REPORT_UISI, false)
    }

    fun setLabsAutoReportUISI(enabled: Boolean) {
        return defaultPrefs.edit {
            putBoolean(SETTINGS_LABS_AUTO_REPORT_UISI, enabled)
        }
    }

    fun prefSpacesShowAllRoomInHome(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_PREF_SPACE_SHOW_ALL_ROOM_IN_HOME, false)
    }

    /*
     * Photo / video picker
     */
    fun getTakePhotoVideoMode(): Int {
        return defaultPrefs.getInt(TAKE_PHOTO_VIDEO_MODE, TAKE_PHOTO_VIDEO_MODE_ALWAYS_ASK)
    }

    fun setTakePhotoVideoMode(mode: Int) {
        return defaultPrefs.edit {
            putInt(TAKE_PHOTO_VIDEO_MODE, mode)
        }
    }

    fun labsEnableLiveLocation(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ENABLE_LIVE_LOCATION, false)
    }

    fun setLiveLocationLabsEnabled(isEnabled: Boolean) {
        defaultPrefs.edit {
            putBoolean(SETTINGS_LABS_ENABLE_LIVE_LOCATION, isEnabled)
        }
    }

    fun labsEnableElementCallPermissionShortcuts(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ENABLE_ELEMENT_CALL_PERMISSION_SHORTCUTS, false)
    }

    /**
     * Indicates whether or not thread messages are enabled.
     */
    fun areThreadMessagesEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ENABLE_THREAD_MESSAGES, getDefault(im.vector.app.config.R.bool.settings_labs_thread_messages_default))
    }

    /**
     * Manually sets thread messages enabled, useful for migrating users from io.element.thread.
     */
    fun setThreadMessagesEnabled() {
        defaultPrefs
                .edit()
                .putBoolean(SETTINGS_LABS_ENABLE_THREAD_MESSAGES, true)
                .apply()
    }

    /**
     * Indicates whether or not user changed threads flag manually. We need this to not force flag to be enabled on app start.
     * Should be removed when Threads flag will be removed
     */
    fun wasThreadFlagChangedManually(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_THREAD_MESSAGES_CHANGED_BY_USER, false)
    }

    /**
     * Sets the flag to indicate that user changed threads flag (e.g. disabled them).
     */
    fun setThreadFlagChangedManually() {
        defaultPrefs
                .edit()
                .putBoolean(SETTINGS_LABS_THREAD_MESSAGES_CHANGED_BY_USER, true)
                .apply()
    }

    /**
     * Indicates whether or not the user will be notified about the new thread support.
     * We should notify the user only if he had old thread support enabled.
     */
    fun shouldNotifyUserAboutThreads(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ENABLE_THREAD_MESSAGES_OLD_CLIENTS, false)
    }

    /**
     * Indicates that the user have been notified about threads migration.
     */
    fun userNotifiedAboutThreads() {
        defaultPrefs
                .edit()
                .putBoolean(SETTINGS_LABS_ENABLE_THREAD_MESSAGES_OLD_CLIENTS, false)
                .apply()
    }

    /**
     * Indicates whether or not we should clear cache for threads migration.
     * Default value is true, for fresh installs and updates
     */
    fun shouldMigrateThreads(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_THREAD_MESSAGES_SYNCED, true)
    }

    /**
     * Indicates that there no longer threads migration needed.
     */
    fun setShouldMigrateThreads(shouldMigrate: Boolean) {
        defaultPrefs
                .edit()
                .putBoolean(SETTINGS_THREAD_MESSAGES_SYNCED, shouldMigrate)
                .apply()
    }

    /**
     * Sets the space backstack that is used for up navigation.
     * This needs to be persisted because navigating up through spaces should work across sessions.
     *
     * Only the IDs of the spaces are stored.
     */
    fun setSpaceBackstack(spaceBackstack: List<String?>) {
        val spaceIdsJoined = spaceBackstack.takeIf { it.isNotEmpty() }?.joinToString(",")
        defaultPrefs.edit().putString(SETTINGS_PERSISTED_SPACE_BACKSTACK, spaceIdsJoined).apply()
    }

    /**
     * Gets the space backstack used for up navigation.
     */
    fun getSpaceBackstack(): List<String?> {
        val spaceIdsJoined = defaultPrefs.getString(SETTINGS_PERSISTED_SPACE_BACKSTACK, null)
        return spaceIdsJoined?.takeIf { it.isNotEmpty() }?.split(",").orEmpty()
    }

    /**
     * Indicates whether or not new app layout is enabled.
     */
    fun isNewAppLayoutEnabled(): Boolean {
        return vectorFeatures.isNewAppLayoutFeatureEnabled() &&
                defaultPrefs.getBoolean(SETTINGS_LABS_NEW_APP_LAYOUT_KEY, getDefault(im.vector.app.config.R.bool.settings_labs_new_app_layout_default))
    }

    /**
     * Indicates whether or not deferred DMs are enabled.
     */
    fun isDeferredDmEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_DEFERRED_DM_KEY, getDefault(im.vector.app.config.R.bool.settings_labs_deferred_dm_default))
    }

    /**
     * Indicates whether or not new session manager screens are enabled.
     */
    fun isNewSessionManagerEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NEW_SESSION_MANAGER_KEY, getDefault(im.vector.app.config.R.bool.settings_labs_new_session_manager_default))
    }

    /**
     * Indicates whether or not client info recording is enabled.
     */
    fun isClientInfoRecordingEnabled(): Boolean {
        return defaultPrefs.getBoolean(
                SETTINGS_LABS_CLIENT_INFO_RECORDING_KEY,
                getDefault(
                        im.vector.app.config.R.bool.settings_labs_client_info_recording_default
                )
        )
    }

    fun showLiveSenderInfo(): Boolean {
        return defaultPrefs.getBoolean(
                SETTINGS_TIMELINE_SHOW_LIVE_SENDER_INFO,
                getDefault(im.vector.app.config.R.bool.settings_timeline_show_live_sender_info_default)
        )
    }

    fun isRichTextEditorEnabled(): Boolean {
        return defaultPrefs.getBoolean(
                SETTINGS_LABS_RICH_TEXT_EDITOR_KEY,
                getDefault(im.vector.app.config.R.bool.settings_labs_rich_text_editor_default)
        )
    }

    fun isVoiceBroadcastEnabled(): Boolean {
        return vectorFeatures.isVoiceBroadcastEnabled() &&
                defaultPrefs.getBoolean(
                        SETTINGS_LABS_VOICE_BROADCAST_KEY,
                        getDefault(im.vector.app.config.R.bool.settings_labs_enable_voice_broadcast_default)
                )
    }

    fun isJumpToDateEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_JUMP_TO_DATE_KEY, true)
    }

    fun isJumpToSourceEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_JUMP_TO_SOURCE_KEY, false)
    }

    fun isJumpToSourceReactionsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_JUMP_TO_SOURCE_REACTIONS, false)
    }

    fun isJumpToSourceRepliesEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_JUMP_TO_SOURCE_REPLIES, false)
    }

    fun isJumpToSourceEditsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_JUMP_TO_SOURCE_EDITS, false)
    }

    fun isChatExportEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_CHAT_EXPORT_KEY, false)
    }

    fun isSkipAvatarCropEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SKIP_AVATAR_CROP_KEY, false)
    }

    fun isTranslateEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_TRANSLATE_KEY, false)
    }

    fun getTranslateApiEndpoint(): String {
        return defaultPrefs.getString(SETTINGS_TRANSLATE_API_ENDPOINT, "") ?: ""
    }

    fun getTranslateApiToken(): String {
        return defaultPrefs.getString(SETTINGS_TRANSLATE_API_TOKEN, "") ?: ""
    }

    fun getTranslateTargetLanguage(): String {
        return defaultPrefs.getString(SETTINGS_TRANSLATE_TARGET_LANGUAGE, "") ?: ""
    }

    fun isTorEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_TOR_KEY, false)
    }

    fun isI2PEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_I2P_KEY, false)
    }

    fun isYggdrasilEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_YGGDRASIL_KEY, false)
    }

    fun isAccountExportEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ACCOUNT_EXPORT_KEY, false)
    }

    fun isAudioStatusbarEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_AUDIO_STATUSBAR_KEY, false)
    }

    fun isMediaFilterEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_MEDIA_FILTER_KEY, false)
    }

    fun isExpiredFilesEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_EXPIRED_FILES_KEY, false)
    }

    fun isSendOriginalSizeEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SEND_ORIGINAL_SIZE_KEY, true)
    }

    fun isSkipPreviewGenerationEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SKIP_PREVIEW_KEY, false)
    }

    fun isBlockRemoteImagesEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_BLOCK_REMOTE_IMAGES_KEY, false)
    }

    fun isKeywordFilterEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_KEYWORD_FILTER_KEY, false)
    }

    fun isNetworkStatsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NETWORK_STATS_KEY, false)
    }

    fun isMasqueradeEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_MASQUERADE_KEY, false)
    }

    fun isUserMaskEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_USER_MASK_KEY, false)
    }

    fun isChunkedUploadEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_CHUNKED_UPLOAD_KEY, false)
    }

    fun isCustomTimezoneEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_CUSTOM_TIMEZONE_KEY, false)
    }

    fun isStripExifEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_STRIP_EXIF_KEY, false)
    }

    fun isHideInvitationEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_HIDE_INVITATION_KEY, false)
    }

    fun isAllThreadsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ALL_THREADS_KEY, false)
    }

    fun isUserMessagesEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_USER_MESSAGES_KEY, false)
    }

    fun isRoomVersionSelectEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ROOM_VERSION_KEY, false)
    }

    fun isExpandedPreviewEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_EXPANDED_PREVIEW_KEY, false)
    }

    fun isRamStatusbarEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_RAM_STATUSBAR_KEY, false)
    }

    fun isCacheManagerEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_CACHE_MANAGER_KEY, false)
    }

    fun isAllMessagesEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ALL_MESSAGES_KEY, false)
    }

    fun isRoomInfoEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ROOM_INFO_KEY, false)
    }

    fun isAutoTranslateEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_AUTO_TRANSLATE_KEY, false)
    }

    fun isDeletedArchiveEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_DELETED_ARCHIVE_KEY, false)
    }

    fun isE2ESearchEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_E2E_SEARCH_KEY, false)
    }

    fun isModuleUpdatesEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_MODULE_UPDATES_KEY, false)
    }

    fun isNotifKeywordsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NOTIF_KEYWORDS_KEY, false)
    }

    fun isReactionPreviewEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_REACTION_PREVIEW_KEY, false)
    }

    fun isRoomMirrorEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ROOM_MIRROR_KEY, false)
    }

    fun isSymbolBarEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SYMBOL_BAR_KEY, false)
    }

    fun isAutoReplaceEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_AUTO_REPLACE_KEY, false)
    }

    fun isLlmSlashEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_LLM_SLASH_KEY, false)
    }

    fun isExtendedViaParamsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_EXTENDED_VIA_PARAMS_KEY, false)
    }

    fun getViaParamCount(): Int {
        val count = defaultPrefs.getString(SETTINGS_LABS_VIA_PARAM_COUNT_KEY, "3")
        return count?.toIntOrNull() ?: 3
    }

    fun isIncludeHistoricalServersEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_INCLUDE_HISTORICAL_SERVERS_KEY, false)
    }

    fun isEditStackingEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_EDIT_STACKING_KEY, false)
    }

    fun isShowEditCountEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SHOW_EDIT_COUNT_KEY, false)
    }

    fun isEmojiAttackProtectionEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_EMOJI_ATTACK_PROTECTION_KEY, false)
    }

    fun getEmojiMaxCount(): Int {
        val count = defaultPrefs.getString(SETTINGS_LABS_EMOJI_MAX_COUNT_KEY, "50")
        return count?.toIntOrNull() ?: 50
    }

    fun getMediaCollapseThreshold(): Int {
        val count = defaultPrefs.getString(SETTINGS_LABS_MEDIA_COLLAPSE_THRESHOLD_KEY, "10")
        return count?.toIntOrNull() ?: 10
    }

    fun isShowHiddenEventsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SHOW_HIDDEN_EVENTS_PREFERENCE_KEY, false)
    }

    fun isShowDebugInfoOnScreenEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_DEVELOPER_MODE_SHOW_INFO_ON_SCREEN_KEY, false)
    }

    fun isFailFastEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_DEVELOPER_MODE_FAIL_FAST_PREFERENCE_KEY, false)
    }

    fun isOfflineCacheEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_OFFLINE_CACHE_KEY, false)
    }

    fun isJumpToUnreadTimeEnabled(): Boolean {
        return defaultPrefs.getBoolean("SETTINGS_LABS_JUMP_TO_UNREAD_TIME", false)
    }

    fun isLoadingProgressEnabled(): Boolean {
        return defaultPrefs.getBoolean("SETTINGS_LABS_LOADING_PROGRESS", false)
    }

    fun isDuplicateNamesEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_DUPLICATE_NAMES_KEY, false)
    }

    fun isReadReceiptsCppEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_READ_RECEIPTS_KEY, false)
    }

    fun getReadReceiptsMaxVisible(): Int {
        return defaultPrefs.getInt(SETTINGS_READ_RECEIPTS_MAX_VISIBLE, 20)
    }

    fun isRoomAnalyticsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ROOM_ANALYTICS_KEY, false)
    }

    fun isUserHideEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_USER_HIDE_KEY, false)
    }

    fun isModerationEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_MODERATION_KEY, false)
    }

    fun isAutoScrollEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_AUTO_SCROLL_KEY, false)
    }

    fun isMsgQueueEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_MSG_QUEUE_KEY, false)
    }

    fun isLosslessCropEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_LOSSLESS_CROP_KEY, false)
    }

    fun isLangDetectEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_LANG_DETECT_KEY, false)
    }

    fun isChatPushDownEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_CHAT_PUSHDOWN_KEY, false)
    }

    fun isEmojiBlacklistEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_EMOJI_BLACKLIST_KEY, false)
    }

    fun toggleEmojiBlacklist() {
        val current = isEmojiBlacklistEnabled()
        defaultPrefs.edit { putBoolean(SETTINGS_LABS_EMOJI_BLACKLIST_KEY, !current) }
    }

    fun isAvatarHistoryEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_AVATAR_HISTORY_KEY, false)
    }

    fun isJumpToRoomEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_JUMPTOROOM_KEY, false)
    }

    fun isEventLinkResolveEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_EVENT_LINK_RESOLVE_KEY, false)
    }

    fun isSecondsTimestampsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SECONDS_TIMESTAMPS_KEY, false)
    }

    fun isLightweightCallEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_LIGHTWEIGHT_CALL_KEY, false)
    }

    fun isScheduledEditsEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SCHEDULED_EDITS_KEY, false)
    }

    fun isSvgDrawEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SVG_DRAW_KEY, false)
    }

    fun isProfileSwiperEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_PROFILE_SWIPER_KEY, false)
    }

    fun isDesyncDetectorEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_DESYNC_DETECTOR_KEY, false)
    }

    fun getDesyncIntervalMinutes(): Int {
        return defaultPrefs.getInt(SETTINGS_DESYNC_INTERVAL_MINUTES, 30)
    }

    fun isLocationSharingEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_LOCATION_SHARING_KEY, false)
    }

    fun isRoomCountEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ROOM_COUNT_KEY, false)
    }

    fun isRoomCountSplitEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ROOM_COUNT_SPLIT_KEY, false)
    }

    fun isRoomCountUniqueEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ROOM_COUNT_UNIQUE_KEY, false)
    }

    fun isRoomNumberingEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_ROOM_NUMBERING_KEY, false)
    }

    fun isMultiServerExportEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_MULTI_SERVER_EXPORT_KEY, false)
    }

    fun isNativeHttpEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NATIVE_HTTP_KEY, false)
    }

    fun isNativeTimelineEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NATIVE_TIMELINE_KEY, false)
    }

    fun isNativeDbEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NATIVE_DB_KEY, false)
    }

    fun isNativeSyncParserEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NATIVE_SYNC_PARSER_KEY, false)
    }

    fun isNativeCryptoEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NATIVE_CRYPTO_KEY, false)
    }

    fun isNativeMarkdownEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NATIVE_MARKDOWN_KEY, false)
    }

    fun isNativeWidgetManagerEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_NATIVE_WIDGET_MANAGER_KEY, false)
    }

    fun isFullAvatarEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_FULL_AVATAR_KEY, false)
    }

    fun isAvatarOriginalRatioEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_AVATAR_ORIGINAL_RATIO, true)
    }

    fun isWebSearchEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_WEB_SEARCH_KEY, false)
    }

    fun isAgentWebAccessEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_AGENT_WEB_ACCESS_KEY, false)
    }

    fun getNotificationIconStyle(): String {
        return defaultPrefs.getString(SETTINGS_NOTIFICATION_ICON_STYLE, "element") ?: "element"
    }

    fun setNotificationIconStyle(style: String) {
        defaultPrefs.edit { putString(SETTINGS_NOTIFICATION_ICON_STYLE, style) }
    }

    fun isNotificationShowCountEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_NOTIFICATION_SHOW_COUNT, false)
    }

    fun isLiveDraftEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_LIVE_DRAFT_KEY, false)
    }

    fun isTransparentOverlayEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_TRANSPARENT_OVERLAY_KEY, false)
    }

    fun isTextUndoEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_TEXT_UNDO_KEY, false)
    }

    fun getUndoMaxDepth(): Int = defaultPrefs.getInt("SETTINGS_UNDO_MAX_DEPTH", 50)
    fun isUndoCheckpointOnPaste(): Boolean = defaultPrefs.getBoolean("SETTINGS_UNDO_CHECKPOINT_PASTE", true)
    fun isUndoCheckpointOnSelectAll(): Boolean = defaultPrefs.getBoolean("SETTINGS_UNDO_CHECKPOINT_SELECT_ALL", true)
    fun isUndoRestoreCursor(): Boolean = defaultPrefs.getBoolean("SETTINGS_UNDO_RESTORE_CURSOR", true)

    fun isSpoilerEnabled(): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_LABS_SPOILER_KEY, false)
    }

    // Transparent Overlay config parameters
    fun getOverlayOneFingerHoldMs(): Int = defaultPrefs.getInt("SETTINGS_OVERLAY_ONE_FINGER_MS", 200)
    fun getOverlayTwoFingerHoldMs(): Int = defaultPrefs.getInt("SETTINGS_OVERLAY_TWO_FINGER_MS", 1000)
    fun getOverlayForegroundDurationMs(): Int = defaultPrefs.getInt("SETTINGS_OVERLAY_FG_DURATION_MS", 2000)
    fun getOverlayForegroundExtendedMs(): Int = defaultPrefs.getInt("SETTINGS_OVERLAY_FG_EXTENDED_MS", 3000)
    fun isOverlayOneFingerEnabled(): Boolean = defaultPrefs.getBoolean("SETTINGS_OVERLAY_ONE_FINGER", true)
    fun isOverlayTwoFingerEnabled(): Boolean = defaultPrefs.getBoolean("SETTINGS_OVERLAY_TWO_FINGER", true)
    fun isOverlayBackEnabled(): Boolean = defaultPrefs.getBoolean("SETTINGS_OVERLAY_BACK", true)
    fun isOverlaySwipeEnabled(): Boolean = defaultPrefs.getBoolean("SETTINGS_OVERLAY_SWIPE", true)

    fun getLiveDraftCharThreshold(): Int {
        return defaultPrefs.getInt(SETTINGS_LIVE_DRAFT_CHAR_THRESHOLD, 20)
    }

    fun getLiveDraftUpdateIntervalMs(): Int {
        return defaultPrefs.getInt(SETTINGS_LIVE_DRAFT_UPDATE_INTERVAL_MS, 3000)
    }

    fun showIpAddressInSessionManagerScreens(): Boolean {
        return defaultPrefs.getBoolean(
                SETTINGS_SESSION_MANAGER_SHOW_IP_ADDRESS,
                getDefault(im.vector.app.config.R.bool.settings_session_manager_show_ip_address)
        )
    }

    fun setIpAddressVisibilityInDeviceManagerScreens(isVisible: Boolean) {
        defaultPrefs.edit {
            putBoolean(SETTINGS_SESSION_MANAGER_SHOW_IP_ADDRESS, isVisible)
        }
    }

    fun getUnverifiedSessionsAlertLastShownMillis(deviceId: String): Long {
        return defaultPrefs.getLong(SETTINGS_UNVERIFIED_SESSIONS_ALERT_LAST_SHOWN_MILLIS + deviceId, 0)
    }

    fun setUnverifiedSessionsAlertLastShownMillis(deviceId: String, lastShownMillis: Long) {
        defaultPrefs.edit {
            putLong(SETTINGS_UNVERIFIED_SESSIONS_ALERT_LAST_SHOWN_MILLIS + deviceId, lastShownMillis)
        }
    }

    fun isNewLoginAlertShownForDevice(deviceId: String): Boolean {
        return defaultPrefs.getBoolean(SETTINGS_NEW_LOGIN_ALERT_SHOWN_FOR_DEVICE + deviceId, false)
    }

    fun setNewLoginAlertShownForDevice(deviceId: String) {
        defaultPrefs.edit {
            putBoolean(SETTINGS_NEW_LOGIN_ALERT_SHOWN_FOR_DEVICE + deviceId, true)
        }
    }

    fun hadExistingLegacyData(): Boolean {
        return defaultPrefs.getBoolean(HAD_EXISTING_LEGACY_DATA, false)
    }

    fun setHadExistingLegacyData(had: Boolean) {
        defaultPrefs.edit {
            putBoolean(HAD_EXISTING_LEGACY_DATA, had)
        }
    }

    fun isOnRustCrypto(): Boolean {
        return defaultPrefs.getBoolean(IS_ON_RUST_CRYPTO, false)
    }

    fun setIsOnRustCrypto(boolean: Boolean) {
        defaultPrefs.edit {
            putBoolean(IS_ON_RUST_CRYPTO, boolean)
        }
    }

    fun didAskLegacyInitSync() {
        defaultPrefs.edit().putBoolean("DID_ASK_LEGACY_INIT_SYNC", true).apply()
    }

    fun didAskOptimizedInitSync() {
        defaultPrefs.edit().putBoolean("DID_ASK_OPTIMIZED_INIT_SYNC", true).apply()
    }
}
