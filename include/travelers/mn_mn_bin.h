/*************************************************************************/
/*  mn_mn_bin.h                                                          */
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
#ifndef MN_MNBIN_H
#define MN_MNBIN_H

#include <iostream>
#include <mn_data_struct.h>
#include <mn_traveler.h>
#include <mn_data_stream.h>
#include <string>
#include <memory>

enum MNPyByteCode
{
    ARGS_PARAM_TYPE,
    BEGIN_FUNCTION_DEC,
    BEGIN_FUNC_PARAM,
    BEGIN_TYPEDEF,
    BINARY_ADD,
    BINARY_AND,
    BINARY_XOR,
    BINARY_OR,
    BINARY_FLOOR_DIVIDE,
    BINARY_LSHIFT,
    BINARY_MULTIPLY,
    BINARY_MATRIX_MULTIPLY,
    BINARY_MODULO,
    BINARY_RSHIFT,
    BINARY_TRUE_DIVIDE,
    BINARY_SUBSTRACT,
    BINARY_SUBSCR,
    BINARY_POWER,
    BUILD_TUPLE,
    BUILD_LIST,
    BUILD_LIST_UNPACK,
    BUILD_TUPLE_UNPACK,
    BUILD_SLICE,
    BUILD_MAP,
    BUILD_SET,
    BUILD_SET_UNPACK,
    BUILD_MAP_UNPACK_WITH_CALL,
    BUILD_STRING,
    BREAK_LOOP,
    COND_OP_AND,
    CALL_FUNCTION,
    CALL_FUNCTION_VAR,
    CALL_FUNCTION_VAR_KW,
    CALL_FUNCTION_KW,
    COMPARE_OP_E,
    COMPARE_OP_EM,
    COMPARE_OP_B,
    COMPARE_OP_A,
    COMPARE_OP_AE,
    COMPARE_OP_BE,
    COMPARE_OP_NE,
    COMPARE_OP_NOT_IN,
    COMPARE_OP_IN,
    COMPARE_OP_IS,
    COMPARE_OP_IS_NOT,
    CONST_16,
    CONST_INT,
    CONST_FLOAT,
    CONST_STR,
    DEFAULT_FUNC_PARAM_VALUE,
    DELETE_NAME,
    DELETE_ATTR,
    DELETE_SUBSCR,
    DUP_TOP,
    END_FUNCTION_DEC,
    END_FUNC_PARAM,
    END_IF,
    END_TYPEDEF,
    END_FINALLY,
    FOR_ITER,
    GET_ITER,
    GET_YIELD_FROM_ITER,
    HALT,
    IF,
    INPLACE_ADD,
    INPLACE_FLOOR_DIVIDE,
    INPLACE_LSHIFT,
    INPLACE_MULTIPLY,
    INPLACE_MATRIX_MULTIPLY,
    INPLACE_MODULO,
    INPLACE_TRUE_DIVIDE,
    INPLACE_SUBSTRACT,
    INPLACE_XOR,
    INPLACE_AND,
    INPLACE_OR,
    INPLACE_POWER,
    IMPORT_FROM,
    IMPORT_NAME,
    KWARGS_PARAM_TYPE,
    LOAD_ATTR,
    LOAD_CONST_STR,
    LOAD_CONST_USTR,
    LOAD_CONST_BSTR,
    LOAD_CONST_RSTR,
    LOAD_CONST_MSTR,
    LOAD_CONST_FSTR,
    LOAD_CONST_BRSTR,
    LOAD_CONST_RBSTR,
    LOAD_CONST_8,
    LOAD_CONST_16,
    LOAD_CONST_32,
    LOAD_CONST_INT,
    LOAD_CONST_FLOAT,
    LOAD_CONST_32F,
    LOAD_CONST_64,
    LOAD_CONST_64F,
    LOAD_CONST_IMG,
    LOAD_CONST_HEX,
    LOAD_CONST_BIN,
    LOAD_CONST_OCT,
    LOAD_CONST_EXP,
    LOAD_BUILD_CLASS,
    LOAD_NAME,
    MAKE_CLASS,
    MN_ERROR,
    POP_BLOCK,
    POP_JUMP_IF_FALSE,
    POP_JUMP_IF_TRUE,
    POP_TOP,
    POP_EXCEPT,
    JUMP,
    JUMP_IF_TRUE_OR_POP,
    JUMP_IF_FALSE_OR_POP,
    RAISE_VARARGS,
    RETURN_VALUE,
    ROT_TWO,
    ROT_THREE,
    STORE_NAME,
    STORE_SUBSCR,
    STORE_ATTR,
    SINGLE_TYPEDEF,
    SETUP_LOOP,
    SETUP_EXCEPT,
    SETUP_FINALLY,
    SETUP_LOOP_FOR,
    SETUP_LOOP_WHILE,
    SETUP_WITH,
    STMT,
    UNPACK_SEQUENCE,
    UNPACK_EX,
    UNARY_NOT,
    UNARY_NEGATIVE,
    UNARY_POSITIVE,
    UNARY_INVERT,
    WITH_CLEANUP_START,
    WITH_CLEANUP_FINISH,
    YIELD_VALUE,
    YIELD_FROM
};

