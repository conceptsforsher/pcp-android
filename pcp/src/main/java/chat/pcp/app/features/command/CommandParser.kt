/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.command

import chat.pcp.app.core.extensions.isMsisdn
import chat.pcp.app.features.home.room.detail.ChatEffect
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.native.PcpNative
import org.json.JSONArray
import org.json.JSONObject
import org.matrix.android.sdk.api.MatrixPatterns
import org.matrix.android.sdk.api.session.identity.ThreePid
import javax.inject.Inject

class CommandParser @Inject constructor(
        private val pcpPreferences: PcpBasePreferences
) {
    /**
     * Convert the text message into a Slash command.
     * Parsing logic is in C++ (command_parser.cpp).
     */
    fun parseSlashCommand(textMessage: CharSequence, formattedMessage: String?, isInThreadTimeline: Boolean): ParsedCommand {
        val json = PcpNative.nativeParseSlashCommand(
            text = textMessage.toString(),
            formatted = formattedMessage ?: "",
            isThread = isInThreadTimeline,
            devMode = pcpPreferences.developerMode()
        )
        val obj = JSONObject(json)
        return when (obj.getString("type")) {
            "error_not_a_command" -> ParsedCommand.ErrorNotACommand
            "error_empty_slash_command" -> ParsedCommand.ErrorEmptySlashCommand
            "error_unknown" -> ParsedCommand.ErrorUnknownSlashCommand(obj.getString("cmd"))
            "error_syntax" -> {
                val cmdName = obj.getString("cmd")
                ParsedCommand.ErrorSyntax(findCommandByName(cmdName))
            }
            "error_threads" -> {
                val cmdName = obj.getString("cmd")
                ParsedCommand.ErrorCommandNotSupportedInThreads(findCommandByName(cmdName))
            }
            "send_plain_text" -> ParsedCommand.SendPlainText(obj.getString("msg"))
            "send_formatted_text" -> ParsedCommand.SendFormattedText(obj.getString("msg"), obj.getString("fmt"))
            "send_emote" -> ParsedCommand.SendEmote(obj.getString("msg"))
            "send_rainbow" -> ParsedCommand.SendRainbow(obj.getString("msg"))
            "send_rainbow_emote" -> ParsedCommand.SendRainbowEmote(obj.getString("msg"))
            "send_spoiler" -> ParsedCommand.SendSpoiler(obj.getString("msg"))
            "send_shrug" -> ParsedCommand.SendShrug(obj.optString("msg", ""))
            "send_lenny" -> ParsedCommand.SendLenny(obj.optString("msg", ""))
            "send_tableflip" -> ParsedCommand.SendTableFlip(obj.optString("msg", ""))
            "send_chat_effect" -> {
                val effect = when (obj.getString("effect")) {
                    "CONFETTI" -> ChatEffect.CONFETTI
                    "SNOWFALL" -> ChatEffect.SNOWFALL
                    else -> ChatEffect.CONFETTI
                }
                ParsedCommand.SendChatEffect(effect, obj.optString("msg", ""))
            }
            "change_display_name" -> ParsedCommand.ChangeDisplayName(obj.getString("name"))
            "change_display_name_room" -> ParsedCommand.ChangeDisplayNameForRoom(obj.getString("name"))
            "change_room_name" -> ParsedCommand.ChangeRoomName(obj.getString("name"))
            "change_topic" -> ParsedCommand.ChangeTopic(obj.getString("topic"))
            "change_room_avatar" -> ParsedCommand.ChangeRoomAvatar(obj.getString("url"))
            "change_avatar_room" -> ParsedCommand.ChangeAvatarForRoom(obj.getString("url"))
            "set_markdown" -> ParsedCommand.SetMarkdown(obj.getBoolean("enable"))
            "invite" -> {
                val userId = obj.getString("user")
                val reason = if (obj.isNull("reason")) null else obj.getString("reason")
                if (!MatrixPatterns.isUserId(userId)) {
                    ParsedCommand.ErrorSyntax(Command.INVITE)
                } else {
                    ParsedCommand.Invite(userId, reason)
                }
            }
            "invite_3pid_email" -> ParsedCommand.Invite3Pid(ThreePid.Email(obj.getString("email")))
            "invite_3pid_msisdn" -> {
                val phone = obj.getString("phone")
                if (phone.isMsisdn()) {
                    ParsedCommand.Invite3Pid(ThreePid.Msisdn(phone))
                } else {
                    ParsedCommand.ErrorSyntax(Command.INVITE)
                }
            }
            "join_room" -> {
                val alias = obj.getString("alias")
                val reason = if (obj.isNull("reason")) null else obj.getString("reason")
                ParsedCommand.JoinRoom(alias, reason)
            }
            "part_room" -> {
                val alias = if (obj.isNull("alias")) null else obj.getString("alias")
                ParsedCommand.PartRoom(alias)
            }
            "ban_user" -> {
                val user = obj.getString("user")
                val reason = if (obj.isNull("reason")) null else obj.getString("reason")
                ParsedCommand.BanUser(user, reason)
            }
            "unban_user" -> {
                val user = obj.getString("user")
                val reason = if (obj.isNull("reason")) null else obj.getString("reason")
                ParsedCommand.UnbanUser(user, reason)
            }
            "remove_user" -> {
                val user = obj.getString("user")
                val reason = if (obj.isNull("reason")) null else obj.getString("reason")
                ParsedCommand.RemoveUser(user, reason)
            }
            "ignore_user" -> ParsedCommand.IgnoreUser(obj.getString("user"))
            "unignore_user" -> ParsedCommand.UnignoreUser(obj.getString("user"))
            "set_power" -> {
                val user = obj.getString("user")
                val level = if (obj.isNull("level")) null else obj.getInt("level")
                ParsedCommand.SetUserPowerLevel(user, level)
            }
            "whois" -> ParsedCommand.ShowUser(obj.getString("user"))
            "devtools" -> ParsedCommand.DevTools
            "clear_scalar_token" -> ParsedCommand.ClearScalarToken
            "discard_session" -> ParsedCommand.DiscardSession
            "create_space" -> {
                val name = obj.getString("name")
                val arr = obj.getJSONArray("invitees")
                val invitees = (0 until arr.length()).map { arr.getString(it) }
                ParsedCommand.CreateSpace(name, invitees)
            }
            "add_to_space" -> ParsedCommand.AddToSpace(obj.getString("space_id"))
            "join_space" -> ParsedCommand.JoinSpace(obj.getString("id"))
            "leave_room" -> ParsedCommand.LeaveRoom(obj.optString("room_id", ""))
            "upgrade_room" -> ParsedCommand.UpgradeRoom(obj.getString("version"))
            "crash_app" -> throw RuntimeException("Application crashed from user demand")
            else -> ParsedCommand.ErrorUnknownSlashCommand(obj.optString("cmd", "/"))
        }
    }

    private fun findCommandByName(name: String): Command {
        return Command.values().firstOrNull { it.name == name } ?: Command.PLAIN
    }
}
