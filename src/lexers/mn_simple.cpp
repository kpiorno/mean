/*************************************************************************/
/*  MN_Simple.cpp                                                        */
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
#include "lexers/mn_simple.h"
enum MN_PY_TOKENS
{
    HEX = MN_CUSTOM_TOKEN_INDEX,
};

namespace mn
{
    namespace lexer
    {
        MN_Simple::MN_Simple(const std::string& source, MNErrorMsg* error_msg, bool from_file)
            :MNLexer(source, error_msg, from_file)
        {
            register_token("HEX", HEX);
        }

        MNToken* MN_Simple::next_token()
        {
            if (source_code->eof())
            {
                return ret_token(  new MNToken(TK_END, "EOL", row, col) );
            }

            while (is_space(current_char) && !source_code->eof())
            {
                eol();
                next_char();
            }

            switch (current_char) {
             case '+':
             case '-':
             case '*':
             case '(':
             case ')':
             case '^':
                {
                    std::string ret = ""; ret += current_char;
                    next_char();
                    return ret_token(  new MNToken(TK_CHARACTER, ret, row, col) );
                }
                break;
            default:
                if (is_letter(current_char))
                    return id();
                if (is_digit(current_char) || current_char == '.')
                    return number();
                break;
            }
            return ret_token(  new MNToken(TK_END, "END", row, col) );



        }

        MNToken* MN_Simple::str(char delimiter)
        {
            int c_col = col;
            next_char();
            bool consume = true;
            std::string lexeme = "";
            while (consume)
            {
                if (eol())
                {
                    consume = false;
                    std::string res = (delimiter == '"' ? "\"" : "'");
                    error_msg->add_error_text("Simple_Lexer: ", "expected '" + res + "' "
                                                         "symbol at " + get_char_position(),
                                                          get_line(row), 0, col);
                }
                else
                if (current_char != delimiter)
                    lexeme += source_code->codepoint_to_string(current_char);
                else
                {
                    consume = false;
                }
                next_char();
            }
            return ret_token(  new MNToken(TK_STRING, lexeme, row, c_col) );
        }

        MNToken* MN_Simple::id()
        {
            int c_col = col;
            bool consume = true;
            std::string lexeme = "";

            while (consume)
            {
                if (is_space(current_char)
                        || !is_alpha_numeric(current_char))
                    consume = false;
                else if (is_letter(current_char) || is_digit(current_char)
                         || (current_char == '_' && !lexeme.empty()))
                    lexeme += source_code->codepoint_to_string(current_char);
                else
                    consume = false;
                if (consume)
                    next_char();
            }
            return ret_token(  new MNToken(TK_ID, lexeme, row, c_col) );
        }

        MNToken* MN_Simple::number()
        {
            int c_col = col;
            bool consume = true;
            std::string lexeme = "";
            bool comma = false;

            while (consume)
            {
                if ((current_char == 'x' || current_char == 'X') && lexeme == "0")
                {
                    next_char();
                    return hex(lexeme + "x");
                }
                if (is_space(current_char) || !(is_digit(current_char) || current_char == '.') ||  eof())
                {
                    if (lexeme == ".")
                        return ret_token(new MNToken(TK_CHARACTER, ".", row, c_col) );
                    consume = false;
                }
                else if (is_digit(current_char) || current_char == '.')
                {
                    if (current_char == '.')
                    {
                        if (!comma)
                        {
                            comma = true;
                        }
                        else
                        {
                            error_msg->add_error_text("Simple_Lexer: ", "duplicated '.' at "
                                                                 + get_char_position(),
                                                                   get_line(row), 0, col);
                        }
                    }
                    lexeme += source_code->codepoint_to_string(current_char);
                }
                else
                    consume = false;
                if (consume)
                    next_char();

            }
            return ret_token(  new MNToken(TK_NUMBER, lexeme, row, c_col) );
        }
        MNToken* MN_Simple::character()
        {
            int c_col = col;
            next_char();
            std::string lexeme = "";
            if (current_char != '\'')
               lexeme += source_code->codepoint_to_string(current_char);
            else
                error_msg->add_error_text("Simple_Lexer: ", "expected a "
                                                     "caracter at " + get_char_position(),
                                                     get_line(row), 0, col);
            next_char();
            if (current_char != '\'')
                error_msg->add_error_text("Simple_Lexer: ", "expected '\'' "
                                                     "symbol at " + get_char_position(),
                                                     get_line(row), 0, col);
            else
                next_char();

            return ret_token(  new MNToken(TK_CHARACTER, lexeme, row, c_col) );
        }

        MNToken* MN_Simple::hex(std::string current)
        {
            int c_col = col;
            bool consume = true;
            std::string lexeme = current;

            while (consume)
            {
                bool hex_letter = ((current_char >= 'a' && current_char <= 'f')
                                   || (current_char >= 'A' && current_char <= 'F') );
                if (is_space(current_char) || !(is_digit(current_char)
                        || hex_letter) ||  eof())
                {
                    consume = false;
                }
                else if (is_digit(current_char) || hex_letter)
                {
                    lexeme += source_code->codepoint_to_string(current_char);
                }
                else
                    consume = false;
                if (consume)
                    next_char();

            }

            return ret_token(  new MNToken(HEX, lexeme, row, c_col) );
        }


    }
}

