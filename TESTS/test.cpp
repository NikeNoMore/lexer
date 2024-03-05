#include "pch.h"
#include "../stat_lib/lexer_frm.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

namespace lex_tests {

    TEST(CorrectCode, BasicProgram)
    {
        using namespace std;
        string prog = R"(
int main() {
    return 0;
}
)";
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
            LEX_EOF
        };

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (auto&& lex : correct) {
            EXPECT_EQ(lex, lexer.getNextLexem());
        }
    }

    TEST(CorrectCode, ComplexProgram)
    {
        using namespace std;
        string prog = R"(
int fib(int n) {
    if (n < 2) return n;
    return fib(n - 1) + fib(n-2);
}

int is_even(int x) {
    while (x > 0) x = x - 2;
    return x == 0;
}

int main() {
    int n;
    in n;
    for (int i = 0; i < n; i++) {
        if (is_even(i)) {
            out fib(i);
        }
    }
    return 0;
}
)";
        Lexem tokens[] = {
            // int fib(int n) {
            { "kw", "int" },
            { "id", "fib" },
            { "lpar", "" },
            { "kw", "int" },
            { "id", "n" },
            { "rpar", "" },
            { "lbrace", "" },

            // if (n < 2) return n;
            { "kw", "if" },
            { "lpar", "" },
            { "id", "n" },
            { "oplt", "" },
            { "num", "2" },
            { "rpar", "" },
            { "kw", "return" },
            { "id", "n" },
            { "semicolon", "" },

            // return fib(n - 1)
            { "kw", "return" },
            { "id", "fib" },
            { "lpar", "" },
            { "id", "n" },
            { "opminus", "" },
            { "num", "1" },
            { "rpar", "" },

            //  + fib(n-2);
            { "opplus", "" },
            { "id", "fib" },
            { "lpar", "" },
            { "id", "n" },
            { "num", "-2" },
            { "rpar", "" },
            { "semicolon", "" },

            // }
            { "rbrace", "" },

            // int is_even(int x) {
            { "kw", "int" },
            { "id", "is_even" },
            { "lpar", "" },
            { "kw", "int" },
            { "id", "x" },
            { "rpar", "" },
            { "lbrace", "" },

            // while (x > 0)
            { "kw", "while" },
            { "lpar", "" },
            { "id", "x" },
            { "opgt", "" },
            { "num", "0" },
            { "rpar", "" },

            // x = x - 2;
            { "id", "x" },
            { "opassign", "" },
            { "id", "x" },
            { "opminus", "" },
            { "num", "2" },
            { "semicolon", "" },

            //    return x == 0;
            { "kw", "return" },
            { "id", "x" },
            { "opeq", "" },
            { "num", "0" },
            { "semicolon", "" },

            //}
            { "rbrace", "" },
            // int main() {
            { "kw", "int" },
            { "id", "main" },
            { "lpar", "" },
            { "rpar", "" },
            { "lbrace", "" },

            //    int n;
            { "kw", "int" },
            { "id", "n" },
            { "semicolon", "" },

            //    in n;
            { "kw", "in" },
            { "id", "n" },
            { "semicolon", "" },

            //    for (int i = 0;
            { "kw", "for" },
            { "lpar", "" },
            { "kw", "int" },
            { "id", "i" },
            { "opassign", "" },
            { "num", "0" },
            { "semicolon", "" },

            //  i < n; i++) {
            { "id", "i" },
            { "oplt", "" },
            { "id", "n" },
            { "semicolon", "" },

            { "id", "i" },
            { "opinc", "" },
            { "rpar", "" },
            { "lbrace", "" },

            //        if (is_even(i)) {
            { "kw", "if" },
            { "lpar", "" },
            { "id", "is_even" },
            { "lpar", "" },
            { "id", "i" },
            { "rpar", "" },
            { "rpar", "" },
            { "lbrace", "" },

            //            out fib(i);
            { "kw", "out" },
            { "id", "fib" },
            { "lpar", "" },
            { "id", "i" },
            { "rpar", "" },
            { "semicolon", "" },

            //        }
            { "rbrace", "" },
            //    }
            { "rbrace", "" },

            //    return 0;
            { "kw", "return" },
            { "num", "0" },
            { "semicolon", "" },

            //}
            { "rbrace", "" },
            LEX_EOF
        };

        vector<Lexem> correct(begin(tokens), end(tokens));

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (int i = 0; i < correct.size(); ++i) {
            EXPECT_EQ(correct[i], lexer.getNextLexem()) << "token #" << i;
        }
    }

    TEST(TrickyCode, BasicProgram)
    {
        using namespace std;
        string prog = "int main(){return 0;}";
        vector<Lexem> correct = {
            { "kwint", "" },
            { "id", "main" },
            { "lpar", "" },
            { "rpar", "" },
            { "lbrace", "" },
            { "kwreturn", "" },
            { "num", "0" },
            { "semicolon", "" },
            { "rbrace", "" },
            LEX_EOF
        };

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (auto&& lex : correct) {
            EXPECT_EQ(lex, lexer.getNextLexem());
        }
    }

    TEST(TrickyCode, MultilineString)
    {
        using namespace std;
        string prog = R"(out "Hello
world
'
for everyone!!!
")";
        string content = "Hello\n"
            "world\n"
            "'\n"
            "for everyone!!!\n";
        vector<Lexem> correct = {
            { "kwout", "" },
            { "str", content },
            LEX_EOF
        };

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (auto&& lex : correct) {
            EXPECT_EQ(lex, lexer.getNextLexem());
        }
    }

    TEST(TrickyCode, Expressions)
    {
        using namespace std;
        string prog = "int a=0;int aa=182;out aa+a==-123;";
        vector<Lexem> correct = {
            // int a=0;
            { "kw", "int" },
            { "id", "a" },
            { "opassign", "" },
            { "num", "0" },
            { "semicolon", "" },

            // int aa=182;
            { "kwint", "" },
            { "id", "aa" },
            { "opassign", "" },
            { "num", "182" },
            { "semicolon", "" },

            // out aa+a==-123;
            { "kwout", "" },
            { "id", "aa" },
            { "opplus", "" },
            { "id", "a" },
            { "opeq", "" },
            { "num", "-123" },
            { "semicolon", "" },
            LEX_EOF
        };

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (auto&& lex : correct) {
            EXPECT_EQ(lex, lexer.getNextLexem());
        }
    }

    TEST(ErrorCode, BadString)
    {
        using namespace std;
        string prog = R"(out "incomplete string;)";
        vector<Lexem> correct = {
            { "kwout", "" },
            LEX_ERROR,
        };

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (auto&& lex : correct) {
            EXPECT_EQ(lex, lexer.getNextLexem());
        }
    }

    TEST(ErrorCode, EmptyChar)
    {
        using namespace std;
        string prog = R"(out '')";
        vector<Lexem> correct = {
            { "kwout", "" },
            LEX_ERROR,
        };

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (auto&& lex : correct) {
            EXPECT_EQ(lex, lexer.getNextLexem());
        }
    }

    TEST(ErrorCode, DoubleChar)
    {
        using namespace std;
        string prog = R"(out 'xy')";
        vector<Lexem> correct = {
            { "kwout", "" },
            LEX_ERROR,
        };

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (auto&& lex : correct) {
            EXPECT_EQ(lex, lexer.getNextLexem());
        }
    }

    TEST(ErrorCode, SingleOperator)
    {
        using namespace std;
        string prog = R"(a | b)";
        vector<Lexem> correct = {
            { "id", "a" },
            LEX_ERROR,
        };

        stringstream stream{ prog };
        Lexer lexer(stream);

        for (auto&& lex : correct) {
            EXPECT_EQ(lex, lexer.getNextLexem());
        }
    }

}