#include <iostream>
#include "../stat_lib/lexer_frm.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>

using namespace std;
using Lexem = pair<string, string>;
using Rule = pair<string, vector<string>>;
map<string, int> num_nodes;
//string starting_symb = "d";
string starting_symb = "e";
vector<string> eps;
map<set<pair<Rule, string>>, map<string, set<pair<Rule, string>>>> items_goto;
int backup_counter = 1;


void unpack(set<set<pair<Rule, string>>>& I) {
    filebuf fb1;
    if (fb1.open("backup.txt", ios::in)) {
        istream is(&fb1);
        Lexer lex(is);
        int n = stoi(lex.getNextLexem().second);
        for (int i = 0; i < n; i++) {
            Lexem tmp = lex.getNextLexem();
            set<pair<Rule, string>> init_set;
            while (tmp.first != "colon") {
                Rule tmp_rule;
                tmp_rule.first = tmp.second;
                while (tmp.first != "bar") {
                    tmp = lex.getNextLexem();
                    if (tmp.first != "bar") {
                        tmp_rule.second.push_back(tmp.second);
                    }
                }
                tmp = lex.getNextLexem();
                init_set.insert({ tmp_rule, tmp.second });
                tmp = lex.getNextLexem();
                tmp = lex.getNextLexem();
            }
            I.insert(init_set);
            tmp = lex.getNextLexem();
            while (tmp.first != "semicolon") {
                string letter = tmp.second;
                set<pair<Rule, string>> tmp_set;
                lex.getNextLexem();
                tmp = lex.getNextLexem();
                while (tmp.first != "opor") {
                    Rule tmp_rule;
                    tmp_rule.first = tmp.second;
                    while (tmp.first != "bar") {
                        tmp = lex.getNextLexem();
                        if (tmp.first != "bar") {
                            tmp_rule.second.push_back(tmp.second);
                        }
                    }
                    tmp = lex.getNextLexem();
                    tmp_set.insert({ tmp_rule, tmp.second });
                    tmp = lex.getNextLexem();
                    tmp = lex.getNextLexem();
                }
                items_goto[init_set][letter] = tmp_set;
                I.insert(tmp_set);
                tmp = lex.getNextLexem();
            }
        }
    }
}

