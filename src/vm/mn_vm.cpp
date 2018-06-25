/*************************************************************************/
/*  mn_vm.cpp                                                            */
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
#include <chrono>
#include <vm/mn_vm.h>

namespace mn
{

MN_VM::MN_VM(std::string name)
{
    this->name = name;
    m_pool = new MemoryPool<MNVariant, ME_MEMPOOL_SIZE>();
    c_reserve = 200;
    e_data = new std::vector<MNVariant*>();
    e_data->reserve(100);
}

MN_VM::~MN_VM()
{
    delete m_pool;
    delete e_data;
}

_FORCE_INLINE_ void MN_VM::push_back(MNVariant *e)
{
    if (e_data->size() > c_reserve-1)
    {
        e_data->reserve(100);
        c_reserve += 100;
    }
    e_data->push_back(e);
}

void MN_VM::add_command(MNPyByteCode type)
{
    auto e = m_pool->allocate();
    //m_pool.construct(e, meta_element());
    e->type = type;
    push_back(e);
}

void MN_VM::add_i16(int16_t value)
{
    auto e = m_pool->allocate();
    //m_pool.construct(e, meta_element());
    e->type = CONST_16;
    e->value.v32 = value;
    push_back(e);
}
void MN_VM::add_i32(int32_t value)
{
    auto e = m_pool->allocate();
    //m_pool.construct(e, meta_element());
    e->type = CONST_INT;
    e->value.v32 = value;
    push_back(e);
}

void MN_VM::add_float(float value)
{
    auto e = m_pool->allocate();
    //m_pool.construct(e, meta_element());
    e->type = CONST_FLOAT;
    e->value.vfloat = value;
    push_back(e);
}

void MN_VM::add_str(std::string value)
{
    auto e = m_pool->allocate();
    m_pool->construct(e, MNVariant());
    e->type = CONST_STR;
    e->svalue = value;
    // e->svalue = 0;
    push_back(e);
}

void MN_VM::add_element(MNPyByteCode type, unsigned int value)
{
    auto e = m_pool->allocate();
    //m_pool.construct(e, meta_element());
    e->type = type;
    e->value.v32 = value;
    push_back(e);
}

void MN_VM::exec_traveler(MNCTrav *trav)
{
    trav->start();
}

void MN_VM::process()
{
    MN_DataStream data_in;
    data_in.open(name.c_str(), 1);
    int16_t current = data_in.ri16();

    unsigned int current_pos = 0;
    long int current_inst_pos = 0;
    dis << std::endl;
    std::string tab = "        ";
    dis.clear();
    //unsigned int stmt_count = 1;

    //dis << stmt_count << " -> Line: 1 " << std::endl << std::endl;

    auto im_command = [&](MNPyByteCode type, std::string type_str)
    {
        std::string spaces = _gen_opcode_spaces(type_str);
        int32_t value = data_in.ri32();
        dis << tab << current_inst_pos << " -- " << type_str.c_str() << spaces << " ("
                  << value << ")" << std::endl;
        current_pos += 4;
        current_inst_pos ++;
        add_command(type);
        add_i32(value);
    };

    auto im_command_real = [&](MNPyByteCode type, std::string type_str)
    {
        std::string spaces = _gen_opcode_spaces(type_str.c_str());
        float value = data_in.rfloat();
        dis << tab << current_inst_pos << " -- " << type_str.c_str() << spaces << " ("
                  << value << ")" << std::endl;
        current_pos += 4;
        current_inst_pos ++;
        add_command(type);
        add_float(value);
    };

    auto im_command_str = [&](MNPyByteCode type, std::string type_str)
    {
        std::string spaces = _gen_opcode_spaces(type_str);
        int16_t pos = data_in.ri16();
        int16_t key = data_in.ri16();
        dis << tab << current_inst_pos << " -- " << type_str.c_str() << spaces << " ("
                  << pos << " positional) -- ("
                  << key << " keyword pair)" << std::endl;
        current_pos += 4;
        current_inst_pos += 2;
        add_command(type);
        add_i16(pos);
        add_i16(key);
    };

    auto imm_command_sparam = [&](MNPyByteCode type, std::string type_str)
    {
        std::string spaces = _gen_opcode_spaces(type_str);
        int32_t str_count = data_in.ri32();
        std::string rstr = data_in.rstr(str_count);
        dis << tab << current_inst_pos << " -- " << type_str.c_str() << spaces << " ("
                  << rstr.c_str()
                  << ")" << std::endl;
        current_pos += 4 + str_count;
        current_inst_pos += 1;
        add_command(type);
        //add_i32(str_count);
        add_str(rstr);
    };
    auto start = std::chrono::system_clock::now();
    while (current != MNPyByteCode::HALT)
    {

        switch (current) {
        case ARGS_PARAM_TYPE:
            dis << tab << current_inst_pos << " -- ARGS_PARAM_TYPE" << std::endl;
            add_command(ARGS_PARAM_TYPE);
            break;
        case BEGIN_FUNCTION_DEC:
            dis << tab << current_inst_pos << " -- BEGIN_FUNCTION_DEC" << std::endl;
            add_command(BEGIN_FUNCTION_DEC);
            break;
        case BEGIN_FUNC_PARAM:
            dis << tab << current_inst_pos << " -- BEGIN_FUNC_PARAM" << std::endl;
            add_command(BEGIN_FUNC_PARAM);
            break;
        case BEGIN_TYPEDEF:
            dis << tab << current_inst_pos << " -- BEGIN_TYPEDEF" << std::endl;
            add_command(BEGIN_TYPEDEF);
            break;
        case BINARY_ADD:
            dis << tab << current_inst_pos << " -- BINARY_ADD" << std::endl;
            add_command(BINARY_ADD);
            break;
        case BINARY_AND:
            dis << tab << current_inst_pos << " -- BINARY_AND" << std::endl;
            add_command(BINARY_AND);
            break;
        case BINARY_XOR:
            dis << tab << current_inst_pos << " -- BINARY_XOR" << std::endl;
            add_command(BINARY_XOR);
            break;
        case BINARY_OR:
            dis << tab << current_inst_pos << " -- BINARY_OR" << std::endl;
            add_command(BINARY_OR);
            break;
        case BINARY_LSHIFT:
            dis << tab << current_inst_pos << " -- BINARY_LSHIFT" << std::endl;
            add_command(BINARY_LSHIFT);
            break;
        case BINARY_RSHIFT:
            dis << tab << current_inst_pos << " -- BINARY_RSHIFT" << std::endl;
            add_command(BINARY_RSHIFT);
            break;
        case BINARY_SUBSTRACT:
            dis << tab << current_inst_pos << " -- BINARY_SUBSTRACT" << std::endl;
            add_command(BINARY_SUBSTRACT);
            break;
        case BINARY_SUBSCR:
            dis << tab << current_inst_pos << " -- BINARY_SUBSCR" << std::endl;
            add_command(BINARY_SUBSCR);
            break;
        case BINARY_POWER:
            dis << tab << current_inst_pos << " -- BINARY_POWER" << std::endl;
            add_command(BINARY_POWER);
            break;
        case BINARY_MULTIPLY:
            dis << tab << current_inst_pos << " -- BINARY_MULTIPLY" << std::endl;
            add_command(BINARY_MULTIPLY);
            break;
        case BINARY_TRUE_DIVIDE:
            dis << tab << current_inst_pos << " -- BINARY_TRUE_DIVIDE" << std::endl;
            add_command(BINARY_TRUE_DIVIDE);
            break;
        case BINARY_MATRIX_MULTIPLY:
            dis << tab << current_inst_pos << " -- BINARY_MATRIX_MULTIPLY" << std::endl;
            add_command(BINARY_MATRIX_MULTIPLY);
            break;
        case BINARY_FLOOR_DIVIDE:
            dis << tab << current_inst_pos << " -- BINARY_FLOOR_DIVIDE" << std::endl;
            add_command(BINARY_FLOOR_DIVIDE);
            break;
        case BINARY_MODULO:
            dis << tab << current_inst_pos << " -- BINARY_MODULO" << std::endl;
            add_command(BINARY_MODULO);
            break;
        case BUILD_TUPLE:
            im_command(BUILD_TUPLE, "BUILD_TUPLE");
            break;
        case BUILD_LIST:
            im_command(BUILD_LIST, "BUILD_LIST");
            break;
        case BUILD_LIST_UNPACK:
            im_command(BUILD_LIST_UNPACK, "BUILD_LIST_UNPACK");
            break;
        case BUILD_TUPLE_UNPACK:
            im_command(BUILD_TUPLE_UNPACK, "BUILD_TUPLE_UNPACK");
            break;
        case BUILD_SET_UNPACK:
            im_command(BUILD_SET_UNPACK, "BUILD_SET_UNPACK");
            break;
        case BUILD_SLICE:
            im_command(BUILD_SLICE, "BUILD_SLICE");
            break;
        case BUILD_SET:
            im_command(BUILD_SET, "BUILD_SET");
            break;
        case BUILD_STRING:
            im_command(BUILD_STRING, "BUILD_STRING");
            break;
        case BUILD_MAP:
            im_command(BUILD_MAP, "BUILD_MAP");
            break;
        case BUILD_MAP_UNPACK_WITH_CALL:
            im_command(BUILD_MAP_UNPACK_WITH_CALL, "BUILD_MAP_UNPACK_WITH_CALL");
            break;
        case BREAK_LOOP:
            dis << tab << current_inst_pos << " -- BREAK_LOOP" << std::endl;
            add_command(BREAK_LOOP);
            break;
        case CALL_FUNCTION:
            im_command_str(CALL_FUNCTION, "CALL_FUNCTION");
            break;
        case CALL_FUNCTION_VAR:
            im_command_str(CALL_FUNCTION_VAR, "CALL_FUNCTION_VAR");
            break;
        case CALL_FUNCTION_VAR_KW:
            im_command_str(CALL_FUNCTION_VAR_KW, "CALL_FUNCTION_VAR_KW");
            break;
        case CALL_FUNCTION_KW:
            im_command_str(CALL_FUNCTION_KW, "CALL_FUNCTION_KW");
            break;
        case COND_OP_AND:
            dis << tab << current_inst_pos << " -- COND_OP_AND" << std::endl;
            add_command(COND_OP_AND);
            break;
        case COMPARE_OP_E:
            dis << tab << current_inst_pos << " -- COMPARE_OP_E" << std::endl;
            add_command(COMPARE_OP_E);
            break;
        case COMPARE_OP_EM:
            dis << tab << current_inst_pos << " -- COMPARE_OP_EM" << std::endl;
            add_command(COMPARE_OP_EM);
            break;
        case COMPARE_OP_B:
            dis << tab << current_inst_pos << " -- COMPARE_OP_B" << std::endl;
            add_command(COMPARE_OP_B);
            break;
        case COMPARE_OP_A:
            dis << tab << current_inst_pos << " -- COMPARE_OP_A" << std::endl;
            add_command(COMPARE_OP_A);
            break;
        case COMPARE_OP_AE:
            dis << tab << current_inst_pos << " -- COMPARE_OP_AE" << std::endl;
            add_command(COMPARE_OP_AE);
            break;
        case COMPARE_OP_BE:
            dis << tab << current_inst_pos << " -- COMPARE_OP_BE" << std::endl;
            add_command(COMPARE_OP_BE);
            break;
        case COMPARE_OP_NE:
            dis << tab << current_inst_pos << " -- COMPARE_OP_NE" << std::endl;
            add_command(COMPARE_OP_NE);
            break;
        case COMPARE_OP_NOT_IN:
            dis << tab << current_inst_pos << " -- COMPARE_OP_NOT_IN" << std::endl;
            add_command(COMPARE_OP_NOT_IN);
            break;
        case COMPARE_OP_IN:
            dis << tab << current_inst_pos << " -- COMPARE_OP_IN" << std::endl;
            add_command(COMPARE_OP_IN);
            break;
        case COMPARE_OP_IS:
            dis << tab << current_inst_pos << " -- COMPARE_OP_IS" << std::endl;
            add_command(COMPARE_OP_IS);
            break;
        case COMPARE_OP_IS_NOT:
            dis << tab << current_inst_pos << " -- COMPARE_OP_IS_NOT" << std::endl;
            add_command(COMPARE_OP_IS_NOT);
            break;
        case DEFAULT_FUNC_PARAM_VALUE:
            dis << tab << current_inst_pos << " -- DEFAULT_FUNC_PARAM_VALUE" << std::endl;
            add_command(DEFAULT_FUNC_PARAM_VALUE);
            break;
        case DELETE_NAME:
            imm_command_sparam(DELETE_NAME, "DELETE_NAME");
            break;
        case DELETE_ATTR:
            imm_command_sparam(DELETE_ATTR, "DELETE_ATTR");
            break;
        case DELETE_SUBSCR:
            dis << tab << current_inst_pos << " -- DELETE_SUBSCR" << std::endl;
            add_command(DELETE_SUBSCR);
            break;
        case DUP_TOP:
            dis << tab << current_inst_pos << " -- DUP_TOP" << std::endl;
            add_command(DUP_TOP);
            break;
        case END_FINALLY:
            dis << tab << current_inst_pos << " -- END_FINALLY" << std::endl;
            add_command(END_FINALLY);
            break;
        case FOR_ITER:
            im_command(FOR_ITER, "FOR_ITER");
            break;
        case GET_ITER:
            dis << tab << current_inst_pos << " -- GET_ITER" << std::endl;
            add_command(GET_ITER);
            break;
        case GET_YIELD_FROM_ITER:
            dis << tab << current_inst_pos << " -- GET_YIELD_FROM_ITER" << std::endl;
            add_command(GET_YIELD_FROM_ITER);
            break;
        case IF:
            dis << tab << current_inst_pos << " -- BEGIN_IF" << std::endl;
            add_command(IF);
            break;
        case INPLACE_ADD:
            dis << tab << current_inst_pos << " -- INPLACE_ADD" << std::endl;
            add_command(INPLACE_ADD);
            break;
        case INPLACE_SUBSTRACT:
            dis << tab << current_inst_pos << " -- INPLACE_SUBSTRACT" << std::endl;
            add_command(INPLACE_SUBSTRACT);
            break;
        case INPLACE_XOR:
            dis << tab << current_inst_pos << " -- INPLACE_XOR" << std::endl;
            add_command(INPLACE_XOR);
            break;
        case INPLACE_AND:
            dis << tab << current_inst_pos << " -- INPLACE_AND" << std::endl;
            add_command(INPLACE_AND);
            break;
        case INPLACE_OR:
            dis << tab << current_inst_pos << " -- INPLACE_OR" << std::endl;
            add_command(INPLACE_OR);
            break;
        case INPLACE_POWER:
            dis << tab << current_inst_pos << " -- INPLACE_POWER" << std::endl;
            add_command(INPLACE_POWER);
            break;
        case INPLACE_MULTIPLY:
            dis << tab << current_inst_pos << " -- INPLACE_MULTIPLY" << std::endl;
            add_command(INPLACE_MULTIPLY);
            break;
        case INPLACE_TRUE_DIVIDE:
            dis << tab << current_inst_pos << " -- INPLACE_TRUE_DIVIDE" << std::endl;
            add_command(INPLACE_TRUE_DIVIDE);
            break;
        case INPLACE_MATRIX_MULTIPLY:
            dis << tab << current_inst_pos << " -- INPLACE_MATRIX_MULTIPLY" << std::endl;
            add_command(INPLACE_MATRIX_MULTIPLY);
            break;
        case INPLACE_FLOOR_DIVIDE:
            dis << tab << current_inst_pos << " -- INPLACE_FLOOR_DIVIDE" << std::endl;
            add_command(INPLACE_FLOOR_DIVIDE);
            break;
        case INPLACE_MODULO:
            dis << tab << current_inst_pos << " -- INPLACE_MODULO" << std::endl;
            add_command(INPLACE_MODULO);
            break;
        case IMPORT_NAME:
            imm_command_sparam(IMPORT_NAME, "IMPORT_NAME");
            break;
        case IMPORT_FROM:
            imm_command_sparam(IMPORT_FROM, "IMPORT_FROM");
            break;
        case KWARGS_PARAM_TYPE:
            dis << tab << current_inst_pos << " -- KWARGS_PARAM_TYPE" << std::endl;
            add_command(KWARGS_PARAM_TYPE);
            break;
        case END_FUNCTION_DEC:
            dis << tab << current_inst_pos << " -- END_FUNCTION_DEC" << std::endl;
            add_command(END_FUNCTION_DEC);
            break;
        case END_FUNC_PARAM:
            dis << tab << current_inst_pos << " -- END_FUNC_PARAM" << std::endl;
            add_command(END_FUNC_PARAM);
            break;
        case END_TYPEDEF:
            dis << tab << current_inst_pos << " -- END_TYPEDEF" << std::endl;
            add_command(END_TYPEDEF);
            break;
        case END_IF:
            dis << tab << current_inst_pos << " -- END_IF" << std::endl;
            add_command(END_IF);
            break;
        case LOAD_CONST_STR:
            imm_command_sparam(LOAD_CONST_STR, "LOAD_CONST_STR");
            break;
        case LOAD_CONST_HEX:
            imm_command_sparam(LOAD_CONST_HEX, "LOAD_CONST_HEX");
            break;
        case LOAD_CONST_BIN:
            imm_command_sparam(LOAD_CONST_BIN, "LOAD_CONST_BIN");
            break;
        case LOAD_CONST_USTR:
            imm_command_sparam(LOAD_CONST_USTR, "LOAD_CONST_USTR");
            break;
        case LOAD_CONST_BSTR:
            imm_command_sparam(LOAD_CONST_BSTR, "LOAD_CONST_BSTR");
            break;
        case LOAD_CONST_RSTR:
            imm_command_sparam(LOAD_CONST_RSTR, "LOAD_CONST_RSTR");
            break;
        case LOAD_CONST_EXP:
            imm_command_sparam(LOAD_CONST_EXP, "LOAD_CONST_EXP");
            break;
        case LOAD_CONST_MSTR:
            imm_command_sparam(LOAD_CONST_MSTR, "LOAD_CONST_MSTR");
            break;
        case LOAD_CONST_FSTR:
            imm_command_sparam(LOAD_CONST_FSTR, "LOAD_CONST_FSTR");
            break;
        case LOAD_CONST_BRSTR:
            imm_command_sparam(LOAD_CONST_BRSTR, "LOAD_CONST_BRSTR");
            break;
        case LOAD_CONST_RBSTR:
            imm_command_sparam(LOAD_CONST_RBSTR, "LOAD_CONST_RBSTR");
            break;
        case LOAD_CONST_INT:
            im_command (LOAD_CONST_INT, "LOAD_CONST_INT");
            break;
        case LOAD_CONST_FLOAT:
            im_command_real(LOAD_CONST_FLOAT, "LOAD_CONST_FLOAT");
            break;
        case LOAD_CONST_IMG:
            imm_command_sparam(LOAD_CONST_IMG, "LOAD_CONST_IMG");
            break;
        case LOAD_CONST_OCT:
            imm_command_sparam(LOAD_CONST_OCT, "LOAD_CONST_OCT");
            break;
        case LOAD_ATTR:
            imm_command_sparam(LOAD_ATTR, "LOAD_ATTR");
            break;
        case LOAD_NAME:
            imm_command_sparam(LOAD_NAME, "LOAD_NAME");
            break;
        case LOAD_BUILD_CLASS:
            dis << tab << current_inst_pos << " -- LOAD_BUILD_CLASS" << std::endl;
            add_command(LOAD_BUILD_CLASS);
            break;
        case MAKE_CLASS:
            dis << tab << current_inst_pos << " -- MAKE_CLASS" << std::endl;
            add_command(MAKE_CLASS);
            break;
        case RETURN_VALUE:
            dis << tab << current_inst_pos << " -- RETURN_VALUE" << std::endl;
            add_command(RETURN_VALUE);
            break;
        case ROT_TWO:
            dis << tab << current_inst_pos << " -- ROT_TWO" << std::endl;
            add_command(ROT_TWO);
            break;
        case ROT_THREE:
            dis << tab << current_inst_pos << " -- ROT_THREE" << std::endl;
            add_command(ROT_THREE);
            break;
        case STORE_NAME:
            imm_command_sparam(STORE_NAME, "STORE_NAME");
            break;
        case POP_BLOCK:
            dis << tab << current_inst_pos << " -- POP_BLOCK" << std::endl;
            add_command(POP_BLOCK);
            break;

        case POP_JUMP_IF_FALSE:
            im_command(POP_JUMP_IF_FALSE, "POP_JUMP_IF_FALSE");
            break;
        case POP_JUMP_IF_TRUE:
            im_command(POP_JUMP_IF_TRUE, "POP_JUMP_IF_TRUE");
            break;
        case POP_TOP:
            dis << tab << current_inst_pos << " -- POP_TOP" << std::endl;
            add_command(POP_TOP);
            break;
        case POP_EXCEPT:
            dis << tab << current_inst_pos << " -- POP_EXCEPT" << std::endl;
            add_command(POP_EXCEPT);
            break;
        case JUMP:
            im_command(JUMP, "JUMP");
            break;
        case JUMP_IF_TRUE_OR_POP:
            im_command(JUMP_IF_TRUE_OR_POP, "JUMP_IF_TRUE_OR_POP");
            break;
        case JUMP_IF_FALSE_OR_POP:
            im_command(JUMP_IF_FALSE_OR_POP, "JUMP_IF_FALSE_OR_POP");
            break;
        case RAISE_VARARGS:
        {
            int16_t value = data_in.ri16();
            std::string spaces = _gen_opcode_spaces("RAISE_VARARGS");
            dis << tab << current_inst_pos << " -- RAISE_VARARGS " << spaces << " ("
                      << value << ")" << std::endl;
            current_pos += 2;
            current_inst_pos ++;
            add_command(RAISE_VARARGS);
            add_i16(value);
        }
            break;
        case STMT:
            {
                int32_t value = data_in.ri32();
                dis << /*++stmt_count <<*/ " Line: " << value
                          << std::endl << std::endl;
                //data_in.ri16();
                current_pos += 4;
                current_inst_pos ++;
                add_command(STMT);
                add_i32(value);
            }
            break;
        case STORE_SUBSCR:
            dis << tab << current_inst_pos << " -- STORE_SUBSCR" << std::endl;
            add_command(STORE_SUBSCR);
            break;
        case STORE_ATTR:
            imm_command_sparam(STORE_ATTR, "STORE_ATTR");
            break;
        case SINGLE_TYPEDEF:
            dis << tab << current_inst_pos << " -- SINGLE_TYPEDEF" << std::endl;
            add_command(SINGLE_TYPEDEF);
            break;
        case SETUP_EXCEPT:
            im_command(SETUP_EXCEPT, "SETUP_EXCEPT");
            break;
        case SETUP_FINALLY:
            im_command(SETUP_FINALLY, "SETUP_FINALLY");
            break;
        case SETUP_LOOP:
            im_command(SETUP_LOOP, "SETUP_LOOP");
            break;
        case SETUP_LOOP_FOR:
            im_command(SETUP_LOOP_FOR, "SETUP_LOOP_FOR");
            break;
        case SETUP_LOOP_WHILE:
            im_command(SETUP_LOOP_WHILE, "SETUP_LOOP_WHILE");
            break;
        case SETUP_WITH:
            im_command(SETUP_WITH, "SETUP_WITH");
            break;
        case UNPACK_SEQUENCE:
            im_command(UNPACK_SEQUENCE, "UNPACK_SEQUENCE");
            break;
        case UNPACK_EX:
            im_command(UNPACK_EX, "UNPACK_EX");
            break;
        case UNARY_NOT:
            dis << tab << current_inst_pos << " -- UNARY NOT" << std::endl;
            add_command(UNARY_NOT);
            break;
        case UNARY_NEGATIVE:
            dis << tab << current_inst_pos << " -- UNARY_NEGATIVE" << std::endl;
            add_command(UNARY_NEGATIVE);
            break;
        case UNARY_POSITIVE:
            dis << tab << current_inst_pos << " -- UNARY_POSITIVE" << std::endl;
            add_command(UNARY_POSITIVE);
            break;
        case UNARY_INVERT:
            dis << tab << current_inst_pos << " -- UNARY_INVERT" << std::endl;
            add_command(UNARY_INVERT);
            break;
        case WITH_CLEANUP_START:
            dis << tab << current_inst_pos << " -- WITH_CLEANUP_START" << std::endl;
            add_command(WITH_CLEANUP_START);
            break;
        case WITH_CLEANUP_FINISH:
            dis << tab << current_inst_pos << " -- WITH_CLEANUP_FINISH" << std::endl;
            add_command(WITH_CLEANUP_FINISH);
            break;
        case YIELD_VALUE:
            dis << tab << current_inst_pos << " -- YIELD_VALUE" << std::endl;
            add_command(YIELD_VALUE);
            break;
        case YIELD_FROM:
            dis << tab << current_inst_pos << " -- YIELD_FROM" << std::endl;
            add_command(YIELD_FROM);
            break;

        default:
            break;
        }
        data_in >> current;
        current_pos += 2;
        current_inst_pos++;
    }
    dis << tab << current_inst_pos << " -- HALT" << std::endl;
    dis <<  " ____________________ " << std::endl << std::endl;
    dis <<  "Total: " << current_pos+2 << " bytes" << std::endl;
    dis <<  " ____________________ " << std::endl << std::endl;
    std::cout << dis.str() << std::endl;
    auto ct = new mn::MNInterp(e_data);
    ct->start();
    delete ct;
    std::chrono::duration<double> dur= std::chrono::system_clock::now() - start;
    std::cout << "time native: " << dur.count() << " seconds" << std::endl;
}
std::string MN_VM::_gen_opcode_spaces(std::string opcode)
{
    unsigned int spaces = 30 - opcode.size();
    std::string result = "";
    for (unsigned int i=0; i < spaces; ++i) result += " ";
    return result;
}
}
