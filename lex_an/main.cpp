#include <iostream>
#include <vector>
#include "lexer.h"

using namespace std;

int dice();

int main()
{
    Lexer lex(cin);
    Lexem l = lex.getNextLexem();
    cout << l.first + "(" + l.second + ")" << endl;
}

/*
===============================================

#include "../stat_lib/lexer_frm.h"
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

int main()
{
    Lexer lex(cin);
    vector<Lexem> data;
    Lexem l = lex.getNextLexem();
    data.push_back(l);
    while (l != Lexem {"", ""} ) {
        l = lex.getNextLexem();
        data.push_back(l);
    }
    vector<string> res;
    vector<string> stack;
    int arg = 0;
    for (auto& x : data) {
        if (x.first == "num") {
            res.push_back(x.second);
            arg += 1;
        }
        else if (x.first == "opplus") {
            if (stack.empty()) {
                stack.push_back(x.first);
            }
            else {
                for (int y = stack.size() - 1; y >= 0; y--) {
                    res.push_back(stack[y]);
                    stack.pop_back();
                    arg -= 1;
                }
                stack.push_back(x.first);
            }
        }
        else if (x.first == "opmul") {
            if (stack.empty()) {
                stack.push_back(x.first);
            }
            else if (*stack.rbegin() == "opplus") {
                stack.push_back("opmul");
            }
            else {
                res.push_back("opmul");
                arg -= 1;

            }
        }
    }
    if (!stack.empty()) {
        for (int y = stack.size() - 1; y >= 0; y--) {
            res.push_back(stack[y]);
            arg -= 1;
            stack.pop_back();
        }
    }
    if (arg != 1) {
        cout << "ERROR";
    }
    else {
        for (auto& x : res) {
            if (x == "opplus") {
                cout << "+ ";
            }
            else if (x == "opmul") {
                cout << "* ";
            }
            else {
                cout << x << " ";
            }
        }
    }
}
*/
