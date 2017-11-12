/*************************************************************************/
/*  mn_config.h                                                            */
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
#ifndef MNCONFIG_H
#define MNCONFIG_H

#include <fstream>
#define MN_VECTOR_NAME "[.]"
#define MN_NT_VALUE 100
#define MN_L_VALUE 101
#define MN_CUSTOM_TOKEN_INDEX MN_NT_VALUE + 100
#define MN_NEXIST_T 17
#define MN_BREAK_NODE 79
#define MN_LOGFILENAME "mn_log.txt"
#define MN_TG_REQ_LIST 31
#define MN_TG_LIST 30
#include <sstream>



namespace mn
{
    template <typename T>
    std::string to_string(T value)
    {
      //create an output string stream
      std::ostringstream os ;

      //throw the value into the string stream
      os << value ;

      //convert the string stream into a string and return
      return os.str() ;
    }


    //Endianess
    static inline int is_big_endian(void)
    {
        union {
            uint32_t i;
            char c[4];
        } __bint = {0x01020304};

        return __bint.c[0] == 1;
    }


    static std::string logfilename="mn_log.txt";
    class Log
    {
    public:
        Log(bool init =false):init(init) {}
        template <typename T>
        Log& operator << (T obj)
        {
            std::ofstream out;
            out.open(logfilename, !init ? std::ios_base:: out | std::ios_base::app : std::ios_base:: out);
            out << obj;
            out.close();
            return *this;
        }
    protected:
        bool init;

    };


    static inline Log log(bool init=false)
    {
        return Log(init);
    }

    static inline void set_log(std::string value)
    {
        logfilename = value;
    }
}
#endif // MNCONFIG_H
