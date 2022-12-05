#include "CYK_algo.h"

Grammatics::Grammatics(int start,
                       const std::set<int>& terminals,
                       const std::map<int, std::set<std::vector<int>>>& rules)
    : _start_not_terminal(start), _alphabet(terminals), _rules(rules) {
  _not_terminals.insert(_start_not_terminal);
  FindNotTerminals();
}


void Grammatics::FindNotTerminals() { //////
  for (const auto& rule: _rules) {
    _not_terminals.insert(rule.first);
    for (auto& word: rule.second){
      for(int letter : word){
        if (!_alphabet.contains(letter)){
          _not_terminals.insert(letter);
        }
      }
    }
  }
}

void Grammatics::SymbolGenerative(int not_term,
                                  std::map<int, std::pair<bool, bool>>& visited) {
  if (visited[not_term].first) {
    return;
  }
  visited[not_term].first = true;
  for (const auto& word: _rules[not_term]) {
    bool output_exists = true;
    for (auto letter : word) {
      if (!_alphabet.contains(letter)) {
        SymbolGenerative(letter, visited);
        output_exists = (output_exists && visited[letter].second);
      }
    }
    if (output_exists) {
      visited[not_term].second = true;
      return;
    }
  }
  visited[not_term].second = false;
}

void Grammatics::RemoveNotGenerative() {
  std::map<int, std::pair<bool, bool>> visited;
  for (auto not_term: _not_terminals) {
    visited[not_term].first = false;
    visited[not_term].second = false;
  }
  std::set<int> bad_not_terminals;
  for (auto not_term: _not_terminals) {
    SymbolGenerative(not_term, visited);
    if (!visited[not_term].second){
      bad_not_terminals.insert(not_term);
    }
  }
  for (auto elem : bad_not_terminals){
    _rules.erase(elem);
    _not_terminals.erase(elem);
  }
}

void Grammatics::ReachSymbols(int not_term, std::map<int, bool>& visited) {
  if (visited[not_term]) {
    return;
  }
  visited[not_term] = true;
  for (const auto& word:_rules[not_term]) {
    for (auto letter: word) {
      if (_not_terminals.contains(letter)) {
        ReachSymbols(letter, visited);
      }
    }
  }
}

void Grammatics::RemoveNotReachable() {
  std::map<int, bool> visited;
  for (auto not_term: _not_terminals) {
    visited[not_term] = false;
  }
  ReachSymbols(_start_not_terminal, visited);
  for (auto not_term : visited) {
    if (!visited[not_term.first]) {
      _rules.erase(not_term.first);
      _not_terminals.erase(not_term.first);
    }
  }
}

void Grammatics::RemoveMixedRules() {
  for (auto& not_term:_not_terminals) {
    std::set<std::vector<int>> new_rules;
    for (const auto& word : _rules[not_term]) {
      bool contains_simple_letter = false;
      if (word.size()<=1){
        new_rules.insert(word);
        continue;
      }
      auto word_for_substitute = word;
      bool contains_bad_letters = false;
      for (int i = 0; i < word.size(); ++i) {
        int letter = word[i];
        if (!_alphabet.contains(letter) && !_not_terminals.contains(letter)){
          contains_bad_letters = true;
        }
        if (_alphabet.contains(letter)) {
          std::vector<int> new_word = {letter};
          std::set<std::vector<int>> new_rule = {new_word};
          int new_not_term = CreateNewNotTerm();
          _rules[new_not_term] = new_rule;
          letter = new_not_term;
          word_for_substitute[i] = letter;
          contains_simple_letter = true;
        }
      }
      if (contains_simple_letter && !contains_bad_letters) {
        new_rules.insert(word_for_substitute);
      }else if (!contains_bad_letters){
        new_rules.insert(word);
      }
    }
    _rules[not_term] = new_rules;
  }
  FindNotTerminals();
}

int Grammatics::CreateNewNotTerm() {
  int new_not_term = static_cast<int>(_rules.rbegin()->first + 1);
  if (new_not_term <= static_cast<int>(*_alphabet.rbegin()+1)) {
    new_not_term = static_cast<int>(*_alphabet.rbegin()) + 1;
  }
  return new_not_term;
}

void Grammatics::RemoveLongRules() {
  for (auto not_term : _not_terminals) {
    std::set<std::vector<int>> bad_not_term;
    for (auto& word : _rules[not_term]) {
      if (word.size() <= 2) {
        continue;
      }
      int curr = not_term;
      for (int i = 0; i < word.size() - 2; ++i) {
        int new_not_term = CreateNewNotTerm();
        std::vector<int> new_rule = {word[i], new_not_term};
        _rules[curr].insert(new_rule);
        curr = new_rule[1];
        _rules[curr] = {};
      }
      _rules[curr].insert({word[word.size() - 2], word[word.size() - 1]});
      bad_not_term.insert(word);
    }
    for(auto& elem : bad_not_term){
      _rules[not_term].erase(elem);
    }
  }
  FindNotTerminals();
}

