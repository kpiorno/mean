#ifndef MN_PYTHON_H
#define MN_PYTHON_H

#include <iostream>
#include <fstream>
#include <mn_lexer.h>
#include <functional>
#include <stack>

namespace mn
{
    namespace lexer
    {
        class MN_Python: public mn::MNLexer
        {
            public:
                MN_Python(const std::string& file_name, MNErrorMsg* error_msg);
                MNToken* next_token();
                std::string next_token_str();

            protected:
                void register_reserverd_word(const std::string& name);
                MNToken* rule();
                MNToken* str(char delimiter='"');
                MNToken* id();
                MNToken* number();
                MNToken* character();
                MNToken* hex(std::string current);

            protected:
                std::stack<unsigned int> indent_values;
                unsigned int current_indent_value;
                unsigned int new_indent;
                bool check_indent;
                unsigned int dedent_count;
                unsigned int indent_check_count;
                unsigned int end_token;

        };
    }
}

#endif // MN_MNLEXER_H
