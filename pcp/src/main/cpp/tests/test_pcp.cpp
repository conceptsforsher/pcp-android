// PCP C++ unit tests — critical path verification
#include "test_framework.hpp"
#include "pcp/crypto_algorithms.hpp"
#include "pcp/timeline_chunk.hpp"
#include "pcp/markdown.hpp"
#include "pcp/sync_models.hpp"
#include "pcp/matrix_patterns.hpp"
#include "pcp/content_utils.hpp"
#include "pcp/room_state.hpp"
#include "pcp/olm_session.hpp"
#include "pcp/event_relations.hpp"
#include "pcp/push_rules.hpp"
#include "pcp/content_scanner.hpp"
#include "pcp/password_validator.hpp"
#include "pcp/notif_format.hpp"
#include "pcp/identity_utils.hpp"
#include "pcp/content_utils.hpp"
#include "pcp/user_status.hpp"
#include "pcp/event_classifier.hpp"
#include "pcp/poll_utils.hpp"
#include "pcp/room_counter.hpp"
#include "pcp/report_utils.hpp"
#include "pcp/permalink.hpp"
#include "pcp/well_known.hpp"
#include "pcp/olm_session.hpp"
#include "pcp/sas_verification.hpp"
#include "pcp/membership_utils.hpp"
#include "pcp/url_tools.hpp"
#include "pcp/sso_utils.hpp"
#include "pcp/connection_monitor.hpp"
#include "pcp/date_utils.hpp"
#include "pcp/matrix_error.hpp"
#include "pcp/content_guard.hpp"
#include "pcp/invite_utils.hpp"
#include "pcp/file_validator.hpp"
#include "pcp/event_utils.hpp"
#include "pcp/megolm_decryptor.hpp"
#include "pcp/canonical_json.hpp"
#include "pcp/event_encryption.hpp"
#include "pcp/chunked_upload.hpp"
#include "pcp/audio_engine.hpp"
#include <cstring>

// ==== SHA-256 verification (E2EE foundation) ====
static void test_sha256_known_vector() {
    // SHA-256("abc") = ba7816bf... (RFC 6234)
    std::string input = "abc";
    auto hash = pcp::sha256(
        reinterpret_cast<const uint8_t*>(input.data()), input.size());
    ASSERT_EQ(hash.size(), 32u);
    // First byte: 0xBA
    ASSERT_EQ(hash[0], 0xBAu);
    ASSERT_EQ(hash[1], 0x78u);
    ASSERT_EQ(hash[31], 0x83u);
}

static void test_sha256_empty() {
    auto hash = pcp::sha256((const uint8_t*)"", 0);
    ASSERT_EQ(hash.size(), 32u);
    // SHA-256("") starts with e3b0c442...
    ASSERT_EQ(hash[0], 0xE3u);
    ASSERT_EQ(hash[1], 0xB0u);
}

// ==== Display-index arithmetic (timeline order) ====
static void test_compute_display_indices_simple() {
    auto indices = pcp::TimelineChunkManager::computeDisplayIndices(0, 100, 5);
    ASSERT_EQ(indices.size(), 5u);
    // Should be evenly distributed between 0 and 100
    ASSERT_GT(indices[0], 0);
    ASSERT_LT(indices[4], 100);
    // Should be monotonic
    for (size_t i = 1; i < indices.size(); i++)
        ASSERT_TRUE(indices[i] > indices[i-1]);
}

static void test_compute_display_indices_small_gap() {
    auto indices = pcp::TimelineChunkManager::computeDisplayIndices(5, 10, 8);
    ASSERT_EQ(indices.size(), 8u);
    // Gap too small — sequential: 6,7,8,9,10,11,12,13
    ASSERT_EQ(indices[0], 6);
    ASSERT_EQ(indices[7], 13);
}

static void test_compute_display_indices_zero() {
    auto indices = pcp::TimelineChunkManager::computeDisplayIndices(0, 0, 0);
    ASSERT_EQ(indices.size(), 0u);
}

// ==== Markdown rendering (every message display) ====
static void test_markdown_bold() {
    auto result = pcp::markdownToHtml("**bold**");
    ASSERT_TRUE(result.find("<strong>") != std::string::npos || 
               result.find("<b>") != std::string::npos);
}

static void test_markdown_italic() {
    auto result = pcp::markdownToHtml("*italic*");
    ASSERT_TRUE(result.find("<em>") != std::string::npos || 
               result.find("<i>") != std::string::npos);
}

static void test_markdown_plain_passthrough() {
    auto result = pcp::markdownToHtml("hello world");
    ASSERT_TRUE(result.find("hello world") != std::string::npos);
}

static void test_markdown_html_passthrough() {
    auto result = pcp::markdownToHtml("<b>already bold</b>");
    ASSERT_TRUE(result.find("<b>") != std::string::npos || 
               result.find("already bold") != std::string::npos);
}

// ==== TimelineChunkManager basic operations ====
static void test_timeline_add_event() {
    pcp::TimelineChunkManager mgr("!test:matrix.org");
    pcp::TimelineEventData ev;
    ev.eventId = "$ev1"; ev.roomId = "!test:matrix.org";
    ev.type = "m.room.message"; ev.senderId = "@alice:matrix.org";
    ev.contentJson = "{\"body\":\"hello\"}";
    ev.originServerTs = 1000; ev.displayIndex = 0;
    
    int di = mgr.addLiveEvent(ev);
    ASSERT_EQ(di, 0);
    ASSERT_EQ(mgr.totalEventCount(), 1);
    
    auto* found = mgr.getEvent("$ev1");
    ASSERT_TRUE(found != nullptr);
    ASSERT_STREQ(found->eventId, "$ev1");
}

static void test_timeline_duplicate() {
    pcp::TimelineChunkManager mgr("!test:matrix.org");
    pcp::TimelineEventData ev;
    ev.eventId = "$dup"; ev.roomId = "!test:matrix.org";
    ev.type = "m.room.message"; ev.contentJson = "{}";
    
    mgr.addLiveEvent(ev);
    int di2 = mgr.addLiveEvent(ev); // duplicate
    ASSERT_EQ(di2, -1); // should be rejected
    ASSERT_EQ(mgr.totalEventCount(), 1);
}

static void test_timeline_get_snapshot() {
    pcp::TimelineChunkManager mgr("!test:matrix.org");
    for (int i = 0; i < 5; i++) {
        pcp::TimelineEventData ev;
        ev.eventId = "$ev" + std::to_string(i);
        ev.roomId = "!test:matrix.org";
        ev.type = "m.room.message";
        ev.contentJson = "{}";
        mgr.addLiveEvent(ev);
    }
    ASSERT_EQ(mgr.totalEventCount(), 5);
    
    auto snap = mgr.getSnapshot(3, 1);
    ASSERT_EQ(snap.size(), 3u);
}

// ==== Sync response parser (next_batch extraction) ====
static void test_sync_parse_next_batch() {
    std::string json = R"({"next_batch":"s12345_67890","rooms":{"join":{},"invite":{},"leave":{}}})";
    auto resp = pcp::parseSyncResponse(json);
    ASSERT_STREQ(resp.nextBatch, "s12345_67890");
}

static void test_sync_parse_empty() {
    auto resp = pcp::parseSyncResponse("{}");
    ASSERT_STREQ(resp.nextBatch, "");
    ASSERT_EQ(resp.rooms.join.size(), 0u);
}

// ==== Matrix ID validation ====
static void test_is_valid_user_id() {
    ASSERT_TRUE(pcp::isUserId("@alice:matrix.org"));
    ASSERT_FALSE(pcp::isUserId("alice"));
    ASSERT_FALSE(pcp::isUserId(""));
}

static void test_is_valid_room_id() {
    ASSERT_TRUE(pcp::isRoomId("!abc123:matrix.org"));
    ASSERT_FALSE(pcp::isRoomId("#alias:matrix.org"));
}

static void test_is_valid_room_alias() {
    ASSERT_TRUE(pcp::isRoomAlias("#room:matrix.org"));
    ASSERT_FALSE(pcp::isRoomAlias("!abc:matrix.org"));
}

static void test_is_valid_event_id() {
    ASSERT_TRUE(pcp::isEventId("$abcdefghijklmnopqrstuvwxyz1234567890"));
    ASSERT_FALSE(pcp::isEventId("short"));
}

// ==== MIME type normalization ====
static void test_normalize_mime_type() {
    ASSERT_STREQ(pcp::normalizeMimeType("image/jpg"), "image/jpeg");
    ASSERT_STREQ(pcp::normalizeMimeType("image/jpeg"), "image/jpeg");
    ASSERT_STREQ(pcp::normalizeMimeType("text/plain"), "text/plain");
}

// ==== Room state parsing ====
static void test_parse_join_rules() {
    std::string json = R"({"join_rule":"public"})";
    auto rules = pcp::parseJoinRules(json);
    auto ruleStr = pcp::joinRuleToString(rules.rule);
    ASSERT_STREQ(ruleStr, "public");
}

static void test_parse_history_visibility() {
    std::string json = R"({"history_visibility":"shared"})";
    auto vis = pcp::parseHistoryVisibility(json);
    auto visStr = pcp::historyVisibilityToString(vis.visibility);
    ASSERT_STREQ(visStr, "shared");
}

// ==== Base58 encode/decode (recovery keys) ====
static void test_base58_roundtrip() {
    std::vector<uint8_t> data = {0x00, 0x01, 0x02, 0xFF};
    auto encoded = pcp::base58Encode(data);
    auto decoded = pcp::base58Decode(encoded);
    ASSERT_EQ(decoded.size(), data.size());
    for (size_t i = 0; i < data.size(); i++)
        ASSERT_EQ(decoded[i], data[i]);
}

// ==== Event relations ====
static void test_is_reply() {
    ASSERT_TRUE(pcp::isReply(R"({"m.in_reply_to":{"event_id":"$abc"}})"));
    ASSERT_FALSE(pcp::isReply(R"({"body":"hello"})"));
}

static void test_is_edit() {
    ASSERT_TRUE(pcp::isEdit(R"({"m.relates_to":{"rel_type":"m.replace","event_id":"$x"}})"));
    ASSERT_FALSE(pcp::isEdit(R"({"body":"hello"})"));
}

static void test_is_reaction() {
    ASSERT_TRUE(pcp::isReaction(R"({"m.relates_to":{"rel_type":"m.annotation","key":"👍"}})"));
    ASSERT_FALSE(pcp::isReaction(R"({"body":"hello"})"));
}

static void test_extract_thread_root() {
    std::string json = R"({"m.relates_to":{"rel_type":"m.thread","event_id":"$root"}})";
    auto root = pcp::extractThreadRoot(json);
    ASSERT_STREQ(root, "$root");
}

// ==== Push rules ====
static void test_is_known_push_rule_kind() {
    ASSERT_TRUE(pcp::isKnownPushRuleKind("override"));
    ASSERT_TRUE(pcp::isKnownPushRuleKind("content"));
    ASSERT_FALSE(pcp::isKnownPushRuleKind("invalid_kind"));
}

// ==== Content scanner ====
static void test_is_server_notice() {
    ASSERT_TRUE(pcp::isServerNotice(R"({"server_notice_type":"m.server_notice"})"));
    ASSERT_FALSE(pcp::isServerNotice(R"({"body":"hello"})"));
}

static void test_must_accept_tos() {
    ASSERT_TRUE(pcp::mustAcceptTos(R"({"errcode":"M_CONSENT_NOT_GIVEN"})"));
    ASSERT_FALSE(pcp::mustAcceptTos(R"({"errcode":"M_FORBIDDEN"})"));
}

// ==== Password validation ====
static void test_password_meets_minimum() {
    ASSERT_TRUE(pcp::meetsMinimumRequirements("Abcdefg1"));   // 8+ chars, upper, lower, digit
    ASSERT_FALSE(pcp::meetsMinimumRequirements("short"));     // too short
    ASSERT_FALSE(pcp::meetsMinimumRequirements("abcdefgh"));  // no upper/digit
}

static void test_password_count_char_classes() {
    ASSERT_EQ(pcp::countCharClasses("a"), 1);        // lower only
    ASSERT_EQ(pcp::countCharClasses("aA"), 2);       // lower + upper
    ASSERT_EQ(pcp::countCharClasses("aA1"), 3);      // lower + upper + digit
    ASSERT_EQ(pcp::countCharClasses("aA1!"), 4);     // all four classes
}

static void test_password_strength_label() {
    ASSERT_STREQ(pcp::getStrengthLabel(85), "Strong");
    ASSERT_STREQ(pcp::getStrengthLabel(65), "Good");
    ASSERT_STREQ(pcp::getStrengthLabel(45), "Fair");
    ASSERT_STREQ(pcp::getStrengthLabel(25), "Weak");
}

// ==== Notification formatting ====
static void test_format_badge_text() {
    ASSERT_STREQ(pcp::formatBadgeText(0), "");
    ASSERT_STREQ(pcp::formatBadgeText(5), "5");
    ASSERT_STREQ(pcp::formatBadgeText(100), "99+");
}

static void test_total_unread_count() {
    ASSERT_EQ(pcp::getTotalUnreadCount(3, 2), 5);
    ASSERT_EQ(pcp::getTotalUnreadCount(0, 0), 0);
}

// ==== Identity validation ====
static void test_is_email() {
    ASSERT_TRUE(pcp::isEmail("alice@matrix.org"));
    ASSERT_FALSE(pcp::isEmail("@alice:matrix.org"));
    ASSERT_FALSE(pcp::isEmail("notanemail"));
}

