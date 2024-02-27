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

===============================================

#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using Lexem = pair<string, string>;

int main()
{
    vector<string> res;
    vector<string> stack;
    vector<Lexem> data = { {"num", "2"}, {"opplus", ""}, {"num", "3"}, {"opmul", ""},  {"num", "4"} };
    for (auto& x : data) {
        if (x.first == "num") {
            res.push_back(x.second);
        }
        else if (x.first == "opplus") {
            if (stack.empty()) {
                stack.push_back(x.first);
            }
            else {
                for (int y = 0; y < stack.size(); y++) {
                    res.push_back(stack.back());
                    stack.pop_back();
                }
                stack.push_back(x.first);
            }
        }
        else {
            if (stack.empty()) {
                stack.push_back(x.first);
            }
            else if (*stack.rbegin() == "opplus") {
                stack.push_back("opmul");
            }
            else {
                res.push_back("opmul");
            }
        }
    }
    for (auto& x : res) {
        cout << x;
    }
}
