#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <chrono>
#include <cassert>
#include <sstream>
#include <cmath>

namespace test {

struct TestCase {
    std::string name;
    std::string suite;
    std::function<void()> func;
    bool passed = false;
    double durationMs = 0.0;
    std::string error;
};

struct TestSuite {
    std::string name;
    std::vector<TestCase> tests;
    int passed = 0, failed = 0;
    double totalDurationMs = 0.0;
};

class TestRunner {
public:
    static TestRunner& instance() {
        static TestRunner runner;
        return runner;
    }

    TestSuite& suite(const std::string& name) {
        for (auto& s : m_suites) if (s.name == name) return s;
        m_suites.push_back({name});
        return m_suites.back();
    }

    void addTest(const std::string& suiteName, const std::string& testName,
                 std::function<void()> func) {
        suite(suiteName).tests.push_back({testName, suiteName, std::move(func)});
    }

    int run() {
        int totalPassed = 0, totalFailed = 0;
        std::cout << "\n=== PCP Test Suite ===\n" << std::endl;

        for (auto& s : m_suites) {
            std::cout << "Suite: " << s.name << " (" << s.tests.size() << " tests)" << std::endl;
            for (auto& t : s.tests) {
                auto start = std::chrono::steady_clock::now();
                try {
                    t.func();
                    t.passed = true;
                    s.passed++;
                    totalPassed++;
                } catch (const std::exception& e) {
                    t.error = e.what();
                    t.passed = false;
                    s.failed++;
                    totalFailed++;
                } catch (...) {
                    t.error = "Unknown error";
                    t.passed = false;
                    s.failed++;
                    totalFailed++;
                }
                auto end = std::chrono::steady_clock::now();
                t.durationMs = std::chrono::duration<double, std::milli>(end - start).count();
                s.totalDurationMs += t.durationMs;

                std::cout << "  " << (t.passed ? "PASS" : "FAIL") << "  "
                          << t.name << " (" << t.durationMs << "ms)";
                if (!t.passed) std::cout << " - " << t.error;
                std::cout << std::endl;
            }
        }

        std::cout << "\n=== Results: " << totalPassed << " passed, "
                  << totalFailed << " failed ===\n" << std::endl;
        return totalFailed;
    }

    void assertTrue(bool condition, const std::string& msg = "") {
        if (!condition) throw std::runtime_error("ASSERT_TRUE failed: " + msg);
    }
    void assertFalse(bool condition, const std::string& msg = "") {
        if (condition) throw std::runtime_error("ASSERT_FALSE failed: " + msg);
    }
    void assertEqual(const std::string& a, const std::string& b, const std::string& msg = "") {
        if (a != b) throw std::runtime_error("ASSERT_EQUAL: '" + a + "' != '" + b + "': " + msg);
    }
    void assertEqual(int a, int b, const std::string& msg = "") {
        if (a != b) throw std::runtime_error("ASSERT_EQUAL int: " + std::to_string(a) + " != " + std::to_string(b) + ": " + msg);
    }
    void assertNear(double a, double b, double epsilon = 0.0001, const std::string& msg = "") {
        if (std::abs(a - b) > epsilon)
            throw std::runtime_error("ASSERT_NEAR failed: " + std::to_string(a) + " vs " + std::to_string(b) + ": " + msg);
    }
    void assertNotNull(const void* ptr, const std::string& msg = "") {
        if (!ptr) throw std::runtime_error("ASSERT_NOT_NULL: " + msg);
    }
    void assertNull(const void* ptr, const std::string& msg = "") {
        if (ptr) throw std::runtime_error("ASSERT_NULL: " + msg);
    }

private:
    TestRunner() = default;
    std::vector<TestSuite> m_suites;
};

} // namespace test

// Convenience macros
#define TEST_SUITE(name) \
    static test::TestSuite& _suite_##name = test::TestRunner::instance().suite(#name)

#define TEST_CASE(suite, name) \
    static struct _TestReg_##suite##_##name { \
        _TestReg_##suite##_##name() { \
            test::TestRunner::instance().addTest(#suite, #name, []() \
        } \
    } _instance_##suite##_##name; \
    static void _testBody_##suite##_##name()

#define ASSERT_TRUE(x) test::TestRunner::instance().assertTrue(x, #x)
#define ASSERT_FALSE(x) test::TestRunner::instance().assertFalse(x, #x)
#define ASSERT_EQ(a, b) test::TestRunner::instance().assertEqual(a, b, #a " == " #b)
#define ASSERT_NEAR(a, b, eps) test::TestRunner::instance().assertNear(a, b, eps)
#define ASSERT_NOT_NULL(x) test::TestRunner::instance().assertNotNull(x, #x)
#define ASSERT_NULL(x) test::TestRunner::instance().assertNull(x, #x)
