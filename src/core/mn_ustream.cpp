/*************************************************************************/
/*  ustream.cpp                                                        */
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
#include "core/mn_ustream.h"
#include <utf8/utf8.h>
#include <locale>

namespace mn
{
    UStream::UStream(std::string file_name, MNErrorMsg* error_msg):error_msg(error_msg)
    {
        _eof = 0;
        value = file_name;
        source_code.open(file_name.c_str());
        //std::cout << file_name.c_str() << std::endl;
        if (!source_code.is_open())
            error_msg->add_error_text("UStream: ", file_name + " not found");
        get_line();
        //next_char();
    }

    bool UStream::eof()
    {
        return _eof == 2;
        //return _eof;
    }

    bool UStream::eol()
    {
        return current_char == '\n';
    }

    bool UStream::is_upper(int32_t c)
    {
        if (c >= 'A' && c <= 'Z')
            return true;
        return false;
    }

    int32_t UStream::next_char()
    {
        current_char = utf8::next(current_line_char, current_line_char+9);
        if (current_char == 0 || current_char == '\n')
        {
            get_line();
            if (current_char == 0 && _eof == 1)
            {
                _eof = 2;
            }
            current_char = '\n';
        }
        return current_char;
    }

    std::string UStream::codepoint_to_string(uint32_t v)
    {
       unsigned char u[5] = {0,0,0,0,0};
       /*unsigned char* end = */utf8::unchecked::append(v, u);
       return std::string((char*)u);
    }

    void UStream::get_line()
    {
        if (source_code.eof())
            _eof = 1;
        current_line = "";
        std::getline(source_code, current_line);
        current_line_char = (char*)current_line.c_str();
    }

}

