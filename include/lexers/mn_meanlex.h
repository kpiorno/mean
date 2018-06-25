#ifndef MN_MEANLEX_H
#define MN_MEANLEX_H

#include <iostream>
#include <fstream>
#include <mn_lexer.h>
#include <functional>
#include <stack>
#include <list>

namespace mn
{
    namespace lexer
    {
        class MNMeanLex: public mn::MNLexer
        {
            public:
                MNMeanLex(const std::string& source, MNErrorMsg* error_msg, bool from_file = true);
                MNToken* next_token();
                std::string next_token_str();
                bool clean_token(MNToken *token);
                bool recuperate_token(MNToken *token);
                void notify_error(mn::MNNode* node=nullptr);

            protected:
                void register_reserverd_word(const std::string& name);
                MNToken* rule();
                MNToken* str(char delimiter='"', std::string begin="");
                MNToken* id();
                MNToken* number(bool c=false);
                MNToken* character();
                MNToken* hex(std::string current);
                int str_type(std::string lexeme);
                void _sub_fstring(std::string lexeme, unsigned int multi_state);
                std::string escape_seq(std::string lexeme);

            protected:
                std::stack<unsigned int> indent_values;
                unsigned int current_indent_value;
                unsigned int new_indent;
                bool check_indent;
                unsigned int dedent_count;
                unsigned int indent_check_count;
                unsigned int end_token;
                std::list<MNToken*> ret_tk_list;
                unsigned int f_string_level;
                bool error;
                bool cont_state;
        };
    }
}

#endif // MN_MEANLEX_H
