/*************************************************************************/
/*  mn_ctrav.cpp                                                         */
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
#include <vm/mn_ctrav.h>

namespace mn
{
    MNCTrav::MNCTrav(std::vector<MNVariant*>* data):e_data(data)
    {
        current_pos = 0;
    }

    MNCTrav::~MNCTrav()
    {

    }

    _FORCE_INLINE_ void MNCTrav::args_param_type()
    {

    }

    _FORCE_INLINE_ void MNCTrav::begin_function_dec()
    {

    }

    _FORCE_INLINE_ void MNCTrav::begin_func_param()
    {

    }

    _FORCE_INLINE_ void MNCTrav::begin_typedef()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_add()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_and()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_xor()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_or()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_lshift()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_rshift()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_substract()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_subscr()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_power()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_multiply()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_true_divide()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_matrix_multiply()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_floor_divide()
    {

    }

    _FORCE_INLINE_ void MNCTrav::binary_modulo()
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_tuple(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_list(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_list_unpack(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_tuple_unpack(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_set_unpack(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_slice(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_set(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_string(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_map(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::build_map_unpack_with_call(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::break_loop()
    {

    }

    _FORCE_INLINE_ void MNCTrav::call_function(const int16_t& /*pos_args*/, const int16_t& /*key_args*/)
    {

    }



    _FORCE_INLINE_ void MNCTrav::call_function_var(const int16_t& /*pos_args*/, const int16_t& /*key_args*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::call_function_var_kw(const int16_t& /*pos_args*/, const int16_t& /*key_args*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::call_function_kw(const int16_t& /*pos_args*/, const int16_t& /*key_args*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::cond_op_and()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_e()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_em()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_b()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_a()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_ae()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_be()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_ne()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_not_in()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_in()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_is()
    {

    }

    _FORCE_INLINE_ void MNCTrav::compare_op_is_not()
    {

    }

    _FORCE_INLINE_ void MNCTrav::default_func_param_value()
    {

    }

    _FORCE_INLINE_ void MNCTrav::delete_name(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::delete_attr(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::delete_subscr()
    {

    }

    _FORCE_INLINE_ void MNCTrav::dup_top()
    {

    }

    _FORCE_INLINE_ void MNCTrav::end_finally()
    {

    }

    _FORCE_INLINE_ void MNCTrav::for_iter(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::get_iter(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::get_yield_from_iter()
    {

    }

    _FORCE_INLINE_ void MNCTrav::cif()
    {

    }

    _FORCE_INLINE_ void MNCTrav::inplace_add()
    {

    }

    _FORCE_INLINE_ void MNCTrav::inplace_substract()
    {

    }

    _FORCE_INLINE_ void MNCTrav::inplace_xor()
    {

    }

    _FORCE_INLINE_ void MNCTrav::inplace_and()
    {

    }


    _FORCE_INLINE_ void MNCTrav::inplace_or()
    {

    }


    _FORCE_INLINE_ void MNCTrav::inplace_power()
    {

    }

    _FORCE_INLINE_ void MNCTrav::inplace_multiply()
    {

    }


    _FORCE_INLINE_ void MNCTrav::inplace_true_divide()
    {

    }

    _FORCE_INLINE_ void MNCTrav::inplace_matrix_multiply()
    {

    }

    _FORCE_INLINE_ void MNCTrav::inplace_floor_divide()
    {

    }

    _FORCE_INLINE_ void MNCTrav::inplace_modulo()
    {

    }

    _FORCE_INLINE_ void MNCTrav::import_name(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::import_from(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::kwargs_param_type()
    {

    }

    _FORCE_INLINE_ void MNCTrav::end_function_dec()
    {

    }

    _FORCE_INLINE_ void MNCTrav::end_func_param()
    {

    }

    _FORCE_INLINE_ void MNCTrav::end_typedef()
    {

    }

    _FORCE_INLINE_ void MNCTrav::end_if()
    {

    }


    _FORCE_INLINE_ void MNCTrav::load_const_str(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_hex(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_bin(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_ustr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_bstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_rstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_exp(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_mstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_fstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_brstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_rbstr(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_32(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_32f(float /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_img(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_const_oct(const std::string& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_attr(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_name(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::load_build_class()
    {

    }

    _FORCE_INLINE_ void MNCTrav::make_class()
    {

    }

    _FORCE_INLINE_ void MNCTrav::return_value()
    {

    }

    _FORCE_INLINE_ void MNCTrav::rot_two()
    {

    }

    _FORCE_INLINE_ void MNCTrav::rot_three()
    {

    }

    _FORCE_INLINE_ void MNCTrav::store_name(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::pop_block()
    {

    }

    _FORCE_INLINE_ void MNCTrav::pop_jump_if_false(const int32_t& /*value*/)
    {
        current_pos += 2;
    }

    _FORCE_INLINE_ void MNCTrav::pop_jump_if_true(const int32_t& /*value*/)
    {
        current_pos += 2;
    }

    _FORCE_INLINE_ void MNCTrav::pop_top()
    {

    }

    _FORCE_INLINE_ void MNCTrav::pop_except()
    {

    }

    _FORCE_INLINE_ void MNCTrav::jump(const int32_t& /*value*/)
    {
        current_pos += 2;
    }

    _FORCE_INLINE_ void MNCTrav::jump_if_true_or_pop(const int32_t& /*value*/)
    {
        current_pos += 2;
    }

    _FORCE_INLINE_ void MNCTrav::jump_if_false_or_pop(const int32_t& /*value*/)
    {
        current_pos += 2;
    }

    _FORCE_INLINE_ void MNCTrav::raise_varargs(const int16_t& /*count*/)
    {
        current_pos += 2;
    }

    _FORCE_INLINE_ void MNCTrav::stmt(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::store_subscr()
    {

    }


    _FORCE_INLINE_ void MNCTrav::store_attr(const std::string& /*name*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::single_typedef()
    {

    }

    _FORCE_INLINE_ void MNCTrav::setup_except(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::setup_finally(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::setup_loop(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::setup_loop_for(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::setup_loop_while(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::setup_with(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::unpack_sequence(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::unpack_ex(const int32_t& /*value*/)
    {

    }

    _FORCE_INLINE_ void MNCTrav::unary_not()
    {

    }

    _FORCE_INLINE_ void MNCTrav::unary_negative()
    {

    }

    _FORCE_INLINE_ void MNCTrav::unary_positive()
    {

    }

    _FORCE_INLINE_ void MNCTrav::unary_invert()
    {

    }

    _FORCE_INLINE_ void MNCTrav::with_cleanup_start()
    {

    }

    _FORCE_INLINE_ void MNCTrav::with_cleanup_finish()
    {

    }

    _FORCE_INLINE_ void MNCTrav::yield_value()
    {

    }

    _FORCE_INLINE_ void MNCTrav::yield_from()
    {

    }


    void MNCTrav::start()
    {
        MNVariant* e = nullptr;
        if (!e_data->empty())
            e = e_data->at(0);
        if (e)
        {
            while (current_pos < e_data->size() && e->type != MNPyByteCode::HALT)
            {
                e = e_data->at(current_pos);

                switch (e->type) {
                case ARGS_PARAM_TYPE:
                    args_param_type();
                    ++current_pos;
                    break;
                case BEGIN_FUNCTION_DEC:
                    begin_function_dec();
                    ++current_pos;
                    break;
                case BEGIN_FUNC_PARAM:
                    begin_func_param();
                    ++current_pos;
                    break;
                case BEGIN_TYPEDEF:
                    begin_typedef();
                    ++current_pos;
                    break;
                case BINARY_ADD:
                    binary_add();
                    ++current_pos;
                    break;
                case BINARY_AND:
                    binary_and();
                    ++current_pos;
                    break;
                case BINARY_XOR:
                    binary_xor();
                    ++current_pos;
                    break;
                case BINARY_OR:
                    binary_or();
                    ++current_pos;
                    break;
                case BINARY_LSHIFT:
                    binary_lshift();
                    ++current_pos;
                    break;
                case BINARY_RSHIFT:
                    binary_rshift();
                    ++current_pos;
                    break;
                case BINARY_SUBSTRACT:
                    binary_substract();
                    ++current_pos;
                    break;
                case BINARY_SUBSCR:
                    binary_subscr();
                    ++current_pos;
                    break;
                case BINARY_POWER:
                    binary_power();
                    ++current_pos;
                    break;
                case BINARY_MULTIPLY:
                    binary_multiply();
                    ++current_pos;
                    break;
                case BINARY_TRUE_DIVIDE:
                    binary_true_divide();
                    ++current_pos;
                    break;
                case BINARY_MATRIX_MULTIPLY:
                    binary_matrix_multiply();
                    ++current_pos;
                    break;
                case BINARY_FLOOR_DIVIDE:
                    binary_floor_divide();
                    ++current_pos;
                    break;
                case BINARY_MODULO:
                    binary_modulo();
                    ++current_pos;
                    break;
                case BUILD_TUPLE:
                    build_tuple(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_LIST:
                    build_list(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_LIST_UNPACK:
                    build_list_unpack(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_TUPLE_UNPACK:
                    build_tuple_unpack(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_SET_UNPACK:
                    build_set_unpack(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_SLICE:
                    build_slice(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_SET:
                    build_set(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_STRING:
                    build_string(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_MAP:
                    build_map(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BUILD_MAP_UNPACK_WITH_CALL:
                    build_map_unpack_with_call(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case BREAK_LOOP:
                    break_loop();
                    ++current_pos;
                    break;
                case CALL_FUNCTION:
                    call_function(e_data->at(current_pos + 1)->value.v16, e_data->at(current_pos+2)->value.v16);
                    current_pos += 3;
                    break;
                case CALL_FUNCTION_VAR:
                    call_function_var(e_data->at(current_pos + 1)->value.v16, e_data->at(current_pos+2)->value.v16);
                    current_pos += 3;
                    break;
                case CALL_FUNCTION_VAR_KW:
                    call_function_var_kw(e_data->at(current_pos + 1)->value.v16, e_data->at(current_pos+2)->value.v16);
                    current_pos += 3;
                    break;
                case CALL_FUNCTION_KW:
                    call_function_kw(e_data->at(current_pos + 1)->value.v16, e_data->at(current_pos+2)->value.v16);
                    current_pos += 3;
                    break;
                case COND_OP_AND:
                    cond_op_and();
                    ++current_pos;
                    break;
                case COMPARE_OP_E:
                    compare_op_e();
                    ++current_pos;
                    break;
                case COMPARE_OP_EM:
                    compare_op_em();
                    ++current_pos;
                    break;
                case COMPARE_OP_B:
                    compare_op_b();
                    ++current_pos;
                    break;
                case COMPARE_OP_A:
                    compare_op_a();
                    ++current_pos;
                    break;
                case COMPARE_OP_AE:
                    compare_op_ae();
                    ++current_pos;
                    break;
                case COMPARE_OP_BE:
                    compare_op_be();
                    ++current_pos;
                    break;
                case COMPARE_OP_NE:
                    compare_op_ne();
                    ++current_pos;
                    break;
                case COMPARE_OP_NOT_IN:
                    compare_op_not_in();
                    ++current_pos;
                    break;
                case COMPARE_OP_IN:
                    compare_op_in();
                    ++current_pos;
                    break;
                case COMPARE_OP_IS:
                    compare_op_is();
                    ++current_pos;
                    break;
                case COMPARE_OP_IS_NOT:
                    compare_op_is_not();
                    ++current_pos;
                    break;
                case DEFAULT_FUNC_PARAM_VALUE:
                    default_func_param_value();
                    ++current_pos;
                    break;
                case DELETE_NAME:
                    delete_name(e_data->at(current_pos + 1)->svalue);
                    current_pos+=2;
                    break;
                case DELETE_ATTR:
                    delete_attr(e_data->at(current_pos + 1)->svalue);
                    current_pos+=2;
                    break;
                case DELETE_SUBSCR:
                    delete_subscr();
                    ++current_pos;
                    break;
                case DUP_TOP:
                    dup_top();
                    ++current_pos;
                    break;
                case END_FINALLY:
                    end_finally();
                    ++current_pos;
                    break;
                case FOR_ITER:
                    for_iter(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case GET_ITER:
                    get_iter(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case GET_YIELD_FROM_ITER:
                    get_yield_from_iter();
                    ++current_pos;
                    break;
                case IF:
                    cif();
                    ++current_pos;
                    break;
                case INPLACE_ADD:
                    inplace_add();
                    ++current_pos;
                    break;
                case INPLACE_SUBSTRACT:
                    inplace_substract();
                    ++current_pos;
                    break;
                case INPLACE_XOR:
                    inplace_xor();
                    ++current_pos;
                    break;
                case INPLACE_AND:
                    inplace_and();
                    ++current_pos;
                    break;
                case INPLACE_OR:
                    inplace_or();
                    ++current_pos;
                    break;
                case INPLACE_POWER:
                    inplace_power();
                    ++current_pos;
                    break;
                case INPLACE_MULTIPLY:
                    inplace_multiply();
                    ++current_pos;
                    break;
                case INPLACE_TRUE_DIVIDE:
                    inplace_true_divide();
                    ++current_pos;
                    break;
                case INPLACE_MATRIX_MULTIPLY:
                    inplace_matrix_multiply();
                    ++current_pos;
                    break;
                case INPLACE_FLOOR_DIVIDE:
                    inplace_floor_divide();
                    ++current_pos;
                    break;
                case INPLACE_MODULO:
                    inplace_modulo();
                    ++current_pos;
                    break;
                case IMPORT_NAME:
                    import_name(e_data->at(current_pos + 1)->svalue);
                    current_pos+=2;
                    break;
                case IMPORT_FROM:
                    import_from(e_data->at(current_pos + 1)->svalue);
                    current_pos+=2;
                    break;
                case KWARGS_PARAM_TYPE:
                    kwargs_param_type();
                    ++current_pos;
                    break;
                case END_FUNCTION_DEC:
                    end_function_dec();
                    ++current_pos;
                    break;
                case END_FUNC_PARAM:
                    end_func_param();
                    ++current_pos;
                    break;
                case END_TYPEDEF:
                    end_typedef();
                    ++current_pos;
                    break;
                case END_IF:
                    end_if();
                    ++current_pos;
                    break;
                case LOAD_CONST_STR:
                    load_const_str(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_HEX:
                    load_const_hex(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_BIN:
                    load_const_bin(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_USTR:
                    load_const_ustr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_BSTR:
                    load_const_bstr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_RSTR:
                    load_const_rstr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_EXP:
                    load_const_exp(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_MSTR:
                    load_const_mstr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_FSTR:
                    load_const_fstr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_BRSTR:
                    load_const_brstr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_RBSTR:
                    load_const_rbstr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_INT:
                    load_const_32(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case LOAD_CONST_FLOAT:
                    load_const_32f(e_data->at(current_pos + 1)->value.vfloat);
                    current_pos += 2;
                    break;
                case LOAD_CONST_IMG:
                    load_const_img(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_CONST_OCT:
                    load_const_oct(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_ATTR:
                    load_attr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_NAME:
                    load_name(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case LOAD_BUILD_CLASS:
                    load_build_class();
                    ++current_pos;
                    break;
                case MAKE_CLASS:
                    make_class();
                    ++current_pos;
                    break;
                case RETURN_VALUE:
                    return_value();
                    ++current_pos;
                    break;
                case ROT_TWO:
                    rot_two();
                    ++current_pos;
                    break;
                case ROT_THREE:
                    rot_three();
                    ++current_pos;
                    break;
                case STORE_NAME:
                    store_name(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case POP_BLOCK:
                    pop_block();
                    ++current_pos;
                    break;
                case POP_JUMP_IF_FALSE:
                    pop_jump_if_false(e_data->at(current_pos + 1)->value.v32);
                    break;
                case POP_JUMP_IF_TRUE:
                    pop_jump_if_true(e_data->at(current_pos + 1)->value.v32);
                    break;
                case POP_TOP:
                    pop_top();
                    ++current_pos;
                    break;
                case POP_EXCEPT:
                    pop_except();
                    ++current_pos;
                    break;
                case JUMP:
                    jump(e_data->at(current_pos + 1)->value.v32);
                    break;
                case JUMP_IF_TRUE_OR_POP:
                    jump_if_true_or_pop(e_data->at(current_pos + 1)->value.v32);
                    break;
                case JUMP_IF_FALSE_OR_POP:
                    jump_if_false_or_pop(e_data->at(current_pos + 1)->value.v32);
                    break;
                case RAISE_VARARGS:
                    raise_varargs(e_data->at(current_pos + 1)->value.v16);
                    current_pos += 2;
                    break;
                case STMT:
                    stmt(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case STORE_SUBSCR:
                    store_subscr();
                    ++current_pos;
                    break;
                case STORE_ATTR:
                    store_attr(e_data->at(current_pos + 1)->svalue);
                    current_pos += 2;
                    break;
                case SINGLE_TYPEDEF:
                    single_typedef();
                    ++current_pos;
                    break;
                case SETUP_EXCEPT:
                    setup_except(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case SETUP_FINALLY:
                    setup_finally(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case SETUP_LOOP:
                    setup_loop(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case SETUP_LOOP_FOR:
                    setup_loop_for(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case SETUP_LOOP_WHILE:
                    setup_loop_while(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case SETUP_WITH:
                    setup_with(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case UNPACK_SEQUENCE:
                    unpack_sequence(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case UNPACK_EX:
                    unpack_ex(e_data->at(current_pos + 1)->value.v32);
                    current_pos += 2;
                    break;
                case UNARY_NOT:
                    unary_not();
                    ++current_pos;
                    break;
                case UNARY_NEGATIVE:
                    unary_negative();
                    ++current_pos;
                    break;
                case UNARY_POSITIVE:
                    unary_positive();
                    ++current_pos;
                    break;
                case UNARY_INVERT:
                    unary_invert();
                    ++current_pos;
                    break;
                case WITH_CLEANUP_START:
                    with_cleanup_start();
                    ++current_pos;
                    break;
                case WITH_CLEANUP_FINISH:
                    with_cleanup_finish();
                    ++current_pos;
                    break;
                case YIELD_VALUE:
                    yield_value();
                    ++current_pos;
                    break;
                case YIELD_FROM:
                    yield_from();
                    ++current_pos;
                    break;
                default:
                    break;
                }

            }
        }
    }

}

