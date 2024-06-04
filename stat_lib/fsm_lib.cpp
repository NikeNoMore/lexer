#include "pch.h"
#include "fsm_frm.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <string>

using namespace std;

struct E {
    E(string f, bool r, bool s, int n_s, string l) : filter{ f }, read{ r }, save{ s }, next_state{ n_s }, lexem{ l }
    {}
    string filter;
    bool read, save;
    int next_state;
    string lexem;
};

using Lexem = pair<string, string>;
set<string> keyword = { "int", "char", "if", "else", "switch", "case", "while", "for", "return", "in", "out" };
string s_quote = "'", d_quote = "\"", letter = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_", digit = "0123456789";
string other = "";
string no_lexem = "";
map<int, vector<E>> G = {
{0, {E{" ", true, false, 0, no_lexem},
    E{".", true, true, 0, "point"},
    E{"\n", true, false, 0, no_lexem},
    E{"", true, false, 0, no_lexem},
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
    E(s_quote, true, false, 14, no_lexem),
    E(d_quote, true, false, 18, no_lexem),
    E(letter, true, true, 21, no_lexem),
    E("-", true, true, 22, no_lexem),
    E(digit, true, true, 23, no_lexem),
    E(other, false, false, -1, "error"),
    E("#", false, false, -1, "end"),}},

{2, {E("=", true, false, 3, no_lexem),
    E(other, false, false, 0, "oplt")}},

{3, {E(other, false, false, 0, "ople")}},

{4, {E("=", true, false, 5, no_lexem),
    E(other, false, false, 0, "opnot")}},

{5, {E(other, false, false, 0, "opne")}},

{6, {E("=", true, false, 7, no_lexem),
    E(other, false, false, 0, "opassign")}},

{7, {E(other, false, false, 0, "opeq")}},

{8, {E("+", true, false, 9, no_lexem),
    E(other, false , false, 0, "opplus")}},

{9, {E(other, false , false, 0, "opinc")}},

{10, {E("|", true, false, 11, no_lexem),
    E(other, false, false, 0, "bar")}},

{11, {E(other, false, false, 0, "opor")}},

{12, {E("&", true, false, 13, no_lexem),
    E(other, false, false, -1, "error")}},

{13, {E(other, false, false, 0, "opand")}},

{14, {E(s_quote, true, false, -1, "error"),
    E(other, true, true, 16, no_lexem)}},

{16, {E(s_quote, true, false, 0, "char"),
    E(other, false, false, -1, "error")}},

{18, {E("#", false, false, -1, "error"),
    E(d_quote, true, false, 0, "str"),
    E(other, true, true, 18, no_lexem)}},

{21, {E(letter + digit, true, true, 21, ""),
    E(other, false, false, 0, "id")}},

{22, {E(digit, true, true, 23, ""),
    E(other, false, false, 0, "opminus")}},

{23, {E(digit, true, true, 23, no_lexem),
    E(other, false, false, 0, "num")}}
};

void read(char& cache, istream& stream) {
    if (stream) {
        stream.get(cache);
    }
    if (!stream) {
        cache = 0;
    }
}

pair<int, Lexem> tick(int state, istream& stream, char& cache, string& memory) {
    E action = { "NO_ACTION", false, false, -1, "" };
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
        read(cache, stream);
    }
    state = action.next_state;
    if (action.lexem != "id") {
        if (action.lexem == "end" or action.lexem == "error" or action.lexem == "opminus") {
            memory = "";
            return { state, Lexem{action.lexem, memory} };
        }
        return { state, Lexem{action.lexem, memory } };
    }
    else if (keyword.contains(memory)) {
        return { state, Lexem{ "kw" + memory, memory } };
    }
    else {
        return { state, Lexem{ action.lexem, memory } };
    }
}