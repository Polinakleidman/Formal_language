#include <iostream>
#include <string>
#include <vector>
#include <stack>

//int INF = 1000000000;

struct Node {
    int max_pref_x = -1;
    int max_total_x = -1;
    int min_len = 1000000000;
    std::vector<int> pref_and_min_len;
    std::vector<bool> all_x;
};

class RegEx {
public:
    int k;
    char x;
    std::string s;
    std::stack<Node> stack_for_expr;

    RegEx(int k, char x, const std::string& s);

    void plus();

    void star_klini();

    void concat();

    void add_letter(int);

    int solve_regex();
};

void get_answer(std::string, char, int);
