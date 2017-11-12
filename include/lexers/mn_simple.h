#ifndef MN_SIMPLE_H
#define MN_SIMPLE_H

#include <iostream>
#include <fstream>
#include <mn_lexer.h>
#include <stack>

namespace mn
{
    namespace lexer
    {
        class MN_Simple: public mn::MNLexer
        {
            public:
                MN_Simple(const std::string& file_name, MNErrorMsg* error_msg);
                MNToken* next_token();

            protected:
                MNToken* str(char delimiter='"');
                MNToken* id();
                MNToken* number();
                MNToken* character();
                MNToken* hex(std::string current);
        };
    }
}

#endif // MN_MN_SIMPLE
