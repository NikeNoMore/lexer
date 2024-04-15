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

set<pair<Rule, string>> Closure(vector<Rule> &G, map<string, set<string>>& F, vector<string>& t, set<pair<Rule, string>>& I) {
    set<pair<Rule, string>> prev = I;
    for (auto& x : I) {
        int flag = 0;
        vector<string> temp;
        string non_term = "";
        set<string> first_;
        for (auto& y : x.first.second) {
            if (flag == 0) {
                if (y == ".") {
                    flag = 1;
                }
            }
            else if(flag == 1) {
                non_term = y;
                flag = 2;
            }
            else {
                temp.push_back(y);
            }
        }
        if (non_term != "") {
            if (find(t.begin(), t.end(), non_term) == t.end()) {
                temp.push_back(x.second);
                for (auto& y : First(temp, F)) {
                    first_.insert(y);
                }
                for (auto& g : G) {
                    if (g.first == non_term) {
                        vector<string> temp1 = { "." };
                        for (auto& y : g.second) {
                            temp1.push_back(y);
                        }
                        for (auto& y : first_) {
                            I.insert({ {non_term, temp1}, y });
                        }
                    }
                }
            }
        }
    }
    if (prev != I) {
        return Closure(G, F, t, I);
    }
    else {
        return I;
    }
}

set<pair<Rule, string>> GoTo(const set<pair<Rule, string>>& I, string& X) {
    set<pair<Rule, string>> res;
    for (auto& x : I) {
        bool point = false;
        bool flag = false;  
        for (auto& y : x.first.second) {
            if (!point) {
                if (y == ".") {
                    point = true;
                }
            }
            else {
                if (y == X) {
                    flag = true;
                }
            }
        }
        if (flag) {
            vector<string> temp;
            point = false;
            for (auto& y : x.first.second) {
                if (y != "." && !point) {
                    temp.push_back(y);
                }
                else if (!point) {
                    point = true;
                }
                else if (point) {
                    temp.push_back(y);
                    temp.push_back(".");
                    point = false;
                }
            }
            res.insert({ {x.first.first, temp}, x.second });
        }
    }
    return res;
}

set<set<pair<Rule, string>>> items(set<set<pair<Rule, string>>>& I, vector<Rule>& G, map<string, set<string>>& F, vector<string>& alph){
    set<set<pair<Rule, string>>> C = I;
    set<set<pair<Rule, string>>> prev = C;
    for (auto& x : C) {
        for (auto& y : alph) {
            set<pair<Rule, string>> temp = GoTo(x, y);
            if (!temp.empty()) {
                C.insert(temp);
            }
        }
    }
    if (C != prev) {
        return items(C, G, F, alph);
    }
    else {
        return C;
    }
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
    vector<string> term = {"opplus", "id", "num", "#"};
    vector<string> alphabet = { "opplus", "id", "num", "#", "e", "E", "E'"};
    vector<Rule> grammar = {
        {"e", {"E"}},
        {"E", {"E", "opplus", "E'"}},
        {"E", {"E'"}},
        {"E'", {"id"}},
        {"E'", {"num"}}
    };
    map<string, set<string>> F = FirstForG(grammar, term);
    set<pair<Rule, string>> start = { { { "e", {".", "E"}}, "#"}};
    set<set<pair<Rule, string>>> I;
    I.insert(Closure(grammar, F, term, start));
    set<set<pair<Rule, string>>> C = items(I, grammar, F, alphabet);
    vector<set<pair<Rule, string>>> e;
    int counter = 0;
    for (auto& x : C) {
        e.push_back(x);
        cout << counter << endl;
        counter += 1;
        for (auto& z : x) {
            cout << z.first.first << " -> ";
            for (auto& y : z.first.second) {
                cout << y << " ";
            }
            cout << "| " << z.second;
            cout << endl;
        }
        cout << "================" << endl;
    }
    int am_i = int(e.size());
    int len_alph = int(alphabet.size());
    vector<vector<string>> E;
    for (int i = 0; i < am_i; i++) {
        E.push_back({});
        for (int j = 0; j < len_alph; j++) {
            E[i].push_back("-");
        }
    }

    for (int i = 0; i < am_i; i++) {
        for (auto& x : e[i]) {
            int flag = 0;
            string t;
            for (auto& y : x.first.second) {
                if (flag == 0 && y == ".") {
                    flag = 1;
                }
                else if (flag == 1) {
                    t = y;
                    flag = 2;
                    break;
                }
            }
            if (flag == 2) {
                if (find(term.begin(), term.end(), t) != term.end()) {
                    auto it = find(e.begin(), e.end(), GoTo(e[i], t));
                    int id = int(distance(e.begin(), it));
                    auto it = find(alphabet.begin(), alphabet.end(), t);
                    int term_id;
                    E[i][term_id] = "s" + to_string(id);
                }
            }
        }
    }
    /*int max_len = 0;
    string f = "E";
    set<pair<Rule, string>> R = GoTo(P, f);
    for (auto& x : R) {
        cout << x.first.first << " -> ";
        for (auto& y : x.first.second) {
            cout << y << " ";
        }
        cout << "| " << x.second;
        cout << endl;
    }*/
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
