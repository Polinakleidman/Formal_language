#include "Erli.h"

State::State(int not_term,
             const std::vector<int>& rule, size_t dot_pos, int prev_bound) :
    _rule_left_part(not_term), _rule_right_part(rule),
    _dot_position(dot_pos), _prev_bound(prev_bound) {};

bool State::finished() const {
  return _dot_position >= _rule_right_part.size();
}

bool State::operator<(const State& other) const {
  if (_rule_left_part < other._rule_left_part) {
    return true;
  }
  if (_rule_left_part > other._rule_left_part) {
    return false;
  }
  if (_rule_right_part < other._rule_right_part) {
    return true;
  }
  if (_rule_right_part > other._rule_right_part) {
    return false;
  }
  if (_dot_position < other._dot_position) {
    return true;
  }
  if (_dot_position > other._dot_position) {
    return false;
  }
  if (_prev_bound > other._prev_bound) {
    return false;
  }
  if (_prev_bound < other._prev_bound) {
    return true;
  }
  return false;
}

Grammatics::Grammatics(int start,
                       const std::set<int>& terminals,
                       const std::map<int, std::set<std::vector<int>>>& rules)
    : _start_not_terminal(start), _alphabet(terminals), _rules(rules) {
  _not_terminals.insert(_start_not_terminal);
  for (const auto& rule: _rules) {
    _not_terminals.insert(rule.first);
  }
  int new_not_term = static_cast<int>(_rules.rbegin()->first + 1);
  if (new_not_term <= static_cast<int>(*_alphabet.rbegin() + 1)) {
    new_not_term = static_cast<int>(*_alphabet.rbegin()) + 1;
  }
  _rules[new_not_term] = {{_start_not_terminal}};
  _start_not_terminal = new_not_term;
}

void Grammatics::Scan(std::vector<std::set<State>>& states, int level,
                      const std::string& word) {
  if (states.size() == level) { states.emplace_back(); }
  std::vector<State> new_states;
  for (auto& state : states[level - 1]) {
    if (!state.finished()
        && state._rule_right_part[state._dot_position] == word[level - 1]) {
      new_states.push_back(State(state._rule_left_part,
                                 state._rule_right_part,
                                 state._dot_position + 1,
                                 state._prev_bound));
    }
  }
  for (auto& elem : new_states) {
    states[level].insert(elem);
  }
}

bool Grammatics::Complete(std::vector<std::set<State>>& states, int level) {
  std::vector<State> new_states_curr_level;
  for (auto& state : states[level]) {
    if (!state.finished()) {
      continue;
    }
    for (auto& prev_state : states[state._prev_bound]) {
      if (!prev_state.finished()
          && prev_state._rule_right_part[prev_state._dot_position]
              == state._rule_left_part) {
        new_states_curr_level.push_back(State(prev_state._rule_left_part,
                                              prev_state._rule_right_part,
                                              prev_state._dot_position + 1,
                                              prev_state._prev_bound));
      }
    }
  }
  size_t prev_size = states[level].size();
  for (const State& elem : new_states_curr_level) {
    states[level].insert(elem);
  }
  return (prev_size != states[level].size());
}

bool Grammatics::Predict(std::vector<std::set<State>>& states, int level) {
  std::vector<State> new_states_curr_level;
  for (auto& state : states[level]) {
    if (state.finished()) {
      continue;
    }
    int curr_letter = state._rule_right_part[state._dot_position];
    if (_not_terminals.contains(curr_letter)) {
      for (auto& rule : _rules[curr_letter]) {
        new_states_curr_level.push_back(State(curr_letter, rule, 0, level));
      }
    }
  }
  size_t prev_size = states[level].size();
  for (auto& elem : new_states_curr_level) {
    states[level].insert(elem);
  }
  return (prev_size != states[level].size());
}

std::string Grammatics::CheckWordContain(const std::string& word) {
  std::vector<std::set<State>> states(word.size() + 2);
  states[0].insert(State(_start_not_terminal,
                         *_rules[_start_not_terminal].begin(), 0, 0));
  for (int level = 0; level <= word.size() + 1; ++level) {
    if (level != 0) {
      Scan(states, level, word);
    }
    int count = 0;
    while (Predict(states, level) || Complete(states, level)) {
      ++count;
    }
  }
  State final_state =
      State(_start_not_terminal, *_rules[_start_not_terminal].begin(), 1, 0);
  for (auto& state : states[word.size()]) {
    if (state == final_state) {
      return "CONTAINS";
    }
  }
  return "DOES NOT CONTAIN";
}
