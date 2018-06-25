/*************************************************************************/
/*  mn_data_stream.cpp                                                   */
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
#include "mn_data_stream.h"
#include <cstring>
#include <mn_config.h>
namespace mn
{
    MN_DataStream::MN_DataStream(const std::string filename, int mode)
    {
        open(filename, mode);
        inst_count = 0;
        little_endian = !mn::is_big_endian();
    }

    MN_DataStream::MN_DataStream()
    {
        inst_count = 0;
        little_endian = !mn::is_big_endian();
    }

    long int MN_DataStream::get_cursor_pos()
    {
        return buffer.size()-1;
    }

    unsigned int MN_DataStream::get_inst_count()
    {
        return inst_count;
    }

    void MN_DataStream::dec_inst_count()
    {
        --inst_count;
    }

    int MN_DataStream::open(const std::string filename, int mode)
    {
        buffer = "";
        if (mode == 0)
            out.open(filename, std::ios_base::binary | std::ios_base:: out );
        else
            out.open(filename, std::ios_base::binary | std::ios_base:: in );

        return 0;
    }

    void MN_DataStream::close()
    {
        if (out.is_open())
        {
            out.write(buffer.c_str(), buffer.size());
            out.close();
        }
    }

    MN_DataStream&  MN_DataStream::ai8(int8_t value, long int pos)
    {
        char* res = reinterpret_cast<char*>(&value);
        if (pos == -1) buffer += res[0]; else buffer[pos] = res[0];
        if (pos == -1)  inst_count++;
        return *this;
    }

    MN_DataStream&  MN_DataStream::ai16(int16_t value, long int pos)
    {
        char* res = reinterpret_cast<char*>(&value);
        add_to_buffer(res, sizeof(int16_t), pos);
        return *this;
    }


    MN_DataStream&  MN_DataStream::ai32(int32_t value, long int pos)
    {
        char* res = reinterpret_cast<char*>(&value);

        add_to_buffer(res, sizeof(int32_t), pos);
        return *this;
    }

    MN_DataStream&  MN_DataStream::ai64(int64_t value, long int pos)
    {
        char* res = reinterpret_cast<char*>(&value);
        add_to_buffer(res, sizeof(int64_t), pos);
        return *this;
    }

    MN_DataStream&  MN_DataStream::afloat(float value, long int pos)
    {
        char* res = reinterpret_cast<char*>(&value);
        add_to_buffer(res, sizeof(float), pos);
        return *this;
    }

    MN_DataStream&  MN_DataStream::adouble(double value, long int pos)
    {
        char* res = reinterpret_cast<char*>(&value);
        add_to_buffer(res, sizeof(double), pos);
        return *this;
    }

    MN_DataStream&  MN_DataStream::astr(const std::string value, long int pos)
    {
        if (pos == -1)
            buffer += value;
        else
        {
            buffer.replace(pos, value.size(), value);
        }
        if (pos == -1)  inst_count++;
        return *this;

    }

    void MN_DataStream::add_to_buffer(char *value, int count, long int pos)
    {

        for (int i=0; i < count;++i)
            if (pos == -1)
                buffer += value[little_endian ? i : count - i - 1];
            else
                buffer[pos + i] = value[little_endian ? i : count - i - 1];
        if (pos == -1)  inst_count++;
    }

    char* MN_DataStream::rcstr(size_t count)
    {
        char* res = new char[count];
        out.read(res, count);
        return res;
    }

    std::string MN_DataStream::rstr(size_t count)
    {
        char* res = rcstr(count);
        std::string ret = "";
        for (size_t i = 0; i < count; ++i)
             ret += res[i];
        delete[] res;
        return ret;
    }

    int8_t MN_DataStream::ri8()
    {
          int8_t ret;
          out.read((char*)&ret, sizeof(int8_t));
          return ret;
    }

    int16_t MN_DataStream::ri16()
    {
          int16_t ret;
          out.read((char*)&ret, sizeof(int16_t));
          unsigned char c1, c2;
          if (!little_endian)
          {
              c1 = ret & 255;
              c2 = (ret >> 8) & 255;
              return (c1 << 8) + c2;
          }
          return ret;
    }

    int32_t MN_DataStream::ri32()
    {
          int32_t ret;
          out.read((char*)&ret, sizeof(int32_t));
          unsigned char c1, c2, c3, c4;
          if (!little_endian)
          {
              c1 = ret & 255;
              c2 = (ret >> 8) & 255;
              c3 = (ret >> 16) & 255;
              c4 = (ret >> 24) & 255;
              return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
          }
          return ret;
    }

    int64_t MN_DataStream::ri64()
    {
          int64_t ret;
          out.read((char*)&ret, sizeof(int64_t));
          char *p = (char *)&ret;
          int64_t res = 0;
          char* r = (char *)&res;
          if (!little_endian)
          {
             r[0] = p[7];
             r[1] = p[6];
             r[2] = p[5];
             r[3] = p[4];
             r[4] = p[3];
             r[5] = p[2];
             r[6] = p[1];
             r[7] = p[0];
          }
          //delete p;
          //delete r;
          return res;
    }

    double MN_DataStream::rdouble()
    {
          double ret;
          out.read((char*)&ret, sizeof(double));
          if (!little_endian)
          {
              size_t s_c = sizeof(double);
              double res = 0;
              char* p = (char*)&ret;
              char* r = (char*)&res;
              for (size_t i = 0; i < s_c; ++i)
                  r[0] = p[s_c - i - 1];
              delete p;
              delete r;
              return res;
          }
          return ret;
    }

    float MN_DataStream::rfloat()
    {
          float ret;
          out.read((char*)&ret, sizeof(float));
          if (!little_endian)
          {
              size_t s_c = sizeof(float);
              float res = 0;
              char* p = (char*)&ret;
              char* r = (char*)&res;
              for (size_t i = 0; i < s_c; ++i)
                  r[0] = p[s_c - i - 1];
              delete p;
              delete r;
              return res;
          }
          return ret;
    }

    int32_t MN_DataStream::gi32(unsigned int pos)
    {
        char* res = new char[4];
        res[0] = buffer[pos];
        res[1] = buffer[pos+1];
        res[2] = buffer[pos+2];
        res[3] = buffer[pos+3];
        int32_t ret = *(int32_t*)res;
        delete[] res;
        return ret;
    }



    MN_DataStream::~MN_DataStream()
    {
        close();
    }


}