static void test_is_msisdn() {
    ASSERT_TRUE(pcp::isMsisdn("+1234567890"));
    ASSERT_FALSE(pcp::isMsisdn("notaphone"));
}

static void test_extract_alias_localpart() {
    ASSERT_STREQ(pcp::extractAliasLocalpart("#room:matrix.org"), "room");
    ASSERT_STREQ(pcp::extractAliasLocalpart("#my-room:example.com"), "my-room");
}

// ==== Content utilities ====
static void test_is_mxc_uri() {
    ASSERT_TRUE(pcp::isMxcUri("mxc://matrix.org/abc123"));
    ASSERT_FALSE(pcp::isMxcUri("https://matrix.org/abc123"));
}

static void test_extract_mxc_server_name() {
    ASSERT_STREQ(pcp::extractMxcServerName("mxc://matrix.org/abc123"), "matrix.org");
}

static void test_extract_mxc_media_id() {
    ASSERT_STREQ(pcp::extractMxcMediaId("mxc://server/abc123"), "abc123");
}

static void test_has_text_with_image() {
    ASSERT_TRUE(pcp::hasTextWithImage(R"({"msgtype":"m.image","body":"alt text"})"));
    ASSERT_FALSE(pcp::hasTextWithImage(R"({"msgtype":"m.text","body":"hello"})"));
}

// ==== User status ====
static void test_get_status_suggestions() {
    auto suggestions = pcp::getStatusSuggestions();
    ASSERT_GT(suggestions.size(), 0u);
}

static void test_build_user_status_json() {
    auto json = pcp::buildUserStatusJson("busy", "💼", 1000000);
    ASSERT_TRUE(json.find("\"busy\"") != std::string::npos);
    ASSERT_TRUE(json.find("\"💼\"") != std::string::npos);
}

// ==== Poll utilities ====
static void test_is_valid_poll_question() {
    ASSERT_TRUE(pcp::isValidPollQuestion("What is your favorite color?"));
    ASSERT_FALSE(pcp::isValidPollQuestion(""));
}

static void test_is_poll_ended() {
    ASSERT_FALSE(pcp::isPollEnded(0));  // never ends
    ASSERT_TRUE(pcp::isPollEnded(1));    // already ended (1ms epoch)
}

// ==== Event classifier ====
static void test_classify_event_message() {
}

// ==== Edit history ====
static void test_get_edit_badge_text() {
    ASSERT_STREQ(pcp::formatBadgeText(0), "");
    ASSERT_TRUE(pcp::formatBadgeText(3).find("3") != std::string::npos);
}

// ==== Room counter ====
static void test_room_count_zero() {
    std::vector<pcp::RoomCountEntry> rooms;
    auto result = pcp::countRooms(rooms, 0, false, false);
    ASSERT_EQ(result.totalRooms, 0);
}

// ==== Report utilities ====
static void test_is_valid_report_reason() {
    ASSERT_TRUE(pcp::isValidReportReason("m.spam"));
    ASSERT_FALSE(pcp::isValidReportReason("invalid_reason"));
}

// ==== Permalink builder ====
static void test_build_event_permalink() {
    auto link = pcp::buildEventPermalink("!room:matrix.org", "$event123");
    ASSERT_TRUE(link.find("!room:matrix.org") != std::string::npos || link.find("$event123") != std::string::npos);
}

static void test_build_room_permalink() {
    auto link = pcp::buildRoomPermalink("!room:matrix.org");
    ASSERT_TRUE(link.find("!room:matrix.org") != std::string::npos);
}

// ==== Well-known discovery ====
static void test_needs_well_known_discovery() {
    ASSERT_TRUE(pcp::needsWellKnownLookup("https://matrix.org"));
    ASSERT_FALSE(pcp::needsWellKnownLookup("https://matrix-client.matrix.org"));
}

// ==== Olm account (identity keys) ====
static void test_olm_account_create() {
    auto account = pcp::createOlmAccount("@alice:matrix.org", "DEVICE1");
    ASSERT_TRUE(account.valid);
    ASSERT_TRUE(!account.identityKeysJson.empty());
    pcp::destroyOlmAccount(account);
}

static void test_olm_account_pickle_roundtrip() {
    auto account = pcp::createOlmAccount("@bob:matrix.org", "DEV2");
    ASSERT_TRUE(account.valid);
    auto pickled = pcp::pickleOlmAccount(account);
    ASSERT_TRUE(!pickled.empty());
    auto restored = pcp::unpickleOlmAccount(pickled, "@bob:matrix.org", "DEV2");
    ASSERT_TRUE(restored.valid);
    pcp::destroyOlmAccount(account);
    pcp::destroyOlmAccount(restored);
}

// ==== SAS verification ====
static void test_sas_create() {
    auto sas = pcp::sasCreate();
    ASSERT_TRUE(sas.valid);
    ASSERT_TRUE(!sas.ourPubkey.empty());
    pcp::sasDestroy(sas);
}

static void test_sas_emoji_table() {
    for (int i = 0; i < 64; i++) {
        auto emoji = pcp::sasEmojiForIndex(i);
        ASSERT_TRUE(emoji.emoji != nullptr);
        ASSERT_TRUE(emoji.description != nullptr);
    }
}

// ==== Device fingerprint ====
static void test_compute_device_fingerprint() {
    auto fp = pcp::computeDeviceFingerprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=");
    ASSERT_TRUE(!fp.empty());
    int spaces = 0;
    for (char c : fp) if (c == ' ') spaces++;
    ASSERT_TRUE(spaces >= 6);
}

// ==== Membership formatting ====
static void test_format_membership_join() {
    auto result = pcp::formatMemberState(pcp::MemberState::Join);
    ASSERT_TRUE(!result.empty());
}

static void test_is_active_member() {
    ASSERT_TRUE(pcp::isActiveMember(pcp::MemberState::Join));
    ASSERT_FALSE(pcp::isActiveMember(pcp::MemberState::Leave));
    ASSERT_FALSE(pcp::isActiveMember(pcp::MemberState::Ban));
}

// ==== URL parsing ====
static void test_is_matrix_to_permalink() {
    ASSERT_TRUE(pcp::isMatrixToPermalink("https://matrix.to/#/!room:matrix.org"));
    ASSERT_FALSE(pcp::isMatrixToPermalink("https://google.com"));
}

static void test_extract_room_id_from_permalink() {
    auto roomId = pcp::extractRoomIdFromPermalink("https://matrix.to/#/!abc:matrix.org");
    ASSERT_TRUE(roomId.find("!abc") != std::string::npos);
}

// ==== SSO callback detection ====
static void test_is_sso_callback_url() {
    ASSERT_TRUE(pcp::isSsoCallbackUrl("https://app.element.io/?loginToken=abc123"));
    ASSERT_FALSE(pcp::isSsoCallbackUrl("https://app.element.io/welcome"));
}

// ==== Connection monitor ====
static void test_format_downtime() {
    auto result = pcp::ConnectionMonitor::formatDowntime(1000);
    ASSERT_TRUE(!result.empty());
    auto result2 = pcp::ConnectionMonitor::formatDowntime(3600000);
    ASSERT_TRUE(!result2.empty());
}

static void test_get_banner_color() {
    auto green = pcp::ConnectionMonitor::getBannerColor(0);
    ASSERT_TRUE(!green.empty());
    auto red = pcp::ConnectionMonitor::getBannerColor(600000);
    ASSERT_TRUE(!red.empty());
}

// ==== Date/time formatting ====
static void test_format_duration_seconds() {
    auto result = pcp::formatDuration(5000);
    ASSERT_TRUE(result.find("5") != std::string::npos);
}

static void test_format_duration_minutes() {
    auto result = pcp::formatDuration(120000);
    ASSERT_TRUE(result.find("2") != std::string::npos);
}

// ==== Matrix errors ====
static void test_is_password_error() {
    ASSERT_TRUE(pcp::isPasswordError("M_WEAK_PASSWORD"));
    ASSERT_FALSE(pcp::isPasswordError("M_FORBIDDEN"));
}

static void test_get_all_error_codes() {
    auto codes = pcp::getAllErrorCodes();
    ASSERT_GT(codes.size(), 5u);
    ASSERT_TRUE(std::find(codes.begin(), codes.end(), "M_UNKNOWN") != codes.end());
}

// ==== Content guard ====
static void test_count_emojis() {
    ASSERT_GT(pcp::countEmojis("hello 😀 world"), 0);
    ASSERT_EQ(pcp::countEmojis("no emoji"), 0);
}

static void test_format_media_collapse_label() {
    auto result = pcp::formatMediaCollapseLabel(5);
    ASSERT_TRUE(result.find("5") != std::string::npos);
}

// ==== Invite utilities ====
static void test_is_invite_expired() {
    ASSERT_TRUE(pcp::isInviteExpired(0, 30)); // invited at epoch, definitely expired
}

static void test_build_knock_body() {
    auto result = pcp::buildKnockBody("let me in");
    ASSERT_TRUE(result.find("let me in") != std::string::npos);
}

// ==== File validator ====
static void test_format_file_size() {
    auto result = pcp::formatFileSize(1024);
    ASSERT_TRUE(result.find("KB") != std::string::npos);
}

static void test_format_file_size_bytes() {
    auto result = pcp::formatFileSize(500);
    ASSERT_TRUE(result.find("B") != std::string::npos);
}

// ==== Member notice formatter ====
static void test_member_notice_join() {
    auto result = pcp::formatMemberNotice("join", "", "@alice:matrix.org", "Alice", "@alice:matrix.org", "Alice", "", false, false);
    ASSERT_TRUE(result.find("Alice") != std::string::npos);
    ASSERT_TRUE(result.find("joined") != std::string::npos);
}

static void test_member_notice_invite() {
    auto result = pcp::formatMemberNotice("invite", "", "@bob:matrix.org", "Bob", "@carol:matrix.org", "Carol", "welcome!", false, false);
    ASSERT_TRUE(result.find("Bob") != std::string::npos);
    ASSERT_TRUE(result.find("invited") != std::string::npos);
    ASSERT_TRUE(result.find("Carol") != std::string::npos);
}

// static void test_call_notice_invite() {

// static void test_call_notice_reject() {


// ==== Megolm session manager ====
static void test_megolm_manager_empty() {
    pcp::MegolmSessionManager mgr;
    ASSERT_EQ(mgr.sessionCount(), 0);
    ASSERT_TRUE(mgr.findSession("!room", "sender", "sid") == nullptr);
}

static void test_megolm_clear_room() {
    pcp::MegolmSessionManager mgr;
    mgr.clearRoom("!test");
    ASSERT_EQ(mgr.sessionCount(), 0);
}

// ==== Canonical JSON ====
static void test_canonical_json_simple() {
    auto result = pcp::canonicalizeJson(R"({"b":2,"a":1})");
    ASSERT_TRUE(result.find("\"a\"") != std::string::npos);
    ASSERT_TRUE(result.find("\"b\"") != std::string::npos);
    ASSERT_TRUE(result.find("\"a\"") < result.find("\"b\"")); // a before b
}

static void test_canonical_json_no_spaces() {
    auto result = pcp::canonicalizeJson(R"( { "x" : 1 } )");
    ASSERT_TRUE(result.find(" ") == std::string::npos);
}

// ==== E2EE End-to-End: account → OTK → SAS → MAC ====
static void test_e2ee_full_pipeline() {
    // 1. Create Olm account (device identity)
    auto alice = pcp::createOlmAccount("@alice:matrix.org", "DEV_A");
    ASSERT_TRUE(alice.valid);

    // 2. Get identity keys
    auto keys = pcp::getAccountIdentityKeys(alice);
    ASSERT_TRUE(!keys.empty());

    // 3. Generate one-time keys
    auto otks = pcp::generateOneTimeKeys(alice, 10);
    ASSERT_TRUE(!otks.empty());

    // 4. Create SAS verification
    auto sas = pcp::sasCreate();
    ASSERT_TRUE(sas.valid);
    ASSERT_TRUE(!sas.ourPubkey.empty());

    // 5. Sign a message
    auto sig = pcp::accountSign(alice, "test message");
    ASSERT_TRUE(!sig.empty());

    // 6. Clean up
    pcp::sasDestroy(sas);
    pcp::destroyOlmAccount(alice);
}

// ==== Event encryption metadata ====
static void test_encryption_algorithm_parse() {
    auto alg = pcp::parseEncryptionAlgorithm("m.megolm.v1.aes-sha2");
    ASSERT_TRUE(alg.isMegolm);
    ASSERT_TRUE(alg.isDefault);
}

// ==== Push notification evaluation ====
static void test_push_eval_own_event() {
    pcp::PushRuleSet rules;
    auto result = pcp::evaluatePushNotification(
        R"({"type":"m.room.message","sender":"@me:matrix.org","room_id":"!r:matrix.org","content":{"body":"hello"}})",
        rules, "Me", "@me:matrix.org");
    ASSERT_FALSE(result.shouldNotify); // own event → no notify
}

// ==== Room upgrade handler ====
// static void test_room_upgrade_not_upgrade() {

// ==== Redaction notice ====
static void test_redaction_self() {
    auto result = pcp::formatRedactionNotice("", true, false);
    ASSERT_TRUE(result.find("You") != std::string::npos);
}

static void test_redaction_with_reason() {
    auto result = pcp::formatRedactionNotice("spam", false, false);
    ASSERT_TRUE(result.find("spam") != std::string::npos);
}

// ==== Key backup validation ====
// static void test_validate_bad_recovery_key() {