enum MN_PYBINP_DATA
{
    MN_AND = 1,
    MN_OR,
    MN_END,
    MN_EXCEPT,
    MN_COMP_FOR,
    MN_FUNC,
    MN_LT,
    MN_TEST
};

struct MN_Jump_Data
{
    unsigned int  pos;
    unsigned int jump_type;
    unsigned int level;
    unsigned int j_pos;
};

struct MN_COMP_FORIF
{
    std::vector<unsigned int>* values;
    std::vector<std::vector<MN_Jump_Data>*>* if_values;

};


class MN_PyBinType
{
public:
    MN_PYBINP_DATA type;
    void* meta;
    ~MN_PyBinType()
    {
        switch (type) {
        case MN_PYBINP_DATA::MN_OR:
        case MN_PYBINP_DATA::MN_AND:
            delete (std::vector<MN_Jump_Data>*)meta;
            break;
        case MN_PYBINP_DATA::MN_COMP_FOR:
            {
                auto m = (MN_COMP_FORIF*)meta;
                delete m->values;
                delete m->if_values;
                delete m;
            }
            break;
        case MN_PYBINP_DATA::MN_LT:
            delete [] (unsigned int*)meta;
        default:
            break;
        }
    }
};





namespace mn
{
    class MN_MNBinRead
    {
    public:
         MN_MNBinRead(std::string name) {this->name = name;}
         void show_bincode();
    private:
         std::string _gen_opcode_spaces(std::string opcode);
         std::string name;
    };

