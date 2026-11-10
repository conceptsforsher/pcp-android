/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.format

import chat.pcp.app.ActiveSessionDataSource
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.features.roomprofile.permissions.RoleFormatter
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.native.PcpNative
import org.json.JSONObject
import org.matrix.android.sdk.api.session.events.model.Event
import org.matrix.android.sdk.api.session.events.model.toModel
import org.matrix.android.sdk.api.session.room.model.RoomNameContent
import org.matrix.android.sdk.api.session.room.model.RoomTopicContent
import org.matrix.android.sdk.api.session.room.model.RoomAvatarContent
import org.matrix.android.sdk.api.session.room.model.RoomMemberContent
import org.matrix.android.sdk.api.session.room.model.call.CallInviteContent
import org.matrix.android.sdk.api.session.room.timeline.TimelineEvent
import timber.log.Timber
import javax.inject.Inject

/**
 * Formats Matrix room notice events into displayable strings.
 * Formatting logic is in C++ (notice_event_formatter.cpp).
 */
class NoticeEventFormatter @Inject constructor(
        private val activeSessionDataSource: ActiveSessionDataSource,
        private val roomHistoryVisibilityFormatter: RoomHistoryVisibilityFormatter,
        private val roleFormatter: RoleFormatter,
        private val pcpPreferences: PcpBasePreferences,
        private val sp: StringProvider
) {

    private val currentUserId: String?
        get() = activeSessionDataSource.currentValue?.orNull()?.myUserId

    fun format(timelineEvent: TimelineEvent, isDm: Boolean): CharSequence? {
        val event = timelineEvent.root
        val senderName = timelineEvent.senderInfo.disambiguatedDisplayName
        val isMe = event.senderId != null && event.senderId == currentUserId
        return tryNative(event, senderName, isDm, isMe)
    }

    fun format(event: Event, senderName: String?, isDm: Boolean): CharSequence? {
        val isMe = event.senderId != null && event.senderId == currentUserId
        return tryNative(event, senderName ?: "", isDm, isMe)
    }

    fun formatRedactedEvent(event: Event): String {
        val reason = (event.unsignedData?.redactedEvent?.content?.get("reason") as? String)
                ?.takeIf { it.isNotBlank() }
        return if (reason == null) {
            if (event.senderId == currentUserId) {
                sp.getString(chat.pcp.lib.strings.CommonStrings.event_redacted_by_user_reason)
            } else {
                sp.getString(chat.pcp.lib.strings.CommonStrings.event_redacted_by_admin_reason)
            }
        } else {
            if (event.senderId == currentUserId) {
                sp.getString(chat.pcp.lib.strings.CommonStrings.event_redacted_by_user_reason_with_reason, reason)
            } else {
                sp.getString(chat.pcp.lib.strings.CommonStrings.event_redacted_by_admin_reason_with_reason, reason)
            }
        }
    }

    private fun tryNative(event: Event, senderName: String, isDm: Boolean, isMe: Boolean): CharSequence? {
        return try {
            val json = PcpNative.nativeFormatNoticeEvent(
                toEventJson(event), senderName, isDm, isMe, currentUserId ?: ""
            )
            val obj = JSONObject(json)
            if (obj.getString("type") == "null") return null
            val key = obj.getString("key")
            val paramsArr = obj.getJSONArray("params")
            val params = Array(paramsArr.length()) { paramsArr.getString(it) }
            val stringId = try {
                chat.pcp.lib.strings.CommonStrings::class.java.getField(key).getInt(null)
            } catch (e: Exception) { 0 }
            if (stringId != 0) sp.getString(stringId, *params) else null
        } catch (e: Exception) {
            null
        }
    }

    private fun toEventJson(event: Event): String {
        val obj = JSONObject()
        val type = event.getClearType()
        obj.put("type", type)
        obj.put("redacted", event.isRedacted())
        obj.put("senderId", event.senderId ?: "")
        try {
            when (type) {
                "m.room.name" -> obj.put("name", event.content?.toModel<RoomNameContent>()?.name ?: "")
                "m.room.topic" -> obj.put("topic", event.content?.toModel<RoomTopicContent>()?.topic ?: "")
                "m.room.avatar" -> obj.put("avatarUrl", event.content?.toModel<RoomAvatarContent>()?.avatarUrl ?: "")
                "m.room.member" -> {
                    val c = event.content?.toModel<RoomMemberContent>()
                    obj.put("membership", c?.membership?.name ?: "")
                    obj.put("displayName", c?.displayName ?: "")
                }
                "m.call.invite" -> obj.put("isVideo", event.content?.toModel<CallInviteContent>()?.isVideo() ?: false)
            }
        } catch (_: Exception) {}
        (event.content?.get("body") as? String)?.let { obj.put("body", it) }
        (event.content?.get("msgtype") as? String)?.let { obj.put("msgType", it) }
        return obj.toString()
    }
}
