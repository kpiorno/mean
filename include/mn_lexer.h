/*************************************************************************/
/*  mn_lexer.h                                                           */
/*************************************************************************/
/* Copyright (c) 2017 Karel Piorno Charchabal   (kpiorno@gmail.com)      */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef MNLEXER_H
#define MNLEXER_H
#include <iostream>
#include <fstream>
#include <stack>
#include <mn_config.h>
#include <unordered_map>
#include <mn_token.h>
#include <mn_error_msg.h>
#include <core/mn_ustream.h>

struct t_data
{
    int row, col;
    int c_count;
};

struct c_token
{
    unsigned int type;
    bool compare_lexeme;
    bool exist_custom_token;

};

struct URange
{
    int32_t begin;
    int32_t end;
};

namespace mn
{
    class MNLexer
    {
        public:
            MNLexer(const std::string& file_name, MNErrorMsg* error_msg);
            virtual ~MNLexer() {delete source_code;}
            virtual MNToken* next_token()=0;
            bool eof();
            void push(MNToken* token);
            void pop();
            t_data current_token_info();
            void set_current_token_info(t_data info);
            bool is_max_token();
            void next_char();
            void register_custom_token(std::string name, unsigned int type, bool compare_lexeme=false);
            c_token get_custom_token(std::string name);
            c_token get_custom_token(unsigned int type);
            MNErrorMsg* get_error_msg();

        protected:
            void register_reserverd_word(const std::string& name);
            std::string get_char_position();
            bool is_letter(int32_t c);
            bool is_digit(int32_t c);
            bool is_space(int32_t c);
            bool is_alpha_numeric(int32_t c);
            bool eol();
            MNToken* ret_token(MNToken* token);
            bool _at_range(int32_t uval, int32_t begin, int32_t end);

        protected:
            int32_t current_char;
            UStream* source_code;
            MNErrorMsg* error_msg;
            int row, col;
            unsigned int tk_count;
            std::stack<MNToken*> token_stack;
            MNToken* last_token;
            bool ret_status;
            std::vector<int32_t> current_consumed_str;
            std::unordered_map<std::string, c_token> custom_tokens;
            std::unordered_map<unsigned int, c_token> custom_tokens_int;

            t_data last_token_pos;
    };
}

#endif // MNLEXER_H
