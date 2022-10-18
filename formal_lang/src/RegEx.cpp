#include "RegEx.h"

int INF = 1000000000;

RegEx::RegEx(int k0, char x0, const std::string &s0) {
    k =k0;
    x= x0;
    s = s0;
}

void RegEx::plus() {
    Node second = stack_for_expr.top();
    stack_for_expr.pop();
    Node first = stack_for_expr.top();
    stack_for_expr.pop();
    Node new_node;
    std::vector<int> new_prefs(std::max(first.max_pref_x, second.max_pref_x) + 1, INF);
    for (int j = 0; j < new_prefs.size(); ++j) {
        if (j <= first.max_pref_x) {
            new_prefs[j] = std::min(INF, first.pref_and_min_len[j]);
        }
        if (j <= second.max_pref_x) {
            new_prefs[j] = std::min(new_prefs[j], second.pref_and_min_len[j]);\

        }
    }
    new_node.max_total_x = std::max(first.max_total_x, second.max_total_x);
    new_node.pref_and_min_len = new_prefs;
    new_node.max_pref_x = (int) new_prefs.size() - 1;
    new_node.min_len = std::min(first.min_len, second.min_len);
    std::vector<bool> all_x(std::max(first.all_x.size(), second.all_x.size()), false);
    for (int j = 0; j < first.all_x.size(); ++j) {
        all_x[j] = (all_x[j] | first.all_x[j]);
    }
    for (int j = 0; j < second.all_x.size(); ++j) {
        all_x[j] = (all_x[j] | second.all_x[j]);
    }
    new_node.all_x = all_x;
    stack_for_expr.push(new_node);
}

void RegEx::star_klini() {
    Node last = stack_for_expr.top();
    stack_for_expr.pop();
    if (last.max_total_x == -1) {
        Node new_node;
        new_node.max_total_x = 0;
        new_node.all_x = {true};
        new_node.pref_and_min_len = last.pref_and_min_len;
        if (last.max_pref_x == -1) {
            new_node.pref_and_min_len = {0};
        }
        new_node.pref_and_min_len[0] = 0;
        new_node.max_pref_x = std::max(last.max_pref_x, 0);
        new_node.min_len = 0;
        stack_for_expr.push(new_node);
        return;
    }
    std::vector<int> new_prefs(k + 1, INF);
    std::vector<bool> new_all_x(k + 1, false);
    new_all_x[0] = true;
    new_prefs[0] = 0;
    int max_x = 0;
    int max_pref_x = std::max(0, last.max_pref_x);
    for (int j = 0; j <= k; ++j) {
        for (int t = 0; t <= last.max_total_x; ++t) {
            if (j <= last.max_pref_x) {
                new_prefs[j] = std::min(new_prefs[j], last.pref_and_min_len[j]);
            }
            if (j <= last.max_total_x) {
                new_all_x[j] = new_all_x[j] || last.all_x[j];
            }
            if (j >= t && last.all_x[t]) {
                new_prefs[j] = std::min(new_prefs[j], new_prefs[j - t] + t);
                new_all_x[j] = new_all_x[j - t] || new_all_x[j];
                if (new_all_x[j]) {
                    max_x = j;
                }
                if (new_prefs[j]) {
                    max_pref_x = std::max(j, max_pref_x);
                }
            }
        }
    }
    Node new_node;
    new_prefs[0] = 0;
    new_all_x[0] = true;
    new_node.max_total_x = max_x;
    new_node.all_x = new_all_x;
    new_node.max_pref_x = max_pref_x;
    new_node.pref_and_min_len = new_prefs;
    new_node.min_len = 0;
    stack_for_expr.push(new_node);
}

void RegEx::concat() {
    Node second = stack_for_expr.top();
    stack_for_expr.pop();
    Node first = stack_for_expr.top();
    stack_for_expr.pop();
    Node new_node;
    int len_pref1 = first.max_total_x + second.max_pref_x;
    if (second.max_pref_x < 0 || first.max_total_x < 0) {
        len_pref1 = -1;
    }
    int len_pref2 = first.max_pref_x;
    std::vector<int> new_prefs(std::max(len_pref1, len_pref2) + 1, INF);
    int sz_x = (second.max_pref_x >= 0 && first.max_total_x >= 0) ? first.max_total_x + second.max_pref_x
                                                                  : -1;
    std::vector<bool> all_x(sz_x + 1, false);
    for (int j = 0; j <= first.max_total_x; ++j) {
        for (int t = 0; t <= second.max_pref_x; ++t) {
            if (first.all_x[j]) {
                new_prefs[j + t] = std::min(new_prefs[j + t], j + second.pref_and_min_len[t]);
            }
            if (first.all_x[j] && t < second.all_x.size() && second.all_x[t]) {
                all_x[t + j] = true;
            }
        }
    }
    for (int j = 0; j <= first.max_pref_x; ++j) {
        new_prefs[j] = std::min(new_prefs[j], first.pref_and_min_len[j] + second.min_len);
    }
    new_node.max_pref_x = std::max(len_pref2, len_pref1);
    new_node.min_len = first.min_len + second.min_len;
    new_node.pref_and_min_len = new_prefs;
    new_node.max_total_x = sz_x;
    new_node.all_x = all_x;
    stack_for_expr.push(new_node);
}

void RegEx::add_letter(int i) {
    Node new_node;
    if (s[i] == x) {
        new_node.max_total_x = 1;
        new_node.min_len = 1;
        new_node.max_pref_x = 1;
        new_node.pref_and_min_len = {INF, 1};
        new_node.all_x = {false, true};
    } else if (s[i] == '1') {
        new_node.max_total_x = 0;
        new_node.min_len = 0;
        new_node.max_pref_x = 0;
        new_node.pref_and_min_len = {1};
        new_node.all_x = {true};
    } else {
        new_node.max_total_x = -1;
        new_node.min_len = 1;
        new_node.max_pref_x = -1;
        new_node.pref_and_min_len = {INF, INF};
        new_node.all_x = {false, false};
    }
    stack_for_expr.push(new_node);
}

    int RegEx::solve_regex() {
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '+') {
            plus();
        } else if (s[i] == '*') {
            star_klini();
        } else if (s[i] == '.') {
            concat();
        } else {
            add_letter(i);
        }
    }
    Node node = stack_for_expr.top();
    if (node.max_pref_x < k) {
        return INF;
    } else {
        return stack_for_expr.top().pref_and_min_len[k];
    }
}

    void get_answer(std::string s, char x, int k){
        RegEx regex(k, x, s);
        int n = regex.solve_regex();
        if (n==INF){
            std::cout<<"INF";
        }else{
            std::cout<<n;
        }
    }
