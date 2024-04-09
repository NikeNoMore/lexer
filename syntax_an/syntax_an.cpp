#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <deque>

using namespace std;
using Lexem = pair<string, string>;
using Rule = pair<string, vector<string>>;

vector<Rule> pointer0(vector<Rule> &G) {
    vector<Rule> pointed;
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
            pointed.push_back({ x.first, temp });
        }
    }
    return pointed;
}

vector<string> First(vector<>) {

}

vector<pair<string, Rule>> pointer1(vector<Rule> &p) {

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
    string starting_symb = "E";
    vector<Rule> grammar = {
        {"E", {"E", "opplus", "E'"}},
        {"E", {"E'"}},
        {"E'", {"id"}},
        {"E'", {"num"}}
    };
    vector<Rule> P = pointer0(grammar);
    for (auto& x : P) {
        cout << x.first << " -> ";
        for (auto& y : x.second) {
            cout << y << " ";
        }
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
