#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>

using namespace std;
using Lexem = pair<string, string>;
using Rule = pair<string, vector<string>>;
map<string, int> num_nodes;
string starting_symb = "e";
vector<string> eps;

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
                else {
                    temp[x.first].insert("epsilon");
                }
            }
            int sz = int(x.second.size());
            for (int i = 0; i < sz; i++) {
                string t1 = x.second[i];
                if (find(t.begin(), t.end(), t1) == t.end()) {
                    if (t1 != *x.second.rbegin()) {
                        string t2 = x.second[i + 1];
                        if (temp.contains(t2)) {
                            for (string y : temp[t2]) {
                                temp[t1].insert(y);
                            }
                        }
                    }
                    else {
                        for (auto& y : temp[x.first]) {
                            temp[t1].insert(y);
                        }
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

set<pair<Rule, string>> Closure(vector<Rule>& G, map<string, set<string>>& F, vector<string>& t, set<pair<Rule, string>>& I) {
    bool changed = true;
    while (changed) {
        auto size_prev = I.size();
        changed = false;
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
                else if (flag == 1) {
                    non_term = y;
                    flag = 2;
                }
                else {
                    temp.push_back(y);
                }
            }
            if (non_term != "") {
                if (find(t.begin(), t.end(), non_term) == t.end()) {
                    /*if (temp.empty()) {
                        for (auto& y : First()) {

                        }
                    }*/
                    temp.push_back(x.second);
                    for (auto& y : First(temp, F)) {
                        first_.insert(y);
                    }
                    for (auto& g : G) {
                        if (g.first == non_term) {
                            vector<string> temp1 = { "." };
                            for (auto& y : g.second) {
                                if (y == "epsilon") {
                                    temp1 = { "epsilon", "." };
                                }
                                else {
                                    temp1.push_back(y);
                                }
                            }
                            for (auto& y : first_) {
                                I.insert({ {non_term, temp1}, y });
                            }
                        }
                    }
                }
            }
        }
        if (size_prev != I.size()) {
            changed = true;
        }
    }
    return I;
}

set<pair<Rule, string>> GoTo(const set<pair<Rule, string>>& I, string& X, vector<Rule>& G, map<string, set<string>>& F, vector<string>& t) {
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
    return Closure(G, F, t, res);
}

set<set<pair<Rule, string>>> items(set<set<pair<Rule, string>>>& I, vector<Rule>& G, map<string, set<string>>& F, vector<string>& alph, vector<string>& t) {
    set<set<pair<Rule, string>>> C = I;
    bool changed = true;
    while (changed) {
        changed = false;
        auto size_prev = C.size();
        for (auto& x : C) {
            for (auto& y : alph) {
                set<pair<Rule, string>> temp = GoTo(x, y, G, F, t);
                if (!temp.empty()) {
                    C.insert(temp);
                }
            }
        }
        if (size_prev != C.size()) {
            changed = true;
        }
    }
    return C;
}

int main()
{
    /*vector<string> term = { "opplus", "id", "num", "end" };
    vector<string> non_term = { "e", "E", "E_" };
    vector<string> alphabet = { "e", "E", "E_",  "opplus", "id", "num", "end" };
    vector<Rule> grammar = {
        {"e", {"E"}},
        {"E", {"E", "opplus", "E_"}},
        {"E", {"E_"}},
        {"E_", {"id"}},
        {"E_", {"num"}}
    };*/


    /*vector<Rule> grammar = {
        {"e", {"E"}},
        {"E", {"T", "E_"}},
        {"E_", {"opplus", "T", "E_"}},
        {"E_", {"epsilon"}},
        {"T", {"id"}},  
        {"T", {"num"}}
    };
    vector<string> term = { "id", "num", "opplus", "end"};
    vector<string> non_term = { "e", "E", "E_", "T" };
    vector<string> alphabet = { "e", "E", "E_", "T", "id", "num", "opplus", "end"};*/

    /*vector<Rule> grammar = {
        {"e", {"E"}},
        {"E", {"T", "E_"}},
        {"E_", {"opplus", "T", "E_"}},
        {"E_", {"epsilon"}},
        {"T", {"id"}},
        {"T", {"num"}}
    };
    vector<string> term = { "id", "num", "opplus", "end"};
    vector<string> non_term = { "e", "E", "E_", "T" };
    vector<string> alphabet = { "e", "E", "E_", "T", "id", "num", "opplus", "end"};*/

    /*vector<Rule> grammar = {
        {"e", {"E"}},
        {"E", {"E", "opplus", "T"}},
        {"E", {"T"}},
        {"T", {"T", "opmul", "F"}},
        {"T", {"F"}},
        {"F", {"opnot", "S"}},
        {"F", {"S"}},
        {"S", {"num"}}
    };
    vector<string> term = {"num", "opplus", "opmul", "opnot", "end"};
    vector<string> non_term = { "e", "E", "T", "F", "S"};
    vector<string> alphabet = { "e", "E", "T", "F", "S", "num", "opplus", "opmul", "opnot", "end"};*/


    /*vector<Rule> grammar = {
        {"e", {"E"}},
        {"E", {"id", "lpar", "ArgList", "rpar"}},
        {"ArgList", {"id", "ArgList_"}},
        {"ArgList", {"epsilon"}},
        {"ArgList_", {"comma", "id", "ArgList_"}},
        {"ArgList_", {"epsilon"}}
    };
    vector<string> term = {"id", "comma", "lpar", "rpar", "end"};
    vector<string> non_term = { "e", "E", "ArgList", "ArgList_"};
    vector<string> alphabet = { "e", "E", "ArgList", "ArgList_", "id", "comma", "lpar", "rpar", "end" };*/


   /*vector<string> term = { "comma", "rpar", "lpar", "opinc", "opnot", "opmul", "opminus", "ople", "opgt", "oplt", "opne", "opeq", "opand", "opor", "opplus", "id", "num", "end" };
    vector<string> non_term = { "d", "E", "E7", "E6", "E5", "E4", "E3", "E2", "E1", "ArgList", "ArgList_" };
    vector<string> alphabet = { "d", "E", "E7", "E6", "E5", "E4", "E3", "E2", "E1", "ArgList", "ArgList_" , "comma", "rpar", "lpar", "opinc", "opnot", "opmul", "opminus", "ople", "opgt", "oplt", "opne", "opeq", "opand", "opor", "opplus", "id", "num", "end" };
    vector<Rule> grammar = {
        {"d", {"StmtList"}},
        {"DeclStmt", {"Type", "id", "DeclStmt_"}},
        {"DeclStmt_", {"lpar", "ParamList", "lbrace", "StmtList", "rbrace"}},
        {"DeclStmt_", {"opassign", "num", "DeclVarList_", "semicolon"}},
        {"DeclStmt_", {"DeclVarList_", "semicolon"}},
        {"Type", {"char"}},
        {"Type", {"int"}},
        {"DeclVarList_", {"DeclVarList_", "semicolon"}},
        {"E", {"E7"}},
        {"E7", {"E6", "opor", "E7"}},
        {"E7", {"E6"}},
        {"E6", {"E5", "opand", "E6"}},
        {"E6", {"E5"}},
        {"E5", {"E4", "opeq", "E4"}},
        {"E5", {"E4", "opne", "E4"}},
        {"E5", {"E4", "oplt", "E4"}},
        {"E5", {"E4", "opgt", "E4"}},
        {"E5", {"E4", "ople", "E4"}},
        {"E5", {"E4"}},
        {"E4", {"E3", "opplus", "E4"}},
        {"E4", {"E3", "opminus", "E4"}},
        {"E4", {"E3"}},
        {"E3", {"E2", "opmul", "E3"}},
        {"E3", {"E2"}},
        {"E2", {"opnot", "E1"}},
        {"E2", {"E1"}},
        {"E1", {"opinc", "id"}},
        {"E1", {"id", "opinc"}},
        {"E1", {"lpar", "E", "rpar"}},
        {"E1", {"num"}},
        {"E1", {"id"}},
        {"E1", {"id", "lpar", "ArgList", "rpar"}},
        {"ArgList", {"E", "ArgList_"}},
        {"ArgList", {"epsilon"}},
        {"ArgList_", {"comma", "E", "ArgList_"}},
        {"ArgList_", {"epsilon"}},

    };*/


    vector<string> term = { "comma", "rpar", "lpar", "opinc", "opnot", "opmul", "opminus", "ople", "opgt", "oplt", "opne", "opeq", "opand", "opor", "opplus", "id", "num", "end" };
    vector<string> non_term = { "e", "E", "E7", "E6", "E5", "E4", "E3", "E2", "E1", "ArgList", "ArgList_" };
    vector<string> alphabet = { "e", "E", "E7", "E6", "E5", "E4", "E3", "E2", "E1", "ArgList", "ArgList_" , "comma", "rpar", "lpar", "opinc", "opnot", "opmul", "opminus", "ople", "opgt", "oplt", "opne", "opeq", "opand", "opor", "opplus", "id", "num", "end" };
    vector<Rule> grammar = {
        {"e", {"E"}},
        {"E", {"E7"}},
        {"E7", {"E6", "opor", "E7"}},
        {"E7", {"E6"}},
        {"E6", {"E5", "opand", "E6"}},
        {"E6", {"E5"}},
        {"E5", {"E4", "opeq", "E4"}},
        {"E5", {"E4", "opne", "E4"}},
        {"E5", {"E4", "oplt", "E4"}},
        {"E5", {"E4", "opgt", "E4"}},
        {"E5", {"E4", "ople", "E4"}},
        {"E5", {"E4"}},
        {"E4", {"E3", "opplus", "E4"}},
        {"E4", {"E3", "opminus", "E4"}},
        {"E4", {"E3"}},
        {"E3", {"E2", "opmul", "E3"}},
        {"E3", {"E2"}},
        {"E2", {"opnot", "E1"}},
        {"E2", {"E1"}},
        {"E1", {"opinc", "id"}},
        {"E1", {"id", "opinc"}},
        {"E1", {"lpar", "E", "rpar"}},
        {"E1", {"num"}},
        {"E1", {"id"}},
        {"E1", {"id", "lpar", "ArgList", "rpar"}},
        {"ArgList", {"E", "ArgList_"}},
        {"ArgList", {"epsilon"}},
        {"ArgList_", {"comma", "E", "ArgList_"}},
        {"ArgList_", {"epsilon"}},
    };


    for (auto& x : grammar) {
        if (x.second[0] == "epsilon") {
            eps.push_back(x.first);
        }
    }


    map<string, set<string>> F = FirstForG(grammar, term);
    for (auto& x : F) {
        cout << x.first << ": ";
        for (auto& y : x.second) {
            cout << y << " ";
        }
        cout << endl;
    }
    set<pair<Rule, string>> start = { { { "d", {".", "StmtList"}}, "end"} };
    set<set<pair<Rule, string>>> I;
    vector<set<pair<Rule, string>>> e;
    auto p = Closure(grammar, F, term, start);
    cout << 0 << ": ";
    for (auto& y : p) {
        cout << y.first.first << " -> ";
        for (auto& z : y.first.second) {
            cout << z << " ";
        }
        cout << ", " << y.second << "; ";
    }
    e.push_back(p);
    cout << endl;
    I.insert(p);
    set<set<pair<Rule, string>>> C = items(I, grammar, F, alphabet, term);
    int counter = 1;
    for (auto& x : C) {
        if (x != p) {
            cout << counter << ": ";
            for (auto& y : x) {
                cout << y.first.first << " -> ";
                for (auto& z : y.first.second) {
                    cout << z << " ";
                }
                cout << "| " << y.second << "; ";
            }
            e.push_back(x);
            cout << endl;
            counter++;
        }
    }
    int am_i = int(e.size());
    int len_alph = int(alphabet.size());
    vector<vector<pair<string, pair<int, Rule>>>> E;
    for (int i = 0; i < am_i; i++) {
        E.push_back({});
        for (int j = 0; j < len_alph; j++) {
            E[i].push_back({ "", { -1, {}} });
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
                    auto it = find(e.begin(), e.end(), GoTo(e[i], t, grammar, F, term));
                    int id = int(distance(e.begin(), it));
                    auto it1 = find(alphabet.begin(), alphabet.end(), t);
                    int term_id = int(distance(alphabet.begin(), it1));
                    E[i][term_id] = { "s", {id, {}} };
                }
            }
            else {
                if (x.first.first != starting_symb) {
                    auto it1 = find(alphabet.begin(), alphabet.end(), x.second);
                    int term_id = int(distance(alphabet.begin(), it1));
                    E[i][term_id] = { "r", { -2, x.first} };
                }
                else {
                    auto it1 = find(alphabet.begin(), alphabet.end(), x.second);
                    int term_id = int(distance(alphabet.begin(), it1));
                    E[i][term_id] = { "acc", { -2, {}} };
                }
            }
        }
        for (auto& y : non_term) {
            auto temp = GoTo(e[i], y, grammar, F, term);
            if (!temp.empty()) {
                auto it = find(e.begin(), e.end(), temp);
                int id = int(distance(e.begin(), it));
                auto it1 = find(alphabet.begin(), alphabet.end(), y);
                int term_id = int(distance(alphabet.begin(), it1));
                E[i][term_id] = { "", {id, {}} };
            }
        }
    }
    ofstream out;
    out.open("table.txt");
    out << non_term.size() << " " << term.size() << " " << counter << endl;
    for (auto& x : non_term) {
        out << x << " ";
    }
    out << endl;
    for (auto& x : term) {
        out << x << " ";
    }
    out << endl;
    for (auto& x : E) {
        for (auto& y : x) {
            if (!y.first.empty()) {
                out << y.first << " ";
            }
            out << y.second.first;
            if (y.first == "r") {
                out << " " << y.second.second.first;
                for (auto& c : y.second.second.second) {
                    out << " " << c;
                }
                out << " ";
            }
            out << " ";
        }
        out << endl;
    }
    out.close();
}

