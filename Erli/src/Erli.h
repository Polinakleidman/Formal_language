#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

class Grammatics;

class State {
 private:
  int _rule_left_part;
  std::vector<int> _rule_right_part;
  size_t _dot_position;
  int _prev_bound;
  State() = default;
  State(int, const std::vector<int>&, size_t, int);
  bool finished() const;
  friend class Grammatics;
  bool operator==(const State& other) const {
    return (_rule_left_part == other._rule_left_part &&
        _rule_right_part == other._rule_right_part &&
        _dot_position == other._dot_position
        && _prev_bound == other._prev_bound);
  };
 public:
  bool operator<(const State& other) const;
};

class Grammatics {
 public:
  Grammatics(int start,
             const std::set<int>& letters,
             const std::map<int, std::set<std::vector<int>>>& rules);
  std::string CheckWordContain(const std::string&);

 private:
  int _start_not_terminal;
  std::set<int> _alphabet;
  std::map<int, std::set<std::vector<int>>> _rules;
  std::set<int> _not_terminals;
  void Scan(std::vector<std::set<State>>& states,
            int level,
            const std::string& word);
  bool Complete(std::vector<std::set<State>>& states, int level);
  bool Predict(std::vector<std::set<State>>& states, int level);
  friend class State;
};
