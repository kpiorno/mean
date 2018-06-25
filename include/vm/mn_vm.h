/*************************************************************************/
/*  mn_vm.h                                                              */
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
#ifndef MNVM_H
#define MNVM_H
#include <iostream>
#include <vector>
#include <travelers/mn_mn_bin.h>
#include <vm/mn_variant.h>
#include <vm/mn_ctrav.h>
#include <vm/mn_interp.h>
#include <mem_pool/MemoryPool.h>


namespace mn
{
const size_t ME_MEMPOOL_SIZE = sizeof(MNVariant)*64;
class MN_VM
{
public:
    MN_VM(std::string name);
    ~MN_VM();
    void process();
    void exec_traveler(mn::MNCTrav* trav);
protected:
    void add_command(MNPyByteCode type);
    void add_i16(int16_t value);
    void add_i32(int32_t value);
    void add_float(float value);
    void add_str(std::string value);
    void add_element(MNPyByteCode type, unsigned int value);
    void push_back(MNVariant* e);
private:
    std::string _gen_opcode_spaces(std::string opcode);
    std::string name;
    std::vector<MNVariant*>* e_data;
    MemoryPool<MNVariant, ME_MEMPOOL_SIZE>* m_pool;
    std::stringstream dis;
    unsigned int c_reserve;
};
}
#endif // MNX86_H
