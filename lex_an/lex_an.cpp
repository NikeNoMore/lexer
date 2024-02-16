#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <set>

using namespace std;

struct E {
    E(string f, bool r, bool s, int n_s, string l) : filter{ f }, read{ r }, save{ s }, next_state{ n_s }, lexem{ l }
    {}
    string filter;
    bool read, save;
    int next_state;
    string lexem;
};

struct config {
    config(int state, char cache) : state{ state }, cache{ cache }
    {}
    int state;
    char cache;
};

void next_char(char& cache, istream& in) {
    if (in) {
        in.get(cache);
    }
    if (!in) {
        cache = 0;
    }
}

using Lexem = pair<string, string>;
set<string> keyword = { "int", "char", "if", "else", "switch", "case", "while", "for", "return", "in", "out" };
string s_quote = "'", d_quote = "\"", letter = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ", digit = "0123456789";
string other = "";
string no_lexem = "";
map<int, vector<E>> G = {
{0, {{" ", true, false, 0, ""},
    E("(", true, false, 0, "lpar"),
    E(")", true, false, 0, "rpar"),
    E("{", true, false, 0, "lbrace"),
    E("}", true, false, 0, "rbrace"),
    E(";", true, false, 0, "semicolon"),
    E(",", true, false, 0, "comma"),
    E(":", true, false, 0, "colon"),
    E(">", true, false, 0, "opgt"),
    E("*", true, false, 0, "opmul"),
    E("<", true, false, 2, no_lexem),
    E("!", true, false, 4, no_lexem),
    E("=", true, false, 6, no_lexem),
    E("+", true, false, 8, no_lexem),
    E("|", true, false, 10, no_lexem),
    E("&", true, false, 12, no_lexem),
    E(s_quote, true, false, 14, ""),
    E(d_quote, true, false, 18, ""),
    E(letter, true, true, 21, ""),
    E("-", true, true, 22, ""),
    E(digit, true, true, 23, ""),
    E(other, false, false, -1, "error"),
    E("#", false, false, -1, ""),}},

{2, {E("=", true, false, 3, ""),
    E(other, false, false, 0, "oplt")}},

{3, {E(other, false, false, 0, "ople")}},

{4, {E("=", true, false, 5, ""),
    E(other, false, false, 0, "opnot")}},

{5, {E(other, false, false, 0, "opne")}},

{6, {E("=", true, false, 7, ""),
    E(other, false, false, 0, "opassign")}},

{7, {E(other, false, false, 0, "opeq")}},

{8, {E("+", true, false, 9, ""),
    E(other, false , false, 0, "opplus")}},

{9, {E(other, false , false, 0, "opinc")}},

{10, {E("|", true, false, 11, ""),
    E(other, false, false, -1, "error")}},

{11, {E(other, false, false, 0, "opor")}},

{12, {E("&", true, false, 13, ""),
    E(other, false, false, -1, "error")}},

{13, {E(other, false, false, 3, "opand")}},

{14, {E(s_quote, true, false, -1, "error"),
    E(other, true, true, 16, "")}},

{16, {E(s_quote, true, false, 0, "char"),
    E(other, false, false, -1, "error")}},

{18, {E("#", false, false, -1, "error"),
    E(d_quote, true, false, 0, "str"),
    E(other, true, true, 18, "")}},

{21, {E(letter + digit, true, true, 21, ""),
    E(other, false, false, 0, "id")}},

{22, {E(digit, true, true, 23, ""),
    E(other, false, false, 0, "opminus")}},

{23, {E(digit, true, true, 23, ""),
    E(other, false, false, 0, "num")}}
};


Lexem tick(istream& inp, config& conf, string& memory) {
    E action = { "NO_ACTION", false, false, -1, "" };
    int& state = conf.state;
    char& cache = conf.cache;
    for (auto& e : G[state]) {
        if (e.filter == "") {
            action = e;
        }
    }
    assert(action.filter.empty());
    for (auto& e : G[state]) {
        if (e.filter.find(cache) != string::npos) {
            action = e;
        }
    }
    if (action.save) {
        memory += cache;
    }
    if (action.read) {
        next_char(cache, inp);
    }
    state = action.next_state;
    //cout << conf.state << ' ' << action.lexem << ' ' << memory << ' ';
    if (!action.lexem.empty()) {
        if (action.lexem != "id") {
            return Lexem{ action.lexem, memory };
        }
        else if (keyword.contains(memory)){
            return Lexem{"kw", memory};
        }
        else {
            return Lexem{ action.lexem, memory };
        }
    }
    else {
        cout << '0' << ' ';
        return tick(inp, conf, memory);
    }
}

int main()
{
    char cache;
    next_char(cache, cin);
    config conf_of_prog = { 0, cache};
    Lexem lex;
    vector<string> vect;
    string memory;
    while (conf_of_prog.state != -1) {
        cout << '1' << ' ';
        lex = tick(cin, conf_of_prog, memory);
        vect.push_back(lex.first + '(' + lex.second + ')');
        memory = "";
    }
    cout << '\n';
    for (auto& x : vect) {
        cout << x << ' ';
    }
}
