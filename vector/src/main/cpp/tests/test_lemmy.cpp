#include <cstdio>
#include <string>
#include <unordered_map>

// Stub out progressive::httpExecute so lemmy_api.cpp links without real HTTP
namespace progressive {
struct HttpRequest { std::string method; std::string url; std::string body; };
struct HttpResponse { int statusCode = 200; std::string body; bool success = true; std::string errorMessage; };
HttpResponse httpExecute(const HttpRequest&) { return {}; }
HttpResponse httpPost(const std::string&, const std::string&,
    const std::unordered_map<std::string, std::string>&, int) { return {}; }
HttpResponse httpPut(const std::string&, const std::string&, int) { return {}; }
}

#include "test_framework.hpp"
#include "progressive/lemmy_api.hpp"

static void test_lemmy_build_json_empty() {
    auto r = progressive::lemmyBuildJson({});
    ASSERT_STREQ(r, "{}");
}

static void test_lemmy_build_json_single_int() {
    auto r = progressive::lemmyBuildJson({{"id", "42"}});
    ASSERT_STREQ(r, R"({"id":42})");
}

static void test_lemmy_build_json_string() {
    auto r = progressive::lemmyBuildJson({{"name", "\"hello\""}});
    ASSERT_STREQ(r, R"({"name":"hello"})");
}

static void test_lemmy_build_json_bool() {
    auto r = progressive::lemmyBuildJson({{"a", "true"}, {"b", "false"}});
    ASSERT_STREQ(r, R"({"a":true,"b":false})");
}

static void test_lemmy_build_query_single() {
    auto r = progressive::lemmyBuildQuery({{"type_", "Local"}});
    ASSERT_STREQ(r, "type_=Local");
}

static void test_lemmy_build_query_multiple() {
    auto r = progressive::lemmyBuildQuery({{"sort", "Active"}, {"page", "1"}});
    ASSERT_STREQ(r, "sort=Active&page=1");
}

static void test_lemmy_build_query_space() {
    auto r = progressive::lemmyBuildQuery({{"q", "hello world"}});
    ASSERT_STREQ(r, "q=hello%20world");
}

static void test_lemmy_build_url() {
    progressive::setLemmyInstance("https://lemmy.ml");
    ASSERT_STREQ(progressive::lemmyBuildUrl("/api/v3/site"), "https://lemmy.ml/api/v3/site");
}

static void test_lemmy_build_url_trailing_slash() {
    progressive::setLemmyInstance("https://lemmy.ml/");
    ASSERT_STREQ(progressive::lemmyBuildUrl("/api/v3/site"), "https://lemmy.ml/api/v3/site");
}

int main() {
    TEST_RUNNER(runner);

    printf("\n-- Lemmy JSON Builder --\n");
    ADD_TEST(runner, test_lemmy_build_json_empty);
    ADD_TEST(runner, test_lemmy_build_json_single_int);
    ADD_TEST(runner, test_lemmy_build_json_string);
    ADD_TEST(runner, test_lemmy_build_json_bool);

    printf("\n-- Lemmy Query Builder --\n");
    ADD_TEST(runner, test_lemmy_build_query_single);
    ADD_TEST(runner, test_lemmy_build_query_multiple);
    ADD_TEST(runner, test_lemmy_build_query_space);

    printf("\n-- Lemmy URL Builder --\n");
    ADD_TEST(runner, test_lemmy_build_url);
    ADD_TEST(runner, test_lemmy_build_url_trailing_slash);

    return runner.summary();
}