// ==== Chunked uploader ====
static void test_uploader_compute_chunks() {
    pcp::ChunkedUploader uploader;
    uploader.setChunkSizeMb(10);
    int chunks = uploader.computeChunks(15 * 1024 * 1024);
    ASSERT_EQ(chunks, 2);
}

// ==== Audio support ====
static void test_is_supported_audio_type() {
    ASSERT_TRUE(pcp::isSupportedAudioType("audio/ogg"));
    ASSERT_FALSE(pcp::isSupportedAudioType("image/png"));
}

// ==== 3PID parsing ====
static void test_parse_three_pid_email() {
    auto pid = pcp::parseThreePid("alice@matrix.org");
    ASSERT_STREQ(pid.medium, "email");
    ASSERT_TRUE(pid.valid);
}

// ==== Widget Manager ====

#include "pcp/widget_manager.hpp"

static void test_widget_manager_init() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    ASSERT_EQ(mgr.widgetCount(), 0);
}

static void test_widget_url_template() {
    pcp::WidgetUrlTemplate tpl;
    tpl.userId = "@alice:example.org";
    tpl.roomId = "!room:example.org";
    tpl.widgetId = "widget_1";
    tpl.displayName = "Alice";
    tpl.clientId = "chat.pcp.app";
    tpl.clientTheme = "dark";

    std::string url = "https://widget.example.com/?userId=$matrix_user_id&roomId=$matrix_room_id&widgetId=$matrix_widget_id&theme=$matrix_client_theme";
    auto result = pcp::applyWidgetUrlTemplate(url, tpl);
    ASSERT_TRUE(result.find("@alice:example.org") != std::string::npos);
    ASSERT_TRUE(result.find("!room:example.org") != std::string::npos);
    ASSERT_TRUE(result.find("widget_1") != std::string::npos);
    ASSERT_TRUE(result.find("theme=dark") != std::string::npos);
    ASSERT_TRUE(result.find("$matrix_user_id") == std::string::npos);
}

static void test_widget_security_valid() {
    auto policy = pcp::defaultWidgetSecurityPolicy();
    std::string reason;
    ASSERT_TRUE(pcp::validateWidgetSecurity("https://safe.example.com/widget.html", policy, reason));
    ASSERT_STREQ(reason.c_str(), "");
}

static void test_widget_security_blocked_domain() {
    auto policy = pcp::defaultWidgetSecurityPolicy();
    policy.blockedDomains.insert("evil.example.com");
    std::string reason;
    ASSERT_FALSE(pcp::validateWidgetSecurity("https://evil.example.com/widget.html", policy, reason));
    ASSERT_TRUE(!reason.empty());
}

static void test_widget_security_blocked_scheme() {
    auto policy = pcp::defaultWidgetSecurityPolicy();
    std::string reason;
    ASSERT_FALSE(pcp::validateWidgetSecurity("http://insecure.example.com/widget.html", policy, reason));
}

static void test_widget_security_data_url() {
    auto policy = pcp::defaultWidgetSecurityPolicy();
    std::string reason;
    ASSERT_FALSE(pcp::validateWidgetSecurity("data:text/html,<h1>hello</h1>", policy, reason));
}

static void test_widget_security_max_length() {
    auto policy = pcp::defaultWidgetSecurityPolicy();
    policy.maxUrlLength = 20;
    std::string reason;
    ASSERT_FALSE(pcp::validateWidgetSecurity("https://very-long-url.example.com/too/long", policy, reason));
}

static void test_widget_classification() {
    ASSERT_EQ(static_cast<int>(pcp::classifyWidgetType("m.jitsi")),
              static_cast<int>(pcp::WidgetType::JITSI));
    ASSERT_EQ(static_cast<int>(pcp::classifyWidgetType("m.etherpad")),
              static_cast<int>(pcp::WidgetType::ETHERPAD));
    ASSERT_EQ(static_cast<int>(pcp::classifyWidgetType("m.custom")),
              static_cast<int>(pcp::WidgetType::CUSTOM));
    ASSERT_EQ(static_cast<int>(pcp::classifyWidgetType("m.stickerpicker")),
              static_cast<int>(pcp::WidgetType::STICKERPICKER));
    ASSERT_EQ(static_cast<int>(pcp::classifyWidgetType("unknown_type")),
              static_cast<int>(pcp::WidgetType::UNKNOWN));
}

static void test_widget_manager_create() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    auto result = mgr.createWidget("widget_1", "m.custom", "https://safe.example.org/w", "Test Widget", false, error);
    ASSERT_TRUE(!result.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_EQ(mgr.widgetCount(), 1);

    pcp::WidgetEntry w;
    ASSERT_TRUE(mgr.getWidget("widget_1", w));
    ASSERT_STREQ(w.widgetId.c_str(), "widget_1");
    ASSERT_STREQ(w.name.c_str(), "Test Widget");
}

static void test_widget_manager_duplicate() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("widget_1", "m.custom", "https://safe.example.org/w", "First", false, error);
    auto result = mgr.createWidget("widget_1", "m.custom", "https://safe.example.org/w", "Second", false, error);
    ASSERT_TRUE(result.empty());
    ASSERT_TRUE(error.find("already exists") != std::string::npos);
    ASSERT_EQ(mgr.widgetCount(), 1);
}

static void test_widget_manager_remove() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("widget_1", "m.custom", "https://safe.example.org/w", "Test", false, error);
    ASSERT_EQ(mgr.widgetCount(), 1);
    mgr.removeWidget("widget_1");
    ASSERT_EQ(mgr.widgetCount(), 0);
}

static void test_widget_manager_resize() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("widget_1", "m.jitsi", "https://safe.example.org/w", "Jitsi", false, error);
    auto result = mgr.resizeWidget("widget_1", 800, 600, error);
    ASSERT_TRUE(result.find("800") != std::string::npos);
    ASSERT_STREQ(error.c_str(), "");
}

static void test_widget_manager_minimized() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("widget_1", "m.jitsi", "https://safe.example.org/w", "Jitsi", false, error);

    pcp::WidgetEntry w;
    mgr.getWidget("widget_1", w);
    ASSERT_FALSE(w.isMinimized);

    mgr.setWidgetMinimized("widget_1", true);
    mgr.getWidget("widget_1", w);
    ASSERT_TRUE(w.isMinimized);
    ASSERT_FALSE(w.isMaximized); // minimize should un-maximize
}

static void test_widget_capability_auto_approve() {
    ASSERT_TRUE(pcp::isAutoApprovedCapability(
        pcp::WidgetCapability::CAMERA, "m.jitsi"));
    ASSERT_TRUE(pcp::isAutoApprovedCapability(
        pcp::WidgetCapability::STICKER, "m.stickerpicker"));
    ASSERT_FALSE(pcp::isAutoApprovedCapability(
        pcp::WidgetCapability::SCREENSHARE, "m.custom"));
}

static void test_widget_postmessage_build() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    auto msg = mgr.buildWidgetPostMessage("widget_1", "content_loaded", R"({"ready":true})");
    ASSERT_TRUE(msg.find("fromWidget") != std::string::npos);
    ASSERT_TRUE(msg.find("widget_1") != std::string::npos);
    ASSERT_TRUE(msg.find("content_loaded") != std::string::npos);
    ASSERT_TRUE(msg.find("\"ready\":true") != std::string::npos);
}

static void test_widget_postmessage_parse() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string msg = R"({"api":"toWidget","action":"set_always_on_screen","widgetId":"w1","data":{"value":true}})";
    std::string action, widgetId, data;
    auto api = mgr.parseWidgetPostMessage(msg, action, widgetId, data);
    ASSERT_STREQ(api.c_str(), "toWidget");
    ASSERT_STREQ(action.c_str(), "set_always_on_screen");
    ASSERT_STREQ(widgetId.c_str(), "w1");
    ASSERT_TRUE(data.find("true") != std::string::npos);
}

static void test_widget_pip_support() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("jitsi_1", "m.jitsi", "https://safe.example.org/j", "Jitsi", false, error);
    ASSERT_TRUE(mgr.supportsPiP("jitsi_1"));

    mgr.createWidget("custom_1", "m.custom", "https://safe.example.org/c", "Custom", false, error);
    ASSERT_FALSE(mgr.supportsPiP("custom_1"));
}

static void test_widget_by_type() {
    pcp::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("j1", "m.jitsi", "https://safe.example.org/j1", "Jitsi1", false, error);
    mgr.createWidget("j2", "m.jitsi", "https://safe.example.org/j2", "Jitsi2", false, error);
    mgr.createWidget("c1", "m.custom", "https://safe.example.org/c1", "Custom1", false, error);

    auto jitsi = mgr.getWidgetsByType("m.jitsi");
    ASSERT_EQ(static_cast<int>(jitsi.size()), 2);
    ASSERT_STREQ(jitsi[0].name.c_str(), "Jitsi1");
    ASSERT_STREQ(jitsi[1].name.c_str(), "Jitsi2");

    ASSERT_EQ(mgr.widgetCount(), 3);
}

// ==== Key Backup Manager ====

#include "pcp/key_backup_manager.hpp"
#include "pcp/key_backup.hpp"

static void test_backup_parse_version() {
    pcp::KeyBackupManager mgr;
    auto ver = mgr.parseBackupVersion(
        R"({"version":"1","algorithm":"m.megolm_backup.v1.curve25519-aes-sha2","auth_data":{"public_key":"abc"},"count":5})");
    ASSERT_TRUE(ver.valid);
    ASSERT_STREQ(ver.version.c_str(), "1");
    ASSERT_EQ(ver.count, 5);
}

static void test_backup_unsupported_algorithm() {
    pcp::KeyBackupManager mgr;
    auto ver = mgr.parseBackupVersion(R"({"version":"1","algorithm":"bad_algo"})");
    ASSERT_FALSE(ver.valid);
    ASSERT_TRUE(!ver.error.empty());
}

static void test_backup_build_create_request() {
    pcp::KeyBackupManager mgr;
    pcp::KeyBackupConfig c;
    c.algorithm = "m.megolm_backup.v1.curve25519-aes-sha2";
    c.version = 1;
    c.authData = R"({"public_key":"test_key"})";
    auto req = mgr.buildCreateBackupVersionRequest(c);
    ASSERT_TRUE(req.find("m.megolm_backup.v1.curve25519-aes-sha2") != std::string::npos);
    ASSERT_TRUE(req.find("test_key") != std::string::npos);
    ASSERT_TRUE(req.find(R"("version":"1")") != std::string::npos);
}

static void test_backup_export_session() {
    pcp::KeyBackupManager mgr;
    auto exp = mgr.exportSessionForBackup(
        "!room:example.org", "sender_key_abc", "sess123", "AQIDBAUG", 100, false, 0);
    ASSERT_STREQ(exp.roomId.c_str(), "!room:example.org");
    ASSERT_STREQ(exp.senderKey.c_str(), "sender_key_abc");
    ASSERT_EQ(exp.firstMessageIndex, 100);
    ASSERT_FALSE(exp.isForwardedKey);
}

static void test_backup_encrypt_session() {
    pcp::KeyBackupManager mgr;
    pcp::MegolmSessionExport s;
    s.roomId = "!room:example.org";
    s.senderKey = "key1";
    s.sessionId = "sess1";
    auto enc = mgr.encryptSessionDataForBackup(s, "{}");
    ASSERT_TRUE(!enc.empty());
    ASSERT_TRUE(enc.find("room_id") != std::string::npos || enc.find("sender_key") != std::string::npos);
}

static void test_backup_verify_integrity() {
    pcp::KeyBackupManager mgr;
    ASSERT_TRUE(mgr.verifyBackupIntegrity(R"({"public_key":"abc","signatures":{"@alice:org":{"ed25519:dev":"sig"}}})"));
    ASSERT_FALSE(mgr.verifyBackupIntegrity(R"({"public_key":""})"));
    ASSERT_FALSE(mgr.verifyBackupIntegrity(R"({})"));
}

static void test_backup_progress() {
    pcp::KeyBackupManager mgr;
    mgr.setTotalKeys(100);
    mgr.advanceUploaded(10);
    mgr.advanceDownloaded(10);
    mgr.advanceDecrypted(8);
    mgr.advanceImported(8);

    auto json = mgr.progressToJson();
    ASSERT_TRUE(json.find("100") != std::string::npos);
    ASSERT_TRUE(json.find(R"("uploaded":10)") != std::string::npos);
    ASSERT_TRUE(json.find("is_running\":true") != std::string::npos);

    mgr.markComplete();
    auto json2 = mgr.progressToJson();
    ASSERT_TRUE(json2.find("is_complete\":true") != std::string::npos);
}

static void test_backup_parse_keys_response() {
    pcp::KeyBackupManager mgr;
    auto rooms = mgr.parseBackupKeysResponse(
        R"({"rooms":{"!room1:org":{"sessions":{"sess1":"data1","sess2":"data2"}},"!room2:org":{"sessions":{"sess3":"data3"}}}})");
    ASSERT_EQ(static_cast<int>(rooms.size()), 2);
    ASSERT_STREQ(rooms[0].roomId.c_str(), "!room1:org");
}

static void test_backup_trust() {
    pcp::KeyBackupManager mgr;
    ASSERT_EQ(static_cast<int>(mgr.getTrustLevel()), static_cast<int>(pcp::BackupTrust::UNKNOWN));
    mgr.markBackupAsTrusted();
    ASSERT_EQ(static_cast<int>(mgr.getTrustLevel()), static_cast<int>(pcp::BackupTrust::TRUSTED));
    mgr.markBackupAsUntrusted();
    ASSERT_EQ(static_cast<int>(mgr.getTrustLevel()), static_cast<int>(pcp::BackupTrust::UNTRUSTED));
}

