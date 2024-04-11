#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <deque>
#include <set>

using namespace std;
using Lexem = pair<string, string>;
using Rule = pair<string, vector<string>>;

map<string, set<string>> FirstForG(vector<Rule>& G, vector<string>& t) {
    map<string, set<string>> res;
    for (auto& x : t) {
        res[x] = { x };
    }
    bool flag = true;
    while (flag) {
        flag = false;
        map<string, set<string>> temp = res;
        for (auto& x : G) {
            for (auto& y : x.second) {
                if (y != "epsilon") {
                    if (temp.contains(y)) {
                        for (string z : temp[y]) {
                            temp[x.first].insert(z);
                        }
                        if (find(temp[y].begin(), temp[y].end(), "epsilon") == temp[y].end()) {
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
        }
        for (auto& x : temp) {
            if (res.contains(x.first)) {
                if (x.second != res[x.first]) {
                    flag = true;
                    res = temp;
                    break;
                }
            }
            else {
                flag = true;
                res = temp;
                break;
            }
        }
    }
    return res;
}

vector<string> First(vector<string>& str, map<string, set<string>>& table) {
    vector<string> res;
    for (auto& x : str) {
        bool flag = false;
        for (auto& y : table[x]) {
            if (y != "epsilon") {
                res.push_back(y);
            }
            else {
                flag = true;
                if (y == *str.rbegin()) {
                    res.push_back("epsilon");
                }
            }
        }
        if (!flag) {
            return res;
        }
    }
    return res;
}

vector<pair<Rule, string>> pointer(vector<Rule> &G, map<string, set<string>>& F, vector<string>& t) {
    vector<pair<Rule, string>> pointed;
    map<string, set<string>> first_;
    for (auto& x : G) {
        for (auto y = x.second.begin(); y != x.second.end(); ++y) {
            if (find(t.begin(), t.end(), *y) == t.end()) {
                auto temp = y;
                vector<string> a;
                for (temp; temp != x.second.end(); temp++) {
                    a.push_back(*temp);
                }
                for (auto& z : First(a, F)) {
                    first_[*y].insert(z);
                }
            }
        }
    }
    vector<Rule> mid;
    for (auto& x : G) {
        for (int i = 0; i <= x.second.size(); i++) {
            bool flag = false;
            vector<string> temp;
            for (int j = 0; j < x.second.size() + 1; j++) {
                if (!flag) {
                    if (i != j) {
                        temp.push_back(x.second[j]);
                    }
                    else {
                        temp.push_back(".");
                        flag = true;
                    }
                }
                else {
                    temp.push_back(x.second[j - 1]);
                }
            }
            mid.push_back({ x.first, temp });
        }
    }
    for (auto& x : mid) {
        for (auto& y : first_[x.first]) {
            pointed.push_back({ x, y });
        }
    }
    return pointed;
}

int main()
{
    /*
    vector<Lexem> correct = {
            { "kw", "int" },
            { "id", "main" },
            { "lpar", "" },
            { "rpar", "" },
            { "lbrace", "" },
            { "kw", "return" },
            { "num", "0" },
            { "semicolon", "" },
            { "rbrace", "" },
            { "end", "" }
    };*/
    string starting_symb = "e";
    vector<string> term = {"opplus", "id", "num"};
    vector<Rule> grammar = {
        {"e", {"E", "#"}},
        {"E", {"E", "opplus", "E'"}},
        {"E", {"E'"}},
        {"E'", {"id"}},
        {"E'", {"num"}}
    };
    map<string, set<string>> F = FirstForG(grammar, term);
    vector<pair<Rule, string>> P = pointer(grammar, F, term);
    for (auto& x : P) {
        cout << x.first.first << " -> ";
        for (auto& y : x.first.second) {
            cout << y << " ";
        }
        cout << "| " << x.second;
        cout << endl;
    }
    int max_len = 0;
    /*
    map<int, vector<pair<string, Rule>>> grammar_by_len;
    int temp_size = grammar.size();
    for (auto& x : grammar) {
        int temp = x.second.size();
        if (temp > max_len) {
            max_len = temp;
        }
        auto &a =  x;
        grammar_by_len[temp].push_back(a);
    }
    vector<Lexem> expr = {
        {"num", "1"},
        {"opplus", ""},
        {"num", "2"}
    };
    map<int, vector<Rule>> rules;
    vector<string> stack;
    for (int x = 0; x < int(expr.size()); x++) {
        stack.push_back(expr[x].first);
        if (rules.empty()) {
            for (int j = int(stack.size()) - 1; j >= 0; j--) {
                for (int i = max_len; i >= 0; i--) {
                    for (auto& a : grammar_by_len[i]) {
                        if (stack[j] == a.first && !(find(rules[i].begin(), rules[i].end(), a.second) != rules[i].end())) {
                                rules[i].push_back(a.second);
                        }
                    }
                }
            }
        }
        if (!rules.empty()){
            for (int i = max_len; i >= 0; i--) {
                for (auto& a : rules[i]) {
                    int size = a.second.size();
                    if (size <= stack.size()) {
                        for (int j = size - 1; ) {

                        }
                    }
                }
            }
        }
    }*/
}
