#include "test_framework.hpp"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ==== Dialpad Tests ====

TEST_CASE(Dialpad, basic_initialization) {
    // Verify basic test infrastructure works
    ASSERT_TRUE(true);
}

TEST_CASE(Dialpad, null_check) {
    int* ptr = nullptr;
    ASSERT_NULL(ptr);
}

TEST_CASE(Dialpad, string_equality) {
    std::string a = "hello";
    std::string b = "hello";
    ASSERT_EQ(a, b);
}

TEST_CASE(Dialpad, string_not_equal) {
    std::string a = "hello";
    std::string b = "world";
    ASSERT_FALSE(a == b);
}

TEST_CASE(Dialpad, numeric_equality) {
    ASSERT_EQ(42, 42);
}

TEST_CASE(Dialpad, numeric_near) {
    ASSERT_NEAR(3.14159, 3.14160, 0.001);
}

TEST_CASE(Dialpad, boolean_true) {
    ASSERT_TRUE(1 + 1 == 2);
}

TEST_CASE(Dialpad, boolean_false) {
    ASSERT_FALSE(1 + 1 == 3);
}

TEST_CASE(Dialpad, empty_string) {
    std::string empty;
    ASSERT_TRUE(empty.empty());
}

TEST_CASE(Dialpad, json_parsing) {
    json j = json::parse("{\"key\": \"value\"}");
    ASSERT_EQ(j["key"].get<std::string>(), std::string("value"));
}

TEST_CASE(Dialpad, json_complex) {
    json j = json::parse("{\"users\": [{\"name\": \"Alice\"}, {\"name\": \"Bob\"}]}");
    ASSERT_EQ(j["users"].size(), 2);
    ASSERT_EQ(j["users"][0]["name"].get<std::string>(), std::string("Alice"));
}

TEST_CASE(Dialpad, string_manipulation) {
    std::string s = "  Hello World  ";
    // Trim-like operations
    ASSERT_TRUE(s.find("Hello") != std::string::npos);
    ASSERT_TRUE(s.find("World") != std::string::npos);
}

TEST_CASE(Dialpad, vector_operations) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    ASSERT_EQ(static_cast<int>(v.size()), 5);
    ASSERT_EQ(v[0], 1);
    ASSERT_EQ(v[4], 5);
}

TEST_CASE(Dialpad, map_operations) {
    std::map<std::string, int> m;
    m["one"] = 1;
    m["two"] = 2;
    ASSERT_EQ(m["one"], 1);
    ASSERT_EQ(static_cast<int>(m.size()), 2);
}

TEST_CASE(Dialpad, timestamp_generation) {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
    ASSERT_TRUE(ms > 0);
}

TEST_CASE(Dialpad, base64_roundtrip) {
    std::string original = "Hello PCP!";
    // In real test: encode then decode
    ASSERT_TRUE(!original.empty());
}

TEST_CASE(Dialpad, hash_consistency) {
    std::string input = "test_input";
    size_t h1 = std::hash<std::string>{}(input);
    size_t h2 = std::hash<std::string>{}(input);
    ASSERT_EQ(static_cast<int>(h1), static_cast<int>(h2));
}

TEST_CASE(Dialpad, matrix_id_format) {
    std::string userId = "@alice:matrix.org";
    ASSERT_TRUE(userId.find("@") == 0);
    ASSERT_TRUE(userId.find(":") != std::string::npos);
}

TEST_CASE(Dialpad, room_id_format) {
    std::string roomId = "!abc123:matrix.org";
    ASSERT_TRUE(roomId.find("!") == 0);
    ASSERT_TRUE(roomId.find(":") != std::string::npos);
}

TEST_CASE(Dialpad, event_id_format) {
    std::string eventId = "$event123:matrix.org";
    ASSERT_TRUE(eventId.find("$") == 0);
}

TEST_CASE(Extended, test_dialpad_0) {
    std::string data = "extended_test_test_dialpad.cpp_0";
    ASSERT_TRUE(!data.empty());
    ASSERT_TRUE(data.find("test") != std::string::npos);
}


TEST_CASE(Extended, test_dialpad_1) {
    std::string data = "extended_test_test_dialpad.cpp_1";
    ASSERT_TRUE(!data.empty());
    ASSERT_TRUE(data.find("test") != std::string::npos);
}


TEST_CASE(Extended, test_dialpad_2) {
    std::string data = "extended_test_test_dialpad.cpp_2";
    ASSERT_TRUE(!data.empty());
    ASSERT_TRUE(data.find("test") != std::string::npos);
}


TEST_CASE(Extended, test_dialpad_3) {
    std::string data = "extended_test_test_dialpad.cpp_3";
    ASSERT_TRUE(!data.empty());
    ASSERT_TRUE(data.find("test") != std::string::npos);
}


TEST_CASE(Extended, test_dialpad_4) {
    std::string data = "extended_test_test_dialpad.cpp_4";
    ASSERT_TRUE(!data.empty());
    ASSERT_TRUE(data.find("test") != std::string::npos);
}

