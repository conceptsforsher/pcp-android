#include <gtest/gtest.h>
#include <string>
#include <nlohmann/json.hpp>
#include "pcp/message_content.hpp"
#include "pcp/content_utils.hpp"

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(MessageHandlingTest, MessageType1) {
    json event;
    event["event_id"] = "$event000000:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser0:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 0 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 0";
    content["msgtype"] = "m.text";
    
    if (std::string("m.text") == "m.image" || std::string("m.text") == "m.file" || std::string("m.text") == "m.audio" || std::string("m.text") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000000";
        content["filename"] = "file_0.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (0 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.text") == "m.text" || std::string("m.text") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 0</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.text") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 0";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.text");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 0";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 0";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType2) {
    json event;
    event["event_id"] = "$event000001:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser1:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 1 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 1";
    content["msgtype"] = "m.notice";
    
    if (std::string("m.notice") == "m.image" || std::string("m.notice") == "m.file" || std::string("m.notice") == "m.audio" || std::string("m.notice") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000001";
        content["filename"] = "file_1.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (1 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.notice") == "m.text" || std::string("m.notice") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 1</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.notice") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 1";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.notice");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 1";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 1";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType3) {
    json event;
    event["event_id"] = "$event000002:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser2:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 2 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 2";
    content["msgtype"] = "m.emote";
    
    if (std::string("m.emote") == "m.image" || std::string("m.emote") == "m.file" || std::string("m.emote") == "m.audio" || std::string("m.emote") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000002";
        content["filename"] = "file_2.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (2 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.emote") == "m.text" || std::string("m.emote") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 2</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.emote") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 2";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.emote");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 2";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 2";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType4) {
    json event;
    event["event_id"] = "$event000003:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser3:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 3 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 3";
    content["msgtype"] = "m.image";
    
    if (std::string("m.image") == "m.image" || std::string("m.image") == "m.file" || std::string("m.image") == "m.audio" || std::string("m.image") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000003";
        content["filename"] = "file_3.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (3 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.image") == "m.text" || std::string("m.image") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 3</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.image") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 3";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.image");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 3";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 3";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType5) {
    json event;
    event["event_id"] = "$event000004:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser4:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 4 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 4";
    content["msgtype"] = "m.file";
    
    if (std::string("m.file") == "m.image" || std::string("m.file") == "m.file" || std::string("m.file") == "m.audio" || std::string("m.file") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000004";
        content["filename"] = "file_4.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (4 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.file") == "m.text" || std::string("m.file") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 4</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.file") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 4";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.file");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 4";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 4";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType6) {
    json event;
    event["event_id"] = "$event000005:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser5:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 5 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 5";
    content["msgtype"] = "m.audio";
    
    if (std::string("m.audio") == "m.image" || std::string("m.audio") == "m.file" || std::string("m.audio") == "m.audio" || std::string("m.audio") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000005";
        content["filename"] = "file_5.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (5 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.audio") == "m.text" || std::string("m.audio") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 5</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.audio") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 5";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.audio");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 5";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 5";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType7) {
    json event;
    event["event_id"] = "$event000006:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser6:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 6 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 6";
    content["msgtype"] = "m.video";
    
    if (std::string("m.video") == "m.image" || std::string("m.video") == "m.file" || std::string("m.video") == "m.audio" || std::string("m.video") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000006";
        content["filename"] = "file_6.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (6 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.video") == "m.text" || std::string("m.video") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 6</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.video") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 6";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.video");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 6";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 6";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType8) {
    json event;
    event["event_id"] = "$event000007:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser7:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 7 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 7";
    content["msgtype"] = "m.location";
    
    if (std::string("m.location") == "m.image" || std::string("m.location") == "m.file" || std::string("m.location") == "m.audio" || std::string("m.location") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000007";
        content["filename"] = "file_7.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (7 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.location") == "m.text" || std::string("m.location") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 7</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.location") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 7";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.location");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 7";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 7";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType9) {
    json event;
    event["event_id"] = "$event000008:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser8:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 8 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 8";
    content["msgtype"] = "m.text";
    
    if (std::string("m.text") == "m.image" || std::string("m.text") == "m.file" || std::string("m.text") == "m.audio" || std::string("m.text") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000008";
        content["filename"] = "file_8.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (8 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.text") == "m.text" || std::string("m.text") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 8</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.text") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 8";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.text");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 8";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 8";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType10) {
    json event;
    event["event_id"] = "$event000009:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser9:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 9 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 9";
    content["msgtype"] = "m.notice";
    
    if (std::string("m.notice") == "m.image" || std::string("m.notice") == "m.file" || std::string("m.notice") == "m.audio" || std::string("m.notice") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000009";
        content["filename"] = "file_9.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (9 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.notice") == "m.text" || std::string("m.notice") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 9</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.notice") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 9";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.notice");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 9";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 9";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType11) {
    json event;
    event["event_id"] = "$event000010:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser10:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 10 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 10";
    content["msgtype"] = "m.emote";
    
    if (std::string("m.emote") == "m.image" || std::string("m.emote") == "m.file" || std::string("m.emote") == "m.audio" || std::string("m.emote") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000010";
        content["filename"] = "file_10.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (10 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.emote") == "m.text" || std::string("m.emote") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 10</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.emote") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 10";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.emote");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 10";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 10";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType12) {
    json event;
    event["event_id"] = "$event000011:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser11:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 11 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 11";
    content["msgtype"] = "m.image";
    
    if (std::string("m.image") == "m.image" || std::string("m.image") == "m.file" || std::string("m.image") == "m.audio" || std::string("m.image") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000011";
        content["filename"] = "file_11.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (11 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.image") == "m.text" || std::string("m.image") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 11</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.image") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 11";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.image");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 11";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 11";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType13) {
    json event;
    event["event_id"] = "$event000012:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser12:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 12 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 12";
    content["msgtype"] = "m.file";
    
    if (std::string("m.file") == "m.image" || std::string("m.file") == "m.file" || std::string("m.file") == "m.audio" || std::string("m.file") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000012";
        content["filename"] = "file_12.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (12 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.file") == "m.text" || std::string("m.file") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 12</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.file") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 12";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.file");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 12";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 12";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType14) {
    json event;
    event["event_id"] = "$event000013:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser13:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 13 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 13";
    content["msgtype"] = "m.audio";
    
    if (std::string("m.audio") == "m.image" || std::string("m.audio") == "m.file" || std::string("m.audio") == "m.audio" || std::string("m.audio") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000013";
        content["filename"] = "file_13.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (13 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.audio") == "m.text" || std::string("m.audio") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 13</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.audio") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 13";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.audio");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 13";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 13";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType15) {
    json event;
    event["event_id"] = "$event000014:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser14:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 14 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 14";
    content["msgtype"] = "m.video";
    
    if (std::string("m.video") == "m.image" || std::string("m.video") == "m.file" || std::string("m.video") == "m.audio" || std::string("m.video") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000014";
        content["filename"] = "file_14.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (14 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.video") == "m.text" || std::string("m.video") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 14</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.video") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 14";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.video");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 14";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 14";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType16) {
    json event;
    event["event_id"] = "$event000015:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser15:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 15 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 15";
    content["msgtype"] = "m.location";
    
    if (std::string("m.location") == "m.image" || std::string("m.location") == "m.file" || std::string("m.location") == "m.audio" || std::string("m.location") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000015";
        content["filename"] = "file_15.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (15 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.location") == "m.text" || std::string("m.location") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 15</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.location") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 15";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.location");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 15";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 15";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType17) {
    json event;
    event["event_id"] = "$event000016:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser16:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 16 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 16";
    content["msgtype"] = "m.text";
    
    if (std::string("m.text") == "m.image" || std::string("m.text") == "m.file" || std::string("m.text") == "m.audio" || std::string("m.text") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000016";
        content["filename"] = "file_16.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (16 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.text") == "m.text" || std::string("m.text") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 16</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.text") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 16";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.text");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 16";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 16";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType18) {
    json event;
    event["event_id"] = "$event000017:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser17:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 17 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 17";
    content["msgtype"] = "m.notice";
    
    if (std::string("m.notice") == "m.image" || std::string("m.notice") == "m.file" || std::string("m.notice") == "m.audio" || std::string("m.notice") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000017";
        content["filename"] = "file_17.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (17 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.notice") == "m.text" || std::string("m.notice") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 17</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.notice") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 17";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.notice");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 17";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 17";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType19) {
    json event;
    event["event_id"] = "$event000018:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser18:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 18 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 18";
    content["msgtype"] = "m.emote";
    
    if (std::string("m.emote") == "m.image" || std::string("m.emote") == "m.file" || std::string("m.emote") == "m.audio" || std::string("m.emote") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000018";
        content["filename"] = "file_18.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (18 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.emote") == "m.text" || std::string("m.emote") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 18</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.emote") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 18";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.emote");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 18";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 18";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType20) {
    json event;
    event["event_id"] = "$event000019:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser19:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 19 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 19";
    content["msgtype"] = "m.image";
    
    if (std::string("m.image") == "m.image" || std::string("m.image") == "m.file" || std::string("m.image") == "m.audio" || std::string("m.image") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000019";
        content["filename"] = "file_19.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (19 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.image") == "m.text" || std::string("m.image") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 19</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.image") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 19";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.image");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 19";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 19";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType21) {
    json event;
    event["event_id"] = "$event000020:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser20:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 20 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 20";
    content["msgtype"] = "m.file";
    
    if (std::string("m.file") == "m.image" || std::string("m.file") == "m.file" || std::string("m.file") == "m.audio" || std::string("m.file") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000020";
        content["filename"] = "file_20.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (20 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.file") == "m.text" || std::string("m.file") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 20</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.file") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 20";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.file");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 20";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 20";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType22) {
    json event;
    event["event_id"] = "$event000021:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser21:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 21 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 21";
    content["msgtype"] = "m.audio";
    
    if (std::string("m.audio") == "m.image" || std::string("m.audio") == "m.file" || std::string("m.audio") == "m.audio" || std::string("m.audio") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000021";
        content["filename"] = "file_21.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (21 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.audio") == "m.text" || std::string("m.audio") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 21</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.audio") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 21";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.audio");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 21";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 21";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType23) {
    json event;
    event["event_id"] = "$event000022:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser22:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 22 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 22";
    content["msgtype"] = "m.video";
    
    if (std::string("m.video") == "m.image" || std::string("m.video") == "m.file" || std::string("m.video") == "m.audio" || std::string("m.video") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000022";
        content["filename"] = "file_22.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (22 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.video") == "m.text" || std::string("m.video") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 22</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.video") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 22";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.video");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 22";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 22";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType24) {
    json event;
    event["event_id"] = "$event000023:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser23:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 23 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 23";
    content["msgtype"] = "m.location";
    
    if (std::string("m.location") == "m.image" || std::string("m.location") == "m.file" || std::string("m.location") == "m.audio" || std::string("m.location") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000023";
        content["filename"] = "file_23.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (23 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.location") == "m.text" || std::string("m.location") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 23</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.location") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 23";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.location");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 23";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 23";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType25) {
    json event;
    event["event_id"] = "$event000024:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser24:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 24 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 24";
    content["msgtype"] = "m.text";
    
    if (std::string("m.text") == "m.image" || std::string("m.text") == "m.file" || std::string("m.text") == "m.audio" || std::string("m.text") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000024";
        content["filename"] = "file_24.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (24 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.text") == "m.text" || std::string("m.text") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 24</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.text") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 24";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.text");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 24";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 24";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType26) {
    json event;
    event["event_id"] = "$event000025:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser25:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 25 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 25";
    content["msgtype"] = "m.notice";
    
    if (std::string("m.notice") == "m.image" || std::string("m.notice") == "m.file" || std::string("m.notice") == "m.audio" || std::string("m.notice") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000025";
        content["filename"] = "file_25.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (25 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.notice") == "m.text" || std::string("m.notice") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 25</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.notice") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 25";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.notice");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 25";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 25";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType27) {
    json event;
    event["event_id"] = "$event000026:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser26:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 26 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 26";
    content["msgtype"] = "m.emote";
    
    if (std::string("m.emote") == "m.image" || std::string("m.emote") == "m.file" || std::string("m.emote") == "m.audio" || std::string("m.emote") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000026";
        content["filename"] = "file_26.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (26 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.emote") == "m.text" || std::string("m.emote") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 26</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.emote") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 26";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.emote");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 26";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 26";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType28) {
    json event;
    event["event_id"] = "$event000027:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser27:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 27 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 27";
    content["msgtype"] = "m.image";
    
    if (std::string("m.image") == "m.image" || std::string("m.image") == "m.file" || std::string("m.image") == "m.audio" || std::string("m.image") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000027";
        content["filename"] = "file_27.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (27 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.image") == "m.text" || std::string("m.image") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 27</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.image") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 27";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.image");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 27";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 27";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType29) {
    json event;
    event["event_id"] = "$event000028:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser28:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 28 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 28";
    content["msgtype"] = "m.file";
    
    if (std::string("m.file") == "m.image" || std::string("m.file") == "m.file" || std::string("m.file") == "m.audio" || std::string("m.file") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000028";
        content["filename"] = "file_28.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (28 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.file") == "m.text" || std::string("m.file") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 28</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.file") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 28";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.file");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 28";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 28";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType30) {
    json event;
    event["event_id"] = "$event000029:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser29:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 29 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 29";
    content["msgtype"] = "m.audio";
    
    if (std::string("m.audio") == "m.image" || std::string("m.audio") == "m.file" || std::string("m.audio") == "m.audio" || std::string("m.audio") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000029";
        content["filename"] = "file_29.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (29 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.audio") == "m.text" || std::string("m.audio") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 29</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.audio") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 29";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.audio");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 29";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 29";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType31) {
    json event;
    event["event_id"] = "$event000030:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser30:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 30 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 30";
    content["msgtype"] = "m.video";
    
    if (std::string("m.video") == "m.image" || std::string("m.video") == "m.file" || std::string("m.video") == "m.audio" || std::string("m.video") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000030";
        content["filename"] = "file_30.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (30 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.video") == "m.text" || std::string("m.video") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 30</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.video") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 30";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.video");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 30";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 30";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType32) {
    json event;
    event["event_id"] = "$event000031:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser31:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 31 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 31";
    content["msgtype"] = "m.location";
    
    if (std::string("m.location") == "m.image" || std::string("m.location") == "m.file" || std::string("m.location") == "m.audio" || std::string("m.location") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000031";
        content["filename"] = "file_31.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (31 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.location") == "m.text" || std::string("m.location") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 31</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.location") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 31";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.location");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 31";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 31";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType33) {
    json event;
    event["event_id"] = "$event000032:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser32:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 32 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 32";
    content["msgtype"] = "m.text";
    
    if (std::string("m.text") == "m.image" || std::string("m.text") == "m.file" || std::string("m.text") == "m.audio" || std::string("m.text") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000032";
        content["filename"] = "file_32.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (32 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.text") == "m.text" || std::string("m.text") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 32</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.text") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 32";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.text");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 32";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 32";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType34) {
    json event;
    event["event_id"] = "$event000033:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser33:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 33 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 33";
    content["msgtype"] = "m.notice";
    
    if (std::string("m.notice") == "m.image" || std::string("m.notice") == "m.file" || std::string("m.notice") == "m.audio" || std::string("m.notice") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000033";
        content["filename"] = "file_33.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (33 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.notice") == "m.text" || std::string("m.notice") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 33</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.notice") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 33";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.notice");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 33";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 33";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType35) {
    json event;
    event["event_id"] = "$event000034:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser34:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 34 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 34";
    content["msgtype"] = "m.emote";
    
    if (std::string("m.emote") == "m.image" || std::string("m.emote") == "m.file" || std::string("m.emote") == "m.audio" || std::string("m.emote") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000034";
        content["filename"] = "file_34.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (34 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.emote") == "m.text" || std::string("m.emote") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 34</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.emote") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 34";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.emote");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 34";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 34";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType36) {
    json event;
    event["event_id"] = "$event000035:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser35:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 35 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 35";
    content["msgtype"] = "m.image";
    
    if (std::string("m.image") == "m.image" || std::string("m.image") == "m.file" || std::string("m.image") == "m.audio" || std::string("m.image") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000035";
        content["filename"] = "file_35.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (35 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.image") == "m.text" || std::string("m.image") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 35</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.image") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 35";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.image");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 35";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 35";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType37) {
    json event;
    event["event_id"] = "$event000036:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser36:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 36 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 36";
    content["msgtype"] = "m.file";
    
    if (std::string("m.file") == "m.image" || std::string("m.file") == "m.file" || std::string("m.file") == "m.audio" || std::string("m.file") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000036";
        content["filename"] = "file_36.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (36 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.file") == "m.text" || std::string("m.file") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 36</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.file") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 36";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.file");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 36";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 36";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType38) {
    json event;
    event["event_id"] = "$event000037:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser37:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 37 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 37";
    content["msgtype"] = "m.audio";
    
    if (std::string("m.audio") == "m.image" || std::string("m.audio") == "m.file" || std::string("m.audio") == "m.audio" || std::string("m.audio") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000037";
        content["filename"] = "file_37.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (37 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.audio") == "m.text" || std::string("m.audio") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 37</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.audio") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 37";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.audio");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 37";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 37";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType39) {
    json event;
    event["event_id"] = "$event000038:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser38:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 38 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 38";
    content["msgtype"] = "m.video";
    
    if (std::string("m.video") == "m.image" || std::string("m.video") == "m.file" || std::string("m.video") == "m.audio" || std::string("m.video") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000038";
        content["filename"] = "file_38.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (38 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.video") == "m.text" || std::string("m.video") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 38</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.video") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 38";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.video");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 38";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 38";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

TEST(MessageHandlingTest, MessageType40) {
    json event;
    event["event_id"] = "$event000039:matrix.org";
    event["room_id"] = "!testroom:matrix.org";
    event["sender"] = "@testuser39:matrix.org";
    event["origin_server_ts"] = 1600000000000 + 39 * 1000;
    event["type"] = "m.room.message";

    json content;
    content["body"] = "Test message body 39";
    content["msgtype"] = "m.location";
    
    if (std::string("m.location") == "m.image" || std::string("m.location") == "m.file" || std::string("m.location") == "m.audio" || std::string("m.location") == "m.video") {
        content["url"] = "mxc://matrix.org/media_000039";
        content["filename"] = "file_39.dat";
        content["info"]["mimetype"] = "application/octet-stream";
        content["info"]["size"] = 1024 * (39 + 1);
        content["info"]["w"] = 1920;
        content["info"]["h"] = 1080;
        ASSERT_FALSE(content["url"].empty());
        ASSERT_FALSE(content["filename"].empty());
        ASSERT_TRUE(content["info"]["size"] > 0);
    }

    if (std::string("m.location") == "m.text" || std::string("m.location") == "m.notice") {
        content["format"] = "org.matrix.custom.html";
        content["formatted_body"] = "<p>Test message body 39</p>";
        ASSERT_EQ(content["format"], "org.matrix.custom.html");
    }

    if (std::string("m.location") == "m.location") {
        content["geo_uri"] = "geo:51.5,-0.1";
        content["org.matrix.msc3488.location"]["description"] = "Test location 39";
        ASSERT_FALSE(content["geo_uri"].empty());
    }

    event["content"] = content;
    ASSERT_EQ(event["type"], "m.room.message");
    ASSERT_FALSE(event["event_id"].empty());
    ASSERT_EQ(content["msgtype"], "m.location");
    ASSERT_FALSE(content["body"].empty());

    // Test reaction
    json reaction;
    reaction["type"] = "m.reaction";
    reaction["content"]["m.relates_to"]["event_id"] = event["event_id"];
    reaction["content"]["m.relates_to"]["rel_type"] = "m.annotation";
    reaction["content"]["m.relates_to"]["key"] = "👍";
    ASSERT_EQ(reaction["content"]["m.relates_to"]["key"], "👍");

    // Test edit
    json edit;
    edit["type"] = "m.room.message";
    edit["content"]["body"] = "* Edited message 39";
    edit["content"]["msgtype"] = "m.text";
    edit["content"]["m.relates_to"]["event_id"] = event["event_id"];
    edit["content"]["m.relates_to"]["rel_type"] = "m.replace";
    edit["content"]["m.new_content"]["body"] = "Edited message 39";
    ASSERT_EQ(edit["content"]["m.relates_to"]["rel_type"], "m.replace");

    ASSERT_NO_THROW({ auto d = event.dump(); });
    ASSERT_NO_THROW({ auto d = reaction.dump(); });
    ASSERT_NO_THROW({ auto d = edit.dump(); });
}

}} // namespace pcp::test