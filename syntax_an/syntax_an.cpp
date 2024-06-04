#include <iostream>
#include "../stat_lib/lexer_frm.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>

using namespace std;
using Lexem = pair<string, string>;
using Rule = pair<string, vector<string>>;
map<string, int> num_nodes;
//string starting_symb = "d";
string starting_symb = "e";
int label_counter = 1;
int numb = 0;
map<int, string> symb_tab;

pair<Lexem, int> numerate(Lexem& lex) {
    if (lex.first != "id") {
        if (num_nodes.contains(lex.first)) {
            num_nodes[lex.first]++;
        }
        else {
            num_nodes[lex.first] = 0;
        }
        return { lex, num_nodes[lex.first] };
    }
    else {
        numb++;
        symb_tab[numb] = lex.second;
        return { lex, numb };
    }
}

void tick(Lexem symb, vector<int>& state, vector<pair<Lexem, int>>& stack, vector<string>& alphabet, vector<vector<pair<string, pair<int, Rule>>>>& E, map<pair<Lexem, int>, vector<pair<Lexem, int>>>& tree, bool& eps_flag) {
    int huh = *state.rbegin();
    int term_id;
    if (symb.first.empty()) {
        symb = (*stack.rbegin()).first;
    }
    auto it1 = find(alphabet.begin(), alphabet.end(), symb.first);
    term_id = int(distance(alphabet.begin(), it1));
    auto heh = E[huh][term_id];
    if (heh.second.first == -1) {
        cout << "FALSE";
        while (!stack.empty()) {
            stack.pop_back();
        }
        return;
    }
    if (heh.first == "acc") {
        tree[{ {starting_symb, ""}, -1}].push_back(*stack.rbegin());
        cout << "TRUE";
        stack.pop_back();
        return;
    }
    if (heh.second.first != -2) {
        state.push_back(heh.second.first);
    }
    if (heh.first == "r") {
        auto it1 = heh.second.second.second.rbegin();
        auto it3 = stack.rbegin();
        while (*it1 != (*it3).first.first) {
            if (*it1 == "epsilon") {
                eps_flag = true;
                break;
            }
            else {
                it3--;
            }
        }
        int counter = 0;
        vector<pair<Lexem, int>> temp_;
        if(!eps_flag){
            for (it1; it1 != heh.second.second.second.rend(); it1++) {
                if ((*it3).first.first == *it1) {
                    counter += 1;
                    temp_.push_back(*it3);
                    it3 = decltype(it3)(stack.erase(next(it3).base()));
                }
                else {
                    cout << "ERROR, STUPID";
                    return;
                }
            }
        }
        if (heh.second.first != -2) {
            state.push_back(heh.second.first);
        }
        for (counter; counter > 0; counter--) {
            state.pop_back();
        }
        if (symb.first == "") {
            stack.pop_back();
        }
        Lexem temp_lex = { heh.second.second.first, "" };
        stack.push_back(numerate(temp_lex));
        for (auto& y : temp_) {
            tree[*stack.rbegin()].push_back(y);
        }
        if (!eps_flag) {
            tick({ "", "" }, state, stack, alphabet, E, tree, eps_flag);
            tick(symb, state, stack, alphabet, E, tree, eps_flag);
            if (eps_flag) {
                eps_flag = false;
                tick({ "", "" }, state, stack, alphabet, E, tree, eps_flag);
                tick(symb, state, stack, alphabet, E, tree, eps_flag);
            }
        }
        else {
            eps_flag = false;
            tick({ "", "" }, state, stack, alphabet, E, tree, eps_flag);
            tick(symb, state, stack, alphabet, E, tree, eps_flag);
        }
    }
    else if (heh.first == "s") {
        stack.push_back(numerate(symb));
    }
}

