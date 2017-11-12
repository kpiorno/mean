/*************************************************************************/
/*  mn_token.cpp                                                         */
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
#include "mn_token.h"

namespace mn
{
    const char* MNToken::token_str[TK_MAX] =
    {
        "id",
        "str",
        "=",
        "<",
        ">",
        "rule",
        "rule_dec",
        "(",
        ")",
        "[",
        "]",
        "{",
        "}",
        "|",
        "*",
        "+",
        "'",
        "\"",
        ".",
        "Character",
        "EOL",
        "INDENT",
        "DEDENT",
        ":",
        ",",
        "@",
        "Number",
        "END",
    };

    MNToken::MNToken(const unsigned int type, const std::string lexeme, const unsigned int row,
                     const unsigned int col)
    {
        this->type = type;
        this->lexeme = lexeme;
        this->row = row;
        this->col = col;
    }

    unsigned int MNToken::get_type() const
    {
        return type;
    }

    std::string MNToken::get_lexeme()
    {
        return lexeme;
    }

    std::string MNToken::get_char_position()
    {
        return "row: " + to_string(row) + " col: " + to_string(col);
    }

    unsigned int  MNToken::get_row()
    {
        return row;
    }

    unsigned int  MNToken::get_col()
    {
        return col;
    }

    std::string MNToken::to_str(unsigned int type)
    {
        std::string result = token_str[type];
        return result;
    }
}