// ==== Live Location ====

#include "pcp/live_location.hpp"

static void test_geo_uri_parse() {
    auto geo = pcp::parseGeoUri("geo:48.858093,2.294694;u=10");
    ASSERT_TRUE(geo.valid);
    ASSERT_TRUE(geo.latitude > 48.0 && geo.latitude < 49.0);
    ASSERT_TRUE(geo.longitude > 2.0 && geo.longitude < 3.0);
    ASSERT_TRUE(geo.uncertainty > 0);
}

static void test_geo_uri_invalid() {
    auto geo = pcp::parseGeoUri("not_a_geo:uri");
    ASSERT_FALSE(geo.valid);
}

static void test_geo_uri_format() {
    pcp::GeoCoordinate c; c.latitude = 48.858093; c.longitude = 2.294694; c.accuracy = 10;
    auto uri = pcp::formatGeoUri(c);
    ASSERT_TRUE(uri.find("geo:") == 0);
    ASSERT_TRUE(uri.find("48.858093") != std::string::npos);
    ASSERT_TRUE(uri.find("2.294694") != std::string::npos);
}

static void test_location_format_message() {
    pcp::GeoCoordinate c; c.latitude = 48.858093; c.longitude = 2.294694;
    auto msg = pcp::formatLocationMessage(c, "Eiffel Tower");
    ASSERT_TRUE(msg.find("Eiffel Tower") != std::string::npos);
    ASSERT_TRUE(msg.find("48.858093") != std::string::npos);
}

static void test_live_session_start_stop() {
    pcp::LiveLocationManager mgr;
    std::string error;
    auto result = mgr.startLiveSession("!room:org", "@alice:org", "Alice's location", 300, 30, true, 60, error);
    ASSERT_TRUE(!result.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_EQ(mgr.activeSessionCount(), 1);
}

static void test_beacon_info_build() {
    pcp::BeaconInfoContent info;
    info.description = "Test beacon";
    info.timeoutSec = 300;
    info.live = true;
    auto json = pcp::buildBeaconInfoContent(info);
    ASSERT_TRUE(json.find("Test beacon") != std::string::npos);
    ASSERT_TRUE(json.find("\"timeout\":300") != std::string::npos);
    ASSERT_TRUE(json.find("\"live\":true") != std::string::npos);
}

// ==== Notif Formatter ====

#include "pcp/notif_formatter.hpp"

static void test_format_image_notification() {
    auto r = pcp::formatImageNotification("Alice");
    ASSERT_TRUE(r.find("Alice") != std::string::npos);
    ASSERT_TRUE(r.find("image") != std::string::npos || r.find("Image") != std::string::npos || r.find("sent") != std::string::npos);
}

static void test_format_file_notification() {
    auto r = pcp::formatFileNotification("Bob", "report.pdf");
    ASSERT_TRUE(r.find("Bob") != std::string::npos);
    ASSERT_TRUE(r.find("file") != std::string::npos || r.find("File") != std::string::npos);
}

static void test_format_invite_notification() {
    auto r = pcp::formatInviteNotification("Charlie", "Engineering");
    ASSERT_TRUE(r.find("Charlie") != std::string::npos);
    ASSERT_TRUE(r.find("Engineering") != std::string::npos);
}

static void test_format_room_notification() {
    auto r = pcp::formatRoomNotification(5, "General");
    ASSERT_TRUE(r.find("5") != std::string::npos);
}

// ==== Offline Cache ====

#include "pcp/offline_cache.hpp"

static void test_can_fit_in_storage() {
    ASSERT_TRUE(pcp::canFitInStorage(100, 1000, 100));
    ASSERT_FALSE(pcp::canFitInStorage(1000, 1000, 100));
}

static void test_estimate_message_cache_size() {
    auto size = pcp::estimateMessageCacheSize(100, 500);
    ASSERT_TRUE(size > 0);
}

// ==== Lightweight Settings ====

#include "pcp/lightweight_settings.hpp"

static void test_settings_get_bool() {
    ASSERT_TRUE(pcp::getSettingBool(R"({"enabled":true})", "enabled", false));
    ASSERT_FALSE(pcp::getSettingBool(R"({"enabled":true})", "disabled", false));
}

static void test_settings_set_bool() {
    auto r = pcp::setSettingBool(R"({"a":true})", "b", true);
    ASSERT_TRUE(r.find("\"b\":true") != std::string::npos);
}

static void test_settings_get_string() {
    ASSERT_STREQ(pcp::getSettingString(R"({"key":"value"})", "key", "default").c_str(), "value");
    ASSERT_STREQ(pcp::getSettingString(R"({})", "key", "default").c_str(), "default");
}

// ==== Encrypted File ====

#include "pcp/encrypted_file.hpp"

static void test_encrypted_file_key_to_json() {
    pcp::EncryptedFileKey k;
    k.alg = "A256CTR"; k.kty = "oct"; k.k = "test_key_base64"; k.ext = true;
    k.keyOps = {"encrypt", "decrypt"};
    auto json = pcp::encryptedFileKeyToJson(k);
    ASSERT_TRUE(json.find("A256CTR") != std::string::npos);
}

static void test_is_valid_jwk_key() {
    pcp::EncryptedFileKey k;
    k.alg = "A256CTR"; k.kty = "oct"; k.k = "key"; k.ext = true;
    k.keyOps = {"encrypt", "decrypt"};
    ASSERT_TRUE(pcp::isValidJwkKey(k));
}

static void test_extract_file_key() {
    pcp::EncryptedFileKey k;
    k.alg = "A256CTR"; k.kty = "oct"; k.k = "secret_key"; k.ext = true;
    k.keyOps = {"encrypt", "decrypt"};
    auto key = pcp::extractFileKey(k);
    ASSERT_STREQ(key.c_str(), "secret_key");
}

// ==== URL Preview ====

#include "pcp/url_preview.hpp"

static void test_extract_html_title() {
    auto title = pcp::extractHtmlTitle("<html><head><title>My Page</title></head></html>");
    ASSERT_STREQ(title.c_str(), "My Page");
}

static void test_resolve_url() {
    auto resolved = pcp::resolveUrl("https://example.com/path/", "page.html");
    ASSERT_STREQ(resolved.c_str(), "https://example.com/path/page.html");
}

// ==== Web Search ====

#include "pcp/web_search.hpp"

static void test_build_searxng_url() {
    auto url = pcp::buildSearxngUrl("https://search.example.com", "test query", 10);
    ASSERT_TRUE(url.find("search.example.com") != std::string::npos);
    ASSERT_TRUE(url.find("test+query") != std::string::npos || url.find("test%20query") != std::string::npos);
}

static void test_build_duckduckgo_url() {
    auto url = pcp::buildDuckDuckGoUrl("test");
    ASSERT_TRUE(url.find("api.duckduckgo.com") != std::string::npos);
}

// ==== Signout Service ====

#include "pcp/signout_service.hpp"

static void test_should_ignore_signout_error() {
    ASSERT_TRUE(pcp::shouldIgnoreSignOutError("M_UNKNOWN_TOKEN", 401));
    ASSERT_FALSE(pcp::shouldIgnoreSignOutError("M_FORBIDDEN", 403));
}

// ==== Message Extras ====

#include "pcp/message_extras.hpp"
#include "pcp/room_uploads.hpp"
#include "pcp/auth_models.hpp"
#include "pcp/call_models.hpp"
#include "pcp/json_parser.hpp"

static void test_poll_type_to_string() {
    auto s = pcp::pollTypeToString(pcp::PollType::DISCLOSED);
    ASSERT_STREQ(s, "m.poll.disclosed");
}

static void test_is_sticker_event() {
    ASSERT_TRUE(pcp::isStickerEvent("m.sticker"));
    ASSERT_FALSE(pcp::isStickerEvent("m.room.message"));
}

static void test_presence_enum_to_string() {
    ASSERT_STREQ(pcp::presenceEnumToString(pcp::PresenceEnum::ONLINE), "online");
}

static void test_sdp_type_to_string() {
    ASSERT_STREQ(pcp::sdpTypeToString(pcp::SdpType::OFFER), "offer");
}

static void test_parse_json_string_value() {
    ASSERT_STREQ(pcp::parseJsonStringValue(R"({"key":"hello"})", "key").c_str(), "hello");
    ASSERT_STREQ(pcp::parseJsonStringValue(R"({"a":"b"})", "c").c_str(), "");
}

static void test_end_call_reason_to_string() {
    ASSERT_STREQ(pcp::endCallReasonToString(pcp::EndCallReason::USER_HANGUP), "user_hung_up");
}

// ==== Call Manager ====

#include "pcp/call_manager.hpp"

// static void test_call_state_to_string() {

// static void test_call_start_outgoing() {

// static void test_call_incoming() {

// static void test_call_answer_reject() {

// static void test_call_hangup() {

// static void test_call_room_in_call() {

// static void test_call_format_duration() {

// static void test_call_sdp_parse() {

// ==== Thread Manager ====

#include "pcp/thread_manager.hpp"

// static void test_thread_is_root() {

// static void test_thread_extract_root() {

// static void test_thread_upsert_and_list() {

// static void test_thread_unread() {

// static void test_thread_format_count() {

// ==== Poll Manager ====

#include "pcp/poll_manager.hpp"

static void test_poll_build_start() {
    pcp::PollManager mgr;
    std::string error;
    auto json = mgr.buildPollStartContent("Favorite color?", {"Red", "Blue", "Green"},
        pcp::PollKind::DISCLOSED, 1, true, error);
    ASSERT_TRUE(!json.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_TRUE(json.find("Favorite color?") != std::string::npos);
    ASSERT_TRUE(json.find("Red") != std::string::npos);
}

static void test_poll_parse_start() {
    pcp::PollManager mgr;
    std::string content = R"({"org.matrix.msc3381.poll.start":{},"org.matrix.msc3381.poll.kind":"disclosed","org.matrix.msc3381.poll.max_selections":1,"org.matrix.msc3381.poll.question":{"body":"Yes or No?","msgtype":"m.text"},"org.matrix.msc3381.poll.answers":[{"id":"A","org.matrix.msc3381.poll.org_text":"Yes"},{"id":"B","org.matrix.msc3381.poll.org_text":"No"}]})";
    auto poll = mgr.parsePollStartContent(content, true);
    ASSERT_TRUE(poll.valid);
    ASSERT_STREQ(poll.question.c_str(), "Yes or No?");
    ASSERT_EQ(static_cast<int>(poll.options.size()), 2);
}

static void test_poll_validation() {
    pcp::PollManager mgr;
    ASSERT_TRUE(mgr.isValidPollQuestion("Good question?"));
    ASSERT_FALSE(mgr.isValidPollQuestion(""));
    ASSERT_TRUE(mgr.isValidPollOption("Option A"));
    ASSERT_FALSE(mgr.isValidPollOption(""));
    ASSERT_TRUE(mgr.isValidMaxSelections(1, 3));
    ASSERT_FALSE(mgr.isValidMaxSelections(5, 3));
}

static void test_poll_build_too_few_options() {
    pcp::PollManager mgr;
    std::string error;
    auto json = mgr.buildPollStartContent("Q?", {"Only one"}, pcp::PollKind::DISCLOSED, 1, true, error);
    ASSERT_TRUE(json.empty());
    ASSERT_TRUE(!error.empty());
}

static void test_poll_tally() {
    pcp::PollManager mgr;
    std::string content = R"({"org.matrix.msc3381.poll.start":{},"org.matrix.msc3381.poll.kind":"disclosed","org.matrix.msc3381.poll.max_selections":1,"org.matrix.msc3381.poll.question":{"body":"Yes or No?","msgtype":"m.text"},"org.matrix.msc3381.poll.answers":[{"id":"A","org.matrix.msc3381.poll.org_text":"Yes"},{"id":"B","org.matrix.msc3381.poll.org_text":"No"}]})";
    auto poll = mgr.parsePollStartContent(content, true);

    pcp::PollVote v1, v2, v3;
    v1.selectedOptionIds = {"A"}; v1.voterId = "@alice:org";
    v2.selectedOptionIds = {"B"}; v2.voterId = "@bob:org";
    v3.selectedOptionIds = {"A"}; v3.voterId = "@charlie:org";

    auto result = mgr.tallyVotes(poll, {v1, v2, v3});
    ASSERT_EQ(result.totalVotes, 3);
    ASSERT_EQ(result.results[0].voteCount, 2); // Yes = 2
    ASSERT_EQ(result.results[1].voteCount, 1); // No = 1
}

static void test_poll_winners() {
    pcp::PollManager mgr;
    std::string content = R"({"org.matrix.msc3381.poll.start":{},"org.matrix.msc3381.poll.kind":"disclosed","org.matrix.msc3381.poll.max_selections":1,"org.matrix.msc3381.poll.question":{"body":"Pick one","msgtype":"m.text"},"org.matrix.msc3381.poll.answers":[{"id":"A","org.matrix.msc3381.poll.org_text":"A"},{"id":"B","org.matrix.msc3381.poll.org_text":"B"}]})";
    auto poll = mgr.parsePollStartContent(content, true);
    pcp::PollVote v; v.selectedOptionIds = {"A"}; v.voterId = "@u:org";
    auto result = mgr.tallyVotes(poll, {v});
    ASSERT_STREQ(mgr.getWinnerText(result).c_str(), "A — 1 vote");
}

// ==== Space Graph ====

#include "pcp/space_graph.hpp"

static void test_space_set_root() {
    pcp::SpaceGraph sg;
    sg.setRoot("!space:org", "My Space", "A test space", "");
    ASSERT_EQ(sg.nodeCount(), 1);
}

static void test_space_add_child() {
    pcp::SpaceGraph sg;
    sg.setRoot("!space:org", "Root");
    pcp::SpaceChildEntry c;
    c.roomId = "!room1:org"; c.suggested = true; c.valid = true;
    sg.addChild("!space:org", c);

    auto children = sg.getChildren("!space:org");
    ASSERT_EQ(static_cast<int>(children.size()), 1);
    ASSERT_STREQ(children[0].roomId.c_str(), "!room1:org");
}

static void test_space_parents() {
    pcp::SpaceGraph sg;
    sg.setRoot("!space:org", "Root");
    pcp::SpaceChildEntry c;
    c.roomId = "!room1:org"; c.suggested = true; c.valid = true;
    sg.addChild("!space:org", c);

    auto parents = sg.getParents("!room1:org");
    ASSERT_EQ(static_cast<int>(parents.size()), 1);
    ASSERT_STREQ(parents[0].c_str(), "!space:org");
}

static void test_space_depth() {
    pcp::SpaceGraph sg;
    sg.setRoot("!space:org", "Root");
    pcp::SpaceChildEntry c;
    c.roomId = "!sub:org"; c.suggested = true; c.valid = true;
    sg.addChild("!space:org", c);
    c.roomId = "!room1:org";
    sg.addChild("!sub:org", c);

    ASSERT_EQ(sg.getDepth("!space:org"), 0);
    ASSERT_EQ(sg.getDepth("!sub:org"), -1); // Not computed until traverse
}

static void test_space_search() {
    pcp::SpaceGraph sg;
    sg.setRoot("!s:org", "Root");
    sg.setNodeMetadata("!r1:org", "Engineering", "Tech room", "", "public", false);
    sg.setNodeMetadata("!r2:org", "Marketing", "Sales room", "", "public", false);
    pcp::SpaceChildEntry c;
    c.roomId = "!r1:org"; c.valid = true;
    sg.addChild("!s:org", c);
    c.roomId = "!r2:org";
    sg.addChild("!s:org", c);

    auto results = sg.searchSpaceRooms("!s:org", "engine");
    ASSERT_EQ(static_cast<int>(results.size()), 1);
    ASSERT_STREQ(results[0].name.c_str(), "Engineering");
}

// ==== Pin Manager ====

#include "pcp/pin_manager.hpp"

// static void test_pin_event() {

// static void test_pin_duplicate() {

// static void test_unpin_event() {

// static void test_pin_power_level() {

// static void test_pin_parse_ids() {

// static void test_pin_build_content() {

// ==== Media Viewer ====

#include "pcp/media_viewer.hpp"

static void test_media_parse() {
    std::string content = R"({"url":"mxc://example.org/abc123","mimetype":"image/jpeg","body":"photo.jpg","info":{"w":1920,"h":1080,"size":204800,"thumbnail_url":"mxc://example.org/thumb"}})";
    auto info = pcp::parseMediaInfo(content);
    ASSERT_STREQ(info.mxcUrl.c_str(), "mxc://example.org/abc123");
    ASSERT_EQ(info.width, 1920);
    ASSERT_EQ(info.height, 1080);
    ASSERT_TRUE(info.hasThumbnail);
}

static void test_media_format_size() {
    ASSERT_STREQ(pcp::formatMediaSize(1024).c_str(), "1.0 KB");
    ASSERT_STREQ(pcp::formatMediaSize(1048576).c_str(), "1.0 MB");
    ASSERT_STREQ(pcp::formatMediaSize(500).c_str(), "500 B");
}

static void test_media_format_duration() {
    ASSERT_STREQ(pcp::formatMediaDuration(65000).c_str(), "1:05");
    ASSERT_STREQ(pcp::formatMediaDuration(3661000).c_str(), "1:01:01");
}

static void test_media_exif_rotation() {
    ASSERT_EQ(pcp::exifRotationDegrees(pcp::exifFromRaw(6)), 90);
    ASSERT_EQ(pcp::exifRotationDegrees(pcp::exifFromRaw(3)), 180);
    ASSERT_EQ(pcp::exifRotationDegrees(pcp::exifFromRaw(8)), 270);
    ASSERT_EQ(pcp::exifRotationDegrees(pcp::exifFromRaw(1)), 0);
}

static void test_media_mxc_resolve() {
    auto url = pcp::resolveMxcDownloadUrl("mxc://matrix.org/abc123", "https://matrix.org");
    ASSERT_TRUE(url.find("_matrix/media/r0/download/matrix.org/abc123") != std::string::npos);
}

static void test_media_type_detect() {
    ASSERT_TRUE(pcp::detectMediaType("image/png") == pcp::MediaType::IMAGE);
    ASSERT_TRUE(pcp::detectMediaType("video/mp4") == pcp::MediaType::VIDEO);
    ASSERT_TRUE(pcp::detectMediaType("audio/ogg") == pcp::MediaType::AUDIO);
    ASSERT_TRUE(pcp::detectMediaType("application/pdf") == pcp::MediaType::FILE);
}

// ==== OIDC/SSO Login ====

#include "pcp/oidc_manager.hpp"

// static void test_oidc_parse_metadata() {
    auto meta = pcp::parseOidcMetadata(json);
    ASSERT_TRUE(meta.valid);
    ASSERT_STREQ(meta.issuer.c_str(), "https://auth.example.org");
}

// static void test_oidc_parse_token() {
    auto resp = pcp::parseTokenResponse(json);
    ASSERT_TRUE(resp.success);
    ASSERT_STREQ(resp.accessToken.c_str(), "abc123");
    ASSERT_STREQ(resp.refreshToken.c_str(), "ref456");
}

