/*************************************************************************/
/*  mn_interp.cpp                                                        */
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
#include <vm/mn_interp.h>
#include <vm/interp/mn_object.h>


#define PUSH_BACK(obj) \
if (obj->get_data().type == MN_ERROR) \
{ \
    std::cout << obj->get_data().svalue << " at line: " << mn::to_string(line) << std::endl; \
    current_pos = e_data->size()-1; \
}\
c_stack.push_back(obj);

namespace mn
{
    MNInterp::MNInterp(std::vector<MNVariant*>* data):MNCTrav(data)
    {
        line = 1;
        context_stack.push_back(MAKE_OBJ(MNObject, MNVariant(0)));
        current_context = context_stack.back();
    }

    _FORCE_INLINE_ void MNInterp::args_param_type()
    {

    }

    _FORCE_INLINE_ void MNInterp::begin_function_dec()
    {

    }

    _FORCE_INLINE_ void MNInterp::begin_func_param()
    {

    }

    _FORCE_INLINE_ void MNInterp::begin_typedef()
    {

    }

    _FORCE_INLINE_ void MNInterp::binary_add()
    {
        auto res = c_stack[c_stack.size()-2]->__add__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::binary_and()
    {
        auto res = c_stack[c_stack.size()-2]->__and__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::binary_xor()
    {

    }

    _FORCE_INLINE_ void MNInterp::binary_or()
    {

    }

    _FORCE_INLINE_ void MNInterp::binary_lshift()
    {

    }

    _FORCE_INLINE_ void MNInterp::binary_rshift()
    {

    }

    _FORCE_INLINE_ void MNInterp::binary_substract()
    {
        auto res = c_stack[c_stack.size()-2]->__sub__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::binary_subscr()
    {

    }

    _FORCE_INLINE_ void MNInterp::binary_power()
    {

    }

    _FORCE_INLINE_ void MNInterp::binary_multiply()
    {
        auto res = c_stack[c_stack.size()-2]->__mul__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::binary_true_divide()
    {
        auto res = c_stack[c_stack.size()-2]->__tdiv__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::binary_matrix_multiply()
    {

    }

    _FORCE_INLINE_ void MNInterp::binary_floor_divide()
    {
        auto res = c_stack[c_stack.size()-2]->__fdiv__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::binary_modulo()
    {

    }

    _FORCE_INLINE_ void MNInterp::build_tuple(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::build_list(const int32_t& value)
    {
        if (c_stack.size() >= (unsigned)value)
        {
            auto list = MAKE_OBJ(MNContainer, MNVariant(0) );
            for (unsigned int i=0; i < (unsigned int)value; ++i)
                ((MNContainer*)list.get())->add_object(c_stack[c_stack.size()-(value-i-1)-1]) ;
            for (unsigned int i=0; i < (unsigned int)value; ++i)
                c_stack.pop_back();
            c_stack.push_back(list);
        }
    }

    _FORCE_INLINE_ void MNInterp::build_list_unpack(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::build_tuple_unpack(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::build_set_unpack(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::build_slice(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::build_set(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::build_string(const int32_t& value)
    {
         if (c_stack.size() >= (unsigned)value)
         {
             std::string res = "";
             for (unsigned int i=0; i < (unsigned int)value; ++i)
                res += c_stack[c_stack.size()-(value-i-1)-1]->get_data().svalue ;
             for (unsigned int i=0; i < (unsigned int)value; ++i)
                 c_stack.pop_back();
             c_stack.push_back(MAKE_OBJ(MNConstStr, MNVariant(res)));
         }
    }

    _FORCE_INLINE_ void MNInterp::build_map(const int32_t& /*value*/)
    {
        //std::cout << "CREATING MAP" << value << std::endl;
    }

    _FORCE_INLINE_ void MNInterp::build_map_unpack_with_call(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::break_loop()
    {

    }

    _FORCE_INLINE_ void MNInterp::call_function(const int16_t& /*pos_args*/, const int16_t& /*key_args*/)
    {

    }



    _FORCE_INLINE_ void MNInterp::call_function_var(const int16_t& /*pos_args*/, const int16_t& /*key_args*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::call_function_var_kw(const int16_t& /*pos_args*/, const int16_t& /*key_args*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::call_function_kw(const int16_t& /*pos_args*/, const int16_t& /*key_args*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::cond_op_and()
    {

    }

    _FORCE_INLINE_ void MNInterp::compare_op_e()
    {
        auto res = c_stack[c_stack.size()-2]->__cmp_e__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::compare_op_em()
    {

    }

    _FORCE_INLINE_ void MNInterp::compare_op_b()
    {
        auto res = c_stack[c_stack.size()-2]->__cmp_b__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::compare_op_a()
    {
        auto res = c_stack[c_stack.size()-2]->__cmp_a__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::compare_op_ae()
    {
        auto res = c_stack[c_stack.size()-2]->__cmp_ae__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::compare_op_be()
    {
        auto res = c_stack[c_stack.size()-2]->__cmp_be__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::compare_op_ne()
    {
        auto res = c_stack[c_stack.size()-2]->__cmp_ne__(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        push_back(res);
    }

    _FORCE_INLINE_ void MNInterp::compare_op_not_in()
    {

    }

    _FORCE_INLINE_ void MNInterp::compare_op_in()
    {

    }

    _FORCE_INLINE_ void MNInterp::compare_op_is()
    {

    }

    _FORCE_INLINE_ void MNInterp::compare_op_is_not()
    {

    }

    _FORCE_INLINE_ void MNInterp::default_func_param_value()
    {

    }

    _FORCE_INLINE_ void MNInterp::delete_name(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::delete_attr(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::delete_subscr()
    {

    }

    _FORCE_INLINE_ void MNInterp::dup_top()
    {

    }

    _FORCE_INLINE_ void MNInterp::end_finally()
    {

    }

    _FORCE_INLINE_ void MNInterp::for_iter(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::get_iter(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::get_yield_from_iter()
    {

    }

    _FORCE_INLINE_ void MNInterp::cif()
    {

    }

    _FORCE_INLINE_ void MNInterp::inplace_add()
    {
//        auto res = c_stack[c_stack.size()-2]->__implace_add__(c_stack[c_stack.size()-1]);
//        c_stack.pop_back(); c_stack.pop_back();
//        c_stack.push_back(res);
    }

    _FORCE_INLINE_ void MNInterp::inplace_substract()
    {

    }

    _FORCE_INLINE_ void MNInterp::inplace_xor()
    {

    }

    _FORCE_INLINE_ void MNInterp::inplace_and()
    {

    }


    _FORCE_INLINE_ void MNInterp::inplace_or()
    {

    }


    _FORCE_INLINE_ void MNInterp::inplace_power()
    {

    }

    _FORCE_INLINE_ void MNInterp::inplace_multiply()
    {

    }


    _FORCE_INLINE_ void MNInterp::inplace_true_divide()
    {

    }

    _FORCE_INLINE_ void MNInterp::inplace_matrix_multiply()
    {

    }

    _FORCE_INLINE_ void MNInterp::inplace_floor_divide()
    {

    }

    _FORCE_INLINE_ void MNInterp::inplace_modulo()
    {

    }

    _FORCE_INLINE_ void MNInterp::import_name(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::import_from(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::kwargs_param_type()
    {

    }

    _FORCE_INLINE_ void MNInterp::end_function_dec()
    {

    }

    _FORCE_INLINE_ void MNInterp::end_func_param()
    {

    }

    _FORCE_INLINE_ void MNInterp::end_typedef()
    {

    }

    _FORCE_INLINE_ void MNInterp::end_if()
    {

    }


    _FORCE_INLINE_ void MNInterp::load_const_str(const std::string& value)
    {
        PUSH_BACK(MAKE_OBJ(MNConstStr, MNVariant(value)));
    }

    _FORCE_INLINE_ void MNInterp::load_const_hex(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_bin(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_ustr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_bstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_rstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_exp(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_mstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_fstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_brstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_rbstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_32(const int32_t& value)
    {

        //PUSH_BACK(MAKE_OBJ(MNIConstInt, MNVariant(value)));
        auto obj = MAKE_OBJ(MNIConstInt, MNVariant(value));
        if (obj->get_data().type == MN_ERROR)
        {
            std::cout << obj->get_data().svalue << " at line: " << mn::to_string(line) << std::endl;
            current_pos = e_data->size()-1;
        }
        c_stack.push_back(obj);

    }

    _FORCE_INLINE_ void MNInterp::load_const_32f(float value)
    {
       PUSH_BACK(MAKE_OBJ(MNIConstFloat, MNVariant(value)));
    }

    _FORCE_INLINE_ void MNInterp::load_const_img(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_const_oct(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_attr(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::load_name(const std::string& name)
    {
        if (!context_stack.empty())
        {
            auto context = context_stack.back();
            PUSH_BACK(context->get_attr(name));
        }

//        if (!context_stack.empty())
//        {
//            auto context = context_stack.back();
//            auto attr = context->get_attrs();
//            if (attr->find(name) != attr->end())
//            {
//                PUSH_BACK(attr->at(name));
//            }
//            else
//            {
//                PUSH_BACK(std::make_shared<MNError>(MNVariant("NameError: name '" + name + "' is not defined" )));
//            }
//        }
    }

    _FORCE_INLINE_ void MNInterp::load_build_class()
    {

    }

    _FORCE_INLINE_ void MNInterp::make_class()
    {

    }

    _FORCE_INLINE_ void MNInterp::return_value()
    {

    }

    _FORCE_INLINE_ void MNInterp::rot_two()
    {

    }

    _FORCE_INLINE_ void MNInterp::rot_three()
    {

    }

    _FORCE_INLINE_ void MNInterp::store_name(const std::string& name)
    {
         if (!context_stack.empty())
         {
             auto context = context_stack.back();
             if (!c_stack.empty())
             {
                context->set_attr(name, c_stack.back());
                std::cout << "Stored On \"" << name << "\": " << c_stack.back()->__str__() << std::endl;
                c_stack.pop_back();

             }
         }
    }

    _FORCE_INLINE_ void MNInterp::pop_block()
    {

    }

    _FORCE_INLINE_ void MNInterp::pop_jump_if_false(const int32_t& value)
    {
        if (!c_stack.empty())
        {
            bool is_true = c_stack.back()->is_true();
            c_stack.pop_back();
            if (!is_true)
                current_pos = value;
            else
                current_pos += 2;
        }
        else
            current_pos += 2;
    }

    _FORCE_INLINE_ void MNInterp::pop_jump_if_true(const int32_t& value)
    {
        if (!c_stack.empty())
        {
            bool is_true = c_stack.back()->is_true();
            c_stack.pop_back();
            if (is_true)
                current_pos = value;
            else
                current_pos += 2;
        }
        else
            current_pos += 2;
    }

    _FORCE_INLINE_ void MNInterp::pop_top()
    {
        //std::cout << "TOP VALUE BEFORE POP_TOP: " << c_stack.back()->__str__() << std::endl;
        if (!c_stack.empty())
            c_stack.pop_back();
    }

    _FORCE_INLINE_ void MNInterp::pop_except()
    {

    }

    _FORCE_INLINE_ void MNInterp::jump(const int32_t& value)
    {
        current_pos = value;
    }

    _FORCE_INLINE_ void MNInterp::jump_if_true_or_pop(const int32_t& value)
    {
        if (!c_stack.empty())
        {
            bool is_true = c_stack.back()->is_true();
            if (is_true)
                current_pos = value;
            else
                current_pos += 2;
        }
        else
        {
            c_stack.pop_back();
            current_pos += 2;
        }
    }

    _FORCE_INLINE_ void MNInterp::jump_if_false_or_pop(const int32_t& value)
    {
        if (!c_stack.empty())
        {
            bool is_true = c_stack.back()->is_true();
            if (is_true)
                current_pos = value;
            else
                current_pos += 2;
        }
        else
        {
            c_stack.pop_back();
            current_pos += 2;
        }
    }

    _FORCE_INLINE_ void MNInterp::raise_varargs(const int16_t& /*count*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::stmt(const int32_t& value)
    {
        line = value;
    }

    _FORCE_INLINE_ void MNInterp::store_subscr()
    {
        auto res = c_stack[c_stack.size()-2]->store_subscr(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }


    _FORCE_INLINE_ void MNInterp::store_attr(const std::string& /*name*/)
    {
        auto res = c_stack[c_stack.size()-2]->load_subscr(c_stack[c_stack.size()-1]);
        c_stack.pop_back(); c_stack.pop_back();
        PUSH_BACK(res);
    }

    _FORCE_INLINE_ void MNInterp::single_typedef()
    {

    }

    _FORCE_INLINE_ void MNInterp::setup_except(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::setup_finally(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::setup_loop(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::setup_loop_for(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::setup_loop_while(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::setup_with(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::unpack_sequence(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::unpack_ex(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNInterp::unary_not()
    {
       if (!c_stack.empty())
       {
           auto is_true = !c_stack.back()->is_true();
           c_stack.pop_back();
           if (is_true)
           {
               PUSH_BACK(MAKE_OBJ(MNIConstInt, MNVariant(1)));
           }
           else
           {
               PUSH_BACK(MAKE_OBJ(MNIConstInt, MNVariant(0)));
           }
       }
    }

    _FORCE_INLINE_ void MNInterp::unary_negative()
    {

    }

    _FORCE_INLINE_ void MNInterp::unary_positive()
    {

    }

    _FORCE_INLINE_ void MNInterp::unary_invert()
    {

    }

    _FORCE_INLINE_ void MNInterp::with_cleanup_start()
    {

    }

    _FORCE_INLINE_ void MNInterp::with_cleanup_finish()
    {

    }

    _FORCE_INLINE_ void MNInterp::yield_value()
    {

    }

    _FORCE_INLINE_ void MNInterp::yield_from()
    {

    }

    _FORCE_INLINE_ void MNInterp::push_back(MNPObject /*obj*/)
    {
//        if (obj->get_data().type == MN_ERROR)
//        {
//            std::cout << obj->get_data().svalue << " at line: " << mn::to_string(line) << std::endl;
//            current_pos = e_data->size()-1;
//        }
//        c_stack.push_back(obj);
    }

}

