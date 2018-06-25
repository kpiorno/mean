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
#include "lexers/mn_meanlex.h"
enum MN_PY_TOKENS
{
    HEX = MN_CUSTOM_TOKEN_INDEX,
    DAS,
    DBACKSLASH,
    BE,
    AE,
    NE,
    ML_STR,
    TDOTS,
    DCOMP_EQ,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    PLUS_EQ,
    MINUS_EQ,
    MULT_EQ,
    MAT_EQ,
    DIV_EQ,
    MOD_EQ,
    AND_EQ,
    OR_EQ,
    XOR_EQ,
    SLEFT_EQ,
    SRIGHT_EQ,
    FDIV_EQ,
    DAS_EQ,
    TK_FLOAT,
    U_TK_STRING,
    B_TK_STRING,
    IMG,
    BIN,
    OCT,
    R_TK_STRING,
    EXP,
    F_TK_STRING,
    BR_TK_STRING,
    RB_TK_STRING,
    FS_INIT,
    FS_END,
    RDOT
};

namespace mn
{
    namespace lexer
    {
        MNMeanLex::MNMeanLex(const std::string& source, MNErrorMsg* error_msg, bool from_file)
            :MNLexer(source, error_msg, from_file)
        {
            new_indent = 0;
            current_indent_value = 0;
            dedent_count = 0;
            check_indent = true;
            indent_check_count = 0;
            end_token = 0;
            error = false;
            f_string_level = 0;
            cont_state = false;
            register_token("HEX", HEX);
            register_token("DAS", DAS);
            register_token("DBACKSLASH", DBACKSLASH);
            register_token("BE", BE);
            register_token("AE", AE);
            register_token("NE", NE);
            register_token("ML_STR", ML_STR);
            register_token("TDOTS", TDOTS);
            register_token("DCOMP_EQ", DCOMP_EQ);
            register_token("SHIFT_LEFT", SHIFT_LEFT);
            register_token("SHIFT_RIGHT", SHIFT_RIGHT);
            register_token("PLUS_EQ", PLUS_EQ);
            register_token("MINUS_EQ", MINUS_EQ);
            register_token("MULT_EQ", MULT_EQ);
            register_token("MAT_EQ", MAT_EQ);
            register_token("DIV_EQ", DIV_EQ);
            register_token("MOD_EQ", MOD_EQ);
            register_token("AND_EQ", AND_EQ);
            register_token("OR_EQ", OR_EQ);
            register_token("XOR_EQ", XOR_EQ);
            register_token("SLEFT_EQ", SLEFT_EQ);
            register_token("SRIGHT_EQ", SRIGHT_EQ);
            register_token("FDIV_EQ", FDIV_EQ);
            register_token("DAS_EQ", DAS_EQ);
            register_token("FLOAT", TK_FLOAT);
            register_token("U_STR", U_TK_STRING);
            register_token("B_STR", B_TK_STRING);
            register_token("IMG", IMG);
            register_token("BIN", BIN);
            register_token("OCT", OCT);
            register_token("R_STR", R_TK_STRING);
            register_token("EXP", EXP);
            register_token("F_STR", F_TK_STRING);
            register_token("BR_STR", BR_TK_STRING);
            register_token("RB_STR", RB_TK_STRING);
            register_token("FS_INIT", FS_INIT);
            register_token("FS_END", FS_END);
            register_token("RDOT", RDOT);
        }

