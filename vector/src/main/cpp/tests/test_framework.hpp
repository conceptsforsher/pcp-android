#pragma once
// Minimal C++ test framework — no external dependencies
// Usage: TEST(name) { ... } → ASSERT_EQ(a,b); ASSERT_TRUE(x);

#include <cstdio>
#include <string>
#include <vector>
#include <cmath>

struct TestRunner {
    int passed = 0, failed = 0;
    const char* currentTest = "";
    
    void run(const char* name, void (*fn)()) {
        currentTest = name;
        try { fn(); passed++; printf("  PASS %s\n", name); }
        catch (const char* e) { failed++; printf("  FAIL %s: %s\n", name, e); }
    }
    
    int summary() {
        printf("\n%d passed, %d failed\n", passed, failed);
        return failed > 0 ? 1 : 0;
    }
};

#define TEST_RUNNER(name) static TestRunner name
#define ADD_TEST(runner, name) runner.run(#name, name)
#define ASSERT_EQ(a,b) if ((a)!=(b)) throw "ASSERT_EQ failed"
#define ASSERT_TRUE(x)  if (!(x)) throw "ASSERT_TRUE failed"
#define ASSERT_FALSE(x) if (x) throw "ASSERT_FALSE failed"
#define ASSERT_NE(a,b)  if ((a)==(b)) throw "ASSERT_NE failed"
#define ASSERT_STREQ(a,b) if (std::string(a)!=(b)) throw "ASSERT_STREQ failed"
#define ASSERT_GT(a,b)  if (!((a)>(b))) throw "ASSERT_GT failed"
#define ASSERT_LT(a,b)  if (!((a)<(b))) throw "ASSERT_LT failed"