void backup() {
    ofstream back;
    back.open("backup.txt");
    back << int(items_goto.size()) << endl;
    for (auto& x : items_goto) {
        for (auto& y : x.first) {
            back << y.first.first << " ";
            for (auto& z : y.first.second) {
                back << z << " ";
            }
            back << "| " << y.second << " || ";
        }
        back << ": ";
        for (auto& y : x.second) {
            back << y.first << " | ";
            for (auto& z : y.second) {
                back << z.first.first << " | ";
                for (auto& q : z.first.second) {
                    back << q << " ";
                }
                back << "| " << z.second << " | ";
            }
            back << "|| ";
        }
        back << ";" << endl << endl;
    }
}

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
    map<pair<Rule, string>, bool> clos_bool;
    while (changed) {
        auto size_prev = I.size();
        changed = false;
        for (auto& x : I) {
            if (!clos_bool.contains(x)) {
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
                clos_bool[x] = true;
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
            if (!items_goto.contains(x)) {
                for (auto& y : alph) {
                    set<pair<Rule, string>> temp = GoTo(x, y, G, F, t);
                    if (!temp.empty()) {
                        C.insert(temp);
                        items_goto[x][y] = temp;
                    }
                }
                backup_counter++;
            }
            if (backup_counter % 100 == 0) {
                backup();
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

    /*vector<Rule> grammar = {
        {"d", {"IfOp"}},
        {"IfOp", {"Matched"}},
        {"IfOp", {"Unmatched"}},
        {"Matched", {"kwif", "lpar", "id", "rpar", "Matched", "kwelse", "Matched"}},
        {"Matched", {"lbrace", "id", "rbrace"}},
        {"Unmatched", {"kwif", "lpar", "id", "rpar", "IfOp"}},
        {"Unmatched", {"kwif", "lpar", "id", "rpar", "Matched", "kwelse", "Unmatched"}},
    };
    vector<string> term = { "id", "lpar", "rpar", "kwif", "kwelse", "lbrace", "rbrace", "end"};
    vector<string> non_term = { "d", "IfOp", "Matched", "Unmatched"};
    vector<string> alphabet = { "d", "IfOp", "Matched", "Unmatched", "id", "lpar", "rpar", "kwif", "kwelse", "lbrace", "rbrace", "end" };*/


   /*vector<string> term = { "comma", "rpar", "lpar", "opinc", "opnot", "opmul", "opminus", "ople", "opgt", "oplt", "opne", "opeq", "opand", "opor", "opplus", "id", "num",
    "lbrace", "rbrace", "semicolon", "opassign", "kwchar", "kwint", "kwreturn", "kwwhile", "kwfor", "kwif", "kwelse", "kwswitch", "kwin", "kwout", "colon", "end"};
    vector<string> non_term = { "d", "E", "E7", "E6", "E5", "E4", "E3", "E2", "E1", "ArgList", "ArgList_", "StmtList", "DeclStmt", "Type", "DeclStmt_", 
    "ParamList", "DeclVarList_", "InitVar", "ParamList_", "Stmt", "DeclStmt", "AoCallOp", "WhileOp", "ForOp", "IfOp", "SwitchOp", "IOp", "OOp", "AOCall", 
    "AOCall_", "ForInit", "ForExp", "ForLoop", "ElsePart", "Cases", "Cases_", "ACase", "OOp_", "OtherStmtList", "OtherStmt"};
    vector<string> alphabet = { "d", "E", "E7", "E6", "E5", "E4", "E3", "E2", "E1", "ArgList", "ArgList_", "StmtList", "DeclStmt", "Type", "DeclStmt_",
    "ParamList", "DeclVarList_", "InitVar", "ParamList_", "Stmt", "DeclStmt", "AoCallOp", "WhileOp", "ForOp", "IfOp", "SwitchOp", "IOp", "OOp", "AOCall",
    "AOCall_", "ForInit", "ForExp", "ForLoop", "ElsePart", "Cases", "Cases_", "ACase", "OOp_", "OtherStmtList", "OtherStmt",  "comma", "rpar", "lpar", "opinc", "opnot", "opmul", "opminus",
    "ople", "opgt", "oplt", "opne", "opeq", "opand", "opor", "opplus", "id", "num", "lbrace", "rbrace", "semicolon", "opassign", "kwchar", "kwint", "kwreturn",
    "kwwhile", "kwfor", "kwif", "kwelse", "kwswitch", "kwin", "kwout", "colon", "end" };
    vector<Rule> grammar = {
        {"d", {"StmtList"}},
        {"DeclStmt", {"Type", "id", "DeclStmt_"}},
        {"DeclStmt_", {"lpar", "ParamList", "rpar", "lbrace", "StmtList", "rbrace"}},
        {"DeclStmt_", {"opassign", "num", "DeclVarList_", "semicolon"}},
        {"DeclStmt_", {"DeclVarList_", "semicolon"}},
        {"Type", {"kwchar"}},
        {"Type", {"kwint"}},
        {"DeclVarList_", {"comma", "id", "InitVar", "DeclVarList_"}},
        {"DeclVarList_", {"epsilon"}},
        {"InitVar", {"opassign",  "num"}},
        {"InitVar", {"opassign",  "kwchar"}},
        {"InitVar", {"epsilon"}},
        {"ParamList", {"Type",  "id", "ParamList_"}},
        {"ParamList", {"epsilon"}},
        {"ParamList_", {"comma",  "Type", "id", "ParamList_"}},
        {"ParamList_", {"epsilon"}},
        {"StmtList", {"Stmt", "StmtList"}},
        {"StmtList", {"epsilon"}},
        {"Stmt", {"DeclStmt"}},
        {"Stmt", {"AoCallOp"}},
        {"Stmt", {"WhileOp"}},
        {"Stmt", {"ForOp"}},
        {"Stmt", {"IfOp"}},
        {"Stmt", {"SwitchOp"}},
        {"Stmt", {"IOp"}},
        {"Stmt", {"OOp"}},
        {"Stmt", {"semicolon"}},
        {"Stmt", {"lbrace", "StmtList", "rbrace"}},
        {"Stmt", {"kwreturn", "E", "semicolon"}},
        {"OtherStmtList", {"OtherStmt", "OtherStmtList"}},
        {"OtherStmtList", {"epsilon"}},
        {"OtherStmt", {"DeclStmt"}},
        {"OtherStmt", {"AoCallOp"}},
        {"OtherStmt", {"WhileOp"}},
        {"OtherStmt", {"ForOp"}},
        {"OtherStmt", {"SwitchOp"}},
        {"OtherStmt", {"IOp"}},
        {"OtherStmt", {"OOp"}},
        {"OtherStmt", {"semicolon"}},
        {"OtherStmt", {"lbrace", "StmtList", "rbrace"}},
        {"OtherStmt", {"kwreturn", "E", "semicolon"}},
        {"AoCallOp", {"AoCall", "semicolon"}},
        {"AoCall", {"id", "AoCall_"}},
        {"AoCall_", {"opassign", "E"}},
        {"AoCall_", {"lpar", "ArgList", "rpar"}},
        {"WhileOp", {"kwwhile", "lpar", "E", "rpar", "Stmt"}},
        {"ForOp", {"kwfor", "lpar", "ForInit", "semicolon", "ForExp", "semicolon", "ForLoop", "rpar", "Stmt"}},
        {"ForInit", {"AoCall"}},
        {"ForInit", {"epsilon"}},
        {"ForExp", {"E"}},
        {"ForExp", {"epsilon"}},
        {"ForLoop", {"AoCall"}},
        {"ForLoop", {"opinc", "id"}},
        {"ForLoop", {"epsilon"}},
        {"IfOp", {"Matched"}},
        {"IfOp", {"Unmatched"}},
        {"Matched", {"kwif", "lpar", "id", "rpar", "Matched", "kwelse", "Matched"}},
        {"Matched", {"OtherStmtList"}},
        {"Unmatched", {"kwif", "lpar", "id", "rpar", "StmtList"}},
        {"Unmatched", {"kwif", "lpar", "id", "rpar", "Matched", "kwelse", "Unmatched"}},
        {"SwitchOp", {"kwswitch", "lpar", "E", "rpar", "lbrace", "Cases", "rbrace"}},
        {"Cases", {"ACase", "Cases_"}},
        {"Cases_", {"ACase", "Cases_"}},
        {"Cases_", {"epsilon"}},
        {"ACase", {"kwcase", "num", "colon", "StmtList"}},
        {"IOp", {"kwin", "id", "semicolon"}},
        {"OOp", {"kwout", "OOp_", "semicolon"}},
        {"OOp_", {"E"}},
        {"OOp_", {"str"}},
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
    //set<pair<Rule, string>> start = { { { "d", {".", "StmtList"}}, "end"} };
    bool unpack_flag = true;
    set<pair<Rule, string>> start;
    set<set<pair<Rule, string>>> I;
    set<pair<Rule, string>> p;
    vector<set<pair<Rule, string>>> e;
    start = { { { "e", {".", "E"}}, "end"} };
    p = Closure(grammar, F, term, start);
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
    if (unpack_flag) {
        unpack(I);
    }
    else {
        I.insert(p);
    }
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
    C = {};
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
                    auto it = find(e.begin(), e.end(), items_goto[e[i]][t]);
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
            auto temp = items_goto[e[i]][y];
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