        MNToken* MNMeanLex::next_token()
        {
            last_token_pos.row = row;
            last_token_pos.col = col;
            last_token_pos.c_count = tk_count;
            if (f_string_level == 0)
            {
                while (!ret_tk_list.empty()) {
                   auto ret = ret_tk_list.front();
                   ret_tk_list.pop_front();
                   return ret_token(ret);
                }
            }
            if (end_token == 1)
            {
                if (!indent_values.empty())
                {
                    if (error)
                        indent_values.pop();
                    if ((last_token->get_type() != TK_EOL && last_token->get_lexeme() != "<DEDENT>") || error)
                    {
                        error = false;
                        return ret_token( new MNToken(TK_EOL, "end of line", row, col) );
                    }
                    indent_values.pop();
                    return ret_token( new MNToken(TK_ID, "<DEDENT>", row, col) );
                }
                return ret_token(  new MNToken(TK_END, "END", row, col) );
            }
            if (eof())
            {
                end_token = 1;
                if (indent_values.empty() )
                    return ret_token(  new MNToken(TK_EOL, "end of line", row, col) );
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
                if (!error)
               {

                   return ret_token( new MNToken(TK_ID, "<DEDENT>", row, col) );
               }
               else
               {
                    error = false;
                    return ret_token( new MNToken(TK_EOL, "end of line", row, col) );
               }
            }

            unsigned int space_count=0;

            while (is_space(current_char) || current_char == '#' /*&& !eof()*/)
            {
                unsigned int c_row = row;
                unsigned int c_col = col;
                space_count++;
                if (current_char == 9)
                    space_count += 3;

                if (current_char == '#')
                {
                    while (!eol()){
                        next_char();
                    }
                    row--;
                }
                if (eol() && current_char == '\n' && indent_check_count == 0)
                {
                    check_indent = true;
                    next_char();
                    if (last_token && last_token->get_type() != TK_EOL)
                        return ret_token( new MNToken(TK_EOL, "end of line", c_row, c_col) );
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
                     error = false;
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
                        if (!error)
                        {

                            return ret_token( new MNToken(TK_ID, "<DEDENT>", row, col) );
                        }
                        else
                        {
                            error = false;
                            return ret_token( new MNToken(TK_EOL, "end of line", row, col) );
                        }
                    }
                    else
                    {
                        current_indent_value = 0;
                        return ret_token( new MNToken(TK_ID, "<INDENT_ERROR>", row, col) );
                    }

                }

            }

            unsigned int c_col = col;

