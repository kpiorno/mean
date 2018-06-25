/*************************************************************************/
/*  mn_ustream.cpp                                                       */
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
    UStream::UStream(std::string source, MNErrorMsg* error_msg, bool from_file):error_msg(error_msg)
    {
        _eof = 0;
        c_line_pos = 0;
        this->from_file = from_file;
        value = source;
        lines.clear();
        current_line_tmp = "";
        if (from_file)
        {
            source_code.open(source.c_str());
            //std::cout << file_name.c_str() << std::endl;
            if (!source_code.is_open())
                error_msg->add_error_text("UStream: ", source + " not found","", 0, 0);
            get_line();
        }
        if (!from_file)
            current_line_char = (char*)value.c_str();
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
        int result=0;
        if (!from_file)
        {
            while (!ret_char.empty()) {
               int32_t r = ret_char.front();
               ret_char.pop_front();
               return r;
            }
        }
        current_char = utf8::next_sec(current_line_char, current_line_char+9, result);
        if (current_char == 0 || current_char == '\n' || current_char == '\r' || int(current_char) == 10
                || result)
        {
            if (from_file)
            {
                get_line();
            }
            if ((current_char == 0 && (_eof == 1 || !from_file)) || result)
            {
                _eof = 2;
            }
            if (!from_file)
            {
                lines.push_back(current_line_tmp);
                current_line_tmp = "";
            }
            current_char = '\n';
        }
        if (result)
            current_char = ' ';
        if (current_char != '\n')
            current_line_tmp += codepoint_to_string(current_char);
        return current_char;
    }

    std::string UStream::codepoint_to_string(uint32_t v)
    {
       unsigned char u[6] = {0,0,0,0,0,0};
       /*unsigned char* end = */utf8::unchecked::append(v, u);
       return std::string((char*)u);
    }

    void UStream::get_line()
    {
        current_line = "";
        if (from_file)
        {
            if (source_code.eof())
                _eof = 1;
            std::getline(source_code, current_line);

            current_line_char = (char*)current_line.c_str();
        }
    }

    std::string UStream::get_line_str(unsigned int line)
    {
        if (from_file)
        {
            std::ifstream source;
            source.open(value.c_str());
            std::string t_line = "";
            bool eof_enc = false;
            for (unsigned int i=0; i < line; ++i)
            {
                if (source.eof())
                {
                    eof_enc = true;
                    break;
                }
                std::getline(source, t_line);
            }
            std::string ret_line = "";

            if (!eof_enc)
            {
                char* current_line_char = (char*)t_line.c_str();

                int32_t current_char = 0;
                for (;;)
                {
                    current_char = utf8::next(current_line_char, current_line_char+9);
                    if (current_char == 0 || current_char == '\n' || current_char == '\r' || int(current_char) == 10)
                       break;
                    ret_line += codepoint_to_string(current_char);
                }
            }
            return ret_line;
        }
        else
        {
            if (line-1 >= lines.size())
            {
                std::string ret_line = "";
                if (_eof == 2)
                {
                    return current_line_tmp;
                }
                int result=0;
                for (;;)
                {
                    current_char = utf8::next_sec(current_line_char, current_line_char+9, result);
                    if (current_char == 0 && result)
                    {
                        current_char = '\n';
                    }
                    if (!result)
                        ret_char.push_back(current_char);
                    else
                        ret_char.push_back(' ');
                    if (current_char == 0 || current_char == '\n' || current_char == '\r' || int(current_char) == 10 || result)
                       break;
                    if(result)
                        current_char = ' ';
                    ret_line += codepoint_to_string(current_char);
                }
                if (result)
                    _eof = 2;
                lines.push_back(current_line_tmp + ret_line);
                ret_line = current_line_tmp + ret_line;
                current_line_tmp = "";
                return ret_line;
            }
            else if (line-1 < lines.size())
            {
                return lines[line-1];
            }
        }
        return "";
    }

}