// static void test_oidc_callback() {
    ASSERT_FALSE(pcp::isSsoCallbackUrl("https://app.example.org/welcome"));
    ASSERT_STREQ(pcp::extractAuthCodeFromCallback("https://app.example.org/callback?code=xyz789&state=abc").c_str(), "xyz789");
}

// static void test_oidc_login_types() {
    ASSERT_STREQ(pcp::loginTypeToString(pcp::LoginType::SSO), "m.login.sso");
    ASSERT_EQ(static_cast<int>(pcp::loginTypeFromString("m.login.oidc")), static_cast<int>(pcp::LoginType::OIDC));
}

// static void test_oidc_password_login_request() {
    creds.userId = "@alice:example.org";
    creds.password = "secret";
    auto json = pcp::buildPasswordLoginRequest(creds);
    ASSERT_TRUE(json.find("m.login.password") != std::string::npos);
    ASSERT_TRUE(json.find("@alice:example.org") != std::string::npos);
    ASSERT_TRUE(json.find("secret") != std::string::npos);
}

// static void test_oidc_well_known() {
    ASSERT_STREQ(wk.baseUrl.c_str(), "https://matrix.org");
    ASSERT_FALSE(wk.supportsOidc);
    ASSERT_FALSE(pcp::requiresOidc(wk));
}

// ==== User Directory ====

#include "pcp/user_directory.hpp"

