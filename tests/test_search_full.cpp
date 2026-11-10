#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SearchTest, FullSearch1) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword0";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom0:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 10;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 1.0 - r * 0.1;
        result["result"]["event_id"] = "$search0_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom0:matrix.org";
        result["result"]["sender"] = "@searcher0:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 0 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user0";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser0_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 0_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_0_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword0</b> match";
    highlight["formatted_body"] = "This is a <b>keyword0</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch2) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword1";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom1:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 11;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.98 - r * 0.1;
        result["result"]["event_id"] = "$search1_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom1:matrix.org";
        result["result"]["sender"] = "@searcher1:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 1 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user1";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser1_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 1_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_1_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword1</b> match";
    highlight["formatted_body"] = "This is a <b>keyword1</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch3) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword2";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom2:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 12;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.96 - r * 0.1;
        result["result"]["event_id"] = "$search2_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom2:matrix.org";
        result["result"]["sender"] = "@searcher2:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 2 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user2";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser2_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 2_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_2_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword2</b> match";
    highlight["formatted_body"] = "This is a <b>keyword2</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch4) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword3";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom3:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 13;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.94 - r * 0.1;
        result["result"]["event_id"] = "$search3_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom3:matrix.org";
        result["result"]["sender"] = "@searcher3:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 3 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user3";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser3_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 3_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_3_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword3</b> match";
    highlight["formatted_body"] = "This is a <b>keyword3</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch5) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword4";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom4:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 14;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.92 - r * 0.1;
        result["result"]["event_id"] = "$search4_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom4:matrix.org";
        result["result"]["sender"] = "@searcher4:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 4 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user4";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser4_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 4_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_4_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword4</b> match";
    highlight["formatted_body"] = "This is a <b>keyword4</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch6) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword5";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom5:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 15;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.9 - r * 0.1;
        result["result"]["event_id"] = "$search5_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom5:matrix.org";
        result["result"]["sender"] = "@searcher5:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 5 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user5";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser5_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 5_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_5_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword5</b> match";
    highlight["formatted_body"] = "This is a <b>keyword5</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch7) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword6";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom6:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 16;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.88 - r * 0.1;
        result["result"]["event_id"] = "$search6_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom6:matrix.org";
        result["result"]["sender"] = "@searcher6:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 6 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user6";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser6_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 6_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_6_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword6</b> match";
    highlight["formatted_body"] = "This is a <b>keyword6</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch8) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword7";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom7:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 17;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.86 - r * 0.1;
        result["result"]["event_id"] = "$search7_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom7:matrix.org";
        result["result"]["sender"] = "@searcher7:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 7 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user7";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser7_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 7_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_7_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword7</b> match";
    highlight["formatted_body"] = "This is a <b>keyword7</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch9) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword8";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom8:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 18;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.84 - r * 0.1;
        result["result"]["event_id"] = "$search8_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom8:matrix.org";
        result["result"]["sender"] = "@searcher8:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 8 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user8";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser8_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 8_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_8_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword8</b> match";
    highlight["formatted_body"] = "This is a <b>keyword8</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch10) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword9";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom9:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 19;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.8200000000000001 - r * 0.1;
        result["result"]["event_id"] = "$search9_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom9:matrix.org";
        result["result"]["sender"] = "@searcher9:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 9 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user9";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser9_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 9_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_9_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword9</b> match";
    highlight["formatted_body"] = "This is a <b>keyword9</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch11) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword10";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom10:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 20;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.8 - r * 0.1;
        result["result"]["event_id"] = "$search10_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom10:matrix.org";
        result["result"]["sender"] = "@searcher10:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 10 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user10";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser10_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 10_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_10_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword10</b> match";
    highlight["formatted_body"] = "This is a <b>keyword10</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch12) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword11";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom11:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 21;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.78 - r * 0.1;
        result["result"]["event_id"] = "$search11_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom11:matrix.org";
        result["result"]["sender"] = "@searcher11:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 11 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user11";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser11_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 11_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_11_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword11</b> match";
    highlight["formatted_body"] = "This is a <b>keyword11</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch13) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword12";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom12:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 22;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.76 - r * 0.1;
        result["result"]["event_id"] = "$search12_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom12:matrix.org";
        result["result"]["sender"] = "@searcher12:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 12 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user12";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser12_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 12_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_12_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword12</b> match";
    highlight["formatted_body"] = "This is a <b>keyword12</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch14) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword13";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom13:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 23;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.74 - r * 0.1;
        result["result"]["event_id"] = "$search13_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom13:matrix.org";
        result["result"]["sender"] = "@searcher13:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 13 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user13";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser13_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 13_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_13_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword13</b> match";
    highlight["formatted_body"] = "This is a <b>keyword13</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch15) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword14";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom14:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 24;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.72 - r * 0.1;
        result["result"]["event_id"] = "$search14_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom14:matrix.org";
        result["result"]["sender"] = "@searcher14:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 14 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user14";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser14_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 14_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_14_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword14</b> match";
    highlight["formatted_body"] = "This is a <b>keyword14</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch16) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword15";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom15:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 25;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.7 - r * 0.1;
        result["result"]["event_id"] = "$search15_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom15:matrix.org";
        result["result"]["sender"] = "@searcher15:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 15 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user15";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser15_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 15_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_15_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword15</b> match";
    highlight["formatted_body"] = "This is a <b>keyword15</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch17) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword16";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom16:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 26;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.6799999999999999 - r * 0.1;
        result["result"]["event_id"] = "$search16_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom16:matrix.org";
        result["result"]["sender"] = "@searcher16:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 16 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user16";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser16_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 16_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_16_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword16</b> match";
    highlight["formatted_body"] = "This is a <b>keyword16</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch18) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword17";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom17:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 27;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.6599999999999999 - r * 0.1;
        result["result"]["event_id"] = "$search17_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom17:matrix.org";
        result["result"]["sender"] = "@searcher17:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 17 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user17";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser17_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 17_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_17_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword17</b> match";
    highlight["formatted_body"] = "This is a <b>keyword17</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch19) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword18";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom18:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 28;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.64 - r * 0.1;
        result["result"]["event_id"] = "$search18_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom18:matrix.org";
        result["result"]["sender"] = "@searcher18:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 18 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user18";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser18_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 18_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_18_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword18</b> match";
    highlight["formatted_body"] = "This is a <b>keyword18</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch20) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword19";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom19:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 29;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.62 - r * 0.1;
        result["result"]["event_id"] = "$search19_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom19:matrix.org";
        result["result"]["sender"] = "@searcher19:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 19 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user19";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser19_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 19_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_19_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword19</b> match";
    highlight["formatted_body"] = "This is a <b>keyword19</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch21) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword20";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom20:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 30;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.6 - r * 0.1;
        result["result"]["event_id"] = "$search20_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom20:matrix.org";
        result["result"]["sender"] = "@searcher20:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 20 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user20";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser20_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 20_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_20_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword20</b> match";
    highlight["formatted_body"] = "This is a <b>keyword20</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch22) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword21";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom21:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 31;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.5800000000000001 - r * 0.1;
        result["result"]["event_id"] = "$search21_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom21:matrix.org";
        result["result"]["sender"] = "@searcher21:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 21 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user21";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser21_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 21_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_21_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword21</b> match";
    highlight["formatted_body"] = "This is a <b>keyword21</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch23) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword22";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom22:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 32;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.56 - r * 0.1;
        result["result"]["event_id"] = "$search22_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom22:matrix.org";
        result["result"]["sender"] = "@searcher22:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 22 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user22";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser22_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 22_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_22_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword22</b> match";
    highlight["formatted_body"] = "This is a <b>keyword22</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch24) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword23";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom23:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 33;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.54 - r * 0.1;
        result["result"]["event_id"] = "$search23_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom23:matrix.org";
        result["result"]["sender"] = "@searcher23:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 23 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user23";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser23_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 23_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_23_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword23</b> match";
    highlight["formatted_body"] = "This is a <b>keyword23</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

