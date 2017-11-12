/*************************************************************************/
/*  mn_mn_lexer.cpp                                                      */
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
#include "mn_mn_lexer.h"

namespace mn
{
    MN_MNLexer::MN_MNLexer(const std::string& file_name, MNErrorMsg* error_msg)
        :MNLexer(file_name, error_msg)
    {
        request_end = false;
    }

    MNToken* MN_MNLexer::next_token()
    {
        /*if (request_end)
            return ret_token(  new MNToken(TK_END, "END", row, col) );*/
        if (source_code->eof())
        {
            request_end = true;
            //return ret_token(  new MNToken(TK_EOL, "EOL", row, col) );
            return ret_token(  new MNToken(TK_END, "EOL", row, col) );
        }

        while (is_space(current_char) && !source_code->eof())
        {
            eol();
            next_char();
        }

        switch (current_char) {
        case '<':
            return rule();
            break;
        case '"':
            return str();
            break;
        case '\'':
            return character();
            break;
        case '|':
            next_char();
            return ret_token(  new MNToken(TK_OR, "|", row, col) );
            break;
        case '.':
            next_char();
            if (current_char == '.')
            {
                next_char();
                return ret_token(  new MNToken(TK_DDOTS, "..", row, col) );
            }
            else
                return ret_token(  new MNToken(TK_DOT, ".", row, col) );
            break;
        case '@':
            next_char();
            return ret_token(  new MNToken(TK_AT, "@", row, col) );
            break;
        case ':':
            next_char();
            return ret_token(  new MNToken(TK_COLON, ":", row, col) );
            break;
        case '+':
            next_char();
            return ret_token(  new MNToken(TK_PLUS, "+", row, col) );
            break;
        case '*':
            next_char();
            return ret_token(  new MNToken(TK_ASTERISK, "*", row, col) );
            break;
        case '(':
            next_char();
            return ret_token(  new MNToken(TK_PARENTHESIS_OPEN, "(", row, col) );
            break;
        case ')':
            next_char();
            return ret_token(  new MNToken(TK_PARENTHESIS_CLOSE, ")", row, col) );
            break;
        case '{':
            next_char();
            return ret_token(  new MNToken(TK_CURLY_BRACKET_OPEN, "{", row, col) );
            break;
        case '}':
            next_char();
            return ret_token(  new MNToken(TK_CURLY_BRACKET_CLOSE, "}", row, col) );
            break;
        case '[':
            next_char();
            return ret_token(  new MNToken(TK_SQUARE_BRACKET_OPEN, "[", row, col) );
            break;
        case ']':
            next_char();
            return ret_token(  new MNToken(TK_SQUARE_BRACKET_CLOSE, "]", row, col) );
            break;
        default:
            if (is_letter(current_char))
                return id();
            else
            {
                if (current_char != 10)
                {
                    std::string res = "";
                    res += current_char;
                    error_msg->add_error_text("Lexer: ", "character '"
                                              + res + "' not expected at "
                                              + get_char_position() );
                    next_char();
                }
            }
            break;
        }
        return ret_token(  new MNToken(TK_END, "END", row, col) );

    }

    std::string MN_MNLexer::next_token_str()
    {
        MNToken* token = next_token();
        return MNToken::to_str(token->get_type());
    }


    MNToken* MN_MNLexer::rule()
    {
        int c_col = col;
        next_char();
        bool consume = true;
        std::string lexeme = "";

        while (consume)
        {
            if (/*current_char == EOF ||*/ eol() || eof())
            {
                consume = false;
                error_msg->add_error_text("Lexer: ", "expected '>' "
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

    MNToken* MN_MNLexer::str()
    {
        int c_col = col;
        next_char();
        bool consume = true;
        std::string lexeme = "";
        while (consume)
        {
            if (/*current_char == EOF || */eol())
            {
                consume = false;
                error_msg->add_error_text("Lexer: ", "expected '\"' "
                                                     "symbol at " + get_char_position() );
            }
            else
            if (current_char != '"')
                lexeme += source_code->codepoint_to_string(current_char);
            else
            {
                consume = false;
            }
            next_char();
        }
        return ret_token(  new MNToken(TK_STRING, lexeme, row, c_col) );
    }

    MNToken* MN_MNLexer::id()
    {
        int c_col = col;
        bool consume = true;
        std::string lexeme = "";
        unsigned int type = TK_ID;
        if (!source_code->is_upper(current_char) && current_char != '_')
            type = TK_RULE;

        while (consume)
        {
            if (/*current_char == EOF || */is_space(current_char) || !is_alpha_numeric(current_char))
                consume = false;
            else if (is_letter(current_char) || is_digit(current_char)
                     || (current_char == '_' && !lexeme.empty()))
                lexeme += source_code->codepoint_to_string(current_char);
            else
                consume = false;
            if (consume)
                next_char();
        }
        if (type == TK_RULE && current_char == ':')
        {
            type = TK_RULE_DEC;
            next_char();
        }
        return ret_token(  new MNToken(type, lexeme, row, c_col) );
    }

    MNToken* MN_MNLexer::character()
    {
        int c_col = col;
        next_char();
        std::string lexeme = "";
        if (current_char != '\'')
           lexeme = source_code->codepoint_to_string(current_char);
        else
            error_msg->add_error_text("Lexer: ", "expected a "
                                                 "caracter at " + get_char_position() );
        next_char();
        if (current_char != '\'')
            error_msg->add_error_text("Lexer: ", "expected '\'' "
                                                 "symbol at " + get_char_position() );
        else
            next_char();

        return ret_token(  new MNToken(TK_CHARACTER, lexeme, row, c_col) );
    }


}