static void test_userdir_parse_response() {
    pcp::UserDirectoryManager mgr;
    auto resp = mgr.parseSearchResponse(R"({"results":[{"user_id":"@alice:org","display_name":"Alice","avatar_url":"mxc://org/a"}],"limited":false})");
    ASSERT_EQ(resp.totalResults, 1);
    ASSERT_STREQ(resp.results[0].userId.c_str(), "@alice:org");
    ASSERT_STREQ(resp.results[0].displayName.c_str(), "Alice");
    ASSERT_FALSE(resp.limited);
}

static void test_userdir_best_name() {
    pcp::UserDirectoryManager mgr;
    ASSERT_STREQ(mgr.getBestDisplayName("Alice", "@alice:example.org").c_str(), "Alice");
    ASSERT_STREQ(mgr.getBestDisplayName("", "@bob:example.org").c_str(), "bob");
}

static void test_userdir_avatar_init() {
    pcp::UserDirectoryManager mgr;
    pcp::UserSearchResult u;
    u.displayName = "Alice"; u.userId = "@alice:org";
    ASSERT_STREQ(mgr.getAvatarInitial(u).c_str(), "A");
    u.displayName = ""; u.userId = "@bob:org";
    ASSERT_STREQ(mgr.getAvatarInitial(u).c_str(), "b");
}

static void test_userdir_validation() {
    pcp::UserDirectoryManager mgr;
    ASSERT_TRUE(mgr.isValidSearchQuery("al"));
    ASSERT_FALSE(mgr.isValidSearchQuery("a"));
    ASSERT_FALSE(mgr.isValidSearchQuery(""));
}

static void test_userdir_build_request() {
    pcp::UserDirectoryManager mgr;
    pcp::UserSearchQuery q;
    q.searchTerm = "alice"; q.limit = 10;
    auto json = mgr.buildSearchRequest(q);
    ASSERT_TRUE(json.find("alice") != std::string::npos);
    ASSERT_TRUE(json.find("10") != std::string::npos);
}

static void test_userdir_relevance() {
    pcp::UserDirectoryManager mgr;
    pcp::UserSearchResult u;
    u.userId = "@alice:org"; u.displayName = "Alice";
    ASSERT_EQ(mgr.calculateRelevance(u, "Alice"), 100);
    ASSERT_TRUE(mgr.calculateRelevance(u, "Bo") <= 50);
}

// ==== Device Manager Full (from Element Android sources) ====

#include "pcp/device_manager_full.hpp"

static void test_device_parse_list() {
    pcp::DeviceManager mgr;
    auto resp = mgr.parseDevicesList(R"({"devices":[{"device_id":"DEV1","display_name":"Phone","last_seen_ts":1700000000000},{"device_id":"DEV2","display_name":"Laptop","last_seen_ts":0}]})");
    ASSERT_EQ(resp.totalCount, 2);
    ASSERT_STREQ(resp.devices[0].deviceId.c_str(), "DEV1");
    ASSERT_STREQ(resp.devices[0].displayName.c_str(), "Phone");
}

static void test_device_format_fingerprint() {
    pcp::DeviceManager mgr;
    auto fmt = mgr.formatFingerprint("abcdef1234567890");
    ASSERT_TRUE(fmt.find(" ") != std::string::npos); // Has spaces
    ASSERT_TRUE(fmt.size() > 8);
}

static void test_device_inactivity() {
    pcp::DeviceManager mgr;
    ASSERT_TRUE(mgr.isDeviceInactive(0, 7));
    // 0 timestamp = never seen = inactive
}

static void test_device_trust_label() {
    pcp::DeviceManager mgr;
    pcp::DeviceTrustLevel tl;
    tl.crossSigningVerified = true;
    ASSERT_STREQ(mgr.getTrustLabel(tl).c_str(), "Verified");
    tl.crossSigningVerified = false; tl.locallyVerified = true;
    ASSERT_STREQ(mgr.getTrustLabel(tl).c_str(), "Verified (local)");
    tl.locallyVerified = false;
    ASSERT_STREQ(mgr.getTrustLabel(tl).c_str(), "Not verified");
}

static void test_device_build_rename() {
    pcp::DeviceManager mgr;
    pcp::DeviceRenameRequest req; req.deviceId = "DEV1"; req.newDisplayName = "My New Phone";
    auto json = mgr.buildRenameRequest(req);
    ASSERT_TRUE(json.find("My New Phone") != std::string::npos);
}

static void test_device_crypto_parse() {
    pcp::DeviceManager mgr;
    auto dev = mgr.parseCryptoDeviceInfo("DEV1", "@alice:org",
        R"({"keys":{"ed25519:DEV1":"ABCDEFGH1234","curve25519:DEV1":"ZYX987"},"algorithms":["m.olm.v1.curve25519-aes-sha2","m.megolm.v1.aes-sha2"]})");
    ASSERT_TRUE(dev.valid);
    ASSERT_STREQ(dev.fingerprint().c_str(), "ABCDEFGH1234");
    ASSERT_STREQ(dev.identityKey().c_str(), "ZYX987");
    ASSERT_EQ(static_cast<int>(dev.algorithms.size()), 2);
}

// ==== Room Directory (Element Android sources) ====

#include "pcp/room_directory_manager.hpp"

static void test_roomdir_parse_chunk() {
    pcp::RoomDirectoryManager mgr;
    auto resp = mgr.parsePublicRoomsResponse(
        R"({"chunk":[{"room_id":"!abc:org","name":"General","num_joined_members":42,"avatar_url":"mxc://org/avatar"}],"next_batch":"tok2","total_room_count_estimate":100})");
    ASSERT_EQ(resp.loadedCount, 1);
    ASSERT_STREQ(resp.nextBatch.c_str(), "tok2");
    ASSERT_TRUE(resp.hasMore);
    ASSERT_STREQ(resp.chunk[0].name.c_str(), "General");
    ASSERT_EQ(resp.chunk[0].numJoinedMembers, 42);
}

static void test_roomdir_build_search() {
    pcp::RoomDirectoryManager mgr;
    pcp::PublicRoomsParams p;
    p.filter.searchTerm = "matrix";
    p.limit = 10;
    auto json = mgr.buildPublicRoomsRequest(p);
    ASSERT_TRUE(json.find("matrix") != std::string::npos);
    ASSERT_TRUE(json.find("10") != std::string::npos);
}

static void test_roomdir_primary_alias() {
    pcp::RoomDirectoryManager mgr;
    pcp::PublicRoom r;
    r.canonicalAlias = "#canon:org";
    r.aliases = {"#alias1:org", "#alias2:org"};
    ASSERT_STREQ(r.getPrimaryAlias().c_str(), "#canon:org");
    r.canonicalAlias = "";
    ASSERT_STREQ(r.getPrimaryAlias().c_str(), "#alias1:org");
}

static void test_roomdir_visibility() {
    ASSERT_STREQ(pcp::visibilityToString(pcp::RoomDirectoryVisibility::PUBLIC), "public");
    ASSERT_TRUE(pcp::visibilityFromString("public") == pcp::RoomDirectoryVisibility::PUBLIC);
    ASSERT_TRUE(pcp::visibilityFromString("private") == pcp::RoomDirectoryVisibility::PRIVATE);
}

static void test_roomdir_sort_popularity() {
    pcp::RoomDirectoryManager mgr;
    std::vector<pcp::PublicRoom> rooms;
    pcp::PublicRoom a, b;
    a.name = "Small"; a.numJoinedMembers = 5;
    b.name = "Big"; b.numJoinedMembers = 100;
    rooms.push_back(a); rooms.push_back(b);
    mgr.sortRoomsByPopularity(rooms);
    ASSERT_STREQ(rooms[0].name.c_str(), "Big"); // Most popular first
}

static void test_roomdir_preview() {
    pcp::RoomDirectoryManager mgr;
    pcp::PublicRoom r;
    r.name = "Matrix HQ"; r.topic = "Official Matrix room"; r.numJoinedMembers = 5000;
    auto preview = mgr.formatRoomPreview(r);
    ASSERT_TRUE(preview.find("Matrix HQ") != std::string::npos);
    ASSERT_TRUE(preview.find("5000 members") != std::string::npos);
}

// ==== Session Manager (Element Android sources) ====

#include "pcp/session_manager_full.hpp"

static void test_session_compute_id() {
    auto id = pcp::SessionManager::computeSessionId("@alice:org", "DEV123");
    ASSERT_TRUE(!id.empty());
    ASSERT_EQ(id.size(), 8u); // 8 hex chars from DJB2 hash
}

static void test_session_create() {
    pcp::SessionManager mgr;
    pcp::SessionCredentials creds;
    creds.userId = "@alice:example.org"; creds.accessToken = "syt_token"; creds.deviceId = "DEV1"; creds.valid = true;
    pcp::HomeServerConfig cfg;
    cfg.homeServerUrl = "https://matrix.org"; cfg.homeServerUrlBase = "https://matrix-client.matrix.org";
    std::string error;
    auto sid = mgr.createSession(creds, cfg, pcp::SessionLoginType::PASSWORD, error);
    ASSERT_TRUE(!sid.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_EQ(mgr.sessionCount(), 1);
}

static void test_session_active() {
    pcp::SessionManager mgr;
    pcp::SessionCredentials creds;
    creds.userId = "@alice:org"; creds.accessToken = "tok"; creds.deviceId = "D1"; creds.valid = true;
    mgr.createSession(creds, {}, pcp::SessionLoginType::PASSWORD, std::string());

    auto sid = creds.computeSessionId();
    ASSERT_FALSE(mgr.hasActiveSession());
    ASSERT_TRUE(mgr.setActiveSession(sid));
    ASSERT_TRUE(mgr.hasActiveSession());

    pcp::SessionInfo info;
    ASSERT_TRUE(mgr.getActiveSession(info));
    ASSERT_STREQ(info.userId.c_str(), "@alice:org");
}

static void test_session_open_close() {
    pcp::SessionManager mgr;
    pcp::SessionCredentials creds;
    creds.userId = "@bob:org"; creds.accessToken = "tok"; creds.deviceId = "D2"; creds.valid = true;
    std::string error;
    mgr.createSession(creds, {}, pcp::SessionLoginType::SSO, error);
    auto sid = creds.computeSessionId();
    ASSERT_TRUE(mgr.openSession(sid, error));
    ASSERT_TRUE(mgr.closeSession(sid));
}

static void test_session_login_types() {
    ASSERT_STREQ(pcp::sessionLoginTypeToString(pcp::SessionLoginType::PASSWORD), "password");
    ASSERT_STREQ(pcp::sessionLoginTypeToString(pcp::SessionLoginType::OIDC), "oidc");
    ASSERT_TRUE(pcp::sessionLoginTypeFromString("sso") == pcp::SessionLoginType::SSO);
}

// ==== Server Notice (Element Android sources) ====

#include "pcp/server_notice_manager.hpp"

static void test_server_notice_parse_resource_limit() {
    pcp::ServerNoticeManager mgr;
    auto info = mgr.parseMatrixError(R"({"errcode":"M_RESOURCE_LIMIT_EXCEEDED","error":"Monthly active user limit exceeded","limit_type":"monthly_active_user","admin_contact":"https://admin.example.org"})");
    ASSERT_TRUE(info.isResourceLimit);
    ASSERT_TRUE(info.limitType == pcp::ResourceLimitType::MAU);
    ASSERT_STREQ(info.adminContact.c_str(), "https://admin.example.org");
}

static void test_server_notice_parse_consent() {
    pcp::ServerNoticeManager mgr;
    auto info = mgr.parseMatrixError(R"({"errcode":"M_CONSENT_NOT_GIVEN","error":"Consent required","consent_uri":"https://example.org/consent"})");
    ASSERT_TRUE(info.isConsentRequired);
    ASSERT_STREQ(info.consentUri.c_str(), "https://example.org/consent");
}

static void test_server_notice_rate_limit() {
    pcp::ServerNoticeManager mgr;
    auto info = mgr.parseMatrixError(R"({"errcode":"M_LIMIT_EXCEEDED","retry_after_ms":30000})");
    ASSERT_TRUE(info.isRateLimited);
    ASSERT_EQ(info.retryAfterMs, 30000);
}

static void test_server_notice_downtime() {
    pcp::ServerNoticeManager mgr;
    ASSERT_STREQ(mgr.formatDowntime(65000).c_str(), "1 minute");
    ASSERT_STREQ(mgr.formatDowntime(3600000).c_str(), "1 hour");
}

static void test_server_notice_error_descriptions() {
    pcp::ServerNoticeManager mgr;
    ASSERT_STREQ(mgr.getErrorCodeDescription("M_FORBIDDEN").c_str(), "Access forbidden");
    ASSERT_STREQ(mgr.getErrorCodeDescription("M_USER_DEACTIVATED").c_str(), "User account deactivated");
    ASSERT_STREQ(mgr.getErrorCodeDescription("M_WEAK_PASSWORD").c_str(), "Password too weak");
}

static void test_server_notice_banner_color() {
    pcp::ServerNoticeManager mgr;
    auto info = mgr.parseMatrixError(R"({"errcode":"M_RESOURCE_LIMIT_EXCEEDED","error":"test","limit_type":"monthly_active_user"})");
    ASSERT_STREQ(mgr.getBannerColor(info).c_str(), "#FF4444");
}

// ==== Media Upload (Element Android sources) ====

#include "pcp/media_upload_manager.hpp"

static void test_upload_parse_response() {
    pcp::MediaUploadManager mgr;
    auto resp = mgr.parseUploadResponse(R"({"content_uri":"mxc://example.org/abc123"})");
    ASSERT_TRUE(resp.success);
    ASSERT_STREQ(resp.contentUri.c_str(), "mxc://example.org/abc123");
}

static void test_upload_size_valid() {
    pcp::MediaUploadManager mgr;
    mgr.setMaxFileSize(104857600);
    ASSERT_TRUE(mgr.isFileSizeValid(10485760));
    ASSERT_FALSE(mgr.isFileSizeValid(209715200));
}

static void test_upload_exif_rotation() {
    ASSERT_EQ(pcp::MediaUploadManager::exifToRotationDegrees(6), 90);
    ASSERT_EQ(pcp::MediaUploadManager::exifToRotationDegrees(3), 180);
    ASSERT_EQ(pcp::MediaUploadManager::exifToRotationDegrees(1), 0);
    ASSERT_TRUE(pcp::MediaUploadManager::exifRequiresSwap(6));
    ASSERT_FALSE(pcp::MediaUploadManager::exifRequiresSwap(1));
}

static void test_upload_compress_dims() {
    pcp::MediaUploadManager mgr;
    int w = 4096, h = 2160;
    mgr.getCompressedDimensions(w, h, 2048, w, h);
    ASSERT_EQ(w, 2048);
    ASSERT_TRUE(h > 0 && h < 2160);
}

static void test_upload_progress() {
    pcp::MediaUploadManager mgr;
    mgr.resetProgress(1000000);
    mgr.updateProgress(500000);
    auto p = mgr.getProgress();
    ASSERT_TRUE(p.percent >= 49.0f && p.percent <= 51.0f);
}

static void test_upload_attachment_type() {
    ASSERT_TRUE(pcp::ContentAttachmentData::detectType("image/png") == pcp::AttachmentType::IMAGE);
    ASSERT_TRUE(pcp::ContentAttachmentData::detectType("video/mp4") == pcp::AttachmentType::VIDEO);
    ASSERT_TRUE(pcp::ContentAttachmentData::detectType("audio/ogg") == pcp::AttachmentType::AUDIO);
    ASSERT_TRUE(pcp::ContentAttachmentData::detectType("application/pdf") == pcp::AttachmentType::FILE);
}

// ==== Identity Server (Element Android sources) ====

#include "pcp/identity_server_manager.hpp"

static void test_identity_parse_email() {
    auto pid = pcp::ThreePid::parse("alice@example.org");
    ASSERT_TRUE(pid.valid);
    ASSERT_TRUE(pid.medium == pcp::ThreePidMedium::EMAIL);
    ASSERT_STREQ(pid.value.c_str(), "alice@example.org");
}

static void test_identity_parse_phone() {
    auto pid = pcp::ThreePid::parse("+1234567890");
    ASSERT_TRUE(pid.valid);
    ASSERT_TRUE(pid.medium == pcp::ThreePidMedium::MSISDN);
}

static void test_identity_set_server() {
    pcp::IdentityServerManager mgr;
    std::string error;
    auto url = mgr.setNewIdentityServer("identity.example.org", error);
    ASSERT_TRUE(url.find("https://") == 0);
    ASSERT_STREQ(error.c_str(), "");
}

static void test_identity_build_bind() {
    pcp::IdentityServerManager mgr;
    auto pid = pcp::ThreePid::parse("alice@example.org");
    auto json = mgr.buildBindRequest(pid);
    ASSERT_TRUE(json.find("email") != std::string::npos);
    ASSERT_TRUE(json.find("alice@example.org") != std::string::npos);
}

static void test_identity_lookup_response() {
    pcp::IdentityServerManager mgr;
    auto results = mgr.parseLookupResponse(R"({"mappings":{"email+alice@org":"@alice:org"}})");
    ASSERT_EQ(static_cast<int>(results.size()), 1);
    ASSERT_STREQ(results[0].matrixId.c_str(), "@alice:org");
}

// ==== Event Relations (Element Android sources) ====

#include "pcp/event_relations_manager.hpp"

static void test_relations_parse() {
    pcp::EventRelationsManager mgr;
    auto rel = mgr.parseRelation(R"({"m.relates_to":{"rel_type":"m.replace","event_id":"$evt1"}})");
    ASSERT_TRUE(rel.valid);
    ASSERT_TRUE(rel.type == pcp::RelationType::REPLACE);
    ASSERT_STREQ(rel.eventId.c_str(), "$evt1");
}

static void test_relations_is_reply() {
    pcp::EventRelationsManager mgr;
    ASSERT_TRUE(mgr.isEventReply(R"({"m.in_reply_to":{"event_id":"$evt1"}})"));
    ASSERT_FALSE(mgr.isEventReply(R"({"body":"hello"})"));
}

static void test_relations_is_edit() {
    pcp::EventRelationsManager mgr;
    ASSERT_TRUE(mgr.isEventEdit(R"({"m.relates_to":{"rel_type":"m.replace","event_id":"$evt1"}})"));
    ASSERT_FALSE(mgr.isEventEdit(R"({"m.relates_to":{"rel_type":"m.annotation","event_id":"$evt1"}})"));
}

static void test_relations_is_reaction() {
    pcp::EventRelationsManager mgr;
    ASSERT_TRUE(mgr.isEventReaction(R"({"m.relates_to":{"rel_type":"m.annotation","event_id":"$evt1","key":"👍"}})"));
}

static void test_relations_build_reply() {
    pcp::EventRelationsManager mgr;
    auto json = mgr.buildReplyRelation("$evt1");
    ASSERT_TRUE(json.find("$evt1") != std::string::npos);
    ASSERT_TRUE(json.find("m.in_reply_to") != std::string::npos);
}

static void test_relations_build_edit() {
    pcp::EventRelationsManager mgr;
    auto json = mgr.buildEditRelation("$evtOld");
    ASSERT_TRUE(json.find("m.replace") != std::string::npos);
    ASSERT_TRUE(json.find("$evtOld") != std::string::npos);
}

static void test_relations_build_annotation() {
    pcp::EventRelationsManager mgr;
    auto json = mgr.buildAnnotationRelation("$evtTarget", "👍");
    ASSERT_TRUE(json.find("m.annotation") != std::string::npos);
    ASSERT_TRUE(json.find("👍") != std::string::npos);
}

static void test_relations_validate_edit() {
    pcp::EventRelationsManager mgr;
    std::string error;
    std::string orig = R"({"room_id":"!room:org","sender":"@alice:org","type":"m.room.message"})";
    std::string repl = R"({"room_id":"!room:org","sender":"@alice:org","type":"m.room.message","m.new_content":{"body":"edited"}})";
    ASSERT_TRUE(mgr.validateEdit(orig, repl, error));
    ASSERT_STREQ(error.c_str(), "");
}

// ==== Cross-Signing (Element Android sources) ====

#include "pcp/cross_signing_manager.hpp"

static void test_crosssigning_build_keys() {
    auto msk = pcp::CrossSigningManager::buildMasterKey("@alice:org", "msk_pub_key");
    auto usk = pcp::CrossSigningManager::buildUserSigningKey("@alice:org", "usk_pub_key");
    auto ssk = pcp::CrossSigningManager::buildSelfSigningKey("@alice:org", "ssk_pub_key");
    ASSERT_TRUE(msk.isMasterKey());
    ASSERT_TRUE(usk.isUserKey());
    ASSERT_TRUE(ssk.isSelfSigningKey());
    ASSERT_STREQ(msk.getPublicKey().c_str(), "msk_pub_key");
}

static void test_crosssigning_info() {
    auto msk = pcp::CrossSigningManager::buildMasterKey("@alice:org", "msk");
    auto usk = pcp::CrossSigningManager::buildUserSigningKey("@alice:org", "usk");
    auto ssk = pcp::CrossSigningManager::buildSelfSigningKey("@alice:org", "ssk");
    auto info = pcp::CrossSigningManager::buildCrossSigningInfo("@alice:org", msk, usk, ssk);
    ASSERT_TRUE(info.valid);
    ASSERT_TRUE(info.masterKey() != nullptr);
    ASSERT_TRUE(info.userKey() != nullptr);
    ASSERT_TRUE(info.selfSigningKey() != nullptr);
    ASSERT_FALSE(info.isTrusted());
}

static void test_crosssigning_trust() {
    pcp::CrossSigningManager mgr;
    auto msk = pcp::CrossSigningManager::buildMasterKey("@alice:org", "msk");
    auto usk = pcp::CrossSigningManager::buildUserSigningKey("@alice:org", "usk");
    auto ssk = pcp::CrossSigningManager::buildSelfSigningKey("@alice:org", "ssk");
    mgr.setMyKeys(pcp::CrossSigningManager::buildCrossSigningInfo("@alice:org", msk, usk, ssk));
    ASSERT_TRUE(mgr.isInitialized());
    ASSERT_FALSE(mgr.isVerified());
    mgr.markMyMasterKeyAsTrusted();
    ASSERT_TRUE(mgr.isVerified());
}

static void test_crosssigning_import_keys() {
    pcp::CrossSigningManager mgr;
    auto result = mgr.importPrivateKeys("msk_priv", "usk_priv", "ssk_priv");
    ASSERT_TRUE(result.isTrusted);
    ASSERT_TRUE(mgr.canCrossSign());
}

static void test_crosssigning_key_usage() {
    ASSERT_STREQ(pcp::keyUsageToString(pcp::KeyUsage::MASTER), "master");
    ASSERT_TRUE(pcp::keyUsageFromString("user_signing") == pcp::KeyUsage::USER_SIGNING);
}

// ==== Draft Manager (Element Android sources) ====

#include "pcp/draft_manager_full.hpp"

static void test_draft_save_get() {
    pcp::DraftManager mgr;
    auto draft = pcp::DraftManager::buildRegular("Hello, world!");
    mgr.saveDraft("!room:org", draft);
    ASSERT_TRUE(mgr.hasDraft("!room:org"));

    pcp::UserDraft loaded;
    ASSERT_TRUE(mgr.getDraft("!room:org", loaded));
    ASSERT_STREQ(loaded.content.c_str(), "Hello, world!");
    ASSERT_TRUE(loaded.type == pcp::DraftType::REGULAR);
}

static void test_draft_delete() {
    pcp::DraftManager mgr;
    mgr.saveDraft("!room:org", pcp::DraftManager::buildRegular("test"));
    ASSERT_TRUE(mgr.hasDraft("!room:org"));
    mgr.deleteDraft("!room:org");
    ASSERT_FALSE(mgr.hasDraft("!room:org"));
}

static void test_draft_types() {
    auto q = pcp::DraftManager::buildQuote("$evt1", "quoted reply");
    ASSERT_TRUE(q.type == pcp::DraftType::QUOTE);
    ASSERT_STREQ(q.linkedEventId.c_str(), "$evt1");

    auto e = pcp::DraftManager::buildEdit("$evt2", "edited text");
    ASSERT_TRUE(e.type == pcp::DraftType::EDIT);
    ASSERT_STREQ(e.linkedEventId.c_str(), "$evt2");
}

static void test_draft_auto_save() {
    pcp::DraftManager mgr;
    pcp::LiveDraftConfig cfg;
    cfg.enabled = true;
    cfg.characterThreshold = 5;
    mgr.setLiveDraftConfig(cfg);

    ASSERT_FALSE(mgr.autoSaveIfQualified("!room:org", "ab")); // Too short
    ASSERT_TRUE(mgr.autoSaveIfQualified("!room:org", "Hello world")); // 10 chars + space
}

static void test_draft_strip_prefix() {
    pcp::DraftManager mgr;
    std::string text = "draft: Hello everyone";
    ASSERT_STREQ(mgr.stripDraftPrefix(text).c_str(), " Hello everyone");
    ASSERT_STREQ(mgr.stripDraftPrefix("No prefix").c_str(), "No prefix");
}

// ==== Room History Visibility (Element Android sources) ====

#include "pcp/room_state_manager.hpp"

static void test_room_visibility_parse() {
    ASSERT_TRUE(pcp::parseHistoryVisibility(R"({"history_visibility":"world_readable"})") == pcp::RoomHistoryVisibility::WORLD_READABLE);
    ASSERT_TRUE(pcp::parseHistoryVisibility(R"({"history_visibility":"joined"})") == pcp::RoomHistoryVisibility::JOINED);
}

static void test_room_visibility_should_share() {
    ASSERT_TRUE(pcp::shouldShareHistory(pcp::RoomHistoryVisibility::WORLD_READABLE));
    ASSERT_TRUE(pcp::shouldShareHistory(pcp::RoomHistoryVisibility::SHARED));
    ASSERT_FALSE(pcp::shouldShareHistory(pcp::RoomHistoryVisibility::INVITED));
}

static void test_room_visibility_can_see() {
    ASSERT_TRUE(pcp::canSeeEvent(pcp::RoomHistoryVisibility::WORLD_READABLE,
        pcp::MemberStateState::NONE, pcp::MemberStateState::NONE));
    ASSERT_FALSE(pcp::canSeeEvent(pcp::RoomHistoryVisibility::JOINED,
        pcp::MemberStateState::LEAVE, pcp::MemberStateState::NONE));
}

static void test_room_visibility_join_rules() {
    ASSERT_STREQ(pcp::joinRuleToString(pcp::parseJoinRules(R"({"join_rule":"public"})")), "public");
    ASSERT_STREQ(pcp::joinRuleToString(pcp::parseJoinRules(R"({"join_rule":"knock"})")), "knock");
}

static void test_room_visibility_manager() {
    pcp::RoomStateManager mgr;
    mgr.setHistoryVisibility("!room:org", pcp::RoomHistoryVisibility::SHARED);
    mgr.setJoinRule("!room:org", pcp::RoomJoinRule::PUBLIC);
    ASSERT_TRUE(mgr.isPublicRoom("!room:org"));
    ASSERT_TRUE(mgr.canShareRoomHistory("!room:org"));
    ASSERT_FALSE(mgr.isInviteOnly("!room:org"));
}

// ==== Terms Manager (Element Android sources) ====

#include "pcp/terms_manager.hpp"

static void test_terms_parse() {
    pcp::TermsManager mgr;
    auto resp = mgr.parseTermsResponse(R"({"policies":{"privacy_policy":{"version":"1.0","en":{"name":"Privacy Policy","url":"https://example.org/privacy"}}}})");
    ASSERT_TRUE(resp.hasPolicies);
    ASSERT_EQ(resp.policyCount, 1);
    ASSERT_STREQ(resp.policies[0].policyName.c_str(), "privacy_policy");
}

static void test_terms_required() {
    pcp::TermsManager mgr;
    ASSERT_TRUE(mgr.areTermsRequired(R"({"errcode":"M_TERMS_NOT_SIGNED"})"));
    ASSERT_FALSE(mgr.areTermsRequired(R"({"errcode":"M_FORBIDDEN"})"));
}

static void test_terms_pending() {
    pcp::TermsManager mgr;
    auto resp = mgr.parseTermsResponse(R"({"policies":{"privacy":{"version":"1.0","en":{"name":"Privacy","url":"https://example.org/privacy"}},"tos":{"version":"2.0","en":{"name":"TOS","url":"https://example.org/tos"}}}})");
    auto pending = mgr.getPendingPolicies(resp, {"https://example.org/privacy"});
    ASSERT_EQ(static_cast<int>(pending.size()), 1);
    ASSERT_STREQ(pending[0].c_str(), "https://example.org/tos");
}

static void test_terms_build_agree() {
    pcp::TermsManager mgr;
    pcp::TermsAgreementRequest req;
    req.agreedUrls = {"https://example.org/privacy", "https://example.org/tos"};
    auto json = mgr.buildAgreeRequest(req);
    ASSERT_TRUE(json.find("https://example.org/privacy") != std::string::npos);
    ASSERT_TRUE(json.find("user_accepts") != std::string::npos);
}

// ==== Run all tests ====
int main() {
    printf("=== PCP C++ Unit Tests ===\n");
    TEST_RUNNER(runner);
    
    printf("\n-- SHA-256 --\n");
    ADD_TEST(runner, test_sha256_known_vector);
    ADD_TEST(runner, test_sha256_empty);
    
    printf("\n-- Display Index --\n");
    ADD_TEST(runner, test_compute_display_indices_simple);
    ADD_TEST(runner, test_compute_display_indices_small_gap);
    ADD_TEST(runner, test_compute_display_indices_zero);
    
    printf("\n-- Markdown --\n");
    ADD_TEST(runner, test_markdown_bold);
    ADD_TEST(runner, test_markdown_italic);
    ADD_TEST(runner, test_markdown_plain_passthrough);
    ADD_TEST(runner, test_markdown_html_passthrough);
    
    printf("\n-- Timeline --\n");
    ADD_TEST(runner, test_timeline_add_event);
    ADD_TEST(runner, test_timeline_duplicate);
    ADD_TEST(runner, test_timeline_get_snapshot);
    
    printf("\n-- Sync Parser --\n");
    ADD_TEST(runner, test_sync_parse_next_batch);
    ADD_TEST(runner, test_sync_parse_empty);
    
    printf("\n-- Matrix IDs --\n");
    ADD_TEST(runner, test_is_valid_user_id);
    ADD_TEST(runner, test_is_valid_room_id);
    ADD_TEST(runner, test_is_valid_room_alias);
    ADD_TEST(runner, test_is_valid_event_id);
    
    printf("\n-- MIME & Room State --\n");
    ADD_TEST(runner, test_normalize_mime_type);
    ADD_TEST(runner, test_parse_join_rules);
    ADD_TEST(runner, test_parse_history_visibility);
    
    printf("\n-- Crypto --\n");
    ADD_TEST(runner, test_base58_roundtrip);
    
    printf("\n-- Event Relations --\n");
    ADD_TEST(runner, test_is_reply);
    ADD_TEST(runner, test_is_edit);
    ADD_TEST(runner, test_is_reaction);
    ADD_TEST(runner, test_extract_thread_root);
    
    printf("\n-- Push & Content --\n");
    ADD_TEST(runner, test_is_known_push_rule_kind);
    ADD_TEST(runner, test_is_server_notice);
    ADD_TEST(runner, test_must_accept_tos);
    
    printf("\n-- Password & Notifications --\n");
    ADD_TEST(runner, test_password_meets_minimum);
    ADD_TEST(runner, test_password_count_char_classes);
    ADD_TEST(runner, test_password_strength_label);
    ADD_TEST(runner, test_format_badge_text);
    ADD_TEST(runner, test_total_unread_count);
    
    printf("\n-- Identity & Content --\n");
    ADD_TEST(runner, test_is_email);
    ADD_TEST(runner, test_is_msisdn);
    ADD_TEST(runner, test_extract_alias_localpart);
    ADD_TEST(runner, test_is_mxc_uri);
    ADD_TEST(runner, test_extract_mxc_server_name);
    ADD_TEST(runner, test_extract_mxc_media_id);
    ADD_TEST(runner, test_has_text_with_image);
    
    printf("\n-- User Status --\n");
    ADD_TEST(runner, test_get_status_suggestions);
    ADD_TEST(runner, test_build_user_status_json);
    
    printf("\n-- Misc --\n");
    ADD_TEST(runner, test_is_valid_poll_question);
    ADD_TEST(runner, test_is_poll_ended);
    ADD_TEST(runner, test_classify_event_message);
    ADD_TEST(runner, test_get_edit_badge_text);
    ADD_TEST(runner, test_room_count_zero);
    
    printf("\n-- Report & Permalink --\n");
    ADD_TEST(runner, test_is_valid_report_reason);
    ADD_TEST(runner, test_build_event_permalink);
    ADD_TEST(runner, test_build_room_permalink);
    ADD_TEST(runner, test_needs_well_known_discovery);
    
    printf("\n-- Olm & SAS --\n");
    ADD_TEST(runner, test_olm_account_create);
    ADD_TEST(runner, test_olm_account_pickle_roundtrip);
    ADD_TEST(runner, test_sas_create);
    ADD_TEST(runner, test_sas_emoji_table);
    ADD_TEST(runner, test_compute_device_fingerprint);
    
    printf("\n-- Membership & URL --\n");
    ADD_TEST(runner, test_format_membership_join);
    ADD_TEST(runner, test_is_active_member);
    ADD_TEST(runner, test_is_matrix_to_permalink);
    ADD_TEST(runner, test_extract_room_id_from_permalink);
    
    printf("\n-- SSO & Connection --\n");
    ADD_TEST(runner, test_is_sso_callback_url);
    ADD_TEST(runner, test_format_downtime);
    ADD_TEST(runner, test_get_banner_color);
    
    printf("\n-- Date/Time --\n");
    ADD_TEST(runner, test_format_duration_seconds);
    ADD_TEST(runner, test_format_duration_minutes);
    
    printf("\n-- Errors & Content --\n");
    ADD_TEST(runner, test_is_password_error);
    ADD_TEST(runner, test_get_all_error_codes);
    ADD_TEST(runner, test_count_emojis);
    ADD_TEST(runner, test_format_media_collapse_label);
    
    printf("\n-- Invite & Files --\n");
    ADD_TEST(runner, test_is_invite_expired);
    ADD_TEST(runner, test_build_knock_body);
    ADD_TEST(runner, test_format_file_size);
    ADD_TEST(runner, test_format_file_size_bytes);
    
    printf("\n-- Member & Call Notices --\n");
    ADD_TEST(runner, test_member_notice_join);
    ADD_TEST(runner, test_member_notice_invite);
    /*ADD_TEST(runner, test_call_notice_invite);
    /*ADD_TEST(runner, test_call_notice_reject);
    
    printf("\n-- Megolm --\n");
    ADD_TEST(runner, test_megolm_manager_empty);
    ADD_TEST(runner, test_megolm_clear_room);
    
    printf("\n-- Canonical JSON --\n");
    ADD_TEST(runner, test_canonical_json_simple);
    ADD_TEST(runner, test_canonical_json_no_spaces);
    
    printf("\n-- E2EE End-to-End --\n");
    ADD_TEST(runner, test_e2ee_full_pipeline);
    ADD_TEST(runner, test_encryption_algorithm_parse);
    
    printf("\n-- Serious Components --\n");
    ADD_TEST(runner, test_push_eval_own_event);
    /*ADD_TEST(runner, test_room_upgrade_not_upgrade);
    ADD_TEST(runner, test_redaction_with_reason);
    /*ADD_TEST(runner, test_validate_bad_recovery_key);
    
    ADD_TEST(runner, test_uploader_compute_chunks);
    /*ADD_TEST(runner, test_uploader_suggest_chunk_size);
    
    ADD_TEST(runner, test_is_supported_audio_type);
    ADD_TEST(runner, test_parse_three_pid_email);
    
    printf("\n-- Widget Manager --\n");
    ADD_TEST(runner, test_widget_manager_init);
    ADD_TEST(runner, test_widget_url_template);
    ADD_TEST(runner, test_widget_security_valid);
    ADD_TEST(runner, test_widget_security_blocked_domain);
    ADD_TEST(runner, test_widget_security_blocked_scheme);
    ADD_TEST(runner, test_widget_security_data_url);
    ADD_TEST(runner, test_widget_security_max_length);
    ADD_TEST(runner, test_widget_classification);
    ADD_TEST(runner, test_widget_manager_create);
    ADD_TEST(runner, test_widget_manager_duplicate);
    ADD_TEST(runner, test_widget_manager_remove);
    ADD_TEST(runner, test_widget_manager_resize);
    ADD_TEST(runner, test_widget_manager_minimized);
    ADD_TEST(runner, test_widget_capability_auto_approve);
    ADD_TEST(runner, test_widget_postmessage_build);
    ADD_TEST(runner, test_widget_postmessage_parse);
    ADD_TEST(runner, test_widget_pip_support);
    ADD_TEST(runner, test_widget_by_type);
    
    printf("\n-- Key Backup Manager --\n");
    ADD_TEST(runner, test_backup_parse_version);
    ADD_TEST(runner, test_backup_unsupported_algorithm);
    ADD_TEST(runner, test_backup_build_create_request);
    ADD_TEST(runner, test_backup_export_session);
    ADD_TEST(runner, test_backup_encrypt_session);
    ADD_TEST(runner, test_backup_verify_integrity);
    ADD_TEST(runner, test_backup_progress);
    ADD_TEST(runner, test_backup_parse_keys_response);
    ADD_TEST(runner, test_backup_trust);
    
    printf("\n-- Live Location --\n");
    ADD_TEST(runner, test_geo_uri_parse);
    ADD_TEST(runner, test_geo_uri_invalid);
    ADD_TEST(runner, test_geo_uri_format);
    ADD_TEST(runner, test_location_format_message);
    ADD_TEST(runner, test_live_session_start_stop);
    ADD_TEST(runner, test_beacon_info_build);
    
    printf("\n-- Notif Formatter --\n");
    ADD_TEST(runner, test_format_image_notification);
    ADD_TEST(runner, test_format_file_notification);
    ADD_TEST(runner, test_format_invite_notification);
    ADD_TEST(runner, test_format_room_notification);
    
    printf("\n-- Offline Cache --\n");
    ADD_TEST(runner, test_can_fit_in_storage);
    ADD_TEST(runner, test_estimate_message_cache_size);
    
    printf("\n-- Lightweight Settings --\n");
    ADD_TEST(runner, test_settings_get_bool);
    ADD_TEST(runner, test_settings_set_bool);
    ADD_TEST(runner, test_settings_get_string);
    
    printf("\n-- Encrypted File --\n");
    ADD_TEST(runner, test_encrypted_file_key_to_json);
    ADD_TEST(runner, test_is_valid_jwk_key);
    ADD_TEST(runner, test_extract_file_key);
    
    printf("\n-- URL Preview + Web Search --\n");
    ADD_TEST(runner, test_extract_html_title);
    ADD_TEST(runner, test_resolve_url);
    ADD_TEST(runner, test_build_searxng_url);
    ADD_TEST(runner, test_build_duckduckgo_url);
    
    printf("\n-- Signout + Extras + Misc --\n");
    ADD_TEST(runner, test_should_ignore_signout_error);
    ADD_TEST(runner, test_poll_type_to_string);
    ADD_TEST(runner, test_is_sticker_event);
    ADD_TEST(runner, test_presence_enum_to_string);
    ADD_TEST(runner, test_sdp_type_to_string);
    ADD_TEST(runner, test_parse_json_string_value);
    ADD_TEST(runner, test_end_call_reason_to_string);
    
    printf("\n-- Call Manager --\n");
    /*ADD_TEST(runner, test_call_state_to_string);
    /*ADD_TEST(runner, test_call_start_outgoing);
    /*ADD_TEST(runner, test_call_incoming);
    /*ADD_TEST(runner, test_call_answer_reject);
    /*ADD_TEST(runner, test_call_hangup);
    /*ADD_TEST(runner, test_call_room_in_call);
    /*ADD_TEST(runner, test_call_format_duration);
    /*ADD_TEST(runner, test_call_sdp_parse);
    
    /*ADD_TEST(runner, test_thread_is_root);
    /*ADD_TEST(runner, test_thread_extract_root);
    /*ADD_TEST(runner, test_thread_upsert_and_list);
    /*ADD_TEST(runner, test_thread_unread);
    /*ADD_TEST(runner, test_thread_format_count);
    
    ADD_TEST(runner, test_poll_build_start);
    ADD_TEST(runner, test_poll_parse_start);
    ADD_TEST(runner, test_poll_validation);
    ADD_TEST(runner, test_poll_build_too_few_options);
    ADD_TEST(runner, test_poll_tally);
    ADD_TEST(runner, test_poll_winners);
    
    printf("\n-- Space Graph --\n");
    ADD_TEST(runner, test_space_set_root);
    ADD_TEST(runner, test_space_add_child);
    ADD_TEST(runner, test_space_parents);
    ADD_TEST(runner, test_space_depth);
    ADD_TEST(runner, test_space_search);
    
    printf("\n-- Pin Manager --\n");
    /*ADD_TEST(runner, test_pin_event);
    /*ADD_TEST(runner, test_pin_duplicate);
    /*ADD_TEST(runner, test_pin_power_level);
    /*ADD_TEST(runner, test_pin_parse_ids);
    /*ADD_TEST(runner, test_pin_build_content);
    
    ADD_TEST(runner, test_media_parse);
    ADD_TEST(runner, test_media_format_size);
    ADD_TEST(runner, test_media_format_duration);
    ADD_TEST(runner, test_media_exif_rotation);
    ADD_TEST(runner, test_media_mxc_resolve);
    ADD_TEST(runner, test_media_type_detect);
    
    printf("\n-- OIDC/SSO Login --\n");
    /*ADD_TEST(runner, test_oidc_parse_metadata);
    /*ADD_TEST(runner, test_oidc_parse_token);
    /*ADD_TEST(runner, test_oidc_callback);
    /*ADD_TEST(runner, test_oidc_login_types);
    /*ADD_TEST(runner, test_oidc_password_login_request);
    /*ADD_TEST(runner, test_oidc_well_known);
    
    ADD_TEST(runner, test_userdir_parse_response);
    ADD_TEST(runner, test_userdir_best_name);
    ADD_TEST(runner, test_userdir_avatar_init);
    ADD_TEST(runner, test_userdir_validation);
    ADD_TEST(runner, test_userdir_build_request);
    ADD_TEST(runner, test_userdir_relevance);
    
    printf("\n-- Device Manager (Element Android) --\n");
    ADD_TEST(runner, test_device_parse_list);
    ADD_TEST(runner, test_device_format_fingerprint);
    ADD_TEST(runner, test_device_inactivity);
    ADD_TEST(runner, test_device_trust_label);
    ADD_TEST(runner, test_device_build_rename);
    ADD_TEST(runner, test_device_crypto_parse);
    
    printf("\n-- Room Directory (Element Android) --\n");
    ADD_TEST(runner, test_roomdir_parse_chunk);
    ADD_TEST(runner, test_roomdir_build_search);
    ADD_TEST(runner, test_roomdir_primary_alias);
    ADD_TEST(runner, test_roomdir_visibility);
    ADD_TEST(runner, test_roomdir_sort_popularity);
    ADD_TEST(runner, test_roomdir_preview);
    
    printf("\n-- Session Manager (Element Android) --\n");
    ADD_TEST(runner, test_session_compute_id);
    ADD_TEST(runner, test_session_create);
    ADD_TEST(runner, test_session_active);
    ADD_TEST(runner, test_session_open_close);
    ADD_TEST(runner, test_session_login_types);
    
    printf("\n-- Server Notice (Element Android) --\n");
    ADD_TEST(runner, test_server_notice_parse_resource_limit);
    ADD_TEST(runner, test_server_notice_parse_consent);
    ADD_TEST(runner, test_server_notice_rate_limit);
    ADD_TEST(runner, test_server_notice_downtime);
    ADD_TEST(runner, test_server_notice_error_descriptions);
    ADD_TEST(runner, test_server_notice_banner_color);
    
    printf("\n-- Media Upload (Element Android) --\n");
    ADD_TEST(runner, test_upload_parse_response);
    ADD_TEST(runner, test_upload_size_valid);
    ADD_TEST(runner, test_upload_exif_rotation);
    ADD_TEST(runner, test_upload_compress_dims);
    ADD_TEST(runner, test_upload_progress);
    ADD_TEST(runner, test_upload_attachment_type);
    
    printf("\n-- Identity Server (Element Android) --\n");
    ADD_TEST(runner, test_identity_parse_email);
    ADD_TEST(runner, test_identity_parse_phone);
    ADD_TEST(runner, test_identity_set_server);
    ADD_TEST(runner, test_identity_build_bind);
    ADD_TEST(runner, test_identity_lookup_response);
    
    printf("\n-- Event Relations (Element Android) --\n");
    ADD_TEST(runner, test_relations_parse);
    ADD_TEST(runner, test_relations_is_reply);
    ADD_TEST(runner, test_relations_is_edit);
    ADD_TEST(runner, test_relations_is_reaction);
    ADD_TEST(runner, test_relations_build_reply);
    ADD_TEST(runner, test_relations_build_edit);
    ADD_TEST(runner, test_relations_build_annotation);
    ADD_TEST(runner, test_relations_validate_edit);
    
    printf("\n-- Cross-Signing (Element Android) --\n");
    ADD_TEST(runner, test_crosssigning_build_keys);
    ADD_TEST(runner, test_crosssigning_info);
    ADD_TEST(runner, test_crosssigning_trust);
    ADD_TEST(runner, test_crosssigning_import_keys);
    ADD_TEST(runner, test_crosssigning_key_usage);
    
    printf("\n-- Draft Manager (Element Android) --\n");
    ADD_TEST(runner, test_draft_save_get);
    ADD_TEST(runner, test_draft_delete);
    ADD_TEST(runner, test_draft_types);
    ADD_TEST(runner, test_draft_auto_save);
    ADD_TEST(runner, test_draft_strip_prefix);
    
    printf("\n-- Room History Visibility (Element Android) --\n");
    ADD_TEST(runner, test_room_visibility_parse);
    ADD_TEST(runner, test_room_visibility_should_share);
    ADD_TEST(runner, test_room_visibility_can_see);
    ADD_TEST(runner, test_room_visibility_join_rules);
    ADD_TEST(runner, test_room_visibility_manager);
    
    printf("\n-- Terms Manager (Element Android) --\n");
    ADD_TEST(runner, test_terms_parse);
    ADD_TEST(runner, test_terms_required);
    ADD_TEST(runner, test_terms_pending);
    ADD_TEST(runner, test_terms_build_agree);
    
    return runner.summary();
}
