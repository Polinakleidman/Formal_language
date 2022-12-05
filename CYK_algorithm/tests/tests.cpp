#include "gtest/gtest.h"
#include "../src/CYK_algo.h"

TEST(grammatics1_test, test0) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'a', 'F', 'b', 'F'}, {'S'}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  rules = {{'a', 'F', 'b'}, {}, {'G', 'a'}};
  gramm_rules['F'] = rules;
  rules = {{'b', 'S', 'G'}};
  gramm_rules['G'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  grammatics.WordReadableByCYK("");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("DOES NOT CONTAIN", output);
}

TEST(grammatics1_test, test1) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'a', 'F', 'b', 'F'}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  rules = {{'a', 'F', 'b'}, {}, {'G', 'a'}};
  gramm_rules['F'] = rules;
  rules = {{'b', 'S', 'G'}};
  gramm_rules['G'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  grammatics.WordReadableByCYK("abaabb");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("CONTAINS", output);
}

TEST(grammatics1_test, test2) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'a', 'F', 'b', 'F'}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  rules = {{'a', 'F', 'b'}, {}, {'G', 'a'}};
  gramm_rules['F'] = rules;
  rules = {{'b', 'S', 'G'}};
  gramm_rules['G'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  grammatics.WordReadableByCYK("ababab");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("DOES NOT CONTAIN", output);
}

TEST(grammatics2_test, test3) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'A', 'B'}, {'C', 'D'}, {'B'}, {'a'}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  rules = {{'A', 'B'}};
  gramm_rules['A'] = rules;
  rules = {{'B', 'D'}};
  gramm_rules['C'] = rules;
  rules = {{'B', 'B'}};
  gramm_rules['D'] = rules;
  rules = {{'b'}};
  gramm_rules['B'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  grammatics.WordReadableByCYK("bbbbb");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("CONTAINS", output);
}

TEST(grammatics2_test, test4) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'A', 'B'}, {'C', 'D'}, {'B'}, {'a'}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  rules = {{'A', 'B'}};
  gramm_rules['A'] = rules;
  rules = {{'B', 'D'}};
  gramm_rules['C'] = rules;
  rules = {{'B', 'B'}};
  gramm_rules['D'] = rules;
  rules = {{'b'}};
  gramm_rules['B'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  grammatics.WordReadableByCYK("bbba");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("DOES NOT CONTAIN", output);
}

TEST(grammatics3_test, test5) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'a', 'S', 'b', 'S'}, {'S', 'a', 'S', 'b'}, {}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  grammatics.WordReadableByCYK("aabbabab");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("CONTAINS", output);
}

TEST(grammatics3_test, test6) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'a', 'S', 'b', 'S'}, {'S', 'a', 'S', 'b'}, {}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  grammatics.WordReadableByCYK("aba");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("DOES NOT CONTAIN", output);
}

TEST(grammatics3_test, test7) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'a', 'S', 'b', 'S'}, {'S', 'a', 'S', 'b'}, {}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  grammatics.WordReadableByCYK("");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("CONTAINS", output);
}

TEST(wrong_grammatics_test, test8) {
  testing::internal::CaptureStdout();
  std::set<std::vector<int>> rules = {{'S'}};
  std::map<int, std::set<std::vector<int>>> gramm_rules;
  gramm_rules['S'] = rules;
  std::set<int> alph = {'a', 'b'};
  Grammatics grammatics('S', alph, gramm_rules);
  grammatics.GetHomskyFormat();
  ASSERT_ANY_THROW(grammatics.WordReadableByCYK("abb"));
}