TEST(SearchTest, FullSearch25) {
    json searchReq;
    searchReq["search_categories"]["room_events"]["search_term"] = "keyword24";
    searchReq["search_categories"]["room_events"]["filter"]["rooms"] = json::array();
    searchReq["search_categories"]["room_events"]["filter"]["rooms"].push_back("!searchroom24:matrix.org");
    searchReq["search_categories"]["room_events"]["order_by"] = "recent";
    searchReq["search_categories"]["room_events"]["event_context"]["before_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["after_limit"] = 5;
    searchReq["search_categories"]["room_events"]["event_context"]["include_profile"] = true;
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["search_term"].empty());
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["order_by"], "recent");

    json searchResults;
    searchResults["search_categories"]["room_events"]["count"] = 34;
    searchResults["search_categories"]["room_events"]["results"] = json::array();
    for (int r = 0; r < 3; ++r) {
        json result;
        result["rank"] = 0.52 - r * 0.1;
        result["result"]["event_id"] = "$search24_" + std::to_string(r) + ":matrix.org";
        result["result"]["room_id"] = "!searchroom24:matrix.org";
        result["result"]["sender"] = "@searcher24:matrix.org";
        result["result"]["type"] = "m.room.message";
        result["result"]["content"]["body"] = "Search result 24 item " + std::to_string(r);
        searchResults["search_categories"]["room_events"]["results"].push_back(result);
    }
    ASSERT_EQ(searchResults["search_categories"]["room_events"]["results"].size(), 3);

    json userSearch;
    userSearch["search_term"] = "user24";
    userSearch["limit"] = 10;
    ASSERT_EQ(userSearch["limit"], 10);
    ASSERT_FALSE(userSearch["search_term"].empty());

    json userResults = json::array();
    for (int u = 0; u < 3; ++u) {
        json ur;
        ur["user_id"] = "@founduser24_" + std::to_string(u) + ":matrix.org";
        ur["display_name"] = "Found User 24_" + std::to_string(u);
        ur["avatar_url"] = "mxc://matrix.org/avatar_24_" + std::to_string(u);
        userResults.push_back(ur);
    }
    ASSERT_EQ(userResults.size(), 3);

    json highlight;
    highlight["body"] = "This is a <b>keyword24</b> match";
    highlight["formatted_body"] = "This is a <b>keyword24</b> match";
    ASSERT_FALSE(highlight["body"].empty());

    ASSERT_NO_THROW({ auto d = searchReq.dump(); });
    ASSERT_NO_THROW({ auto d = searchResults.dump(); });
}

}} // namespace pcp::test
