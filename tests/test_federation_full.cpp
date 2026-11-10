#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(FederationTest, Scenario1) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000000000;
    pdu["sender"] = "@fedsender0:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom0:matrix.org";
    pdu["event_id"] = "$fed00000000:matrix.org";
    pdu["content"]["body"] = "Federation test 0";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000000_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser0_0:matrix.org";
        se["event_id"] = "$state0_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser0_1:matrix.org";
        se["event_id"] = "$state0_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser0_2:matrix.org";
        se["event_id"] = "$state0_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator0:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator0:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator0:matrix.org"; ae["sender"] = "@creator0:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000000000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom0:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold0:matrix.org"});
    missing["latest_events"] = json::array({"$fednew0:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario2) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000001000;
    pdu["sender"] = "@fedsender1:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom1:matrix.org";
    pdu["event_id"] = "$fed00000001:matrix.org";
    pdu["content"]["body"] = "Federation test 1";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000001_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser1_0:matrix.org";
        se["event_id"] = "$state1_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser1_1:matrix.org";
        se["event_id"] = "$state1_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser1_2:matrix.org";
        se["event_id"] = "$state1_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator1:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator1:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator1:matrix.org"; ae["sender"] = "@creator1:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000001000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom1:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold1:matrix.org"});
    missing["latest_events"] = json::array({"$fednew1:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario3) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000002000;
    pdu["sender"] = "@fedsender2:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom2:matrix.org";
    pdu["event_id"] = "$fed00000002:matrix.org";
    pdu["content"]["body"] = "Federation test 2";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000002_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser2_0:matrix.org";
        se["event_id"] = "$state2_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser2_1:matrix.org";
        se["event_id"] = "$state2_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser2_2:matrix.org";
        se["event_id"] = "$state2_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator2:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator2:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator2:matrix.org"; ae["sender"] = "@creator2:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000002000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom2:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold2:matrix.org"});
    missing["latest_events"] = json::array({"$fednew2:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario4) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000003000;
    pdu["sender"] = "@fedsender3:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom3:matrix.org";
    pdu["event_id"] = "$fed00000003:matrix.org";
    pdu["content"]["body"] = "Federation test 3";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000003_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser3_0:matrix.org";
        se["event_id"] = "$state3_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser3_1:matrix.org";
        se["event_id"] = "$state3_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser3_2:matrix.org";
        se["event_id"] = "$state3_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator3:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator3:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator3:matrix.org"; ae["sender"] = "@creator3:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000003000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom3:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold3:matrix.org"});
    missing["latest_events"] = json::array({"$fednew3:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario5) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000004000;
    pdu["sender"] = "@fedsender4:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom4:matrix.org";
    pdu["event_id"] = "$fed00000004:matrix.org";
    pdu["content"]["body"] = "Federation test 4";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000004_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser4_0:matrix.org";
        se["event_id"] = "$state4_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser4_1:matrix.org";
        se["event_id"] = "$state4_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser4_2:matrix.org";
        se["event_id"] = "$state4_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator4:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator4:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator4:matrix.org"; ae["sender"] = "@creator4:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000004000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom4:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold4:matrix.org"});
    missing["latest_events"] = json::array({"$fednew4:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario6) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000005000;
    pdu["sender"] = "@fedsender5:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom5:matrix.org";
    pdu["event_id"] = "$fed00000005:matrix.org";
    pdu["content"]["body"] = "Federation test 5";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000005_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser5_0:matrix.org";
        se["event_id"] = "$state5_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser5_1:matrix.org";
        se["event_id"] = "$state5_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser5_2:matrix.org";
        se["event_id"] = "$state5_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator5:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator5:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator5:matrix.org"; ae["sender"] = "@creator5:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000005000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom5:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold5:matrix.org"});
    missing["latest_events"] = json::array({"$fednew5:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario7) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000006000;
    pdu["sender"] = "@fedsender6:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom6:matrix.org";
    pdu["event_id"] = "$fed00000006:matrix.org";
    pdu["content"]["body"] = "Federation test 6";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000006_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser6_0:matrix.org";
        se["event_id"] = "$state6_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser6_1:matrix.org";
        se["event_id"] = "$state6_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser6_2:matrix.org";
        se["event_id"] = "$state6_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator6:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator6:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator6:matrix.org"; ae["sender"] = "@creator6:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000006000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom6:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold6:matrix.org"});
    missing["latest_events"] = json::array({"$fednew6:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario8) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000007000;
    pdu["sender"] = "@fedsender7:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom7:matrix.org";
    pdu["event_id"] = "$fed00000007:matrix.org";
    pdu["content"]["body"] = "Federation test 7";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000007_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser7_0:matrix.org";
        se["event_id"] = "$state7_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser7_1:matrix.org";
        se["event_id"] = "$state7_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser7_2:matrix.org";
        se["event_id"] = "$state7_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator7:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator7:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator7:matrix.org"; ae["sender"] = "@creator7:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000007000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom7:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold7:matrix.org"});
    missing["latest_events"] = json::array({"$fednew7:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario9) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000008000;
    pdu["sender"] = "@fedsender8:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom8:matrix.org";
    pdu["event_id"] = "$fed00000008:matrix.org";
    pdu["content"]["body"] = "Federation test 8";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000008_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser8_0:matrix.org";
        se["event_id"] = "$state8_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser8_1:matrix.org";
        se["event_id"] = "$state8_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser8_2:matrix.org";
        se["event_id"] = "$state8_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator8:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator8:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator8:matrix.org"; ae["sender"] = "@creator8:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000008000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom8:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold8:matrix.org"});
    missing["latest_events"] = json::array({"$fednew8:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario10) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000009000;
    pdu["sender"] = "@fedsender9:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom9:matrix.org";
    pdu["event_id"] = "$fed00000009:matrix.org";
    pdu["content"]["body"] = "Federation test 9";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000009_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser9_0:matrix.org";
        se["event_id"] = "$state9_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser9_1:matrix.org";
        se["event_id"] = "$state9_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser9_2:matrix.org";
        se["event_id"] = "$state9_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator9:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator9:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator9:matrix.org"; ae["sender"] = "@creator9:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000009000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom9:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold9:matrix.org"});
    missing["latest_events"] = json::array({"$fednew9:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario11) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000010000;
    pdu["sender"] = "@fedsender10:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom10:matrix.org";
    pdu["event_id"] = "$fed00000010:matrix.org";
    pdu["content"]["body"] = "Federation test 10";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000010_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser10_0:matrix.org";
        se["event_id"] = "$state10_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser10_1:matrix.org";
        se["event_id"] = "$state10_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser10_2:matrix.org";
        se["event_id"] = "$state10_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator10:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator10:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator10:matrix.org"; ae["sender"] = "@creator10:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000010000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom10:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold10:matrix.org"});
    missing["latest_events"] = json::array({"$fednew10:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario12) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000011000;
    pdu["sender"] = "@fedsender11:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom11:matrix.org";
    pdu["event_id"] = "$fed00000011:matrix.org";
    pdu["content"]["body"] = "Federation test 11";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000011_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser11_0:matrix.org";
        se["event_id"] = "$state11_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser11_1:matrix.org";
        se["event_id"] = "$state11_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser11_2:matrix.org";
        se["event_id"] = "$state11_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator11:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator11:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator11:matrix.org"; ae["sender"] = "@creator11:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000011000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom11:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold11:matrix.org"});
    missing["latest_events"] = json::array({"$fednew11:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario13) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000012000;
    pdu["sender"] = "@fedsender12:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom12:matrix.org";
    pdu["event_id"] = "$fed00000012:matrix.org";
    pdu["content"]["body"] = "Federation test 12";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000012_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser12_0:matrix.org";
        se["event_id"] = "$state12_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser12_1:matrix.org";
        se["event_id"] = "$state12_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser12_2:matrix.org";
        se["event_id"] = "$state12_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator12:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator12:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator12:matrix.org"; ae["sender"] = "@creator12:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000012000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom12:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold12:matrix.org"});
    missing["latest_events"] = json::array({"$fednew12:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario14) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000013000;
    pdu["sender"] = "@fedsender13:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom13:matrix.org";
    pdu["event_id"] = "$fed00000013:matrix.org";
    pdu["content"]["body"] = "Federation test 13";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000013_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser13_0:matrix.org";
        se["event_id"] = "$state13_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser13_1:matrix.org";
        se["event_id"] = "$state13_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser13_2:matrix.org";
        se["event_id"] = "$state13_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator13:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator13:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator13:matrix.org"; ae["sender"] = "@creator13:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000013000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom13:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold13:matrix.org"});
    missing["latest_events"] = json::array({"$fednew13:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario15) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000014000;
    pdu["sender"] = "@fedsender14:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom14:matrix.org";
    pdu["event_id"] = "$fed00000014:matrix.org";
    pdu["content"]["body"] = "Federation test 14";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000014_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser14_0:matrix.org";
        se["event_id"] = "$state14_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser14_1:matrix.org";
        se["event_id"] = "$state14_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser14_2:matrix.org";
        se["event_id"] = "$state14_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator14:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator14:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator14:matrix.org"; ae["sender"] = "@creator14:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000014000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom14:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold14:matrix.org"});
    missing["latest_events"] = json::array({"$fednew14:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario16) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000015000;
    pdu["sender"] = "@fedsender15:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom15:matrix.org";
    pdu["event_id"] = "$fed00000015:matrix.org";
    pdu["content"]["body"] = "Federation test 15";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000015_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser15_0:matrix.org";
        se["event_id"] = "$state15_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser15_1:matrix.org";
        se["event_id"] = "$state15_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser15_2:matrix.org";
        se["event_id"] = "$state15_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator15:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator15:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator15:matrix.org"; ae["sender"] = "@creator15:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000015000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom15:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold15:matrix.org"});
    missing["latest_events"] = json::array({"$fednew15:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario17) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000016000;
    pdu["sender"] = "@fedsender16:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom16:matrix.org";
    pdu["event_id"] = "$fed00000016:matrix.org";
    pdu["content"]["body"] = "Federation test 16";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000016_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser16_0:matrix.org";
        se["event_id"] = "$state16_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser16_1:matrix.org";
        se["event_id"] = "$state16_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser16_2:matrix.org";
        se["event_id"] = "$state16_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator16:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator16:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator16:matrix.org"; ae["sender"] = "@creator16:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000016000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom16:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold16:matrix.org"});
    missing["latest_events"] = json::array({"$fednew16:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario18) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000017000;
    pdu["sender"] = "@fedsender17:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom17:matrix.org";
    pdu["event_id"] = "$fed00000017:matrix.org";
    pdu["content"]["body"] = "Federation test 17";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000017_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser17_0:matrix.org";
        se["event_id"] = "$state17_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser17_1:matrix.org";
        se["event_id"] = "$state17_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser17_2:matrix.org";
        se["event_id"] = "$state17_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator17:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator17:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator17:matrix.org"; ae["sender"] = "@creator17:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000017000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom17:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold17:matrix.org"});
    missing["latest_events"] = json::array({"$fednew17:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario19) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000018000;
    pdu["sender"] = "@fedsender18:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom18:matrix.org";
    pdu["event_id"] = "$fed00000018:matrix.org";
    pdu["content"]["body"] = "Federation test 18";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000018_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser18_0:matrix.org";
        se["event_id"] = "$state18_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser18_1:matrix.org";
        se["event_id"] = "$state18_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser18_2:matrix.org";
        se["event_id"] = "$state18_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator18:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator18:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator18:matrix.org"; ae["sender"] = "@creator18:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000018000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom18:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold18:matrix.org"});
    missing["latest_events"] = json::array({"$fednew18:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario20) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000019000;
    pdu["sender"] = "@fedsender19:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom19:matrix.org";
    pdu["event_id"] = "$fed00000019:matrix.org";
    pdu["content"]["body"] = "Federation test 19";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000019_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser19_0:matrix.org";
        se["event_id"] = "$state19_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser19_1:matrix.org";
        se["event_id"] = "$state19_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser19_2:matrix.org";
        se["event_id"] = "$state19_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator19:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator19:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator19:matrix.org"; ae["sender"] = "@creator19:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000019000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom19:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold19:matrix.org"});
    missing["latest_events"] = json::array({"$fednew19:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario21) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000020000;
    pdu["sender"] = "@fedsender20:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom20:matrix.org";
    pdu["event_id"] = "$fed00000020:matrix.org";
    pdu["content"]["body"] = "Federation test 20";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000020_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser20_0:matrix.org";
        se["event_id"] = "$state20_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser20_1:matrix.org";
        se["event_id"] = "$state20_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser20_2:matrix.org";
        se["event_id"] = "$state20_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator20:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator20:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator20:matrix.org"; ae["sender"] = "@creator20:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000020000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom20:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold20:matrix.org"});
    missing["latest_events"] = json::array({"$fednew20:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario22) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000021000;
    pdu["sender"] = "@fedsender21:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom21:matrix.org";
    pdu["event_id"] = "$fed00000021:matrix.org";
    pdu["content"]["body"] = "Federation test 21";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000021_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser21_0:matrix.org";
        se["event_id"] = "$state21_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser21_1:matrix.org";
        se["event_id"] = "$state21_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser21_2:matrix.org";
        se["event_id"] = "$state21_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator21:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator21:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator21:matrix.org"; ae["sender"] = "@creator21:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000021000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom21:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold21:matrix.org"});
    missing["latest_events"] = json::array({"$fednew21:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario23) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000022000;
    pdu["sender"] = "@fedsender22:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom22:matrix.org";
    pdu["event_id"] = "$fed00000022:matrix.org";
    pdu["content"]["body"] = "Federation test 22";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000022_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser22_0:matrix.org";
        se["event_id"] = "$state22_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser22_1:matrix.org";
        se["event_id"] = "$state22_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser22_2:matrix.org";
        se["event_id"] = "$state22_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator22:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator22:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator22:matrix.org"; ae["sender"] = "@creator22:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000022000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom22:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold22:matrix.org"});
    missing["latest_events"] = json::array({"$fednew22:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario24) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000023000;
    pdu["sender"] = "@fedsender23:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom23:matrix.org";
    pdu["event_id"] = "$fed00000023:matrix.org";
    pdu["content"]["body"] = "Federation test 23";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000023_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser23_0:matrix.org";
        se["event_id"] = "$state23_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser23_1:matrix.org";
        se["event_id"] = "$state23_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser23_2:matrix.org";
        se["event_id"] = "$state23_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator23:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator23:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator23:matrix.org"; ae["sender"] = "@creator23:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000023000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom23:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold23:matrix.org"});
    missing["latest_events"] = json::array({"$fednew23:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario25) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000024000;
    pdu["sender"] = "@fedsender24:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom24:matrix.org";
    pdu["event_id"] = "$fed00000024:matrix.org";
    pdu["content"]["body"] = "Federation test 24";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000024_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser24_0:matrix.org";
        se["event_id"] = "$state24_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser24_1:matrix.org";
        se["event_id"] = "$state24_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser24_2:matrix.org";
        se["event_id"] = "$state24_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator24:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator24:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator24:matrix.org"; ae["sender"] = "@creator24:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000024000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom24:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold24:matrix.org"});
    missing["latest_events"] = json::array({"$fednew24:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario26) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000025000;
    pdu["sender"] = "@fedsender25:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom25:matrix.org";
    pdu["event_id"] = "$fed00000025:matrix.org";
    pdu["content"]["body"] = "Federation test 25";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000025_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser25_0:matrix.org";
        se["event_id"] = "$state25_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser25_1:matrix.org";
        se["event_id"] = "$state25_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser25_2:matrix.org";
        se["event_id"] = "$state25_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator25:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator25:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator25:matrix.org"; ae["sender"] = "@creator25:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000025000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom25:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold25:matrix.org"});
    missing["latest_events"] = json::array({"$fednew25:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario27) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000026000;
    pdu["sender"] = "@fedsender26:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom26:matrix.org";
    pdu["event_id"] = "$fed00000026:matrix.org";
    pdu["content"]["body"] = "Federation test 26";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000026_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser26_0:matrix.org";
        se["event_id"] = "$state26_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser26_1:matrix.org";
        se["event_id"] = "$state26_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser26_2:matrix.org";
        se["event_id"] = "$state26_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator26:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator26:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator26:matrix.org"; ae["sender"] = "@creator26:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000026000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom26:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold26:matrix.org"});
    missing["latest_events"] = json::array({"$fednew26:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario28) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000027000;
    pdu["sender"] = "@fedsender27:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom27:matrix.org";
    pdu["event_id"] = "$fed00000027:matrix.org";
    pdu["content"]["body"] = "Federation test 27";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000027_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser27_0:matrix.org";
        se["event_id"] = "$state27_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser27_1:matrix.org";
        se["event_id"] = "$state27_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser27_2:matrix.org";
        se["event_id"] = "$state27_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator27:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator27:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator27:matrix.org"; ae["sender"] = "@creator27:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000027000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom27:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold27:matrix.org"});
    missing["latest_events"] = json::array({"$fednew27:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario29) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000028000;
    pdu["sender"] = "@fedsender28:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom28:matrix.org";
    pdu["event_id"] = "$fed00000028:matrix.org";
    pdu["content"]["body"] = "Federation test 28";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000028_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser28_0:matrix.org";
        se["event_id"] = "$state28_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser28_1:matrix.org";
        se["event_id"] = "$state28_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser28_2:matrix.org";
        se["event_id"] = "$state28_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator28:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator28:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator28:matrix.org"; ae["sender"] = "@creator28:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000028000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom28:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold28:matrix.org"});
    missing["latest_events"] = json::array({"$fednew28:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

TEST(FederationTest, Scenario30) {
    json pdu;
    pdu["origin"] = "matrix.org";
    pdu["origin_server_ts"] = 1600000029000;
    pdu["sender"] = "@fedsender29:matrix.org";
    pdu["type"] = "m.room.message";
    pdu["room_id"] = "!fedroom29:matrix.org";
    pdu["event_id"] = "$fed00000029:matrix.org";
    pdu["content"]["body"] = "Federation test 29";
    ASSERT_EQ(pdu["origin"], "matrix.org");
    ASSERT_FALSE(pdu["event_id"].empty());

    json signed;
    signed["signatures"]["matrix.org"]["ed25519:abc"] = "SIG_FED_00000029_BASE64";
    ASSERT_FALSE(signed["signatures"]["matrix.org"]["ed25519:abc"].empty());

    json stateRes;
    stateRes["state_set1"] = json::array();
    stateRes["state_set2"] = json::array();
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser29_0:matrix.org";
        se["event_id"] = "$state29_0:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser29_1:matrix.org";
        se["event_id"] = "$state29_1:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    {
        json se;
        se["type"] = "m.room.member";
        se["state_key"] = "@stateuser29_2:matrix.org";
        se["event_id"] = "$state29_2:matrix.org";
        stateRes["state_set1"].push_back(se);
    }
    ASSERT_EQ(stateRes["state_set1"].size(), 3);
    ASSERT_EQ(stateRes["state_set2"].size(), 0);

    json authEvents = json::array();
    { json ae; ae["type"] = "m.room.create"; ae["sender"] = "@creator29:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.power_levels"; ae["sender"] = "@creator29:matrix.org"; authEvents.push_back(ae); }
    { json ae; ae["type"] = "m.room.member"; ae["state_key"] = "@creator29:matrix.org"; ae["sender"] = "@creator29:matrix.org"; authEvents.push_back(ae); }
    ASSERT_EQ(authEvents.size(), 3);

    json txn;
    txn["origin"] = "matrix.org";
    txn["origin_server_ts"] = 1600000029000;
    txn["pdus"] = json::array();
    txn["pdus"].push_back(pdu);
    txn["edus"] = json::array();
    ASSERT_EQ(txn["pdus"].size(), 1);

    json backfill;
    backfill["origin"] = "matrix.org";
    backfill["room_id"] = "!fedroom29:matrix.org";
    backfill["limit"] = 50;
    backfill["v"] = json::array({"server.org"});
    ASSERT_EQ(backfill["limit"], 50);

    json missing;
    missing["earliest_events"] = json::array({"$fedold29:matrix.org"});
    missing["latest_events"] = json::array({"$fednew29:matrix.org"});
    missing["limit"] = 10;
    ASSERT_EQ(missing["limit"], 10);

    ASSERT_NO_THROW({ auto d = pdu.dump(); });
    ASSERT_NO_THROW({ auto d = txn.dump(); });
    ASSERT_NO_THROW({ auto d = signed.dump(); });
}

}} // namespace pcp::test