void tree_print(vector<pair<Lexem, int>>& node, map<pair<Lexem, int>, vector<pair<Lexem, int>>>& tree, vector<bool>& son, vector<string>& t) {
    int end = int(son.size());
    for (int i = 0; i < end; i++) {
        if (i != end - 1) {
            if (!son[i]) {
                cout << "  ";
            }
            else {
                cout << "| ";
            }
        }
        else {
            if (!son[i]) {
                cout << "|_";
            }
            else {
                cout << "|-";
            }
        }
    }
    int a = 0;
    for (a; a < int(node.size()); a++) {
        Lexem temp = node[a].first;
        cout << temp.first;
        if (temp.second != "") {
            cout << "(" << temp.second << ")";
        }
        cout << " ";
    }
    a--;
    end = int(tree[node[a]].size()) - 1;
    bool flag = false;
    if (find(t.begin(), t.end(), node[a].first.first) == t.end()) {
        flag = true;
    }
    for (int i = end; i >= 0; i--) {
        if (!flag) {
            Lexem temp;
            temp = tree[node[a]][i].first;
            if (find(t.begin(), t.end(), temp.first) == t.end()) {
                flag = true;
            }
            cout << " " << temp.first;
            if (temp.second != "") {
                cout << "(" << temp.second << ")";
            }
        }
        if(flag) {
            vector<pair<Lexem, int>> temp;
            temp.push_back(tree[node[a]][i]);
            if (find(t.begin(), t.end(), tree[node[a]][i].first.first) != t.end()) {
                while (find(t.begin(), t.end(), tree[node[a]][i].first.first) != t.end()) {
                    if (tree[node[a]][i] != *tree[node[a]].begin()) {
                        i--;
                        temp.push_back(tree[node[a]][i]);
                    }
                    else {
                        break;
                    }
                }
            }
            if (i != 0) {
                son.push_back(true);
            }
            else {
                son.push_back(false);
            }
            cout << endl;
            tree_print(temp, tree, son, t);
            son.pop_back();
        }
    }
}

