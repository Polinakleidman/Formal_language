#include "gtest/gtest.h"
#include "../src/RegEx.h"

TEST(main_test, test1) {
    testing::internal::CaptureStdout();
    get_answer("acb..bab.c.*.ab.ba.+.+*a.", 'b', 2);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("4", output);
}

TEST(main_test, test2){
    testing::internal::CaptureStdout();
    get_answer("a*", 'b', 2);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("INF", output);
}

TEST(main_test, test3){
    testing::internal::CaptureStdout();
    get_answer("a*", 'a', 3);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("3", output);
}

TEST(main_test, test4){
    testing::internal::CaptureStdout();
    get_answer("ab+c.aba.*.bac.+.+*", 'c', 4);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("INF", output);
}

TEST(main_test, test5){
    testing::internal::CaptureStdout();
    get_answer("1ab+c.aba.*.bac.+.+*+*", 'a', 2);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("3", output);
}


TEST(main_test, test6){
    testing::internal::CaptureStdout();
    get_answer("aa.a.a.a.1+", 'a', 1);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("5", output);
}

TEST(main_test, test7){
    testing::internal::CaptureStdout();
    get_answer("1b.c+*", 'b', 3);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("3", output);
}

TEST(main_test, test8){
    testing::internal::CaptureStdout();
    get_answer("abcd..+*1.", 'e', 3);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("INF", output);
}

TEST(main_test, test9){
    testing::internal::CaptureStdout();
    get_answer("aaa.ab.bc.1+++ccb.a.+ab.c.a.+a+1+.bc.+*bb+*+.", 'a', 3);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("3", output);
}