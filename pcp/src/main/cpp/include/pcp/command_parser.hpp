#pragma once
#include <string>

namespace pcp {

// Parses a slash command from a text message.
// Returns a JSON string with the parsed result.
// The JSON has a "type" field indicating the result kind.
//
// Result types:
//   {"type":"error_not_a_command"}
//   {"type":"error_empty_slash_command"}
//   {"type":"error_unknown","cmd":"/foo"}
//   {"type":"error_syntax","cmd":"PLAIN"}
//   {"type":"error_threads","cmd":"BAN_USER"}
//   {"type":"send_plain_text","msg":"hello"}
//   {"type":"send_formatted_text","msg":"hello","fmt":"<b>hello</b>"}
//   {"type":"send_emote","msg":"waves"}
//   {"type":"send_rainbow","msg":"hello"}
//   {"type":"send_rainbow_emote","msg":"hello"}
//   {"type":"send_spoiler","msg":"secret"}
//   {"type":"send_shrug","msg":"extra text"}
//   {"type":"send_lenny","msg":"extra text"}
//   {"type":"send_tableflip","msg":"extra text"}
//   {"type":"send_chat_effect","effect":"CONFETTI","msg":"yay"}
//   {"type":"change_display_name","name":"Alice"}
//   {"type":"change_display_name_room","name":"Alice"}
//   {"type":"change_room_name","name":"NewName"}
//   {"type":"change_topic","topic":"hello"}
//   {"type":"change_room_avatar","url":"mxc://..."}
//   {"type":"change_avatar_room","url":"mxc://..."}
//   {"type":"set_markdown","enable":true}
//   {"type":"invite","user":"@a:b","reason":"come"}
//   {"type":"invite_3pid_email","email":"a@b.c"}
//   {"type":"invite_3pid_msisdn","phone":"+123"}
//   {"type":"join_room","alias":"#room:h","reason":"invited"}
//   {"type":"part_room","alias":"#room:h"}  // null if no alias
//   {"type":"ban_user","user":"@a:b","reason":"spam"}
//   {"type":"unban_user","user":"@a:b","reason":"sorry"}
//   {"type":"remove_user","user":"@a:b","reason":"bye"}
//   {"type":"ignore_user","user":"@a:b"}
//   {"type":"unignore_user","user":"@a:b"}
//   {"type":"set_power","user":"@a:b","level":50}  // level can be null
//   {"type":"whois","user":"@a:b"}
//   {"type":"devtools"}
//   {"type":"clear_scalar_token"}
//   {"type":"discard_session"}
//   {"type":"create_space","name":"MySpace","invitees":["@a:b","@c:d"]}
//   {"type":"add_to_space","space_id":"!abc:h"}
//   {"type":"join_space","id":"#alias:h"}
//   {"type":"leave_room","room_id":"!abc:h"}
//   {"type":"upgrade_room","version":"10"}
//
std::string parseSlashCommand(const std::string& textMessage,
                               const std::string& formattedMessage,
                               bool isInThreadTimeline,
                               bool developerMode);

} // namespace pcp
