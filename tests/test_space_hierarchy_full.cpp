#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SpaceTest, Hierarchy1) {
    json space;
    space["room_id"] = "!space0:matrix.org";
    space["name"] = "Space 0";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child0_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 0_" + std::to_string(c);
        child["topic"] = "Child topic 0_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_0_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_0_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild0:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_0";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child0_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_0";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy2) {
    json space;
    space["room_id"] = "!space1:matrix.org";
    space["name"] = "Space 1";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child1_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 1_" + std::to_string(c);
        child["topic"] = "Child topic 1_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_1_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_1_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild1:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_1";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child1_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_1";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy3) {
    json space;
    space["room_id"] = "!space2:matrix.org";
    space["name"] = "Space 2";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child2_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 2_" + std::to_string(c);
        child["topic"] = "Child topic 2_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_2_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_2_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild2:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_2";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child2_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_2";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy4) {
    json space;
    space["room_id"] = "!space3:matrix.org";
    space["name"] = "Space 3";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child3_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 3_" + std::to_string(c);
        child["topic"] = "Child topic 3_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_3_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_3_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild3:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_3";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child3_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_3";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy5) {
    json space;
    space["room_id"] = "!space4:matrix.org";
    space["name"] = "Space 4";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child4_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 4_" + std::to_string(c);
        child["topic"] = "Child topic 4_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_4_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_4_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild4:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_4";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child4_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_4";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy6) {
    json space;
    space["room_id"] = "!space5:matrix.org";
    space["name"] = "Space 5";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child5_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 5_" + std::to_string(c);
        child["topic"] = "Child topic 5_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_5_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_5_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild5:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_5";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child5_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_5";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy7) {
    json space;
    space["room_id"] = "!space6:matrix.org";
    space["name"] = "Space 6";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child6_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 6_" + std::to_string(c);
        child["topic"] = "Child topic 6_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_6_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_6_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild6:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_6";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child6_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_6";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy8) {
    json space;
    space["room_id"] = "!space7:matrix.org";
    space["name"] = "Space 7";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child7_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 7_" + std::to_string(c);
        child["topic"] = "Child topic 7_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_7_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_7_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild7:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_7";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child7_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_7";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy9) {
    json space;
    space["room_id"] = "!space8:matrix.org";
    space["name"] = "Space 8";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child8_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 8_" + std::to_string(c);
        child["topic"] = "Child topic 8_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_8_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_8_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild8:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_8";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child8_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_8";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy10) {
    json space;
    space["room_id"] = "!space9:matrix.org";
    space["name"] = "Space 9";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child9_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 9_" + std::to_string(c);
        child["topic"] = "Child topic 9_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_9_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_9_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild9:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_9";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child9_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_9";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy11) {
    json space;
    space["room_id"] = "!space10:matrix.org";
    space["name"] = "Space 10";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child10_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 10_" + std::to_string(c);
        child["topic"] = "Child topic 10_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_10_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_10_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild10:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_10";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child10_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_10";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy12) {
    json space;
    space["room_id"] = "!space11:matrix.org";
    space["name"] = "Space 11";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child11_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 11_" + std::to_string(c);
        child["topic"] = "Child topic 11_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_11_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_11_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild11:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_11";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child11_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_11";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy13) {
    json space;
    space["room_id"] = "!space12:matrix.org";
    space["name"] = "Space 12";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child12_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 12_" + std::to_string(c);
        child["topic"] = "Child topic 12_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_12_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_12_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild12:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_12";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child12_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_12";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy14) {
    json space;
    space["room_id"] = "!space13:matrix.org";
    space["name"] = "Space 13";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child13_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 13_" + std::to_string(c);
        child["topic"] = "Child topic 13_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_13_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_13_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild13:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_13";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child13_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_13";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy15) {
    json space;
    space["room_id"] = "!space14:matrix.org";
    space["name"] = "Space 14";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child14_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 14_" + std::to_string(c);
        child["topic"] = "Child topic 14_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_14_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_14_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild14:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_14";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child14_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_14";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy16) {
    json space;
    space["room_id"] = "!space15:matrix.org";
    space["name"] = "Space 15";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child15_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 15_" + std::to_string(c);
        child["topic"] = "Child topic 15_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_15_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_15_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild15:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_15";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child15_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_15";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy17) {
    json space;
    space["room_id"] = "!space16:matrix.org";
    space["name"] = "Space 16";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child16_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 16_" + std::to_string(c);
        child["topic"] = "Child topic 16_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_16_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_16_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild16:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_16";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child16_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_16";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy18) {
    json space;
    space["room_id"] = "!space17:matrix.org";
    space["name"] = "Space 17";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child17_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 17_" + std::to_string(c);
        child["topic"] = "Child topic 17_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_17_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_17_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild17:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_17";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child17_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_17";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy19) {
    json space;
    space["room_id"] = "!space18:matrix.org";
    space["name"] = "Space 18";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child18_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 18_" + std::to_string(c);
        child["topic"] = "Child topic 18_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_18_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_18_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild18:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_18";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child18_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_18";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy20) {
    json space;
    space["room_id"] = "!space19:matrix.org";
    space["name"] = "Space 19";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child19_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 19_" + std::to_string(c);
        child["topic"] = "Child topic 19_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_19_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_19_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild19:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_19";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child19_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_19";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy21) {
    json space;
    space["room_id"] = "!space20:matrix.org";
    space["name"] = "Space 20";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child20_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 20_" + std::to_string(c);
        child["topic"] = "Child topic 20_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_20_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_20_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild20:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_20";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child20_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_20";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy22) {
    json space;
    space["room_id"] = "!space21:matrix.org";
    space["name"] = "Space 21";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child21_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 21_" + std::to_string(c);
        child["topic"] = "Child topic 21_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_21_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_21_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild21:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_21";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child21_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_21";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy23) {
    json space;
    space["room_id"] = "!space22:matrix.org";
    space["name"] = "Space 22";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child22_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 22_" + std::to_string(c);
        child["topic"] = "Child topic 22_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_22_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_22_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild22:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_22";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child22_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_22";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy24) {
    json space;
    space["room_id"] = "!space23:matrix.org";
    space["name"] = "Space 23";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child23_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 23_" + std::to_string(c);
        child["topic"] = "Child topic 23_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_23_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_23_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild23:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_23";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child23_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_23";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

