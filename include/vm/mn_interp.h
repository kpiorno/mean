/*************************************************************************/
/*  mn_interp.h                                                          */
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
#ifndef MNINTERP_H
#define MNINTERP_H
#include <iostream>
#include <vector>
#include <vm/mn_ctrav.h>
#include <vm/interp/mn_object.h>

namespace mn
{
class MNInterp: public MNCTrav
{
public:
     MNInterp(std::vector<MNVariant*>* data);
     virtual void args_param_type();
     virtual void begin_function_dec();
     virtual void begin_func_param();
     virtual void begin_typedef();
     virtual void binary_add();
     virtual void binary_and();
     virtual void binary_xor();
     virtual void binary_or();
     virtual void binary_lshift();
     virtual void binary_rshift();
     virtual void binary_substract();
     virtual void binary_subscr();
     virtual void binary_power();
     virtual void binary_multiply();
     virtual void binary_true_divide();
     virtual void binary_matrix_multiply();
     virtual void binary_floor_divide();
     virtual void binary_modulo();
     virtual void build_tuple(const int32_t& value);
     virtual void build_list(const int32_t& value);
     virtual void build_list_unpack(const int32_t& value);
     virtual void build_tuple_unpack(const int32_t& value);
     virtual void build_set_unpack(const int32_t& value);
     virtual void build_slice(const int32_t& value);
     virtual void build_set(const int32_t& value);
     virtual void build_string(const int32_t& value);
     virtual void build_map(const int32_t& value);
     virtual void build_map_unpack_with_call(const int32_t& value);
     virtual void break_loop();
     virtual void call_function(const int16_t& pos_args, const int16_t& key_args);
     virtual void call_function_var(const int16_t& pos_args, const int16_t& key_args);
     virtual void call_function_var_kw(const int16_t& pos_args, const int16_t& key_args);
     virtual void call_function_kw(const int16_t& pos_args, const int16_t& key_args);
     virtual void cond_op_and();
     virtual void compare_op_e();
     virtual void compare_op_em();
     virtual void compare_op_b();
     virtual void compare_op_a();
     virtual void compare_op_ae();
     virtual void compare_op_be();
     virtual void compare_op_ne();
     virtual void compare_op_not_in();
     virtual void compare_op_in();
     virtual void compare_op_is();
     virtual void compare_op_is_not();
     virtual void default_func_param_value();
     virtual void delete_name(const std::string& name);
     virtual void delete_attr(const std::string& name);
     virtual void delete_subscr();
     virtual void dup_top();
     virtual void end_finally();
     virtual void for_iter(const int32_t& value);
     virtual void get_iter(const int32_t& value);
     virtual void get_yield_from_iter();
     virtual void cif();
     virtual void inplace_add();
     virtual void inplace_substract();
     virtual void inplace_xor();
     virtual void inplace_and();
     virtual void inplace_or();
     virtual void inplace_power();
     virtual void inplace_multiply();
     virtual void inplace_true_divide();
     virtual void inplace_matrix_multiply();
     virtual void inplace_floor_divide();
     virtual void inplace_modulo();
     virtual void import_name(const std::string& name);
     virtual void import_from(const std::string& name);
     virtual void kwargs_param_type();
     virtual void end_function_dec();
     virtual void end_func_param();
     virtual void end_typedef();
     virtual void end_if();
     virtual void load_const_str(const std::string& value);
     virtual void load_const_hex(const std::string& value);
     virtual void load_const_bin(const std::string& value);
     virtual void load_const_ustr(const std::string& value);
     virtual void load_const_bstr(const std::string& value);
     virtual void load_const_rstr(const std::string& value);
     virtual void load_const_exp(const std::string& value);
     virtual void load_const_mstr(const std::string& value);
     virtual void load_const_fstr(const std::string& value);
     virtual void load_const_brstr(const std::string& value);
     virtual void load_const_rbstr(const std::string& value);
     virtual void load_const_32(const int32_t& value);
     virtual void load_const_32f(float value);
     virtual void load_const_img(const std::string& value);
     virtual void load_const_oct(const std::string& value);
     virtual void load_attr(const std::string& name);
     virtual void load_name(const std::string& name);
     virtual void load_build_class();
     virtual void make_class();
     virtual void return_value();
     virtual void rot_two();
     virtual void rot_three();
     virtual void store_name(const std::string& name);
     virtual void pop_block();
     virtual void pop_jump_if_false(const int32_t& value);
     virtual void pop_jump_if_true(const int32_t& value);
     virtual void pop_top();
     virtual void pop_except();
     virtual void jump(const int32_t& value);
     virtual void jump_if_true_or_pop(const int32_t& value);
     virtual void jump_if_false_or_pop(const int32_t& value);
     virtual void raise_varargs(const int16_t& count);
     virtual void stmt(const int32_t& value);
     virtual void store_subscr();
     virtual void store_attr(const std::string& name);
     virtual void single_typedef();
     virtual void setup_except(const int32_t& value);
     virtual void setup_finally(const int32_t& value);
     virtual void setup_loop(const int32_t& value);
     virtual void setup_loop_for(const int32_t& value);
     virtual void setup_loop_while(const int32_t& value);
     virtual void setup_with(const int32_t& value);
     virtual void unpack_sequence(const int32_t& value);
     virtual void unpack_ex(const int32_t& value);
     virtual void unary_not();
     virtual void unary_negative();
     virtual void unary_positive();
     virtual void unary_invert();
     virtual void with_cleanup_start();
     virtual void with_cleanup_finish();
     virtual void yield_value();
     virtual void yield_from();
public:
     void push_back(MNPObject obj);
protected:
     std::vector<MNPObject> c_stack;
     std::vector<MNPObject> context_stack;
     MNPObject current_context;
     unsigned int line;
};
}
#endif // MNVARIANT_H
