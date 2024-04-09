#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <deque>

using namespace std;
using Lexem = pair<string, string>;
using Rule = pair<string, vector<string>>;

void pointer(map<string, Rule> &G) {
    for (auto& x : G) {
        x.second.second.
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
    string starting_symb = "E";
    map<string, Rule> grammar = {
        {"E'", {"E", {"E", "opplus", "E'"}}},
        {"E'", {"E", {"E'"}}},
        {"id", {"E'", {"id"}}},
        {"num", {"E'", {"num"}}}
    };
    int max_len = 0;
    map<int, vector<pair<string, Rule>>> grammar_by_len;
    int temp_size = grammar.size();
    for (auto& x : grammar) {
        int temp = x.second.second.size();
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
    }
}
