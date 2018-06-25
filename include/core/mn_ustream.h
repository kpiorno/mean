/*************************************************************************/
/*  ustream.h                                                            */
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
#ifndef MN_USTREAM
#define MN_USTREAM
#include <vector>
#include <iostream>
#include <fstream>
#include <mn_error_msg.h>
#include <mn_config.h>
#include <list>
namespace mn
{
    class UStream
    {
    public:
        UStream(std::string source, MNErrorMsg* error_msg, bool from_file);
        int32_t next_char();
        std::string codepoint_to_string(uint32_t v);
        bool eol();
        bool eof();
        bool is_upper(int32_t c);
        std::string get_line_str(unsigned int line);
    private:
        std::string value;
        std::ifstream source_code;
        std::string current_line;
        MNErrorMsg* error_msg;
        int _eof;
        char* current_line_char;
        int32_t current_char;
        bool from_file;
        unsigned int c_line_pos;
        std::vector<std::string> lines;
        std::string current_line_tmp;
        std::list<uint32_t> ret_char;
    private:
        void get_line();
    };
}
#endif // MN_USTREAM
