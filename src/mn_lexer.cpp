/*************************************************************************/
/*  mn_lexer.cpp                                                         */
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
#include "mn_lexer.h"


namespace mn
{
    MNLexer::MNLexer(const std::string& file_name, MNErrorMsg* error_msg)
    {
        source_code = new UStream(file_name.c_str(), error_msg);
        //source_code.open(file_name.c_str());
        //std::cout << file_name.c_str() << std::endl;
        /*if (!source_code.is_open())
            error_msg->add_error_text("MN_Lexer: ", file_name + " not found");*/
        next_char();
        this->error_msg = error_msg;
        row = 1;
        col = 1;
        tk_count=0;
        last_token = nullptr;
        ret_status = false;

        last_token_pos.c_count = 0;
        last_token_pos.col = 1;
        last_token_pos.row = 1;

        current_consumed_str.clear();
        current_consumed_str.reserve(100);
    }

    bool MNLexer::eof()
    {
        return source_code->eof() && !(tk_count < current_consumed_str.size());
        //return current_char == 0 && !(tk_count < current_consumed_str.size());
    }

    std::string MNLexer::get_char_position()
    {
        return "row: " + to_string(row) + " col: " + to_string(col);
    }


    bool MNLexer::is_letter(int32_t c)
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')
                || c > 0xCD)
            return true;
        return false;
    }

    bool MNLexer::is_digit(int32_t c)
    {
        if (c >= '0' && c <= '9')
            return true;
        return false;
    }

    bool MNLexer::is_space(int32_t c)
    {
        if (c == ' ' || c == '\n' || c == '\r' || c == 0x0020)
            return true;
        return false;
    }

    bool MNLexer::is_alpha_numeric(int32_t c)
    {
        return is_digit(c) || is_letter(c) || c == '_';
    }

    bool MNLexer::eol()
    {
        if (current_char == '\n' || current_char == '\r' || int(current_char) == 10)
        {
            col = 0;
            ++row;
            return true;
        }
        return false;

    }

    void MNLexer::next_char()
    {
        if (!eof())
        {
            if (tk_count < current_consumed_str.size())
            {
                current_char = current_consumed_str[tk_count-1];
            }
            else
            {
                //source_code.get(current_char);
                current_char = source_code->next_char();
                current_consumed_str.push_back(current_char);
            }
            ++col;
            ++tk_count;

        }
    }

    void MNLexer::register_custom_token(std::string name, unsigned int type, bool compare_lexeme)
    {
       c_token reg; reg.type = type; reg.compare_lexeme = compare_lexeme; reg.exist_custom_token = true;
       custom_tokens[name] = reg;
       custom_tokens_int[type] = reg;
    }

    c_token MNLexer::get_custom_token(std::string name)
    {
         if (custom_tokens.find(name) != custom_tokens.end())
         {
             return custom_tokens[name];
         }
         c_token ret; ret.exist_custom_token = false;
         return ret;
    }

    c_token MNLexer::get_custom_token(unsigned int type)
    {
         if (custom_tokens_int.find(type) != custom_tokens_int.end())
         {
             return custom_tokens_int[type];
         }
         c_token ret; ret.exist_custom_token = false;
         return ret;
    }


    void MNLexer::push(MNToken* token)
    {
        token_stack.push(token);
        ret_status = true;
    }

    void MNLexer::pop()
    {
        token_stack.pop();
    }

    t_data MNLexer::current_token_info()
    {
        t_data info_current_token;
        info_current_token.col = last_token_pos.col;
        info_current_token.row = last_token_pos.row;
        info_current_token.c_count = last_token_pos.c_count;
        return info_current_token;
    }

    void MNLexer::set_current_token_info(t_data info)
    {

        row = info.row;
        col = info.col;
        tk_count = info.c_count;

    }

    bool MNLexer::is_max_token()
    {
        if (token_stack.size() >= tk_count)
            return true;
        else
            return false;
    }


    MNToken* MNLexer::ret_token(MNToken *token)
    {
        last_token = token;
        return token;
    }

    bool MNLexer::_at_range(int32_t uval, int32_t begin, int32_t end)
    {
        if (uval >= begin && uval <= end)
            return true;
        return false;
    }

    MNErrorMsg* MNLexer::get_error_msg()
    {
        return error_msg;
    }



}


/*|| (c == 0x00A8) || (c == 0x00AA) || (c == 0x00AD ) || (c == 0x00AF) || (_at_range(c, 0x00B2, 0x00B5))
|| (_at_range(c, 0x00B7, 0x00BA))
|| (_at_range(c, 0x00BC, 0x00BE))
|| (_at_range(c, 0x00C0,  0x00D6))
|| (_at_range(c, 0x00D8, 0x00F6))
|| (_at_range(c, 0x00F8, 0x167F))
|| (_at_range(c, 0x1681, 0x180D))
|| (_at_range(c, 0x180F, 0x1FFF))
|| (_at_range(c, 0x200B, 0x200D))
|| (_at_range(c, 0x202A, 0x202E))
|| (_at_range(c, 0x203F, 0x2040))
|| (_at_range(c, 0x203F, 0x2040))
|| (c == 0x2054)
|| (_at_range(c, 0x2060, 0x218F))
|| (_at_range(c, 0x2460, 0x24FF))
|| (_at_range(c, 0x2776, 0x2793))
|| (_at_range(c, 0x2C00, 0x2DFF))
|| (_at_range(c, 0x2E80, 0x2FFF))
|| (_at_range(c, 0x3004, 0x3007))
|| (_at_range(c, 0x3021, 0x302F))
|| (_at_range(c, 0xF900, 0xFD3D))
|| (_at_range(c, 0xFD40, 0xFDCF))
|| (_at_range(c, 0xFDF0, 0xFE44))
|| (_at_range(c, 0xFE47, 0xFFFD))
|| (_at_range(c, 0x10000, 0x1FFFD))
|| (_at_range(c, 0x20000, 0x2FFFD))
|| (_at_range(c, 0x30000, 0x3FFFD))
|| (_at_range(c, 0x40000, 0x4FFFD))
|| (_at_range(c, 0x50000, 0x5FFFD))
|| (_at_range(c, 0x60000, 0x6FFFD))
|| (_at_range(c, 0x70000, 0x7FFFD))
|| (_at_range(c, 0x80000, 0x8FFFD))
|| (_at_range(c, 0x90000, 0x9FFFD))
|| (_at_range(c, 0xA0000, 0xAFFFD))
|| (_at_range(c, 0xB0000, 0xBFFFD))
|| (_at_range(c, 0xC0000, 0xCFFFD))
|| (_at_range(c, 0xD0000, 0xDFFFD))
|| (_at_range(c, 0xE0000, 0xEFFFD))
|| std::isalpha(c)*/
