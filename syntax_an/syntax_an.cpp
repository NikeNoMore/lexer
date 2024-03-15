#include <iostream>
#include <vector>
#include <string>

using namespace std;
using Lexem = pair<string, string>;

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
    vector<Lexem> expr = {
        {"num", "1"},
        {"opplus", ""},
        {"num", "2"}
    };
    for (int x = int(expr.size()) - 1; x >= 0; x--) {
        cout << expr[x].first;
    }
}