            switch (current_char) {
            case '#':
                while (!eol()){
                    next_char();
                }
                next_char();
                return ret_token( next_token());
            case '\\':
                next_char();
//                while (is_space(current_char) && !eof())
//                {
//                    if (eol() && current_char == '\n' )
//                    {
//                        check_indent = false;
//                        next_char();
//                        return next_token();
//                    }
//                    next_char();
//                }
                if (current_char != '\n')
                    error_msg->add_error_text("PyLexer: ", "unexpected character after line continuation character at "
                                                         + get_char_position(), get_line(row), 0, col );
                else
                    eol();
                next_char();
                cont_state = true;
                return next_token();
                break;
            case '*':
                next_char();
                if (current_char == '*')
                {
                    next_char();
                    if (current_char == '=')
                    {
                        next_char();
                        return ret_token(new MNToken(DAS_EQ, "**=", row, col) );
                    }
                    else
                        return ret_token(new MNToken(DAS, "**", row, c_col) );
                }
                else if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(MULT_EQ, "*=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "*", row, c_col) );
                break;
            case '/':
                next_char();
                if (current_char == '/')
                {
                    next_char();
                    if (current_char == '=')
                    {
                        next_char();
                        return ret_token(new MNToken(FDIV_EQ, "//=", row, c_col) );
                    }
                    else
                        return ret_token(new MNToken(DBACKSLASH, "DBACKSLASH", row, c_col) );
                }
                else if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(DIV_EQ, "/=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "/", row, c_col) );
                break;
            case '<':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(BE, "BE", row, c_col) );
                }
                if (current_char == '<')
                {
                    next_char();
                    if (current_char == '=')
                    {
                        next_char();
                        return ret_token(new MNToken(SLEFT_EQ, "<<=", row, c_col) );
                    }
                    else
                        return ret_token(new MNToken(SHIFT_RIGHT, "<<", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "<", row, c_col) );
                break;
            case '.':
                {
                    //unsigned int ccol = c_col;
                    next_char();
                    if (current_char == '.')
                    {
                        //ccol = col;
                        next_char();
                        if (current_char == '.')
                        {
                            next_char();
                            return ret_token(new MNToken(TDOTS, "...", row, c_col) );
                        }
                        else
                        {
                            return ret_token(new MNToken(RDOT, "..", row, c_col) );
                        }
                    }
                    else if (is_digit(current_char))
                    {
                        return number();
                    }
                }
                return ret_token(new MNToken(TK_CHARACTER, ".", row, c_col) );
                break;
            case '+':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(PLUS_EQ, "+=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "+", row, c_col) );
                break;
            case '-':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(MINUS_EQ, "-=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "-", row, c_col) );
                break;
            case '@':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(MAT_EQ, "@=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "@", row, c_col) );
                break;
            case '%':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(MOD_EQ, "%=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "%", row, c_col) );
                break;
            case '&':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(AND_EQ, "&=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "&", row, c_col) );
                break;
            case '|':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(OR_EQ, "|=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "|", row, c_col) );
                break;
            case '^':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(XOR_EQ, "^=", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "^", row, c_col) );
                break;
            case '>':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(AE, "AE", row, c_col) );
                }
                else
                if (current_char == '>')
                {
                    next_char();
                    if (current_char == '=')
                    {
                        next_char();
                        return ret_token(new MNToken(SRIGHT_EQ, ">>=", row, c_col) );
                    }
                    else
                        return ret_token(new MNToken(SHIFT_RIGHT, ">>", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, ">", row, c_col) );
                break;
            case '=':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(DCOMP_EQ, "==", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "=", row, c_col) );
                break;
            case '!':
                next_char();
                if (current_char == '=')
                {
                    next_char();
                    return ret_token(new MNToken(NE, "NE", row, c_col) );
                }
                return ret_token(new MNToken(TK_CHARACTER, "!", row, c_col) );
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
                    if (!error)
                        indent_check_count++;
                    break;
                case '}':
                case ']':
                case ')':
                    indent_check_count = indent_check_count > 0 ? indent_check_count-1 : 0;
                    break;
                default:
                    break;
                }
                std::string res = ""; res += c_c;
                return ret_token( new  MNToken(TK_CHARACTER, res, row, c_col) );
            }

            if (!indent_values.empty())
            {
                if (error)
                    indent_values.pop();
                if ((last_token->get_type() != TK_EOL && last_token->get_lexeme() != "<DEDENT>") || error)
                {
                    error = false;
                    return ret_token( new MNToken(TK_EOL, "end of line", row, c_col) );
                }
                indent_values.pop();
                return ret_token( new MNToken(TK_ID, "<DEDENT>", row, c_col) );
            }
            return ret_token(  new MNToken(TK_END, "END", row, c_col) );

        }

        std::string MNMeanLex::next_token_str()
        {
            MNToken* token = next_token();
            return MNToken::to_str(token->get_type());
        }

        bool MNMeanLex::clean_token(MNToken *token)
        {
            std::string lex = token->get_lexeme();
            if (token->get_col() == 1
                && lex != "<DEDENT>"
                && lex != "elif"
                && lex != "else"
                && lex != "except"
                && token->get_type() != TK_EOL
                && indent_check_count == 0  )
            {
                if (!cont_state)
                    return true;
                else
                    cont_state = false;
                return true;
            }
            return false;
        }

        bool MNMeanLex::recuperate_token(MNToken *token)
        {
            if (indent_check_count == 0 && token->get_type() == TK_EOL)
            {
                indent_check_count = 0;
                return true;
            }
            return false;
        }

        inline void MNMeanLex::notify_error(mn::MNNode * /*node*/)
        {
            error = true;
            indent_check_count = 0;
        }

        MNToken* MNMeanLex::rule()
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
                                                         "symbol at " + get_char_position(),
                                                         get_line(row), 0, col);
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

        MNToken* MNMeanLex::str(char delimiter, std::string begin)
        {
            int c_col = col;
            next_char();
            unsigned int multi_state = 0;
            if (delimiter == current_char)
            {
                multi_state = 1;
            }
            bool consume = true;
            std::string lexeme = "";
            std::string del_str = "";
            unsigned int type = TK_STRING;

            if (begin == "U" || begin == "u")
                type = U_TK_STRING;
            else if (begin == "B" || begin == "b")
                type = B_TK_STRING;
            else if (begin == "R" || begin == "r")
                type = R_TK_STRING;
            else if (begin == "f" || begin == "F")
            {
                type = F_TK_STRING;
                ret_tk_list.push_back(new MNToken(FS_INIT, "init of fstring", row, col));
            }
            else if (str_type(begin) == 1)
                type = RB_TK_STRING;
            else if (str_type(begin) == 2)
                type = BR_TK_STRING;

            unsigned int c_fstring_level = f_string_level;
            f_string_level += 1;

            while (consume)
            {
                if (current_char == '\\')
                {
                   lexeme = escape_seq(lexeme);
                   continue;
                }
                if (multi_state == 1 && current_char == delimiter)
                {
                    multi_state = 2;
                    del_str += delimiter;
                    next_char();
                    if (multi_state == 2 && current_char == delimiter)
                    {
                        del_str += delimiter;
                        multi_state = 3;
                        next_char();
                    }
                    else {
                        break;
                        multi_state = 0;
                    }
                }
                else
                {
                    if (multi_state != 3)
                        multi_state = 0;
                }
                if (type == F_TK_STRING)
                {
                    if (current_char == '{')
                    {
                        next_char();
                        if (current_char != '{')
                        {
                            _sub_fstring(lexeme, multi_state);
                            lexeme = "";
                        }
                    }
                    if (current_char == '\\')
                    {
                       lexeme = escape_seq(lexeme);
                    }

                    //_sub_fstring(lexeme, multi_state);
                    if (current_char == '}')
                    {
                        next_char();
                        if (current_char == '}')
                             lexeme += source_code->codepoint_to_string(current_char);
                        else
                        {
                            //return ret_token(new MNToken(TK_CHARACTER, "}", row, col));
                            ret_tk_list.push_back(new MNToken(TK_CHARACTER, "}", row, col));
                        }
                    }
                }
                if ((eol() && multi_state == 0) || (eof() && multi_state == 3))
                {
                    consume = false;
                    std::string res = (delimiter == '"' ? (multi_state == 0 ? "\"": "\"\"\"")
                                                        : (multi_state == 0 ? "'": "\"\"\""));
                    error_msg->add_error_text("PyLexer: ", "expected '" + res + "' "
                                                         "symbol at " + get_char_position(),
                                                         get_line(row), 0, col);
                }
                else
                if (current_char != delimiter)
                {
                    if (current_char != 10)
                        lexeme += source_code->codepoint_to_string(current_char);
                    else
                        lexeme += "\\n";
                }
                else
                {
                    std::string c_v = "";
                    if (multi_state == 3)
                    {
                        unsigned int c_s = 0;
                        c_v += delimiter;
                        next_char();
                        eol();
                        for (unsigned int i=0; i < 2; ++i)
                        {
                            if (current_char == delimiter)
                            {
                                c_s++;
                                c_v += delimiter;
                                if (i == 0 )
                                {
                                    next_char();
                                    eol();
                                }
                            }
                        }
                        if (c_s == 2)
                        {
                            consume = false;
                        }
                        else
                        {
                            c_v += source_code->codepoint_to_string(current_char);
                            lexeme += c_v;
                        }
                    }
                    else
                    {
                        consume = false;
                    }
                }
                next_char();
            }

            f_string_level = c_fstring_level;

            if (type != F_TK_STRING)
                return ret_token(  new MNToken(type, lexeme, row, c_col) );
            else
            {
                if (!lexeme.empty())
                {
                    ret_tk_list.push_back(new MNToken(TK_STRING, lexeme, row, col));
                }
                ret_tk_list.push_back(new MNToken(FS_END, "end of fstring", row, col));
                return next_token();
            }
        }

        MNToken* MNMeanLex::id()
        {
            int c_col = col;
            bool consume = true;
            std::string lexeme = "";

            while (consume)
            {
                if (current_char == '"' || current_char == '\'')
                {
                    if (lexeme == "b" || lexeme == "u" || lexeme == "B"
                            || lexeme == "U" || lexeme == "R" || lexeme == "r"
                            || lexeme == "f" || lexeme == "F" || str_type(lexeme))
                        return str(current_char, lexeme);
                }
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

        MNToken* MNMeanLex::number(bool c)
        {
            int c_col = col;
            bool consume = true;
            std::string lexeme = c ? "0." : "";
            bool comma = c;
            bool binary = false;
            bool oct = false;
            bool allow_underscore = false;

            while (consume)
            {
                bool consumed_underscore = false;

                if ((current_char == 'e' || current_char == 'E') && lexeme.size() > 0 && !binary && !oct)
                {
                    lexeme += 'e';
                    allow_underscore = true;
                    next_char();
                    if (current_char == '-' || current_char == '+')
                    {
                        lexeme += source_code->codepoint_to_string(current_char);
                        next_char();

                    }
                    while (is_digit(current_char) || current_char == '_') {
                        if (allow_underscore && current_char == '_')
                        {
                            consumed_underscore = true;
                            allow_underscore = false;
                            next_char();
                        }
                        if ((!allow_underscore && current_char == '_')
                                || !(is_digit(current_char) || current_char == '_') )
                            return ret_token(new MNToken(TK_CHARACTER, "_", row, col) );
                        lexeme += source_code->codepoint_to_string(current_char);
                        allow_underscore = true;

                        next_char();
                    }
                    consume = false;
                    return ret_token(new MNToken(EXP, lexeme, row, c_col) );


                }
                if (lexeme == "0" && (current_char == 'b' || current_char == 'B' ))
                {
                    next_char();
                    lexeme += 'b';
                    allow_underscore = true;
                    //comma = true;
                    binary = true;
                }
                if (lexeme == "0" && (current_char == 'o' || current_char == 'O' ))
                {
                    next_char();
                    lexeme += 'o';
                    allow_underscore = true;
                    //comma = true;
                    oct = true;
                }
                if (allow_underscore && current_char == '_')
                {
                    consumed_underscore = true;
                    next_char();
                }
                //allow_underscore = true;
                if (binary)
                {

                    if (current_char == '0' || current_char == '1')
                    {
                        allow_underscore = true;
                        lexeme += source_code->codepoint_to_string(current_char);
                        next_char();
                    }
                    else
                    {
                       consume = false;
                    }
                }
                else if (oct)
                {
                    if (current_char >= '0' && current_char <= '8')
                    {
                        allow_underscore = true;
                        lexeme += source_code->codepoint_to_string(current_char);
                        next_char();
                    }
                    else
                    {
                       consume = false;
                    }
                }
                else if ((current_char == 'x' || current_char == 'X') && lexeme == "0")
                {
                    next_char();
                    return hex(lexeme + "x");
                }
                if ((is_space(current_char) || !(is_digit(current_char) || current_char == '.')) ||  eof())
                {
                    if (lexeme == ".")
                        return ret_token(new MNToken(TK_CHARACTER, ".", row, col) );
                    if (consumed_underscore)
                        return ret_token(new MNToken(TK_CHARACTER, "_", row, col) );
                    consume = false;
                }
                else if (is_digit(current_char) || current_char == '.')
                {
                    allow_underscore = true;
                    if (current_char == '.')
                    {
                        if (!comma)
                        {
                            comma = true;
                            allow_underscore = false;
                        }
                        else
                        {
                            error_msg->add_error_text("PyLexer: ", "unexpected '.' at "
                                                                 + get_char_position(),
                                                                   get_line(row), 0, col);
                        }
                    }
                    lexeme += source_code->codepoint_to_string(current_char);
                }
                else
                {
                    if (allow_underscore)
                        consume = false;

                }
                if (consume)
                    next_char();

            }
            if (binary)
                return ret_token(  new MNToken(BIN, lexeme, row, c_col) );
            if (oct)
                return ret_token(  new MNToken(OCT, lexeme, row, c_col) );
            if (current_char == 'j')
            {
                next_char();
                return ret_token(  new MNToken(IMG, lexeme, row, c_col) );
            }
            else if (comma)
                return ret_token(  new MNToken(TK_FLOAT, lexeme, row, c_col) );
            return ret_token(  new MNToken(TK_NUMBER, lexeme, row, c_col) );
        }
        MNToken* MNMeanLex::character()
        {
            int c_col = col;
            next_char();
            std::string lexeme = "";
            if (current_char != '\'')
               lexeme += source_code->codepoint_to_string(current_char);
            else
                error_msg->add_error_text("PyLexer: ", "expected a "
                                                     "caracter at " + get_char_position(),
                                                     get_line(row), 0, col);
            next_char();
            if (current_char != '\'')
                error_msg->add_error_text("PyLexer: ", "expected '\'' "
                                                     "symbol at " + get_char_position(),
                                                     get_line(row), 0, col);
            else
                next_char();

            return ret_token(  new MNToken(TK_CHARACTER, lexeme, row, c_col) );
        }

        MNToken* MNMeanLex::hex(std::string current)
        {
            int c_col = col;
            bool consume = true;
            std::string lexeme = current;
            bool allow_underscore = false;

            while (consume)
            {

                bool consumed_underscore = false;
                allow_underscore = true;
                if (allow_underscore && current_char == '_')
                {
                    consumed_underscore = true;
                    next_char();
                }

                bool hex_letter = ((current_char >= 'a' && current_char <= 'f')
                                   || (current_char >= 'A' && current_char <= 'F') );
                if (is_space(current_char) || !(is_digit(current_char)
                        || hex_letter) ||  eof())
                {
                    if (consumed_underscore)
                        return ret_token(new MNToken(TK_CHARACTER, "_", row, c_col) );
                    consume = false;
                }
                else if (is_digit(current_char) || hex_letter)
                {
                    lexeme += source_code->codepoint_to_string(current_char);
                }
                else
                {
                    if (consumed_underscore)
                        return ret_token(new MNToken(TK_CHARACTER, "_", row, c_col) );
                    consume = false;
                }
                if (consume)
                    next_char();

            }

            return ret_token(  new MNToken(HEX, lexeme, row, c_col) );
        }

        int MNMeanLex::str_type(std::string lexeme)
        {
            if (lexeme.size() == 2)
            {
                if ((lexeme[0] == 'r' || lexeme[0] == 'R') && (lexeme[1] == 'b' || lexeme[1] == 'R'))
                    return 1;
                if ((lexeme[0] == 'b' || lexeme[0] == 'B') && (lexeme[1] == 'r' || lexeme[1] == 'R'))
                    return 2;
            }
            return 0;
        }

        void MNMeanLex::_sub_fstring(std::string lex, unsigned int multi_state)
        {
            std::string lexeme=lex;

            auto c_token = new MNToken(TK_CHARACTER, "{", row, col);
           // next_char();
            if (!lexeme.empty())
            {
                ret_tk_list.push_back(new MNToken(TK_STRING, lexeme, row, col));
                lexeme = "";
            }
            ret_tk_list.push_back(c_token);
            unsigned int c_indent_check_count = indent_check_count;
            indent_check_count += 1;


            while (!((eol() && multi_state == 0) || (eof() && multi_state == 3) || c_token->get_type() == TK_END))
            {
                c_token = next_token();

                if (c_token->get_lexeme() == ":"
                        && c_token->get_type() == TK_CHARACTER
                        && c_indent_check_count+1 == indent_check_count)
                {
                    ret_tk_list.push_back(c_token);
                    while (!((eol() && multi_state == 0) || (eof() && multi_state == 3) || c_token->get_type() == TK_END))
                    {
                        if (current_char == '\\')
                        {
                           lexeme = escape_seq(lexeme);
                        }
                        if (current_char == '{')
                        {
                            next_char();
                            if (!lexeme.empty())
                            {
                                ret_tk_list.push_back(new MNToken(TK_STRING, lexeme, row, col));
                                lexeme = "";
                            }
                            _sub_fstring(lexeme, multi_state);
                        }
                        else
                        {
                            if (current_char == '}')
                            {
                               break;
                            }
                            lexeme += source_code->codepoint_to_string(current_char);
                            next_char();
                        }

                        if (current_char == '}'
                                && c_indent_check_count+1 == indent_check_count)
                        {
                            break;
                        }
                    }

                }
                else if (c_token->get_lexeme() == "}"
                        && c_token->get_type() == TK_CHARACTER)
                        //&& c_indent_check_count == indent_check_count)
                {
                    ret_tk_list.push_back(c_token);
                    break;
                }
                else
                {
                    ret_tk_list.push_back(c_token);
                }
            }
            indent_check_count = c_indent_check_count;
        }
        std::string MNMeanLex::escape_seq(std::string lexeme)
        {
            std::string c_lex = lexeme;
            next_char();
            if (current_char == '\'' || current_char == '"' || current_char == 'n' || current_char == 't')
            {
                c_lex += current_char;
                next_char();
            }
            else
            {
                c_lex += "\\\\";
                c_lex += source_code->codepoint_to_string(current_char);
                next_char();
            }
            return c_lex;

        }
    }
}

