/*************************************************************************/
/*  mn_variant.h                                                         */
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
#ifndef MNVARIANT_H
#define MNVARIANT_H
#include <iostream>
#include <vector>
#include <travelers/mn_mn_bin.h>
#include <mem_pool/MemoryPool.h>
#include <atomic>
namespace mn
{
struct MNVariant
{
   MNPyByteCode type;
   std::string svalue;
   union {
      char character;
      int16_t v16;
      int32_t v32;
      int vint;
      float vfloat;
      int64_t v64;
      double vdouble;
   } value;
   explicit MNVariant(){}
   explicit MNVariant(char value)
   {
       this->value.character = value;
   }
   explicit MNVariant(int16_t value)
   {
       this->value.v16 = value;
   }
   explicit MNVariant(int32_t value)
   {
       this->value.v32 = value;
   }
   explicit MNVariant(float value)
   {
       this->value.vfloat = value;
   }
   explicit MNVariant(int64_t value)
   {
       this->value.v64 = value;
   }
   explicit MNVariant(double value)
   {
       this->value.vdouble = value;
   }

   explicit MNVariant(std::string value)
   {
       this->svalue = value;
   }

   _FORCE_INLINE_ void set_float(float value)
   {
       this->value.vfloat = value;
   }

   _FORCE_INLINE_ float get_float()
   {
       return this->value.vfloat;
   }

   _FORCE_INLINE_ void set_double(double value)
   {
       this->value.vdouble = value;
   }

   _FORCE_INLINE_ double get_double()
   {
       return this->value.vdouble;
   }

};
}
#endif // MNVARIANT_H
