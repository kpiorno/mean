/*************************************************************************/
/*  mn_python.cpp                                                        */
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
#include "lexers/mn_python.h"
enum MN_PY_TOKENS
{
    HEX = MN_CUSTOM_TOKEN_INDEX,
    DAS,
    DBACKSLASH,
    BE,
    AE,
    NE
};

namespace mn
{
    namespace lexer
    {
        MN_Python::MN_Python(const std::string& file_name, MNErrorMsg* error_msg)
            :MNLexer(file_name, error_msg)
        {
            new_indent = 0;
            current_indent_value = 0;
            dedent_count = 0;
            check_indent = true;
            indent_check_count = 0;
            end_token = 0;
            register_custom_token("HEX", HEX);
            register_custom_token("DAS", DAS);
            register_custom_token("DBACKSLASH", DBACKSLASH);
            register_custom_token("BE", BE);
            register_custom_token("AE", AE);
            register_custom_token("NE", NE);
        }

        MNToken* MN_Python::next_token()
        {

            last_token_pos.row = row;
            last_token_pos.col = col;
            last_token_pos.c_count = tk_count;
            if (end_token == 1)
            {
                if (!indent_values.empty())
                {
                    if (last_token->get_type() != TK_EOL && last_token->get_lexeme() != "<DEDENT>")
                        return ret_token( new MNToken(TK_EOL, "EOL", row, col) );
                    indent_values.pop();
                    return ret_token( new MNToken(TK_ID, "<DEDENT>", row, col) );
                }
                return ret_token(  new MNToken(TK_END, "END", row, col) );
            }
            if (eof())
            {
                end_token = 1;
                if (indent_values.empty() )
                    return ret_token(  new MNToken(TK_EOL, "EOL", row, col) );
                else
                    return next_token();
            }

            if (!token_stack.empty())
            {
                //ret_status = false;
                auto ret = token_stack.top();
                token_stack.pop();
                return ret_token( ret );
            }
            if (dedent_count > 0)
            {
                   dedent_count--;
                   return ret_token( new MNToken(TK_ID, "<DEDENT>", row, col) );
            }

            unsigned int space_count=0;

            while (is_space(current_char) /*&& !eof()*/)
            {
                unsigned int c_row = row;
                unsigned int c_col = col;
                space_count++;

                if (eol() && current_char == '\n' && indent_check_count==0)
                {
                    check_indent = true;
                    next_char();
                    if (last_token && last_token->get_type() != TK_EOL)
                        return ret_token( new MNToken(TK_EOL, "EOL", c_row, c_col) );
                    else
                        return next_token();
                }
                if (eof())
                    return ret_token(  new MNToken(TK_END, "END", row, col) );

                next_char();
            }
            if (check_indent && indent_check_count == 0)
            {
                check_indent = false;
                if (space_count > current_indent_value)
                {
                     indent_values.push(current_indent_value);
                     current_indent_value = space_count;
                     return ret_token( new MNToken(TK_ID, "<INDENT>", row, col) );
                }

                if (space_count != current_indent_value)
                {
                    while (space_count != current_indent_value && !indent_values.empty())
                    {
                        current_indent_value = indent_values.top();
                        dedent_count++;
                        indent_values.pop();
                    }

                    if (!indent_values.empty() || (space_count == current_indent_value))
                    {
                        dedent_count--;
                        return ret_token( new MNToken(TK_ID, "<DEDENT>", row, col) );
                    }
                    else
                    {
                        return ret_token( new MNToken(TK_ID, "<INDENT_ERROR>", row, col) );
                    }

                }

            }

            switch (current_char) {
            case '\\':
                next_char();
                while (is_space(current_char) && !eof())
                {
                    if (eol() && current_char == '\n' )
                    {
                        check_indent = false;
                        next_char();
                        return next_token();
                    }
                    next_char();
                }
                next_char();
                return next_token();
                break;
            case '$':
                   next_char();
                   return ret_token (new MNToken(8080, "$", row, col));
                   break;
            case '*':
                next_char();
                if (current_char == '*')
                {
                    next_char();
                    return ret_token(new MNToken(DAS, "**", row, col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "*", row, col) );
                break;
            case '/':
                next_char();
                if (current_char == '/')
                {
                    next_char();
                    return ret_token(new MNToken(DBACKSLASH, "DBACKSLASH", row, col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "/", row, col) );
                break;
            case '<':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(BE, "BE", row, col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "<", row, col) );
                break;
            case '>':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(AE, "AE", row, col) );
                }
                else
                if (current_char == '>')
                {
                    next_char();
                    return ret_token(new MNToken(NE, "NE", row, col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, ">", row, col) );
                break;
            case '!':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(NE, "NE", row, col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "!", row, col) );
                break;
            case '"':
                return str();
                break;
            case '\'':
                return str('\'');
                break;
            default:
                if (is_letter(current_char))
                    return id();
                if (is_digit(current_char) || current_char == '.')
                    return number();
                break;
            }
            if (!eol() && !is_space(current_char) )
            {
                char c_c = current_char;
                next_char();
                switch (c_c) {
                case '{':
                case '[':
                case '(':
                    indent_check_count++;
                    break;
                case '}':
                case ']':
                case ')':
                    indent_check_count--;
                    break;
                default:
                    break;
                }
                std::string res = ""; res += c_c;
                return ret_token( new  MNToken(TK_CHARACTER, res, row, col) );
            }

            if (!indent_values.empty())
            {
                if (last_token->get_type() != TK_EOL && last_token->get_lexeme() != "<DEDENT>")
                    return ret_token( new MNToken(TK_EOL, "EOL", row, col) );
                indent_values.pop();
                return ret_token( new MNToken(TK_ID, "<DEDENT>", row, col) );
            }
            return ret_token(  new MNToken(TK_END, "END", row, col) );

        }

        std::string MN_Python::next_token_str()
        {
            MNToken* token = next_token();
            return MNToken::to_str(token->get_type());
        }


        MNToken* MN_Python::rule()
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
                    error_msg->add_error_text("PyLexer: ", "expected '>' "
                                                         "symbol at " + get_char_position() );
                }
                else
                if (current_char != '>')
                    lexeme += source_code->codepoint_to_string(current_char);
                else
                {
                    consume = false;
                }
                next_char();
            }
            return ret_token(  new MNToken(TK_RULE, lexeme, row, c_col) );
        }

        MNToken* MN_Python::str(char delimiter)
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
                    error_msg->add_error_text("PyLexer: ", "expected '" + res + "' "
                                                         "symbol at " + get_char_position() );
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

        MNToken* MN_Python::id()
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

        MNToken* MN_Python::number()
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
                            error_msg->add_error_text("PyLexer: ", "duplicated '.' at "
                                                                 + get_char_position() );
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
        MNToken* MN_Python::character()
        {
            int c_col = col;
            next_char();
            std::string lexeme = "";
            if (current_char != '\'')
               lexeme += source_code->codepoint_to_string(current_char);
            else
                error_msg->add_error_text("PyLexer: ", "expected a "
                                                     "caracter at " + get_char_position() );
            next_char();
            if (current_char != '\'')
                error_msg->add_error_text("PyLexer: ", "expected '\'' "
                                                     "symbol at " + get_char_position() );
            else
                next_char();

            return ret_token(  new MNToken(TK_CHARACTER, lexeme, row, c_col) );
        }

        MNToken* MN_Python::hex(std::string current)
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