TEST(SpaceTest, Hierarchy25) {
    json space;
    space["room_id"] = "!space24:matrix.org";
    space["name"] = "Space 24";
    space["room_type"] = "m.space";
    ASSERT_EQ(space["room_type"], "m.space");

    json hierarchy;
    hierarchy["rooms"] = json::array();
    for (int c = 0; c < 4; ++c) {
        json child;
        child["room_id"] = "!child24_" + std::to_string(c) + ":matrix.org";
        child["name"] = "Child Room 24_" + std::to_string(c);
        child["topic"] = "Child topic 24_" + std::to_string(c);
        child["avatar_url"] = "mxc://matrix.org/avatar_24_" + std::to_string(c);
        child["num_joined_members"] = 5 + c;
        child["room_type"] = c % 2 == 0 ? "m.space" : "";
        child["children_state"] = json::array();
        child["suggested"] = c < 2;
        child["order"] = "order_24_" + std::to_string(c);
        hierarchy["rooms"].push_back(child);
    }
    ASSERT_EQ(hierarchy["rooms"].size(), 4);

    json suggested = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["suggested"]) suggested.push_back(room["room_id"]);
    }
    ASSERT_EQ(suggested.size(), 2);

    json subspaces = json::array();
    for (const auto& room : hierarchy["rooms"]) {
        if (room["room_type"] == "m.space") subspaces.push_back(room["room_id"]);
    }
    ASSERT_EQ(subspaces.size(), 2);

    json addChild;
    addChild["space_id"] = space["room_id"];
    addChild["child_room_id"] = "!newchild24:matrix.org";
    addChild["suggested"] = true;
    addChild["order"] = "new_order_24";
    ASSERT_TRUE(addChild["suggested"]);
    ASSERT_FALSE(addChild["child_room_id"].empty());

    json removeChild;
    removeChild["space_id"] = space["room_id"];
    removeChild["child_room_id"] = "!child24_0:matrix.org";
    ASSERT_FALSE(removeChild["child_room_id"].empty());

    json order;
    order["space_id"] = space["room_id"];
    order["order"] = "new_order_24";
    ASSERT_FALSE(order["order"].empty());

    ASSERT_NO_THROW({ auto d = space.dump(); });
    ASSERT_NO_THROW({ auto d = hierarchy.dump(); });
}

}} // namespace pcp::test
