# Cocke - Younger - Kasami algorithm & Chomsky normal form
----
## Grammatic description:
1. Both letters from alphabet and not terminals have type int
2. Alphabet is contained in set _alphabet
3. Not terminals are contained in set _not_terminals
4. Right part in every rule is vector<int>
    #### example:
  
| rules | how stored |
|----------------|:---------:|
| S -> eps | {} |
| A -> deG | {'d', 'e', 'G'} |

  + (there is a cast char->int inside every vector)
5. Rules with the same left part are contained in one set4
    #### example:
  
| rules | how stored |
|----------------|:---------:|
| S -> AB, S -> a, S -> cFdEAc | {{'A', 'B'}, {'a'}, {'c', 'F', 'd', 'A', 'c'}} |
6. Sets of rules are included in map called _rules
    #### example:
  
| rules| how stored 
|----------------|:---------:|
| - S -> AB, S -> a, S -> cFdEAc | set1 = {{'A', 'B'}, {'a'}, {'c', 'F', 'd', 'A', 'c'}} |
| A -> 'qwe', A -> BB | set2 = {{'q', 'w', 'e'}, {'B', 'B'}} |
| B -> FD, B -> eps | set3 = {{'F', 'D'}, {}} |
|all rules | {('S', set1), ('A', set2), ('B', set3), ('F', {}), ('E', {}) | 
 ----
 ## Providing Chomsky normal form
| possible rules | types of symbols |
|----------------|:---------:|
| A -> a | A from not terminals, a from alphabet |
| A -> BC | B, C, from not terminals |
| S -> eps | S is start not terminal |

  ### 7 steps to get Chomsky form:
everywhere it's considered that rules have short enough length and their processing is not taken into asymptotics
  
| step | asymptotics | name of function |
|---------------|:--------:|:--------:|
| deleting not generating & not reaching not-terminals | O(nlog(n)) (DFS - O(n), removing one bad symbol O(log(n) as it's in set) | RemoveNotGenerative(); RemoveNotReachable() |
| deleting mixed rules (A-> aBcDEg) | O(nlog(n)), as every substituion takes O(nlogn) in set and map | RemoveMixedRules() |
| deleting long rules (>=3 symbols in right part) | O(nlogn) (BFS - O(n), deleting O(log(n))| RemoveLongRules() |
| deleting empty rules (A -> eps ) | O(nlog(n)) | RemoveEmptyRules() |
| returning rule S -> eps if eps was in output of grammatics | O(logn) | ReturnEmptyWord() |
| Again deleting useless rules | first step | first step |
| removing unary rules ( A-> B) | O(nlog(n))| RemoveUnaryRules() |

 ----
 ## Checking word content in grammatics by Cocke - Younger - Kasami algorithm
 
  1. checking w[i:i]:
      + is in output of not-terminal A takes log(n) * size(_rules[A])
      + is in output for all not_terminals takes sum( long(n) * size(_rules[A]) ) = log(n)*n 
  2. checking w[i:j]:
      + is in output of not-terminal A takes log(n) * size(_rules[A]) * (j - i)
      + is in output for all not_terminals takes sum( long(n) * size(_rules[A]) * (j - i)) = log(n) * n * (j - i)
  
  *So, total time is O(n*log(n) * sizeof(word)), it can be imroved to O(n * sizeof(word)) using unordered set instead od set and unordered map instead of map)
  ----