bool Grammatics::ContainsEmptyWord(int symbol, std::map<int, bool>& visited) {
  if (visited[symbol]) {
    return false;
  }
  visited[symbol] = true;
  for (auto& word:_rules[symbol]) {
    bool ContainsEmpty = true;
    if (word.size() == 0) {
      return true;
    }
    for (int letter:word) {
      if (_not_terminals.contains(letter)
          && ContainsEmptyWord(letter, visited)) {
        continue;
      }
      ContainsEmpty = false;
    }
    if (ContainsEmpty) {
      return true;
    }
  }
  return false;
}

void Grammatics::RemoveEmptyRules() {
  std::map<int, bool> contains_empty_output;
  for (auto& not_term : _not_terminals) {
    std::map<int, bool> visited;
    for (auto t:_not_terminals) {
      visited[t] = false;
    }
    contains_empty_output[not_term] = ContainsEmptyWord(not_term, visited);
  }
  if (contains_empty_output[_start_not_terminal]) {
    _consists_empty_word = true;
  }
  for (auto& not_term : _not_terminals) {
    std::vector<std::vector<int>> new_rules;
    for (auto& word : _rules[not_term]) {
      if (word.size() < 2) {
        continue;
      }
      if (_not_terminals.contains(word[0]) && contains_empty_output[word[0]]) {
        new_rules.push_back({word[1]});
      }
      if (_not_terminals.contains(word[1]) && contains_empty_output[word[1]]) {
        new_rules.push_back({word[0]});
      }
    }
    for (auto elem : new_rules){
      _rules[not_term].insert(elem);
    }
  }
  for (auto& not_term : _not_terminals) {
    for (std::vector<int> word : _rules[not_term]) {
      if (word.size() == 0) {
        _rules[not_term].erase(word);
        break;
      }
    }
  }
}

void Grammatics::ReturnEmptyWord() {
  if (_consists_empty_word) {
    int new_not_term = CreateNewNotTerm();
    _not_terminals.insert(new_not_term);
    _rules[new_not_term] = {{}, {_start_not_terminal}};
    _start_not_terminal = new_not_term;
  }
}

void Grammatics::RemoveUnaryRules() {
  for (auto& not_term : _not_terminals) {
    std::vector<std::vector<int>> new_words;
    for (auto word : _rules[not_term]) {
      if (word.size() == 1 && !_alphabet.contains(word[0])) {
        int new_not_term = word[0];
        std::queue<int> q;
        q.push(new_not_term);
        while (!q.empty()) {
          int curr_not_term = q.front();
          q.pop();
          for (auto curr_word : _rules[curr_not_term]) {
            if (curr_word.size() == 1 && !_alphabet.contains(curr_word[0])) {
              if (curr_word[0] == curr_not_term){
                continue;
              }
              q.push(curr_word[0]);
            } else {
              new_words.push_back(curr_word);
            }
          }
        }
      }else{
        new_words.push_back(word);
      }
    }
    _rules[not_term].clear();
    for(auto elem : new_words){
      _rules[not_term].insert(elem);
    }
  }
}

/*
void Grammatics::PrintAllRules() {
  for (auto not_term: _not_terminals){
    std::cerr<<not_term<<": ";
    for(auto word: _rules[not_term]){
      for(auto t:word){
        std::cerr<<t<<" ";
      }
      std::cerr<<", ";
    }
    std::cerr<<"***\n";
  }
}
*/

void Grammatics::GetHomskyFormat(){
  RemoveNotGenerative();
  RemoveNotReachable();
  RemoveMixedRules();
  RemoveLongRules();
  RemoveEmptyRules();
  ReturnEmptyWord();
  RemoveNotGenerative();
  RemoveNotReachable();
  RemoveUnaryRules();
}

void Grammatics::WordReadableByCYK(const std::string& word){
  std::vector<int> _word(word.size());
  if (word.size() == 0 && _consists_empty_word){
    std::cout<<"CONTAINS";
    return;
  }
  if (word.size() == 0){
    std::cout<<"DOES NOT CONTAIN";
    return;
  }
  for (int i = 0; i < word.size(); ++i) {
    _word[i] = static_cast<int>(static_cast<unsigned char>(word[i]));
  }
  std::map<int, std::vector<std::vector<bool>>> dp;
  if (_not_terminals.size() == 0 || _rules.size() == 0){
    throw ("Grammatics is empty, this is incorrect");
  }
  for (auto& not_term: _not_terminals) {
    dp[not_term] = std::vector<std::vector<bool>>(word.size(),
                                                  std::vector<bool>(word.size(),
                                                                    false));
  }
  for (int j = 0; j < word.size(); ++j) {
    for (int i = j; i >=0; --i) {
      for (auto& not_term: _not_terminals) {
        if (i == j) {
          for (auto& output : _rules[not_term]) {
            if (output.size() == 1 && output[0] == _word[i]) {
              dp[not_term][i][j] = true;
              break;
            }
          }
        } else {
          for (auto& output : _rules[not_term]) {
            if (output.size() == 2) {
              int not_term_first = output[0];
              int not_term_second = output[1];
              for (int k = i; k < j; ++k) {
                bool possible =
                    (dp[not_term_first][i][k] && dp[not_term_second][k + 1][j]);
                dp[not_term][i][j] = (dp[not_term][i][j] || possible);
              }
            }
          }
        }
      }
    }
  }
  if (dp[_start_not_terminal][0][word.size() - 1]){
    std::cout<<"CONTAINS";
  }else{
    std::cout<<"DOES NOT CONTAIN";
  }
}
