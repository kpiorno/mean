/*************************************************************************/
/*  mn_data_stream.h                                                     */
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
#ifndef MN_HDATASTREAM
#define MN_HDATASTREAM
#include <iostream>
#include <fstream>

namespace mn
{
    class MN_DataStream
    {
    public:
        MN_DataStream() {inst_count = 0;}
        MN_DataStream(std::string filename, int mode=0);
        ~MN_DataStream();

        long int get_cursor_pos();
        unsigned int get_inst_count();
        int open(std::string filename, int mode=0);
        void close();
        MN_DataStream& ai8(int8_t value, long int pos = -1);
        MN_DataStream& ai16(int16_t value, long int pos = -1);
        MN_DataStream& ai32(int32_t value, long int pos = -1);
        MN_DataStream& ai64(int64_t value, long int pos = -1);
        MN_DataStream& afloat(float value, long int pos = -1);
        MN_DataStream& adouble(double value, long int pos = -1);
        MN_DataStream& achar(char value, long int pos = -1);
        MN_DataStream& acstr(char* value, long int pos = -1);
        MN_DataStream& astr(std::string value, long int pos = -1);

        char* rcstr(size_t count);
        std::string rstr(size_t count);

        int8_t ri8();
        int16_t ri16();
        int32_t ri32();
        int64_t ri64();
        double rdouble();
        float rfloat();

        template <typename T>
        MN_DataStream& operator << (T& obj)
        {
            char* res = reinterpret_cast<char*>(&obj);
            add_to_buffer(res, sizeof(T));
            return *this;
        }

        template <typename T>
        MN_DataStream& operator >> (T& obj)
        {
            char* res = new char[sizeof(T)];
            out.read(res, sizeof(T));
            obj = *((T*)res);
            delete[] res;

            return *this;
        }

    protected:
        std::fstream out;
        std::string buffer;
        unsigned int inst_count;
        void add_to_buffer(char* value, int count, long int pos = -1);
    };
}
#endif // MN_HDATASTREAM
