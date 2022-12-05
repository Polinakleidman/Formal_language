#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <queue>

class Grammatics{
  int _start_not_terminal;
  std::set<int> _alphabet;
  std::map<int, std::set<std::vector<int>>> _rules;
  bool _consists_empty_word = false;
  std::set<int> _not_terminals;
 public:
  Grammatics(int start, const std::set<int>& letters, const std::map<int, std::set<std::vector<int>>>& rules);
  void FindNotTerminals();
  void SymbolGenerative(int not_term, std::map<int, std::pair<bool, bool>>&);
  void ReachSymbols(int not_term, std::map<int, bool>&);
  bool ContainsEmptyWord(int symbol, std::map<int, bool>&);
  void RemoveNotGenerative();
  int CreateNewNotTerm();
  void RemoveNotReachable();
  void RemoveMixedRules();
  void RemoveLongRules();
  void RemoveEmptyRules();
  void ReturnEmptyWord();
  void RemoveUnaryRules();
  void GetHomskyFormat();
  void WordReadableByCYK(const std::string&);
  //void PrintAllRules();
};