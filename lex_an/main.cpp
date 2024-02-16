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