    class MN_MNBin: public MNTraveler<MN_MNBin, void*>
    {
        public:
            MN_MNBin(std::string name);
            virtual ~MN_MNBin();
            void* walk(MNNode* node);
            void* file_input(MNNode* node);
            void* decorated(MNNode* node);
            void* funcdef(MNNode* node);
            void* parameters(MNNode* node);
            void* typedargslist(MNNode* node);
            void* tfpdef(MNNode* node);
            void* vfpdef(MNNode* node);
            void* small_stmt(MNNode* node);
            void* expr_stmt(MNNode* node);
            void* testlist_star_expr(MNNode* node);
            void* del_stmt(MNNode* node);
            void* break_stmt(MNNode* node);
            void* return_stmt(MNNode* node);
            void* yield_stmt(MNNode* node);
            void* raise_stmt(MNNode* node);
            void* import_name(MNNode* node);
            void* import_from(MNNode *node);
            void* import_as_name(MNNode *node);
            void* dotted_as_name(MNNode* node);
            void* dotted_name(MNNode* node);
            void* assert_stmt(MNNode* node);
            void* stmt(MNNode* node);
            void* simple_stmt(MNNode* node);
            void* if_stmt(MNNode* node);
            void* while_stmt(MNNode* node);
            void* for_stmt(MNNode* node);
            void* try_stmt(MNNode* node);
            void* with_stmt(MNNode* node);
            void* with_item(MNNode* node);
            void* except_clause(MNNode* node);
            void* suite(MNNode* node);
            void* test(MNNode* node);
            void* lambdef(MNNode* node);
            void* or_test(MNNode* node);
            void* not_test(MNNode* node);
            void* and_test(MNNode* node);
            void* comparison(MNNode* node);
            void* star_expr(MNNode* node);
            void* expr(MNNode* node);
            void* xor_expr(MNNode* node);
            void* and_expr(MNNode* node);
            void* shift_expr(MNNode* node);
            void* arith_expr(MNNode* node);
            void* term(MNNode* node);
            void* factor(MNNode* node);
            void* power(MNNode* node);
            void* atom_expr(MNNode* node);
            void* atom(MNNode* node);
            void* testlist_comp(MNNode* node);
            void* trailer(MNNode* node);
            void* subscript(MNNode* node);
            void* arglist(MNNode* node);
            void* testlist(MNNode* node);
            void* dictorsetmaker(MNNode* node);
            void* classdef(MNNode* node);
            void* comp_for(MNNode* node);
            void* comp_if(MNNode* node);
            void* f_string(MNNode* node);
            void* replacement_field(MNNode* node);
            void* f_expression(MNNode* node);
            void* conditional_expression(MNNode* node);
            void* expression(MNNode* node);
            void* format_spec(MNNode* node);
            void* lambda_expr(MNNode* node);
            void* yield_expression(MNNode* node);
            void* expression_list(MNNode* node);
            void* not_impl(MNNode* node);

        protected:
            //MNTraveler<MN_MNBin>* trav;
            unsigned int depth;
            std::string space_gen(unsigned int count);
            MN_DataStream data;
            bool store_flag;
            bool if_flag;
            bool if_stmt_flag;
            bool jump_flag;
            bool unpack_flag;
            bool build_lt_flag;
            bool dup_top_flag;
            bool call_gen;
            int store_sscr_load;
            bool last_trailer;
            bool last_stmt;
            bool del_flag;
            int trailer_level;
            std::string current_imp_name;
            bool import_from_flag;
            unsigned int current_line;
            unsigned int and_or_level;
            std::vector <unsigned int> comp_hist;
            std::vector <unsigned int> unpack_values;
            std::vector <int> build_list;
        protected:
            void* create_bin_type(MN_PYBINP_DATA type, void* meta=nullptr);
            std::shared_ptr<MN_PyBinType> bin_cast(void* value);
            MN_Jump_Data create_jump_data(unsigned int pos, int jump_type, unsigned int level, unsigned int j_pos = 0);
            long int _process_if_stmt(MNNode node, MNNode stmt, unsigned int offset = 0);
            void* _process_type(MNNode* node);
            void _gen_str(MNPyByteCode type, std::string value);
            void _gen_loadname(std::string value);
            void _gen_storename(std::string value);
            void _gen_container_type();
            bool _stpack(MNNode* node);
            void _r_add(std::shared_ptr<MN_PyBinType>& node, bool reduce=true);
            void _v_add(std::shared_ptr<MN_PyBinType>& node, std::vector<MN_Jump_Data>* values);
            bool _test_if_list(MNNode& node, MNNode& second);
            bool _is_and_or(std::shared_ptr<MN_PyBinType> type);
            void _process_jumps (unsigned int type, bool cond, std::shared_ptr<MN_PyBinType> expr,
                                 std::vector<MN_Jump_Data>* values=nullptr, int meta_val=0);
            void* _and_or_test(unsigned int type, MNNode* node);
            void _op_err(MNNode* node, std::string type);
            void _add_line(MNNode* node);

    };
}

#endif // MN_MNBIN_H
