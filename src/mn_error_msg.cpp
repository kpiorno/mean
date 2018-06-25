/*************************************************************************/
/*  mn_error_msg.cpp                                                     */
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
#include "mn_error_msg.h"
#include <fstream>
namespace mn
{
    MNErrorMsg::MNErrorMsg(unsigned int max_err):max_err(max_err)
    {

    }

    void MNErrorMsg::add_error_text(const std::string &location, const std::string &body,
                                    std::string line, unsigned int /*row*/, int col)
    {
        msgs.push_back(location + body);
        if (!line.empty())
        {
            msgs.push_back(line);
            std::string err_sym = "";
            for (int i=0; i < col-1; ++i)
               err_sym += ' ';
            err_sym += '^';
            msgs.push_back(err_sym);

        }
    }

    std::vector<std::string>& MNErrorMsg::get_error_list()
    {
        return msgs;
    }

    bool MNErrorMsg::has_errors()
    {
        if (!msgs.empty())
            return true;
        return false;
    }

    void MNErrorMsg::std_out_errors(std::string filename)
    {
        if (filename.empty())
        {
            for (unsigned int i=0; i < (msgs.size() < max_err ? msgs.size(): max_err); ++i)
            {
                std::cout << msgs[i].c_str() << std::endl;
            }
        }
        else
        {
            std::ofstream out; out.open(filename);
            for (unsigned int i=0; i < (msgs.size() < max_err ? msgs.size(): max_err); ++i)
            {
                out << msgs[i].c_str() << std::endl;
            }
            out.close();

        }

    }
}