void translator(pair<Lexem, int>& node, map<pair<Lexem, int>, vector<pair<Lexem, int>>>& tree, vector<string>& t, vector<string>& atoms) {
    string temp = node.first.first;
    if (temp == "e") {
        translator(tree[node][0], tree, t, atoms);
        atoms.push_back("(OUT,,,[" + to_string(numb) + "])");
    }
    else if (temp == "E") {
        translator(tree[node][0], tree, t, atoms);
    }
    else if (temp == "E7") {
        if (int(tree[node].size()) > 1) {
            translator(tree[node][2], tree, t, atoms);
            string temp1 = "(OR,[" + to_string(numb) + "],";
            translator(tree[node][0], tree, t, atoms);
            temp1.append("[" + to_string(numb) + "],[" + to_string(numb + 1) + "])");
            numb++;
            atoms.push_back(temp1);
        }
        else {
            translator(tree[node][0], tree, t, atoms);
        }
    }
    else if (temp == "E6") {
        if (int(tree[node].size()) > 1) {
            translator(tree[node][2], tree, t, atoms);
            string temp1 = "(AND,[" + to_string(numb) + "],[";
            translator(tree[node][0], tree, t, atoms);
            temp1.append(to_string(numb) + "],[" + to_string(numb + 1) + "])");
            numb++;
            atoms.push_back(temp1);
        }
        else {
            translator(tree[node][0], tree, t, atoms);
        }
    }
    else if (temp == "E5") {
        if (int(tree[node].size()) > 1) {
            if (tree[node][1].first.first == "opeq") {
                translator(tree[node][2], tree, t, atoms);
                string temp1 = "(EQ,[" + to_string(numb) + "],";
                translator(tree[node][0], tree, t, atoms);
                temp1.append("[" + to_string(numb) + ",L" + to_string(label_counter) + ")");
                numb++;
                atoms.push_back("(MOV,1,,[" + to_string(numb) + "])");
                atoms.push_back(temp1);
                atoms.push_back("(MOV,0,,[" + to_string(numb) + "])");
                atoms.push_back("(LBL,,,L" + to_string(label_counter) + ")");
                label_counter++;
            }
            else if (tree[node][1].first.first == "opne") {
                translator(tree[node][2], tree, t, atoms);
                string temp1 = "(NE,[" + to_string(numb) + "],";
                translator(tree[node][0], tree, t, atoms);
                temp1.append("[" + to_string(numb) + "],L" + to_string(label_counter) + ")");
                numb++;
                atoms.push_back("(MOV,1,,[" + to_string(numb) + "])");
                atoms.push_back(temp1);
                atoms.push_back("(MOV,0,,[" + to_string(numb) + "])");
                atoms.push_back("(LBL,,,L" + to_string(label_counter) + ")");
                label_counter++;
            }
            else if (tree[node][1].first.first == "opgt") {
                translator(tree[node][2], tree, t, atoms);
                string temp1 = "(GT,[" + to_string(numb) + "],";
                translator(tree[node][0], tree, t, atoms);
                temp1.append("[" + to_string(numb) + "],L" + to_string(label_counter) + ")");
                numb++;
                atoms.push_back("(MOV,1,,[" + to_string(numb) + "])");
                atoms.push_back(temp1);
                atoms.push_back("(MOV,0,,[" + to_string(numb) + "])");
                atoms.push_back("(LBL,,,L" + to_string(label_counter) + ")");
                label_counter++;
            }
            else if (tree[node][1].first.first == "oplt") {
                translator(tree[node][2], tree, t, atoms);
                string temp1 = "(LT,[" + to_string(numb) + "],";
                translator(tree[node][0], tree, t, atoms);
                temp1.append("[" + to_string(numb) + "],L" + to_string(label_counter) + ")");
                numb++;
                atoms.push_back("(MOV,1,,[" + to_string(numb) + "])");
                atoms.push_back(temp1);
                atoms.push_back("(MOV,0,,[" + to_string(numb) + "])");
                atoms.push_back("(LBL,,,L" + to_string(label_counter) + ")");
                label_counter++;
            }
            else if (tree[node][1].first.first == "ople") {
                translator(tree[node][2], tree, t, atoms);
                string temp1 = "(LE,[" + to_string(numb) + "],";
                translator(tree[node][0], tree, t, atoms);
                temp1.append("[" + to_string(numb) + "],L" + to_string(label_counter) + ")");
                numb++;
                atoms.push_back("(MOV,1,,[" + to_string(numb) + "])");
                atoms.push_back(temp1);
                atoms.push_back("(MOV,0,,[" + to_string(numb) + "])");
                atoms.push_back("(LBL,,,L" + to_string(label_counter) + ")");
                label_counter++;
            }
        }
        else {
            translator(tree[node][0], tree, t, atoms);
        }
    }
    else if (temp == "E4") {
        if (int(tree[node].size()) > 1) {
            if (tree[node][1].first.first == "opplus") {
                translator(tree[node][2], tree, t, atoms);
                string temp1 = "(ADD,[" + to_string(numb) + "],";
                translator(tree[node][0], tree, t, atoms);
                temp1.append("[" + to_string(numb) + "],[" + to_string(numb + 1) + "])");
                atoms.push_back(temp1);
                numb++;
            }
            else if (tree[node][1].first.first == "opminus") {
                translator(tree[node][2], tree, t, atoms);
                string temp1 = "(SUB,[" + to_string(numb) + "],";
                translator(tree[node][0], tree, t, atoms);
                temp1.append("[" + to_string(numb) + "],[" + to_string(numb + 1) + "])");
                atoms.push_back(temp1);
                numb++;
            }
        }
        else {
            translator(tree[node][0], tree, t, atoms);
        }
    }
    else if (temp == "E3") {
        if (int(tree[node].size()) > 1) {
            if (tree[node][1].first.first == "opmul") {
                translator(tree[node][2], tree, t, atoms);
                string temp1 = "(MUL,[t]" + to_string(numb) + "],";
                translator(tree[node][0], tree, t, atoms);
                temp1.append("[" + to_string(numb) + "],[" + to_string(numb + 1) + "])");
                atoms.push_back(temp1);
                numb++;
            }
        }
        else {
            translator(tree[node][0], tree, t, atoms);
        }
    }
    else if (temp == "E2") {
        if (int(tree[node].size()) > 1) {
            if (tree[node][1].first.first == "opnot") {
                translator(tree[node][0], tree, t, atoms);
                atoms.push_back("(NOT,[" + to_string(numb) + "],,[" + to_string(numb + 1) + "])");
                numb++;
            }
        }
        else {
            translator(tree[node][0], tree, t, atoms);
        }
    }
    else if (temp == "E1") {
        if (tree[node][0].first.first == "opinc") {
            numb++;
            atoms.push_back("(MOV,[" + to_string(tree[node][1].second) + "],,[" + to_string(numb) + "])");
            atoms.push_back("(ADD,[" + to_string(tree[node][1].second) + "],1,[" + to_string(tree[node][1].second) + "])");
        }
        else if (tree[node][0].first.first == "num") {
            numb++;
            atoms.push_back("(MOV,[" + to_string(tree[node][0].second) + "],,[" + to_string(numb) + "])");
        }
        else if (tree[node][0].first.first == "id") {
            if (int(tree[node].size()) > 1) {
                if (tree[node][1].first.first == "opinc") {
                    numb++;
                    atoms.push_back("(ADD,[" + to_string(tree[node][0].second) + "],1,[" + to_string(tree[node][0].second) + "])");
                    atoms.push_back("(MOV,[" + to_string(tree[node][0].second) + "],,[" + to_string(numb) + "])");
                }
            }
            else {
                numb++;
                atoms.push_back("(MOV,[" + to_string(tree[node][0].second) + "],,[" + to_string(numb) + "])");
            }
        }
        else if (tree[node][2].first.first == "lpar" && int(tree[node].size()) == 3) {
            translator(tree[node][1], tree, t, atoms);
        }
        else if (tree[node][3].first.first == "id") {
            translator(tree[node][1], tree, t, atoms);
            numb++;
            atoms.push_back("(CALL,[" + to_string(tree[node][3].second) + "],,[" + to_string(numb) + "])");
        }
    }
    else if (temp == "ArgList") {
        if (!tree[node].empty()) {
            translator(tree[node][1], tree, t, atoms);
            atoms.push_back("(PARAM,,,[" + to_string(numb) + "])");
            translator(tree[node][0], tree, t, atoms);
        }
    }
    else if (temp == "ArgList_") {
        if (!tree[node].empty()) {
            translator(tree[node][1], tree, t, atoms);
            atoms.push_back("(PARAM,,,[" + to_string(numb) + "])");
            translator(tree[node][0], tree, t, atoms);
        }
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
    vector<string> term;
    vector<string> non_term;
    vector<string> alphabet;
    vector<Rule> grammar;
    filebuf fb1;
    vector<vector<pair<string, pair<int, Rule>>>> E;
    if (fb1.open("table.txt", ios::in)) {
        istream is1(&fb1);
        Lexer lex1(is1);
        string a = lex1.getNextLexem().second;
        int A = stoi(a);
        string b = lex1.getNextLexem().second;
        int B = stoi(b);
        string sts = lex1.getNextLexem().second;
        int states = stoi(sts);
        for (int i = 0; i < A; i++) {
            string temp = lex1.getNextLexem().second;
            non_term.push_back(temp);
            alphabet.push_back(temp);
        }
        for (int i = 0; i < B; i++) {
            string temp = lex1.getNextLexem().second;
            alphabet.push_back(temp);
            term.push_back(temp);
        }
        for (int i = 0; i < states; i++) {
            E.push_back({});
            for (int j = 0; j < A + B; j++) {
                E[i].push_back({});
                if (j < A) {
                    string temp = lex1.getNextLexem().second;
                    E[i][j] = { "", {stoi(temp), {}} };
                }
                else {
                    string temp = lex1.getNextLexem().second;
                    if (temp != "r" && temp != "s" && temp != "acc") {
                        E[i][j] = { "", {stoi(temp), {}} };
                    }
                    else if (temp == "s" || temp == "acc") {
                        E[i][j] = { temp, {stoi(lex1.getNextLexem().second), {}} };
                    }
                    else {
                        int st = stoi(lex1.getNextLexem().second);
                        Rule temp_r;
                        temp_r.first = lex1.getNextLexem().second;
                        temp = lex1.getNextLexem().second;
                        while (temp != ".") {
                            temp_r.second.push_back(temp);
                            temp = lex1.getNextLexem().second;
                        }
                        E[i][j] = { "r", {st, temp_r} };
                    }
                }
            }
        }
    }
    /*for (auto& x : E) {
        for (auto& y : x) {
            if (!y.first.empty()) {
                cout << y.first << " ";
            }
            cout << y.second.first;
            if (y.first == "r") {
                cout << " " << y.second.second.first;
                for (auto& c : y.second.second.second) {
                    cout << " " << c;
                }
                cout << " ";
            }
            cout << " ";
        }
        cout << endl;
    }*/
    filebuf fb;
    map<pair<Lexem, int>, vector<pair<Lexem, int>>> tree;
    vector<int> state = { 0 };
    vector<pair<Lexem, int>> stack;
    if (fb.open("test.txt", ios::in)) {
        bool eps = false;
        istream is(&fb);
        Lexer lex(is);
        while (is) {
            Lexem l = lex.getNextLexem();
            tick(l, state, stack, alphabet, E, tree, eps);
            if (l == LEX_EOF) {
                break;
            }
        }
        fb.close();
    }
    while (!stack.empty()) {
        bool eps = false;
        tick({"", ""}, state, stack, alphabet, E, tree, eps);
    }
    cout << endl;
    /*for (auto& x : tree) {
        cout << x.first.first.first << x.first.second << "->";
        for (auto& y : x.second) {
            cout << y.first.first << y.first.second << y.second << " ";
        }
        cout << endl;
    }*/
    vector<bool> son;
    vector<pair<Lexem, int>> start_node = { { {starting_symb, ""}, -1}};
    tree_print(start_node, tree, son, term);
    vector<string> atoms;
    int id_end = numb;
    numb++;
    translator(start_node[0], tree, term, atoms);
    int t_count = 0;
    cout << endl << endl << "Table of symbols:" << endl;
    for (int i = 1; i <= numb; i++) {
        if (i <= id_end) {
            cout << i << " : " << symb_tab[i] << endl;
        }
        else {
            t_count++;
            cout << i << " : " << "t" + to_string(t_count) << endl;
        }
    }
    for (auto i = atoms.begin(); i != atoms.end(); i++) {
        cout << endl << *i;
    }
}
