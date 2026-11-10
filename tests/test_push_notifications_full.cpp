#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(PushNotificationTest, Scenario1) {
    json rule;
    rule["rule_id"] = "override.rule0";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000000:matrix.org";
    event["sender"] = "@pushuser0:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 0";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom0:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 0;
    counts["highlight_count"] = 0;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000000";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 0";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario2) {
    json rule;
    rule["rule_id"] = "under.rule1";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000001:matrix.org";
    event["sender"] = "@pushuser1:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 1";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom1:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 1;
    counts["highlight_count"] = 1;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000001";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 1";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario3) {
    json rule;
    rule["rule_id"] = "sender.rule2";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "@notifyuser2:matrix.org";

    json event;
    event["event_id"] = "$push00000002:matrix.org";
    event["sender"] = "@pushuser2:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 2";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom2:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 2;
    counts["highlight_count"] = 2;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000002";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 2";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario4) {
    json rule;
    rule["rule_id"] = "room.rule3";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "!notifyroom3:matrix.org";

    json event;
    event["event_id"] = "$push00000003:matrix.org";
    event["sender"] = "@pushuser3:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 3";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom3:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 3;
    counts["highlight_count"] = 3;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000003";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 3";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario5) {
    json rule;
    rule["rule_id"] = "content.rule4";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["pattern"] = "keyword4";

    json event;
    event["event_id"] = "$push00000004:matrix.org";
    event["sender"] = "@pushuser4:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 4";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom4:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 4;
    counts["highlight_count"] = 4;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000004";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 4";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario6) {
    json rule;
    rule["rule_id"] = "override.rule5";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000005:matrix.org";
    event["sender"] = "@pushuser5:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 5";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom5:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 5;
    counts["highlight_count"] = 5;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000005";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 5";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario7) {
    json rule;
    rule["rule_id"] = "under.rule6";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000006:matrix.org";
    event["sender"] = "@pushuser6:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 6";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom6:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 6;
    counts["highlight_count"] = 6;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000006";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 6";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario8) {
    json rule;
    rule["rule_id"] = "sender.rule7";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "@notifyuser7:matrix.org";

    json event;
    event["event_id"] = "$push00000007:matrix.org";
    event["sender"] = "@pushuser7:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 7";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom7:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 7;
    counts["highlight_count"] = 0;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000007";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 7";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario9) {
    json rule;
    rule["rule_id"] = "room.rule8";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "!notifyroom8:matrix.org";

    json event;
    event["event_id"] = "$push00000008:matrix.org";
    event["sender"] = "@pushuser8:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 8";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom8:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 8;
    counts["highlight_count"] = 1;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000008";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 8";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario10) {
    json rule;
    rule["rule_id"] = "content.rule9";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["pattern"] = "keyword9";

    json event;
    event["event_id"] = "$push00000009:matrix.org";
    event["sender"] = "@pushuser9:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 9";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom9:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 9;
    counts["highlight_count"] = 2;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000009";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 9";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario11) {
    json rule;
    rule["rule_id"] = "override.rule10";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000010:matrix.org";
    event["sender"] = "@pushuser10:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 10";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom10:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 10;
    counts["highlight_count"] = 3;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000010";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 10";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario12) {
    json rule;
    rule["rule_id"] = "under.rule11";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000011:matrix.org";
    event["sender"] = "@pushuser11:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 11";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom11:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 11;
    counts["highlight_count"] = 4;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000011";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 11";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario13) {
    json rule;
    rule["rule_id"] = "sender.rule12";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "@notifyuser12:matrix.org";

    json event;
    event["event_id"] = "$push00000012:matrix.org";
    event["sender"] = "@pushuser12:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 12";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom12:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 12;
    counts["highlight_count"] = 5;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000012";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 12";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario14) {
    json rule;
    rule["rule_id"] = "room.rule13";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "!notifyroom13:matrix.org";

    json event;
    event["event_id"] = "$push00000013:matrix.org";
    event["sender"] = "@pushuser13:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 13";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom13:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 13;
    counts["highlight_count"] = 6;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000013";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 13";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario15) {
    json rule;
    rule["rule_id"] = "content.rule14";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["pattern"] = "keyword14";

    json event;
    event["event_id"] = "$push00000014:matrix.org";
    event["sender"] = "@pushuser14:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 14";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom14:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 14;
    counts["highlight_count"] = 0;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000014";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 14";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario16) {
    json rule;
    rule["rule_id"] = "override.rule15";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000015:matrix.org";
    event["sender"] = "@pushuser15:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 15";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom15:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 15;
    counts["highlight_count"] = 1;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000015";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 15";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario17) {
    json rule;
    rule["rule_id"] = "under.rule16";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000016:matrix.org";
    event["sender"] = "@pushuser16:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 16";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom16:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 16;
    counts["highlight_count"] = 2;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000016";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 16";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario18) {
    json rule;
    rule["rule_id"] = "sender.rule17";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "@notifyuser17:matrix.org";

    json event;
    event["event_id"] = "$push00000017:matrix.org";
    event["sender"] = "@pushuser17:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 17";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom17:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 17;
    counts["highlight_count"] = 3;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000017";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 17";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario19) {
    json rule;
    rule["rule_id"] = "room.rule18";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "!notifyroom18:matrix.org";

    json event;
    event["event_id"] = "$push00000018:matrix.org";
    event["sender"] = "@pushuser18:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 18";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom18:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 18;
    counts["highlight_count"] = 4;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000018";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 18";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario20) {
    json rule;
    rule["rule_id"] = "content.rule19";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["pattern"] = "keyword19";

    json event;
    event["event_id"] = "$push00000019:matrix.org";
    event["sender"] = "@pushuser19:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 19";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom19:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 19;
    counts["highlight_count"] = 5;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000019";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 19";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario21) {
    json rule;
    rule["rule_id"] = "override.rule20";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000020:matrix.org";
    event["sender"] = "@pushuser20:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 20";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom20:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 0;
    counts["highlight_count"] = 6;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000020";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 20";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario22) {
    json rule;
    rule["rule_id"] = "under.rule21";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000021:matrix.org";
    event["sender"] = "@pushuser21:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 21";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom21:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 1;
    counts["highlight_count"] = 0;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000021";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 21";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario23) {
    json rule;
    rule["rule_id"] = "sender.rule22";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "@notifyuser22:matrix.org";

    json event;
    event["event_id"] = "$push00000022:matrix.org";
    event["sender"] = "@pushuser22:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 22";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom22:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 2;
    counts["highlight_count"] = 1;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000022";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 22";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario24) {
    json rule;
    rule["rule_id"] = "room.rule23";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "!notifyroom23:matrix.org";

    json event;
    event["event_id"] = "$push00000023:matrix.org";
    event["sender"] = "@pushuser23:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 23";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom23:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 3;
    counts["highlight_count"] = 2;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000023";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 23";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario25) {
    json rule;
    rule["rule_id"] = "content.rule24";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["pattern"] = "keyword24";

    json event;
    event["event_id"] = "$push00000024:matrix.org";
    event["sender"] = "@pushuser24:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 24";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom24:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 4;
    counts["highlight_count"] = 3;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000024";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 24";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario26) {
    json rule;
    rule["rule_id"] = "override.rule25";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000025:matrix.org";
    event["sender"] = "@pushuser25:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 25";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom25:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 5;
    counts["highlight_count"] = 4;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000025";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 25";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario27) {
    json rule;
    rule["rule_id"] = "under.rule26";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);


    json event;
    event["event_id"] = "$push00000026:matrix.org";
    event["sender"] = "@pushuser26:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 26";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom26:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 6;
    counts["highlight_count"] = 5;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000026";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 26";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario28) {
    json rule;
    rule["rule_id"] = "sender.rule27";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "@notifyuser27:matrix.org";

    json event;
    event["event_id"] = "$push00000027:matrix.org";
    event["sender"] = "@pushuser27:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 27";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom27:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = true;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 7;
    counts["highlight_count"] = 6;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000027";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 27";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario29) {
    json rule;
    rule["rule_id"] = "room.rule28";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["rule_id"] = "!notifyroom28:matrix.org";

    json event;
    event["event_id"] = "$push00000028:matrix.org";
    event["sender"] = "@pushuser28:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 28";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom28:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 8;
    counts["highlight_count"] = 0;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000028";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 28";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

TEST(PushNotificationTest, Scenario30) {
    json rule;
    rule["rule_id"] = "content.rule29";
    rule["enabled"] = true;
    rule["default"] = true;
    rule["actions"] = json::array();
    rule["actions"].push_back("notify");
    { json st; st["set_tweak"] = "sound"; st["value"] = "default"; rule["actions"].push_back(st); }
    ASSERT_TRUE(rule["enabled"]);
    ASSERT_EQ(rule["actions"].size(), 2);

    rule["pattern"] = "keyword29";

    json event;
    event["event_id"] = "$push00000029:matrix.org";
    event["sender"] = "@pushuser29:matrix.org";
    event["type"] = "m.room.message";
    event["content"]["body"] = "Push notification test 29";
    event["content"]["msgtype"] = "m.text";
    event["room_id"] = "!pushroom29:matrix.org";
    ASSERT_FALSE(event["event_id"].empty());

    json evaluation;
    evaluation["matched_rule"] = rule["rule_id"];
    evaluation["action"] = "notify";
    evaluation["highlight"] = false;
    evaluation["sound"] = "default";
    ASSERT_FALSE(evaluation["matched_rule"].empty());
    ASSERT_EQ(evaluation["action"], "notify");

    json counts;
    counts["notification_count"] = 9;
    counts["highlight_count"] = 1;
    counts["unread"] = true;
    ASSERT_TRUE(counts["notification_count"] >= 0);
    ASSERT_TRUE(counts["highlight_count"] >= 0);

    json pusher;
    pusher["pushkey"] = "PUSH_KEY_00000029";
    pusher["app_id"] = "org.matrix.pcp";
    pusher["app_display_name"] = "PCP";
    pusher["device_display_name"] = "Android Device 29";
    pusher["kind"] = "http";
    pusher["lang"] = "en";
    pusher["data"]["url"] = "https://push.matrix.org/notify";
    pusher["data"]["format"] = "event_id_only";
    ASSERT_EQ(pusher["app_id"], "org.matrix.pcp");
    ASSERT_EQ(pusher["kind"], "http");

    ASSERT_NO_THROW({ auto d = rule.dump(); });
    ASSERT_NO_THROW({ auto d = evaluation.dump(); });
    ASSERT_NO_THROW({ auto d = pusher.dump(); });
}

}} // namespace pcp::test
