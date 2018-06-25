/*************************************************************************/
/*  mn_mn_bin.cpp                                                        */
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
#include "travelers/mn_mn_bin.h"
#include <stdlib.h>
namespace mn
{
    MN_MNBin::MN_MNBin(std::string name):MNTraveler<mn::MN_MNBin, void*>()
    {
        depth = 0;
        store_flag = false;
        if_flag = false;
        jump_flag = true;
        unpack_flag = false;
        build_lt_flag = true;
        dup_top_flag = false;
        call_gen = true;
        store_sscr_load = -1;
        last_trailer = false;
        if_stmt_flag = false;
        last_stmt = false;
        del_flag = false;
        current_line = 0;
        and_or_level = 0;
        trailer_level = 0;
        current_imp_name = "";
        import_from_flag = false;

        bind("file_input", &MN_MNBin::file_input, this);
        bind("eval_input", &MN_MNBin::not_impl, this);
        bind("decorator", &MN_MNBin::not_impl, this);
        bind("decorators", &MN_MNBin::not_impl, this);
        bind("decorated", &MN_MNBin::decorated, this);
        bind("async_funcdef", &MN_MNBin::not_impl, this);
        bind("funcdef", &MN_MNBin::funcdef, this);
        bind("parameters", &MN_MNBin::parameters, this);
        bind("typedargslist", &MN_MNBin::typedargslist, this);
        bind("tfpdef", &MN_MNBin::tfpdef, this);
        bind("varargslist", &MN_MNBin::typedargslist, this);
        bind("vfpdef", &MN_MNBin::vfpdef, this);
        bind("stmt", &MN_MNBin::stmt, this);
        bind("simple_stmt", &MN_MNBin::simple_stmt, this);
        bind("small_stmt", &MN_MNBin::small_stmt, this);
        bind("expr_stmt", &MN_MNBin::expr_stmt, this);
        bind("testlist_star_expr", &MN_MNBin::testlist_star_expr, this);
        bind("augassign", &MN_MNBin::not_impl, this);
        bind("del_stmt", &MN_MNBin::del_stmt, this);
        bind("pass_stmt", &MN_MNBin::not_impl, this);
        bind("flow_stmt", &MN_MNBin::not_impl, this);
        bind("break_stmt", &MN_MNBin::break_stmt, this);
        bind("continue_stmt", &MN_MNBin::not_impl, this);
        bind("return_stmt", &MN_MNBin::return_stmt, this);
        bind("yield_stmt", &MN_MNBin::yield_stmt, this);
        bind("raise_stmt", &MN_MNBin::raise_stmt, this);
        bind("import_stmt", &MN_MNBin::not_impl, this);
        bind("import_name", &MN_MNBin::import_name, this);
        bind("import_from", &MN_MNBin::import_from, this);
        bind("import_as_name", &MN_MNBin::import_as_name, this);
        bind("dotted_as_name", &MN_MNBin::not_impl, this);
        bind("import_as_names", &MN_MNBin::not_impl, this);
        bind("dotted_as_name", &MN_MNBin::dotted_as_name, this);
        bind("dotted_as_names", &MN_MNBin::not_impl, this);
        bind("dotted_name", &MN_MNBin::dotted_name, this);
        bind("global_stmt", &MN_MNBin::not_impl, this);
        bind("nonlocal_stmt", &MN_MNBin::not_impl, this);
        bind("assert_stmt", &MN_MNBin::assert_stmt, this);
        bind("compound_stmt", &MN_MNBin::not_impl, this);
        bind("async_stmt", &MN_MNBin::not_impl, this);
        bind("if_stmt", &MN_MNBin::if_stmt, this);
        bind("while_stmt", &MN_MNBin::while_stmt, this);
        bind("for_stmt", &MN_MNBin::for_stmt, this);
        bind("try_stmt", &MN_MNBin::try_stmt, this);
        bind("with_stmt", &MN_MNBin::with_stmt, this);
        bind("with_item", &MN_MNBin::with_item, this);
        bind("except_clause", &MN_MNBin::except_clause, this);
        bind("suite", &MN_MNBin::suite, this);
        bind("test", &MN_MNBin::test, this);
        bind("test_nocond", &MN_MNBin::not_impl, this);
        bind("lambdef", &MN_MNBin::lambdef, this);
        bind("lambdef_nocond", &MN_MNBin::lambdef, this);
        bind("or_test", &MN_MNBin::or_test, this);
        bind("and_test", &MN_MNBin::and_test, this);
        bind("not_test", &MN_MNBin::not_test, this);
        bind("comparison", &MN_MNBin::comparison, this);
        bind("comp_op", &MN_MNBin::not_impl, this);
        bind("star_expr", &MN_MNBin::star_expr, this);
        bind("expr", &MN_MNBin::expr, this);
        bind("xor_expr", &MN_MNBin::xor_expr, this);
        bind("and_expr", &MN_MNBin::and_expr, this);
        bind("shift_expr", &MN_MNBin::shift_expr, this);
        bind("arith_expr", &MN_MNBin::arith_expr, this);
        bind("term", &MN_MNBin::term, this);
        bind("factor", &MN_MNBin::factor, this);
        bind("power", &MN_MNBin::power, this);
        bind("atom_expr", &MN_MNBin::atom_expr, this);
        bind("atom", &MN_MNBin::atom, this);
        bind("testlist_comp", &MN_MNBin::testlist_comp, this);
        bind("trailer", &MN_MNBin::trailer, this);
        bind("subscriptlist", &MN_MNBin::not_impl, this);
        bind("subscript", &MN_MNBin::subscript, this);
        bind("sliceop", &MN_MNBin::not_impl, this);
        bind("exprlist", &MN_MNBin::not_impl, this);
        bind("testlist", &MN_MNBin::testlist, this);
        bind("dictorsetmaker", &MN_MNBin::dictorsetmaker, this);
        bind("classdef", &MN_MNBin::classdef, this);
        bind("arglist", &MN_MNBin::arglist, this);
        bind("argument", &MN_MNBin::not_impl, this);
        bind("comp_iter", &MN_MNBin::not_impl, this);
        bind("comp_for", &MN_MNBin::comp_for, this);
        bind("comp_if", &MN_MNBin::comp_if, this);
        bind("encoding_decl", &MN_MNBin::not_impl, this);
        bind("yield_expr", &MN_MNBin::yield_stmt, this);
        bind("yield_arg", &MN_MNBin::not_impl, this);
        bind("f_string", &MN_MNBin::f_string, this);
        bind("replacement_field", &MN_MNBin::replacement_field, this);
        bind("f_expression", &MN_MNBin::f_expression, this);
        bind("format_spec", &MN_MNBin::format_spec, this);
        bind("conditional_expression", &MN_MNBin::conditional_expression, this);
        bind("expression", &MN_MNBin::expression, this);
        bind("lambda_expr", &MN_MNBin::lambda_expr, this);
        bind("yield_expression", &MN_MNBin::yield_expression, this);
        bind("expression_list", &MN_MNBin::expression_list, this);
        data.open(name.c_str());

        //decltype
    }

    MN_MNBin::~MN_MNBin()
    {
        data.ai16(MNPyByteCode::HALT);
        data.close();
    }

    void* MN_MNBin::walk(MNNode *node)
    {
         //return nullptr;
         if (node->get_lexeme() == "file_input")
         {
             auto first = node->at(0);
             if (first.get_meta() == 30 || first.get_meta() == 31)
             {
                 for (auto &it : first)
                     if (it.non_terminal())
                        bin_cast(call(&it));
             }
             else
                 if (first.non_terminal())
                     bin_cast(call(&first));
         }
         else
         {
             for (auto it : *node)
             {
                 if (it.non_terminal())
                     if (bin_cast(call(&it)) == nullptr)
                         walk(&it);
             }
         }
         //return bin_cast(create_bin_type(MN_PYBINP_DATA::MN_END));
         return nullptr;
    }

    void* MN_MNBin::file_input(MNNode* )
    {
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::decorated(MNNode* node)
    {
        auto second = node->at(1);
        bin_cast(call(&second));
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::stmt(MNNode *node)
    {
        auto c_test = node->at(0);
        if (c_test.non_terminal())
            bin_cast(call(&c_test));
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::simple_stmt(MNNode *node)
    {

        auto first = node->at(0);
        _add_line(&first);
        if (first.non_terminal())
            bin_cast(call(&first));
        auto second = node->at(1);


        for (auto &it : second)
        {
            if (it.non_terminal())
            {
//                data.ai16(MNPyByteCode::STMT);
//                data.ai32(it.get_row());

                if (it.non_terminal())
                    bin_cast(call(&it));
            }

        }
        if (second.get_count()==0)
        {
//            data.ai16(MNPyByteCode::STMT);
//            data.ai32(node->get_row());
        }

        if (first.get_lexeme() != "if_stmt"
                && first.get_lexeme() != "funcdef"
                && first.get_lexeme() != "decorated"
                && first.get_lexeme() != "small_stmt"
                && first.get_lexeme() != "expr_stmt"
                && first.get_lexeme() != "while_stmt"
                && first.get_lexeme() != "classdef"
                && first.get_lexeme() != "for_stmt"
                && first.non_terminal())
            data.ai16(MNPyByteCode::POP_TOP);


        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::funcdef(MNNode *node)
    {
        data.ai16(MNPyByteCode::STMT);
        data.ai32(node->get_row());
        comp_hist.push_back(0);
        auto c_test = node->at(1);
        auto c_cont = node->at(3);
        auto c_params = node->at(2);

        data.ai16(MNPyByteCode::BEGIN_FUNCTION_DEC);
        _gen_str(MNPyByteCode::LOAD_NAME, c_test.get_lexeme());


        if (c_cont.get_meta() != MN_NEXIST_T)
        {
            auto c_type = node->at(5);
            //bin_cast(call(&c_type));
            _process_type(&c_type);
        }
        else
        {
            _gen_str(MNPyByteCode::LOAD_NAME, "None");
        }


        if (c_params.non_terminal())
            bin_cast(call(&c_params));

        if (c_cont.get_meta() != MN_NEXIST_T)
        {
            auto suite = node->at(7);
            if (suite.non_terminal())
                bin_cast(call(&suite));
        }
        else
        {
            auto suite = node->at(5);
            if (suite.non_terminal())
                bin_cast(call(&suite));
        }
        _gen_loadname("None");
        data.ai16(MNPyByteCode::RETURN_VALUE);
        data.ai16(MNPyByteCode::END_FUNCTION_DEC);
        comp_hist.pop_back();
        return create_bin_type(MN_PYBINP_DATA::MN_FUNC);
    }

    void* MN_MNBin::parameters(MNNode *node)
    {
        auto c_args = node->at(1);
        data.ai16(MNPyByteCode::BEGIN_FUNC_PARAM);
        if (c_args.non_terminal())
            bin_cast(call(&c_args));
        data.ai16(MNPyByteCode::END_FUNC_PARAM);
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::typedargslist(MNNode *node)
    {
        unsigned int c_index = 0;
        unsigned int p_count = 0;
        auto c_args = node->at(0);
        bool default_func_param = false;
        if (c_args.get_meta() == 2 && c_args.get_lexeme() == "*")
        {
            auto c = node->at(1);
            bin_cast(call(&c));
            c_index=3;
        }
        else if (c_args.get_meta() == 201 && c_args.get_lexeme() == "**")
        {
            auto c = node->at(1);
            bin_cast(call(&c));
            c_index=4;
        }
        else
            bin_cast(call(&c_args));

        auto second = node->at(1);
        if (second.get_lexeme() == "=" && second.get_meta() == 2)
           c_index = 2;
        else if (c_index != 3 && c_index != 4)
           c_index = 1;

        if (c_index == 3)
            data.ai16(MNPyByteCode::ARGS_PARAM_TYPE);
        else if (c_index == 4)
            data.ai16(MNPyByteCode::KWARGS_PARAM_TYPE);

        if (c_index == 2)
        {
            auto eq = node->at(c_index);
            if (eq.non_terminal())
                bin_cast(call(&eq));
            data.ai16(MNPyByteCode::DEFAULT_FUNC_PARAM_VALUE);
            default_func_param = true;
        }
        if (c_index == 3 || c_index == 4) c_index = 1;
        c_index++;
        auto iter = node->at(c_index);

        auto p_args = [&](MNNode& iter)
        {
            unsigned int slice = 0;
            for (auto &it : iter)
            {
                if (it.get_meta() == 2 && it.get_lexeme() == ",")
                {
                    //auto start = iter.at(slice);
                    auto e = iter.at(slice+1);
                    if (e.non_terminal())
                        bin_cast(call(&e));
                    auto eq = iter.at(slice+2);
                    if (eq.get_meta() == 2 && eq.get_lexeme() == "=")
                    {
                        auto arg = iter.at(slice+3);
                        if (arg.non_terminal())
                            bin_cast(call(&arg));
                        data.ai16(MNPyByteCode::DEFAULT_FUNC_PARAM_VALUE);
                        default_func_param = true;
                    }
                    else
                    {
                        if (default_func_param)
                        {
                            error_msg->add_error_text("SyntaxError: ",  "non-default argument follows default argument at "
                                      + to_string(e.get_row()) + ":" + to_string(e.get_col()),
                                      source_lexer ? source_lexer->get_line(e.get_row()) : "", 0, e.get_col());
                        }
                    }

                    p_count++;
                }
                slice++;
            }
        };
        p_args(iter);

        auto args_c = [&]() {
            if (c_index+1 < node->get_count())
            {
                auto kargs = node->at(c_index+1);
                if (kargs.is_found())
                {
                    auto start = node->at(c_index+2);
                    c_index+=3;
                    if (start.get_lexeme() == "*" || start.get_lexeme() == "**" )
                    {
                        auto e = node->at(c_index);
                        if (e.non_terminal())
                            bin_cast(call(&e));
                        if (start.get_lexeme() == "*")
                            data.ai16(MNPyByteCode::ARGS_PARAM_TYPE);
                        else
                            data.ai16(MNPyByteCode::KWARGS_PARAM_TYPE);

                    }
                    auto ret = node->at(c_index+1);
                    p_args(ret);
                    c_index++;
                }
            }
        };
        args_c();
        args_c();

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::tfpdef(MNNode *node)
    {
        auto c_args = node->at(0);

        _gen_str(MNPyByteCode::LOAD_NAME, c_args.get_lexeme());


        auto c_test = node->at(1);
        if (c_test.get_meta() != MN_NEXIST_T)
        {
            auto r_test = node->at(2);
            if (r_test.non_terminal())
            {
                _process_type(&r_test);
            }
        }
        else
        {
            _gen_str(MNPyByteCode::LOAD_NAME, "None");
        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::vfpdef(MNNode *node)
    {
        auto c_args = node->at(0);

        _gen_str(MNPyByteCode::LOAD_NAME, c_args.get_lexeme());
        _gen_str(MNPyByteCode::LOAD_NAME, "None");

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::small_stmt(MNNode *node)
    {
        auto c_test = node->at(0);
        unsigned int c_value = comp_hist.empty() ? 0 : comp_hist.back();
        if (c_test.get_lexeme() == "break")
        {
            if (c_value)
            {
                data.ai16(MNPyByteCode::BREAK_LOOP);
            }
            else
            {
                error_msg->add_error_text("MN_Error: ",  "'break' outside loop at line "
                          + to_string(node->get_row()) + " column " + to_string(node->get_col()),
                          source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
            }
        }
        else if (c_test.get_lexeme() == "continue")
        {
            if (c_value)
            {
                data.ai16(MNPyByteCode::JUMP);
                data.ai32(comp_hist.back());
            }
            else
            {
                error_msg->add_error_text("MN_Error: ",  "'continue' not properly in loop at row "
                          + to_string(node->get_row()) + " column " + to_string(node->get_col()),
                          source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
            }
        }
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::expr_stmt(MNNode *node)
    {
        auto c_cont = node->at(1);
        unsigned int e_count = 0;
        dup_top_flag = false;
        if (c_cont.get_meta() == 30 && c_cont.get_count() > 0)
        {
            auto c_t = c_cont.at(0);
            if (c_t.get_lexeme() == "=")
            {
                e_count = c_cont.get_count() / 2;
                unsigned int c_i = c_cont.get_count()-2;
                auto c_t = c_cont.at(c_i+1);

                //bin_cast(call(&c_t));
                build_lt_flag = true;
                bin_cast(call(&c_t));

                unpack_flag = true;
                build_lt_flag = false;

                store_flag = true;

                if (e_count > 1)
                {
                    if (e_count >= 2)
                        dup_top_flag = true;
                    auto c_test = node->at(0);
                    store_sscr_load = 0;
                    //bin_cast(call(&c_test));
                    bin_cast(call(&c_test));
                    store_sscr_load = -1;
                    dup_top_flag = false;
                }

                for (unsigned int i=0; i < c_cont.get_count()-2; ++i)
                {
                    if (i % 2 == 0)
                    {
                        auto c_t = c_cont.at(i+1);
                        if (c_cont.get_count()-2 - i >=4)
                            dup_top_flag = true;
                        store_sscr_load = 0;
                        //bin_cast(call(&c_t));
                        bin_cast(call(&c_t));
                        store_sscr_load = -1;
                        dup_top_flag = false;
                    }
                }
                unpack_flag = false;
                store_flag = false;
                build_lt_flag = false;
            }
            else
            {
                auto c_t = node->at(2);
                //bin_cast(call(&c_t));
                bin_cast(call(&c_t));
            }
        }
        else if (c_cont.get_lexeme() == "augassign")
        {
           auto c_test = node->at(0);
           //bin_cast(call(&c_test));
           bin_cast(call(&c_test));
           auto parm = node->at(2);

           if (parm.get_lexeme() == "testlist")
               testlist_star_expr(&parm);
           else
           {
               //bin_cast(call(&parm));
               bin_cast(call(&parm));
           }

           auto type = c_cont.at(0);
           if (type.get_lexeme() == "+=")
               data.ai16(MNPyByteCode::INPLACE_ADD);
           if (type.get_lexeme() == "//=")
               data.ai16(MNPyByteCode::INPLACE_FLOOR_DIVIDE);
           if (type.get_lexeme() == "<<")
               data.ai16(MNPyByteCode::INPLACE_LSHIFT);
           if (type.get_lexeme() == "*=")
               data.ai16(MNPyByteCode::INPLACE_MULTIPLY);
           if (type.get_lexeme() == "@=")
               data.ai16(MNPyByteCode::INPLACE_MATRIX_MULTIPLY);
           if (type.get_lexeme() == "%=")
               data.ai16(MNPyByteCode::INPLACE_MODULO);
           if (type.get_lexeme() == "/=")
               data.ai16(MNPyByteCode::INPLACE_TRUE_DIVIDE);
           if (type.get_lexeme() == "-=")
               data.ai16(MNPyByteCode::INPLACE_SUBSTRACT);
           if (type.get_lexeme() == "^=")
               data.ai16(MNPyByteCode::INPLACE_XOR);
           if (type.get_lexeme() == "|=")
               data.ai16(MNPyByteCode::INPLACE_OR);
           if (type.get_lexeme() == "&=")
               data.ai16(MNPyByteCode::INPLACE_AND);
           if (type.get_lexeme() == "**=")
               data.ai16(MNPyByteCode::INPLACE_POWER);


        }
        else if (c_cont.get_lexeme() == "annassign")
        {
           auto c_types = c_cont.at(1);
           data.ai16(MNPyByteCode::BEGIN_TYPEDEF);
           _process_type(&c_types);


           auto assign = c_cont.at(3);
           bin_cast(call(&assign));
           data.ai16(MNPyByteCode::END_TYPEDEF);
        }
        if (e_count <= 1)
        {
            store_flag = true;
            unpack_flag = true;
            build_lt_flag = false;

            auto c_test = node->at(0);
            store_sscr_load = 0;
            //bin_cast(call(&c_test));
            bin_cast(call(&c_test));
            store_sscr_load = -1;

            store_flag = false;
            unpack_flag = false;
            build_lt_flag = true;

        }
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::testlist_star_expr(MNNode *node)
    {
        if (dup_top_flag)
        {
            dup_top_flag = false;
            data.ai16(MNPyByteCode::DUP_TOP);
        }


        if (unpack_flag && store_sscr_load <= 0)
        {

//            if (!store_flag)
//                data.ai16(MNPyByteCode::UNPACK_SEQUENCE);
//            else
//                data.ai16(MNPyByteCode::UNPACK_EX);

            data.ai16(MNPyByteCode::UNPACK_SEQUENCE);
            unsigned int pos = data.get_cursor_pos()+1;
            data.ai32(0);
            unpack_values.push_back(pos);

        }
        build_list.push_back(1);
        //auto second = node->at(1);
        auto ret = testlist_comp(node);
        auto res = reinterpret_cast<MN_PyBinType*>(ret);

        unsigned int count = 0;
        if (res->type == MN_PYBINP_DATA::MN_LT)
        {
            unsigned int* count_p = reinterpret_cast<unsigned int*>(res->meta);
            count = count_p[0];
        }
        if (!unpack_values.empty())
        {
            unsigned int pos = unpack_values.back();
            data.ai32(count, pos);
            unpack_values.pop_back();

        }

        build_list.pop_back();

        return ret;


        //return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::del_stmt(MNNode* node)
    {
        //data.ai16(MNPyByteCode::BREAK_LOOP);
        auto first = node->at(1);

        del_flag = true;
        store_flag = true;
        unpack_flag = true;
        build_lt_flag = false;
        unpack_flag = false;
        store_sscr_load = 0;
        if (first.get_lexeme() == "exprlist" && first.non_terminal())
            testlist_star_expr(&first);
        else
            bin_cast(call(&first));
        store_sscr_load = -1;
        del_flag = false;
        store_flag = false;
        unpack_flag = true;
        build_lt_flag = true;

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::break_stmt(MNNode* /*node*/)
    {
        data.ai16(MNPyByteCode::BREAK_LOOP);

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::return_stmt(MNNode *node)
    {
        auto c_retlist = node->at(1);
        if (c_retlist.non_terminal())
            bin_cast(call(&c_retlist));
        data.ai16(MNPyByteCode::RETURN_VALUE);

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::yield_stmt(MNNode *node)
    {
        auto c_yield = node->at(1);
        if (c_yield.non_terminal() && c_yield.get_lexeme() == "testlist")
        {
            testlist_star_expr(&c_yield);
            data.ai16(MNPyByteCode::YIELD_VALUE);
        }
        else if (c_yield.non_terminal() && c_yield.get_lexeme() != "yield_arg")
        {
            bin_cast(call(&c_yield));
            data.ai16(MNPyByteCode::YIELD_VALUE);
        }
        else
        {
            auto expr = c_yield.at(1);
            bin_cast(call(&expr));
            data.ai16(MNPyByteCode::GET_YIELD_FROM_ITER);
            data.ai16(MNPyByteCode::LOAD_CONST_STR);
            data.ai32(4);
            data.astr("None");
            data.ai16(MNPyByteCode::YIELD_FROM);
        }


        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::raise_stmt(MNNode *node)
    {
        auto first = node->at(1);

        unsigned int var_count = 0;
        if (first.is_found())
        {
            bin_cast(call(&first));
            var_count = 1;

            if (node->get_count() > 2)
            {
                auto n_node = node->at(2);

                if (n_node.get_lexeme() == "from" && n_node.get_meta() == 1)
                {
                   auto second = node->at(3);
                   bin_cast(call(&second));
                   var_count = 2;
                }
            }
        }
        data.ai16(MNPyByteCode::RAISE_VARARGS);
        data.ai16(var_count);

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::import_name(MNNode *node)
    {
        auto e = node->at(1);
        if (e.non_terminal() && e.get_lexeme() == "dotted_as_names")
        {
           auto first = e.at(0);
           bin_cast(call(&first));
           auto loop = e.at(1);
           for (auto it : loop)
           {
               if (it.non_terminal())
                   bin_cast(call(&it));
           }
        }
        else if (e.non_terminal())
        {
            bin_cast(call(&e));
        }
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::import_from(MNNode *node)
    {

        auto first = node->at(1);
        import_from_flag = true;
        unsigned int c_index = 4;
        data.ai16(MNPyByteCode::LOAD_CONST_INT);
        unsigned int pos = data.get_cursor_pos()+1;
        data.ai32(0);
        if (first.get_lexeme() == ".")
        {
            _gen_str(MNPyByteCode::IMPORT_FROM, "<.>");
            data.ai32(1, pos);

        }
        else if (first.get_lexeme() == "..")
        {
            _gen_str(MNPyByteCode::IMPORT_FROM, "<.>");
            data.ai32(2, pos);
        }
        else if (first.get_lexeme() == "...")
        {
            _gen_str(MNPyByteCode::IMPORT_FROM, "<.>");
            data.ai32(3, pos);
        }
        else if (first.is_req_list())
        {
            _gen_str(MNPyByteCode::IMPORT_FROM, "<.>");

            unsigned int count = 0;
            for (auto it : first)
            {
                unsigned int sum = 1;
                if (it.get_lexeme() == "..")
                    sum = 2;
                else if (it.get_lexeme() == "...")
                    sum = 3;
                count += sum;

            }
            data.ai32(count, pos);
        }
        else
        {
            c_index = 3;

            bin_cast(call(&first));
        }

        import_from_flag = false;
        auto e = node->at(c_index);
        if (e.non_terminal() && e.get_lexeme() == "import_as_names")
        {
           auto first = e.at(0);
           bin_cast(call(&first));
           auto loop = e.at(1);
           for (auto it : loop)
           {
               if (it.non_terminal())
                   bin_cast(call(&it));
           }
        }
        else if (e.non_terminal())
        {
            bin_cast(call(&e));
        }
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::import_as_name(MNNode *node)
    {
        auto first = node->at(0);
        std::string r_name = first.get_lexeme();
        _gen_str(MNPyByteCode::IMPORT_FROM, r_name.c_str());
        auto second = node->at(1);
        if (second.is_found())
        {
            auto third = node->at(2);
            r_name = third.get_lexeme();
        }

        _gen_storename(r_name.c_str());
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::dotted_as_name(MNNode *node)
    {
        auto first = node->at(0);
        auto third = node->at(2);

        current_imp_name = third.get_lexeme();
        bin_cast(call(&first));
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::dotted_name(MNNode *node)
    {
        auto name = node->at(0);
        auto loop = node->at(1);
        std::string r_name = name.get_lexeme();

        for (auto it : loop)
            r_name += it.get_lexeme();
        _gen_str(MNPyByteCode::IMPORT_NAME, r_name);

        if (!current_imp_name.empty())
        {
            for (auto it : loop)
            {
                if (it.get_lexeme() != "." && it.get_meta() != 2)
                {
                    _gen_str(MNPyByteCode::LOAD_ATTR, it.get_lexeme());
                }
            }
        }

        if (!import_from_flag)
        {
            if (current_imp_name.empty())
            {
                _gen_storename(name.get_lexeme().c_str());
            }
            else
            {
               _gen_storename(current_imp_name.c_str());
               current_imp_name = "";
            }
        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }
    void* MN_MNBin::assert_stmt(MNNode *node)
    {
        auto expr = node->at(1);
        bin_cast(call(&expr));
        data.ai16(MNPyByteCode::POP_JUMP_IF_TRUE);
        unsigned int j_pos = data.get_cursor_pos()+1;
        data.ai32(0);
        _gen_loadname("AssertionError");
        auto exp2 = node->at(2);
        if (exp2.is_found())
        {
            auto exp_e = node->at(3);
            bin_cast(call(&exp_e));
            data.ai16(MNPyByteCode::CALL_FUNCTION);
            data.ai16(1);
            data.ai16(0);
        }
        data.ai16(MNPyByteCode::RAISE_VARARGS);
        data.ai16(1);
        data.ai32(data.get_inst_count(), j_pos);
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::test(MNNode *node)
    {

        if (trailer_level == 0 && store_flag)
        {
            error_msg->add_error_text("SyntaxError: ",  "can't assign to conditional expression "
                      + to_string(node->get_row()) + ":" + to_string(node->get_col()),
                      source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
        }
        std::vector<MN_Jump_Data>* values = new std::vector<MN_Jump_Data>();
        auto if_expr = node->at(2);

        //data.ai16(MNPyByteCode::IF);


        auto if_expr_res = bin_cast( call(&if_expr) );

        data.ai16(MNPyByteCode::POP_JUMP_IF_FALSE);
        unsigned int if_pos = data.get_cursor_pos() + 1;
        data.ai32(0);

        _process_jumps(1, true, if_expr_res);


        //data.ai16(MNPyByteCode::END_IF);

        auto second = node->at(0);
        auto res1 = bin_cast(call(&second));


        data.ai16(MNPyByteCode::JUMP);
        unsigned int jump_pos = data.get_cursor_pos()+1;
        data.ai32(0);
        _process_jumps(0, true, if_expr_res);

        auto third = node->at(4);
        data.ai32(data.get_inst_count(), if_pos);
        auto res2 = bin_cast(call(&third));
        data.ai32(data.get_inst_count(), jump_pos);

        _process_jumps(0, false, res1, nullptr, 2);
        _process_jumps(1, false, res1, nullptr, 2);

        _process_jumps(0, false, res2, values);
        _process_jumps(1, false, res2, values);

        return create_bin_type(MN_PYBINP_DATA::MN_TEST, reinterpret_cast<void*>(values));
    }

    void* MN_MNBin::lambdef(MNNode *node)
    {
        if (trailer_level == 0 && store_flag)
        {
            error_msg->add_error_text("SyntaxError: ",  "can't assign to lambda "
                      + to_string(node->get_row()) + ":" + to_string(node->get_col()),
                      source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
        }
        data.ai16(MNPyByteCode::STMT);
        data.ai32(node->get_row());
        comp_hist.push_back(0);
        auto c_test = node->at(1);
        auto expr = node->at(3);

        data.ai16(MNPyByteCode::BEGIN_FUNCTION_DEC);



        _gen_loadname("<lambda>");
        data.ai16(MNPyByteCode::BEGIN_FUNC_PARAM);
        if (c_test.get_count() > 1)
            bin_cast(call(&c_test));
        else
            bin_cast(vfpdef(&c_test));
        data.ai16(MNPyByteCode::END_FUNC_PARAM);

        bin_cast(call(&expr));
        data.ai16(MNPyByteCode::RETURN_VALUE);
        data.ai16(MNPyByteCode::END_FUNCTION_DEC);
        comp_hist.pop_back();
        return create_bin_type(MN_PYBINP_DATA::MN_FUNC);
    }



    void* MN_MNBin::or_test(MNNode *node)
    {
        _op_err(node, "operator");
        return _and_or_test(1, node);
//        ++and_or_level;
//        std::vector<MN_Jump_Data>* values = new std::vector<MN_Jump_Data>();
//        //std::vector<MN_Jump_Data>* v_local = new std::vector<MN_Jump_Data>();

//        auto c_test = node->at(0);
//        auto c_loop = node->at(1);
//        auto v_type = bin_cast(call(&c_test));
//        c_loop.start_cache();

//        data.ai16(MNPyByteCode::JUMP_IF_TRUE_OR_POP);
//        values->push_back(create_jump_data(data.get_cursor_pos()+1, 1, and_or_level, 0));
//        data.ai32(0);


//        auto fill_jumps = [&](bool add=false) {
//            if (_is_and_or(v_type))
//            {
//                auto meta = reinterpret_cast<std::vector<MN_Jump_Data>*>(v_type->meta);
//                for (auto it : *meta)
//                {
//                    if (it.jump_type == 1)
//                    {
//                        values->push_back(it);
//                        unsigned int c_pos = it.pos - 2;
//                        data.ai16(MNPyByteCode::JUMP_IF_TRUE_OR_POP, c_pos);
//                    }
//                    else
//                    {
//                        data.ai32(data.get_inst_count(), it.pos);

//                        if (!add)
//                        {
//                            unsigned int c_pos = it.pos - 2;
//                            data.ai16(MNPyByteCode::POP_JUMP_IF_FALSE, c_pos);
//                        }
//                        if (add)
//                            values->push_back(it);
//                    }
//                }
//            }
//        };
//        fill_jumps();
//        for (unsigned int i=0; i < c_loop.get_count(); ++i)
//        {
//            if (i % 2 == 0)
//            {
//                auto c_test = c_loop.at(i+1);
//                v_type = bin_cast(call(&c_test));

//                if (i < c_loop.get_count()-2)
//                {
//                    data.ai16(MNPyByteCode::JUMP_IF_TRUE_OR_POP);
//                    values->push_back(create_jump_data(data.get_cursor_pos()+1, 1, and_or_level, 0));
//                    data.ai32(0);
//                }
//                fill_jumps(i >= c_loop.get_count()-2 ? true : false);
//            }
//        }
//        for (auto it : *values)
//            data.ai32(data.get_inst_count(), it.pos);
//        --and_or_level;
//        return create_bin_type(MN_PYBINP_DATA::MN_OR, reinterpret_cast<void*>(values));
    }

    void* MN_MNBin::not_test(MNNode *node)
    {
        auto c_test = node->at(0);
        _op_err(node, "operator");
        if (c_test.get_lexeme() == "not")
        {
            auto c_test = node->at(1);
            bin_cast(call(&c_test));
            data.ai16(MNPyByteCode::UNARY_NOT);
        }
        else
        {
            auto c_test = node->at(0);
            bin_cast(call(&c_test));

        }
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::and_test(MNNode *node)
    {
        _op_err(node, "operator");
        return _and_or_test(0, node);
//        ++and_or_level;
//        std::vector<MN_Jump_Data>* values = new std::vector<MN_Jump_Data>();
//        //std::vector<MN_Jump_Data>* v_local = new std::vector<MN_Jump_Data>();

//        auto c_test = node->at(0);
//        auto c_loop = node->at(1);
//        auto v_type = bin_cast(call(&c_test));
//        c_loop.start_cache();

//        data.ai16(MNPyByteCode::JUMP_IF_FALSE_OR_POP);
//        values->push_back(create_jump_data(data.get_cursor_pos()+1, 0, and_or_level, 0));
//        data.ai32(0);


//        auto fill_jumps = [&](bool add=false) {
//            if (_is_and_or(v_type))
//            {
//                auto meta = reinterpret_cast<std::vector<MN_Jump_Data>*>(v_type->meta);
//                for (auto it : *meta)
//                {
//                    if (it.jump_type == 0)
//                    {
//                        values->push_back(it);
//                        unsigned int c_pos = it.pos - 2;
//                        data.ai16(MNPyByteCode::JUMP_IF_FALSE_OR_POP, c_pos);

//                    }
//                    else
//                    {
//                        data.ai32(data.get_inst_count(), it.pos);
//                        if (!add)
//                        {
//                            unsigned int c_pos = it.pos - 2;
//                            data.ai16(MNPyByteCode::POP_JUMP_IF_TRUE, c_pos);
//                        }
//                        if (add)
//                            values->push_back(it);
//                    }
//                }
//            }
//        };
//        fill_jumps();
//        for (unsigned int i=0; i < c_loop.get_count(); ++i)
//        {
//            if (i % 2 == 0)
//            {
//                auto c_test = c_loop.at(i+1);
//                v_type = bin_cast(call(&c_test));

//                if (i < c_loop.get_count()-2)
//                {
//                    data.ai16(MNPyByteCode::JUMP_IF_FALSE_OR_POP);
//                    values->push_back(create_jump_data(data.get_cursor_pos()+1, 0, and_or_level, 0));
//                    data.ai32(0);
//                }
//                fill_jumps(i >= c_loop.get_count()-2 ? true : false);
//            }
//        }
//        for (auto it : *values)
//            data.ai32(data.get_inst_count(), it.pos);
//        --and_or_level;
//        return create_bin_type(MN_PYBINP_DATA::MN_AND, reinterpret_cast<void*>(values));
    }

    void* MN_MNBin::comparison(MNNode *node)
    {
        auto c_loop = node->at(1);
        auto c_test = node->at(0);
        bin_cast(call(&c_test));
        std::vector<unsigned int> pos;

        c_loop.start_cache();

        if (trailer_level == 0 && store_flag)
        {
            error_msg->add_error_text("SyntaxError: ",  "can't assign to comparison "
                      + to_string(node->get_row()) + ":" + to_string(node->get_col()),
                      source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
        }
        for (unsigned int i=0; i < c_loop.get_count(); ++i)
        {
            if (i % 2 == 0)
            {
                auto c_c = c_loop.at(i);
                std::string lex="";
                for (auto &it : c_c)
                    lex += it.get_lexeme();
                auto c_test = c_loop.at(i+1);
                bin_cast(call(&c_test));
                unsigned int c_pos = 0;

                if (c_loop.get_count()>2)
                {
                    data.ai16(MNPyByteCode::DUP_TOP);
                    if (i < c_loop.get_count()-2)
                    {
                        data.ai16(MNPyByteCode::ROT_THREE);
                        data.ai16(JUMP_IF_FALSE_OR_POP);
                        pos.push_back(data.get_cursor_pos()+1);
                        data.ai32(0);
                    }
                    else
                    {
                        data.ai16(MNPyByteCode::JUMP);
                        c_pos = data.get_cursor_pos()+1;
                        data.ai32(0);
                        for (auto it : pos)
                            data.ai16(data.get_inst_count(), it);
                        data.ai16(MNPyByteCode::ROT_TWO);
                        data.ai16(MNPyByteCode::POP_TOP);
                    }


                }

                if (lex == "<")
                    data.ai16(MNPyByteCode::COMPARE_OP_B);
                else
                if (lex == ">")
                    data.ai16(MNPyByteCode::COMPARE_OP_A);
                else
                if (lex == "==")
                    data.ai16(MNPyByteCode::COMPARE_OP_E);
                else
                if (lex == "AE")
                    data.ai16(MNPyByteCode::COMPARE_OP_AE);
                else
                if (lex == "BE")
                    data.ai16(MNPyByteCode::COMPARE_OP_BE);
                else
                if (lex == "NE")
                    data.ai16(MNPyByteCode::COMPARE_OP_NE);
                else
                if (lex == "notin")
                    data.ai16(MNPyByteCode::COMPARE_OP_NOT_IN);
                else
                if (lex == "in")
                    data.ai16(MNPyByteCode::COMPARE_OP_IN);
                else
                if (lex == "is")
                    data.ai16(MNPyByteCode::COMPARE_OP_IS);
                else
                if (lex == "isnot")
                    data.ai16(MNPyByteCode::COMPARE_OP_IS_NOT);

                if (c_pos)
                    data.ai32(data.get_inst_count(), c_pos);
            }
        }
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }
    void* MN_MNBin::star_expr(MNNode *node)
    {
        if (!store_flag && build_list.empty())
        {
            if (trailer_level == 0)
            {
                error_msg->add_error_text("SyntaxError: ",  "can't use starred expression here "
                          + to_string(node->get_row()) + ":" + to_string(node->get_col()),
                          source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
            }
        }
        else if (store_flag && build_list.empty())
        {
            if (trailer_level == 0)
            {
                error_msg->add_error_text("SyntaxError: ",  "starred assigment target must be in a list or tuple "
                          + to_string(node->get_row()) + ":" + to_string(node->get_col()),
                          source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
            }
        }
        auto c_node = node->at(1);
        bin_cast(call(&c_node));

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::expr(MNNode *node)
    {
        auto c_loop = node->at(1);
        auto c_test = node->at(0);
        bin_cast(call(&c_test));
        _op_err(node, "operator");

        c_loop.start_cache();
        for (unsigned int i=0; i < c_loop.get_count(); ++i)
        {
            if (i % 2 == 0)
            {
                auto c_test = c_loop.at(i+1);
                bin_cast(call(&c_test));
                data.ai16(MNPyByteCode::BINARY_OR);
            }
        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::xor_expr(MNNode *node)
    {
        auto c_loop = node->at(1);
        auto c_test = node->at(0);
        bin_cast(call(&c_test));
        _op_err(node, "operator");

        c_loop.start_cache();
        for (unsigned int i=0; i < c_loop.get_count(); ++i)
        {
            if (i % 2 == 0)
            {
                auto c_test = c_loop.at(i+1);
                bin_cast(call(&c_test));
                data.ai16(MNPyByteCode::BINARY_XOR);
            }
        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::and_expr(MNNode *node)
    {
        auto c_loop = node->at(1);
        auto c_test = node->at(0);
        bin_cast(call(&c_test));
        _op_err(node, "operator");
        c_loop.start_cache();
        for (unsigned int i=0; i < c_loop.get_count(); ++i)
        {
            if (i % 2 == 0)
            {
                auto c_test = c_loop.at(i+1);
                bin_cast(call(&c_test));
                data.ai16(MNPyByteCode::BINARY_AND);
            }
        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::shift_expr(MNNode *node)
    {
        auto c_loop = node->at(1);
        auto c_test = node->at(0);
        bin_cast(call(&c_test));
        c_loop.start_cache();
        _op_err(node, "operator");
        for (unsigned int i=0; i < c_loop.get_count(); ++i)
        {
            if (i % 2 == 0)
            {
                auto c_c = c_loop.at(i);
                // auto c_c2 = c_loop.at(i+1);

                auto c_test = c_loop.at(i+1);
                bin_cast(call(&c_test));
                std::string lex = c_c.get_lexeme();
                if (lex == "<<")
                   data.ai16(MNPyByteCode::BINARY_LSHIFT);
                else
                   data.ai16(MNPyByteCode::BINARY_RSHIFT);
            }
        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::arith_expr(MNNode *node)
    {
        auto c_loop = node->at(1);
        auto c_test = node->at(0);
        bin_cast(call(&c_test));
        c_loop.start_cache();
        _op_err(node, "operator");
        for (unsigned int i=0; i < c_loop.get_count(); ++i)
        {
            if (i % 2 == 0)
            {
                auto c_c = c_loop.at(i);
                auto c_test = c_loop.at(i+1);
                bin_cast(call(&c_test));
                std::string lex = c_c.get_lexeme();
                if (lex == "+")
                   data.ai16(MNPyByteCode::BINARY_ADD);
                else
                if (lex == "-")
                    data.ai16(MNPyByteCode::BINARY_SUBSTRACT);
            }
        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::term(MNNode *node)
    {
        auto c_loop = node->at(1);
        auto c_test = node->at(0);
        bin_cast(call(&c_test));
        c_loop.start_cache();
        _op_err(node, "operator");
        for (unsigned int i=0; i < c_loop.get_count(); ++i)
        {
            if (i % 2 == 0)
            {
                auto c_c = c_loop.at(i);
                std::string lex = c_c.get_lexeme();
                auto c_test = c_loop.at(i+1);
                bin_cast(call(&c_test));

                if (lex == "*")
                   data.ai16(MNPyByteCode::BINARY_MULTIPLY);
                else
                if (lex == "/")
                    data.ai16(MNPyByteCode::BINARY_TRUE_DIVIDE);
                else
                if (lex == "DBACKSLASH")
                    data.ai16(MNPyByteCode::BINARY_FLOOR_DIVIDE);
                else
                if (lex == "%")
                    data.ai16(MNPyByteCode::BINARY_MODULO);
                else
                if (lex == "@")
                    data.ai16(MNPyByteCode::BINARY_MATRIX_MULTIPLY);
            }
        }


        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }
    void* MN_MNBin::factor(MNNode *node)
    {
        _op_err(node, "operator");
        auto c_test = node->at(0);
        if (c_test.get_lexeme() != "power")
        {
            auto c_test = node->at(1);
            auto i_node = node->at(0);
            bin_cast(call(&c_test));
            if (i_node.get_lexeme() == "+")
              data.ai16(MNPyByteCode::UNARY_POSITIVE);
            else if (i_node.get_lexeme() == "-")
                data.ai16(MNPyByteCode::UNARY_NEGATIVE);
            else
                data.ai16(MNPyByteCode::UNARY_INVERT);
        }
        else
        {
            auto c_test = node->at(0);
            bin_cast(call(&c_test));
        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::power(MNNode *node)
    {
        _op_err(node, "operator");
        auto c_loop = node->at(1);
        if (c_loop.get_meta() != MN_NEXIST_T)
        {
            auto c_test = node->at(0);
            bin_cast(call(&c_test));
            auto c_power = node->at(2);
            bin_cast(call(&c_power));
            data.ai16(MNPyByteCode::BINARY_POWER);
       }
        else
        {
            auto c_test = node->at(0);
            bin_cast(call(&c_test));
        }


        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::atom_expr(MNNode *node)
    {
        bool has_trailer = true;
        unsigned c_store = store_sscr_load;

        auto first = node->at(1);
        auto loop = node->at(2);

        if (loop.get_lexeme() != "trailer" && (loop.is_list() && loop.get_count() <= 0))
            has_trailer = false;
        else
        {
            if (store_sscr_load == 0) ++store_sscr_load;
        }
//        if (first.get_lexeme() == "atom" && !store_sscr_load)
//        {
//            auto type = first.at(0);
//            if (type.get_lexeme() == "[" || type.get_lexeme() == "(" || type.get_lexeme() == "{")
//            {
//                auto child = first.at(1);
//                if (child.get_lexeme() == "testlist_comp")
//                    store_sscr_load = true;
//            }
//        }
        bool c_store_flag = store_flag ;
        if (store_sscr_load == 1 && has_trailer)
            store_flag = false;
        auto res = call(&first);
        store_flag = c_store_flag;

        ++trailer_level;
        if (loop.is_list() && loop.get_count() > 0)
        {

            for (unsigned int i=0; i < loop.get_count(); ++i)
            {
                auto it = loop.at(i);

                if (i >= loop.get_count()-1 && store_sscr_load == 1)
                    last_trailer = true;
                bin_cast(call(&it));
            }
        }
        else
        {
            if (loop.non_terminal())
            {
                if (store_sscr_load == 1)
                    last_trailer = true;
                bin_cast(call(&loop));
            }
        }
        --trailer_level;
        store_sscr_load = c_store;
        return res;
    }

    void* MN_MNBin::atom(MNNode *node)
    {
        auto c_t = node->at(0);
        current_line = c_t.get_row();
        if (dup_top_flag)
        {
            data.ai16(MNPyByteCode::DUP_TOP);
            dup_top_flag = false;
        }
        if (_stpack(&c_t))
        {
            auto second = node->at(1);
            if (unpack_flag && store_sscr_load <= 0)
            {

                data.ai16(MNPyByteCode::UNPACK_SEQUENCE);
                unsigned int pos = data.get_cursor_pos()+1;
                data.ai32(0);
                unpack_values.push_back(pos);

            }
            if (c_t.get_lexeme() == "[" )
                build_list.push_back(0);
            else if (c_t.get_lexeme() == "(" )
                build_list.push_back(1);
            else
                build_list.push_back(3);

            unsigned int count = 0;

//            auto ret = c_t.get_lexeme() == "(" ? create_bin_type(MN_PYBINP_DATA::MN_AND)
//                                               : create_bin_type(MN_PYBINP_DATA::MN_END);
            auto ret = create_bin_type(MN_PYBINP_DATA::MN_END);
            if (second.is_found())
            {

                if ((c_t.get_lexeme() == "[" && c_t.get_meta() == 2)
                        || (_test_if_list(c_t, second)))
                {
                    //NEW AND_OR Context
                    bool c_last_stmt = last_stmt;
                    bool c_if_stmt_flag = if_stmt_flag;
                    unsigned int c_and_or_level = and_or_level;
                    and_or_level = 0;
                    if_stmt_flag = false;
                    last_stmt = false;
                    delete (MN_PyBinType*)ret;
                    ret = call(&second);
                    auto c_ret = reinterpret_cast<MN_PyBinType*>(ret);
                    if (c_ret->type ==  MN_PYBINP_DATA::MN_AND
                            || c_ret->type ==  MN_PYBINP_DATA::MN_OR
                            || c_ret->type ==  MN_PYBINP_DATA::MN_TEST)
                        c_ret->type = MN_PYBINP_DATA::MN_END;
                    last_stmt = c_last_stmt;
                    if_stmt_flag = c_if_stmt_flag;
                    and_or_level = c_and_or_level;

                }
                else
                {
                    delete (MN_PyBinType*)ret;
                    ret = call(&second);
                }
                if ((c_t.get_lexeme() == "{" && c_t.get_meta() == 2))
                    _op_err(&c_t, "literal");

                if ((c_t.get_lexeme() == "{" && c_t.get_meta() == 2) && second.get_lexeme() != "dictorsetmaker")
                {
                    if (second.get_lexeme() == "star_expr")
                        data.ai16(MNPyByteCode::BUILD_SET_UNPACK);
                    else
                        data.ai16(MNPyByteCode::BUILD_SET);
                    data.ai32(1);
                }
                if ((c_t.get_lexeme() == "[" && c_t.get_meta() == 2) /*&& second.get_lexeme() != "testlist_comp"*/)
                    count = 1;

                auto res = reinterpret_cast<MN_PyBinType*>(ret);


                if (res->type == MN_PYBINP_DATA::MN_LT)
                {
                    unsigned int* count_p = reinterpret_cast<unsigned int*>(res->meta);
                    count = count_p[0];
                }
            }
            else
            {

                if(!(unpack_flag && store_sscr_load <= 0))
                {
                    _gen_container_type();
                    data.ai32(0);
                }
            }
            if (!unpack_values.empty())
            {
                unsigned int pos = unpack_values.back();
                data.ai32(count, pos);
                unpack_values.pop_back();

            }

            build_list.pop_back();

            return ret;
        }
        else
        {
            //Bytecode Features Begin
            if (!c_t.is_req_list())
            {
                bool check_err = true;
                switch (c_t.get_meta()) {
                case 1:
                    _gen_str(MNPyByteCode::LOAD_CONST_STR, c_t.get_lexeme());
                    break;
                case 7:
                    {
                        MNPyByteCode type = !del_flag ? MNPyByteCode::STORE_NAME : MNPyByteCode::DELETE_NAME;
                        if (!store_flag || store_sscr_load > 0)
                            type = MNPyByteCode::LOAD_NAME;
                        _gen_str(type, c_t.get_lexeme());
                        check_err = false;
                    }
                    break;
                case 26:
                    data.ai16(MNPyByteCode::LOAD_CONST_INT);
                    data.ai32(atol(c_t.get_lexeme().c_str()));

                    break;
                case 100:
                    break;
                case 200:
                    _gen_str(MNPyByteCode::LOAD_CONST_HEX, c_t.get_lexeme());
                    break;
                case 224:
                    data.ai16(MNPyByteCode::LOAD_CONST_FLOAT);
                    data.afloat(atof(c_t.get_lexeme().c_str()));

                    break;
                case 225:
                    _gen_str(MNPyByteCode::LOAD_CONST_USTR, c_t.get_lexeme());
                    break;
                case 226:
                    _gen_str(MNPyByteCode::LOAD_CONST_BSTR, c_t.get_lexeme());
                    break;
                case 227:
                    data.ai16(MNPyByteCode::LOAD_CONST_IMG);
                    data.afloat(atof(c_t.get_lexeme().c_str()));
                    break;
                case 228:
                    _gen_str(MNPyByteCode::LOAD_CONST_BIN, c_t.get_lexeme());
                    break;
                case 229:
                    _gen_str(MNPyByteCode::LOAD_CONST_OCT, c_t.get_lexeme());
                    break;
                case 230:
                    _gen_str(MNPyByteCode::LOAD_CONST_RSTR, c_t.get_lexeme());
                    break;
                case 231:
                    _gen_str(MNPyByteCode::LOAD_CONST_EXP, c_t.get_lexeme());
                    break;
                case 232:
                    _gen_str(MNPyByteCode::LOAD_CONST_FSTR, c_t.get_lexeme());
                    break;
                case 233:
                    _gen_str(MNPyByteCode::LOAD_CONST_BRSTR, c_t.get_lexeme());
                    break;
                case 234:
                    _gen_str(MNPyByteCode::LOAD_CONST_RBSTR, c_t.get_lexeme());
                    break;
                default:
                    break;
                }
                if (check_err)
                   _op_err(&c_t, "literal");
            }
            else
            {
                _op_err(&c_t, "literal");
                std::string concat = "";
                MNPyByteCode type = LOAD_CONST_STR;
                for (auto it : c_t)
                {
                    concat += it.get_lexeme();
                    switch (it.get_meta()) {
                    case 225:
                        type = LOAD_CONST_USTR;
                        break;
                    case 226:
                        type = LOAD_CONST_BSTR;
                        break;
                    case 230:
                        type = LOAD_CONST_RSTR;
                        break;
                    case 232:
                        type = LOAD_CONST_FSTR;
                        break;
                    case 233:
                        type = LOAD_CONST_BRSTR;
                        break;
                    case 234:
                        type = LOAD_CONST_RBSTR;
                        break;
                    default:
                        break;
                    }
                }

                _gen_str(type, concat);

            }
            //Bytecode Features End


        }

        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::testlist_comp(MNNode *node)
    {
        auto first = node->at(0);
        auto second = node->at(1);
        unsigned int count=0;
        unsigned int g_count = 0;
        unsigned int i_inc = 2;
        unsigned int s_exp = 0;

        if (second.get_lexeme() == ":")
        {
            i_inc+=2;
            second = node->at(3);
        }
        else
        {
            if (build_list.back() == 3)
            {
                build_list.pop_back();
                build_list.push_back(2);
            }
        }

        if (first.non_terminal() && !second.is_found() )
        {
            //std::cout << first.get_lexeme() << std::endl;
            auto ret = call(&first);
            if (i_inc == 4)
            {
                auto sec_parm = node->at(2);
                if (sec_parm.non_terminal())
                    bin_cast(call(&sec_parm));
            }
            if (!build_list.empty() && (build_list.back()==0 || build_list.back()==2 || build_list.back()==3))
            {
                if (first.get_lexeme() != "star_expr" && !store_flag)
                {
                    _gen_container_type();
                    data.ai32(1);
                }
            }
            if (first.get_lexeme() == "star_expr" && !store_flag)
            {
                 data.ai16(MNPyByteCode::BUILD_LIST_UNPACK);
                 data.ai32(1);
                 s_exp++;
            }
            return ret;
        }
        else
        if(second.is_found())
        {
            if (second.is_list())
            {
                if (first.non_terminal())
                    bin_cast(call(&first));
                if (i_inc == 4)
                {
                    auto sec_parm = node->at(2);
                    if (sec_parm.non_terminal())
                        bin_cast(call(&sec_parm));
                }
                bool c_unpack = false;
                unsigned int c_c = 0;
                if (first.get_lexeme() == "star_expr")
                    c_unpack=true;
                else
                    c_c = 1;
                count=1;
                g_count=1;

                for (unsigned int i=0; i < second.get_count();++i)
                {
                    if (i % i_inc == 0)
                    {
                        auto it = second.at(i+1);
                        //if (it.get_lexeme() != "test")
                        g_count++;

                        if (it.get_lexeme() == "star_expr" && !del_flag)
                        {

                            if (c_c != 0)
                            {
                                _gen_container_type();
                                data.ai32(c_c);
                            }

                            c_c = 0;
                            count++;
                            c_unpack = true;
                        }
                        else
                        {
                            c_c++;
                        }
                        if (it.non_terminal())
                            bin_cast(call(&it));
                        if (i_inc == 4)
                        {
                            auto sec_parm = second.at(i+3);
                            if (sec_parm.non_terminal())
                                bin_cast(call(&sec_parm));
                        }
                        if (i >= second.get_count()-2 && c_unpack && it.get_lexeme() != "star_expr")
                        {

                            if (c_c != 0)
                            {
                                _gen_container_type();
                                data.ai32(c_c);
                            }
                            count++;
                        }
                    }
                }
                if (store_sscr_load > 0)
                {
                    _gen_container_type();
                    data.ai32(g_count);
                }
                if (build_lt_flag || c_unpack /*|| store_sscr_load*/)
                {

                    if (!c_unpack /*|| store_sscr_load*/)
                    {
                        _gen_container_type();
                        data.ai32(g_count);
                    }
                    else
                    {
                        if (!store_flag)
                        {
                            if (build_list.back()==0)
                                data.ai16(MNPyByteCode::BUILD_LIST_UNPACK);
                            else if (build_list.back()==1)
                                data.ai16(MNPyByteCode::BUILD_TUPLE_UNPACK);
                            else
                            {
                                data.ai16(MNPyByteCode::BUILD_SET_UNPACK);
                            }
                            data.ai32(count);
                        }
                    }
                }
            }
            else
            if (second.non_terminal())
            {
                 _gen_loadname("<genexp>");
                _gen_container_type();
                 data.ai32(1);
                 data.ai16(MNPyByteCode::SETUP_LOOP_FOR);
                 unsigned int c_pos = data.get_cursor_pos()+1;
                 data.ai32(0);


                 auto j_value = bin_cast(call(&second));


                 MN_COMP_FORIF* ret = reinterpret_cast<MN_COMP_FORIF*>(j_value->meta);
                 std::vector<unsigned int>* j_v = reinterpret_cast<std::vector<unsigned int>*>(ret->values);
                 //std::vector<std::vector<MN_Jump_Data>*>* i_v = ret->if_values;

                 _gen_loadname("<genexp>");

                 if (first.non_terminal())
                    bin_cast(call(&first));
                 if (i_inc == 4)
                 {
                     auto sec_parm = node->at(2);
                     if (sec_parm.non_terminal())
                        bin_cast(call(&sec_parm));
                 }

                 _gen_container_type();
                 data.ai32(1);
                 data.ai16(MNPyByteCode::INPLACE_ADD);
                 _gen_storename("<genexp>");



                 for (unsigned int i=j_v->size(); i > 0; --i)
                 {
                     if ((i-1) % 2 == 0)
                     {
                         data.ai16(MNPyByteCode::JUMP);
                         data.ai32(j_v->at(i-1));

                         unsigned int e_pos = data.get_inst_count();

                         data.ai16(MNPyByteCode::POP_BLOCK);
                         data.ai32(e_pos, j_v->at(i));
                     }
                 }
                 data.ai32(data.get_inst_count(), c_pos);
            }
            else
            {
                auto j_value = bin_cast(call(&second));
                MN_COMP_FORIF* ret = reinterpret_cast<MN_COMP_FORIF*>(j_value->meta);
                std::vector<unsigned int>* j_v = reinterpret_cast<std::vector<unsigned int>*>(ret->values);
                if (first.non_terminal())
                    bin_cast(call(&first));
                data.ai16(MNPyByteCode::JUMP);
                data.ai32(j_v->at(0));
            }
        }
        unsigned int *p_c = new unsigned int[1];
        p_c[0] = g_count;
        return create_bin_type(MN_PYBINP_DATA::MN_LT, reinterpret_cast<unsigned int*>(p_c));
    }

    void* MN_MNBin::trailer(MNNode *node)
    {
        bool l_trailer = last_trailer;
        if (last_trailer)
            last_trailer = false;
        auto first = node->at(0);
        if (first.get_lexeme() == "[")
        {
           auto second = node->at(1);
           if (second.get_lexeme() == "subscriptlist")
           {
              testlist_star_expr(&second);
           }
           else
           {
              bin_cast(call(&second));
           }
           if (!l_trailer)
               data.ai16(MNPyByteCode::BINARY_SUBSCR);
           else
               data.ai16(!del_flag ? MNPyByteCode::STORE_SUBSCR : MNPyByteCode::DELETE_SUBSCR);
        }
        else if (first.get_lexeme() == ".")
        {
            auto second = node->at(1);
            MNPyByteCode type = !del_flag ? MNPyByteCode::STORE_ATTR : MNPyByteCode::DELETE_ATTR;
            if (!l_trailer)
               type = MNPyByteCode::LOAD_ATTR;
            _gen_str(type, second.get_lexeme());

        }
        else
        {
            auto second = node->at(1);
            if (trailer_level < 2)
            {

                if (trailer_level == 1 && l_trailer)
                {
                    error_msg->add_error_text("MN_Error: ",  "can't assign to function call at row "
                              + to_string(second.get_row()) + " column " + to_string(second.get_col()),
                              source_lexer ? source_lexer->get_line(second.get_row()) : "", 0, second.get_col());

                }
            }
            if (second.get_lexeme() != "argument" && second.get_lexeme() != "arglist")
            {
                if (second.non_terminal())
                    bin_cast(call(&second));
                data.ai16(MNPyByteCode::CALL_FUNCTION);
                data.ai16(second.non_terminal() ? 1 : 0);
                data.ai16(0);
            }
            else
            {
                if (second.get_lexeme() == "argument")
                    bin_cast(arglist(&second));
                else
                    if (second.non_terminal())
                        bin_cast(call(&second));
            }
        }
        //store_sscr_load = c_store;
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::subscript(MNNode *node)
    {
        unsigned int b_slice = 2;
        auto f_s = node->at(0);
        auto s_s = node->at(2);
        auto t_s = node->at(3);
        if (f_s.non_terminal())
            bin_cast(call(&f_s));
        else
            _gen_loadname("None");
        if (s_s.non_terminal())
            bin_cast(call(&s_s));
        else
            _gen_loadname("None");
        if (t_s.get_lexeme() == "sliceop")
        {
            auto f_s = t_s.at(1);
            if (f_s.is_found())
            {
                ++b_slice;
                bin_cast(call(&f_s));
            }
        }
        data.ai16(MNPyByteCode::BUILD_SLICE);
        data.ai32(b_slice);
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::arglist(MNNode *node)
    {
        bool kw = false;
        bool arg = false;
        bool pos = false;
        bool key = false;
        unsigned int positional = 0;
        unsigned int keywords = 0;
        unsigned int kwargs = 0;
        //std::vector<MNNode*> args_v;
        std::vector<MNNode*> key_v;
        unsigned int count = 0;
        unsigned int c_c = 0;

        auto args = [&](MNNode& node)
        {
            auto f = node.at(0);
            if (f.get_lexeme() == "*")
            {
                if (c_c != 0 && (arg || key || kw))
                {
                    data.ai16(MNPyByteCode::BUILD_TUPLE);
                    data.ai32(c_c);
                    c_c = 0;
                }
                if (kw)
                {
                    error_msg->add_error_text("MN_Error: ",  "iterable argument unpacking follows keyword argument unpacking at "
                              + to_string(node.get_row()) + " column " + to_string(node.get_col()),
                              source_lexer ? source_lexer->get_line(node.get_row()) : "", 0, node.get_col() );
                }
                else {
                    auto second = node.at(1);
                    bin_cast(call(&second));
                }
                arg = true;
                count++;
                return 0;
            }
            else if (f.get_lexeme() == "**")
            {
                kw = true;
                kwargs++;
                if (count > 1)
                {
                    data.ai16(MNPyByteCode::BUILD_LIST_UNPACK);
                    data.ai32(count);
                    count = 0;
                }
                if (!key_v.empty())
                {
                    for (auto it : key_v)
                    {
                        bin_cast(call(it));
                        delete it;
                    }
                    key_v.clear();
                }
                auto second = node.at(1);
                bin_cast(call(&second));

                return 1;
            }
            else
            {
                key = true;
                keywords++;
                auto second = node.ptr_at(2);
                key_v.push_back(node.ptr_at(0));
                key_v.push_back(second);
                return 2;
            }
        };

        auto non_args = [&](MNNode& node)
        {
            auto f = node.at(0);
            positional++;
            if (arg || key || kw)
                c_c++;
            if (key)
                error_msg->add_error_text("MN_Error: ",  "positional argument follows keyword argument at "
                          + to_string(node.get_row()) + " column " + to_string(node.get_col()),
                          source_lexer ? source_lexer->get_line(node.get_row()) : "", 0, node.get_col() );
            else if (kw)
            {
                error_msg->add_error_text("MN_Error: ",  "positional argument follows keyword argument unpacking at "
                          + to_string(node.get_row()) + " column " + to_string(node.get_col()),
                          source_lexer ? source_lexer->get_line(node.get_row()) : "", 0, node.get_col() );
            }
            else
                bin_cast(call(&node));
            pos = true;
        };

        if (node->get_lexeme() == "argument")
        {
            args(*node);
        }
        else
        {
            auto first = node->at(0);



            if (first.get_lexeme() != "argument")
                non_args(first);
            else
            {
                args(first);
            }
            auto second = node->at(1);
            if (second.get_meta() == 30)
            {
                for (unsigned int i=0; i < second.get_count(); ++i)
                {
                    if ( i % 2 == 0)
                    {
                        auto it = second.at(i+1);
                        if (it.get_lexeme() != "argument")
                            non_args(it);
                        else
                        {
                            args(it);
                        }
                    }
                }
            }
        }
        if (count > 1)
        {
            data.ai16(MNPyByteCode::BUILD_LIST_UNPACK);
            data.ai32(count);
            count = 0;
        }
        if (!key_v.empty())
        {
            for (auto it : key_v)
            {
                bin_cast(call(it));
                delete it;
            }
            key_v.clear();
        }

        if (call_gen)
        {
            if (kwargs > 1)
            {
                data.ai16(MNPyByteCode::BUILD_MAP_UNPACK_WITH_CALL);
                data.ai32(kwargs);
            }
            if (!arg && !kw )
                data.ai16(MNPyByteCode::CALL_FUNCTION);
            else if (arg && !kw )
                data.ai16(MNPyByteCode::CALL_FUNCTION_VAR);
            else if (!arg && kw )
                data.ai16(MNPyByteCode::CALL_FUNCTION_KW);
            else if (arg && kw )
                data.ai16(MNPyByteCode::CALL_FUNCTION_VAR_KW);

            data.ai16(positional);
            data.ai16(keywords);
        }
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::testlist(MNNode *node)
    {

        auto first = node->at(0);

        bin_cast(call(&first));
        auto second = node->at(1);
        if (second.get_meta() == 30)
        {
            for (unsigned int i=0; i < second.get_count(); ++i)
            {
                if ( i % 2 == 0)
                {
                    auto it = second.at(i+1);
                    bin_cast(call(&it));
                }
            }
        }


        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::dictorsetmaker(MNNode* node)
    {
          return testlist_comp(node);
    }

    void* MN_MNBin::classdef(MNNode *node)
    {
        data.ai16(MNPyByteCode::LOAD_BUILD_CLASS);
        auto name = node->at(1);
        _gen_loadname(name.get_lexeme());
        unsigned int index = 4;
        auto c_i = node->at(2);
        if (c_i.get_lexeme() == "(")
        {
            index+=2;
            auto arglist = node->at(3);
            call_gen = false;
            data.ai16(MNPyByteCode::BEGIN_FUNC_PARAM);
            if (arglist.non_terminal())
                bin_cast(call(&arglist));
            data.ai16(MNPyByteCode::END_FUNC_PARAM);
            call_gen = true;
        }
        auto suite = node->at(index);
        bin_cast(call(&suite));
        data.ai16(MNPyByteCode::MAKE_CLASS);
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::comp_for(MNNode *node)
    {
           std::vector<unsigned int>* values = new std::vector<unsigned int>();
           std::vector<std::vector<MN_Jump_Data>*>* if_values = new std::vector<std::vector<MN_Jump_Data>*>();
           MN_COMP_FORIF* ret = new MN_COMP_FORIF();
           ret->values = values;
           ret->if_values = if_values;


           auto for_expr = node->at(2);

           auto for_in = node->at(4);
           if ((for_in.get_lexeme() == "exprlist" || for_in.get_lexeme() == "testlist")
                   && for_in.non_terminal() )
               testlist_star_expr(&for_in);
           else
               bin_cast(call(&for_in));
           data.ai16(MNPyByteCode::GET_ITER);
           unsigned int pos = data.get_inst_count();
           values->push_back(pos);
           comp_hist.push_back(pos);
           data.ai16(MNPyByteCode::FOR_ITER);
           values->push_back(data.get_cursor_pos()+1);
           data.ai32(0);

           store_flag = true;
           unpack_flag = true;
           build_lt_flag = false;
           if (for_expr.get_lexeme() == "exprlist" && for_expr.non_terminal())
               testlist_star_expr(&for_expr);
           else
               bin_cast(call(&for_expr));

           store_flag = false;
           unpack_flag = false;
           build_lt_flag = true;

           auto last = node->at(5);
           if (last.get_meta() != MN_NEXIST_T)
           {
               auto j_value = bin_cast(call(&last));
               if (j_value->type == MN_COMP_FOR)
               {
                   MN_COMP_FORIF* ret = reinterpret_cast<MN_COMP_FORIF*>(j_value->meta);
                   std::vector<unsigned int>* j_v = ret->values;
                   for (auto it: *j_v)
                       values->push_back(it);
               }
           }
           comp_hist.pop_back();


        return create_bin_type(MN_PYBINP_DATA::MN_COMP_FOR, reinterpret_cast<void*>(ret));
    }

    void* MN_MNBin::comp_if(MNNode *node)
    {
        std::vector<unsigned int>* values = new std::vector<unsigned int>();
        std::vector<std::vector<MN_Jump_Data>*>* if_values = new std::vector<std::vector<MN_Jump_Data>*>();
        MN_COMP_FORIF* ret = new MN_COMP_FORIF();
        ret->values = values;
        ret->if_values = if_values;



        auto if_expr = node->at(1);


        //data.ai16(MNPyByteCode::IF);

        if_flag = true;
        bool c_if_stmt_flag = if_stmt_flag;
        unsigned int c_and_or_level = and_or_level;
        if_stmt_flag = true;
        jump_flag = true;

        auto if_expr_res = bin_cast( call(&if_expr) );

        data.ai16(MNPyByteCode::POP_JUMP_IF_FALSE);
        unsigned int pos = 0;
        if (!comp_hist.empty())
        {
            pos = comp_hist.back();
        }
        data.ai32(pos);

        and_or_level = c_and_or_level;

        if_flag = false;
        if_stmt_flag = c_if_stmt_flag;
        //data.ai16(MNPyByteCode::END_IF);

        auto last = node->at(2);
        if (last.get_meta() != MN_NEXIST_T)
        {

            auto j_value = bin_cast(call(&last));
            if (j_value->type == MN_COMP_FOR)
            {
                MN_COMP_FORIF* ret = reinterpret_cast<MN_COMP_FORIF*>(j_value->meta);
                std::vector<unsigned int>* j_v = ret->values;
                for (auto it: *j_v)
                    values->push_back(it);

            }
        }

        //AND JUMP Transformations
        if (if_expr_res->type == MN_PYBINP_DATA::MN_AND
                || if_expr_res->type == MN_PYBINP_DATA::MN_OR
                || if_expr_res->type == MN_PYBINP_DATA::MN_TEST)
        {
            unsigned int pos = 0;
            if (!comp_hist.empty())
            {
                pos = comp_hist.back();
            }
            auto meta = reinterpret_cast<std::vector<MN_Jump_Data>*>(if_expr_res->meta);
            for (auto &it : *meta)
            {
                if (it.jump_type == 0)
                {
                    data.ai32(pos, it.pos);
                    unsigned int c_pos = it.pos - 2;
                    data.ai16(MNPyByteCode::POP_JUMP_IF_FALSE, c_pos);
                }
                else
                {
                    unsigned int c_pos = it.pos - 2;
                    data.ai32(data.get_inst_count(), it.pos);
                    data.ai16(MNPyByteCode::POP_JUMP_IF_TRUE, c_pos);

                }
            }
        }



        return create_bin_type(MN_PYBINP_DATA::MN_COMP_FOR, reinterpret_cast<void*>(ret));
    }

    void* MN_MNBin::if_stmt(MNNode *node)
    {
        //if_stmt_flag = true;
        std::vector <unsigned int> saved_pos;

        auto elseif_cond = node->at(4);
        auto else_cond = node->at(5);

        bool e_cond = elseif_cond.get_count() > 0 || else_cond.get_meta() != MN_NEXIST_T;


        long int if_pos = _process_if_stmt(node->at(1),
                         node->at(3), e_cond ? 1 : 0);
        long int if_pos_2 = -1;

        //if_stmt_flag = false;

        if (e_cond)
        {
            data.ai16(MNPyByteCode::JUMP);
            saved_pos.push_back(data.get_cursor_pos()+1);
            data.ai32(0);
        }

        if (if_pos != -1)
        {
            data.ai32(data.get_inst_count(), if_pos);
        }

        for (unsigned int i=0; i < elseif_cond.get_count();++i)
        {
            if (i % 4 == 0)
            {
                //bool n_last = node->at(5).get_meta() == MN_NEXIST_T;
                //bool n_last = (i < elseif_cond.get_count() - 4);
                //if_pos_2 = _process_if_stmt(elseif_cond.at(i+1),
                //                 elseif_cond.at(i+3), !n_last ? 2 : 0);
                if_pos_2 = _process_if_stmt(elseif_cond.at(i+1),
                                 elseif_cond.at(i+3), 1);
                if (e_cond)
                {
                    data.ai16(MNPyByteCode::JUMP);
                    saved_pos.push_back(data.get_cursor_pos()+1);
                    data.ai32(0);
                }

                if (if_pos_2 != -1)
                {
                    data.ai32(data.get_inst_count(), if_pos_2);
                }
            }
        }


        if (else_cond.get_meta() != MN_NEXIST_T)
        {
            /*data.ai16(MNPyByteCode::JUMP);
            saved_pos.push_back(data.get_cursor_pos()+1);
            data.ai32(0);*/

            auto else_suite = node->at(7);

            if (if_pos_2 != -1)
            {
                data.ai32(data.get_inst_count(), if_pos_2);
            }
            bin_cast(call(&else_suite));

        }

        unsigned int c_pos = data.get_inst_count();
        for (auto it : saved_pos)
        {
            data.ai32(c_pos, it);
        }


        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::while_stmt(MNNode *node)
    {
        data.ai16(MNPyByteCode::SETUP_LOOP_WHILE);
        unsigned int l_pos = data.get_cursor_pos()+1;
        data.ai32(0);

        auto if_expr = node->at(1);
        long int jump_pos = data.get_inst_count();
        comp_hist.push_back(jump_pos);
        //data.ai16(MNPyByteCode::IF);

        if_flag = true;
        if_stmt_flag = true;
        jump_flag = true;

        auto if_expr_res = bin_cast( call(&if_expr) );
        unsigned int f_pos = 0;

        data.ai16(MNPyByteCode::POP_JUMP_IF_FALSE);
        f_pos = data.get_cursor_pos() + 1;
        data.ai32(0);


        if_flag = false;
        if_stmt_flag = false;
        //data.ai16(MNPyByteCode::END_IF);

        auto suite = node->at(3);
        _process_jumps(1, true, if_expr_res);
        bin_cast(call(&suite));

        data.ai16(MNPyByteCode::JUMP);
        data.ai32(jump_pos);


        _process_jumps(0, true, if_expr_res);
        data.ai32(data.get_inst_count(), f_pos);

        data.ai16(MNPyByteCode::POP_BLOCK);
        data.ai32(data.get_inst_count(), l_pos);
        auto else_s = node->at(4);
        if (else_s.get_meta() == 1 && else_s.get_lexeme() == "else")
        {
            auto e_suite = node->at(6);
            bin_cast(call(&e_suite));
        }
        comp_hist.pop_back();
        return create_bin_type(MN_PYBINP_DATA::MN_AND);
    }

    void* MN_MNBin::for_stmt(MNNode *node)
    {
        data.ai16(MNPyByteCode::SETUP_LOOP_FOR);
        unsigned int c_pos = data.get_cursor_pos()+1;
        data.ai32(0);
        auto for_expr = node->at(1);

        auto for_in = node->at(3);
        if ((for_in.get_lexeme() == "exprlist" || for_in.get_lexeme() == "testlist")
                && for_in.non_terminal() )
            testlist_star_expr(&for_in);
        else
            bin_cast(call(&for_in));
        data.ai16(MNPyByteCode::GET_ITER);
        unsigned int pos = data.get_inst_count();
        comp_hist.push_back(pos);
        data.ai16(MNPyByteCode::FOR_ITER);
        unsigned int for_pos = data.get_cursor_pos()+1;
        data.ai32(0);

        store_flag = true;
        unpack_flag = true;
        build_lt_flag = false;
        if (for_expr.get_lexeme() == "exprlist" && for_expr.non_terminal())
            testlist_star_expr(&for_expr);
        else
            bin_cast(call(&for_expr));

        store_flag = false;
        unpack_flag = false;
        build_lt_flag = true;

        auto suite = node->at(5);
        bin_cast(call(&suite));
        data.ai16(MNPyByteCode::JUMP);
        data.ai32(pos);
        data.ai32(data.get_inst_count(), for_pos);
        data.ai16(MNPyByteCode::POP_BLOCK);
        data.ai32(data.get_inst_count(), c_pos);
        auto else_s = node->at(6);
        if (else_s.get_meta() == 1 && else_s.get_lexeme() == "else")
        {
            auto e_suite = node->at(8);
            bin_cast(call(&e_suite));
        }

        comp_hist.pop_back();

        return create_bin_type(MN_PYBINP_DATA::MN_AND);
    }

    void* MN_MNBin::try_stmt(MNNode *node)
    {

        std::vector<unsigned int> j_pos;
        unsigned int f_pos = 0;
        auto cont = node->at(3);
        unsigned int c_index = 5;
        auto e_node = node->at(4);
        if (e_node.get_lexeme() == "else")
            c_index += 2;
        auto f_node = node->at(c_index);

        if (cont.get_lexeme() == "finally" || f_node.get_lexeme() == "finally")
        {
            data.ai16(MNPyByteCode::SETUP_FINALLY);
            f_pos = data.get_inst_count()+1;
            data.ai32(0);
        }
        data.ai16(MNPyByteCode::SETUP_EXCEPT);
        unsigned int s_pos = data.get_cursor_pos()+1;
        unsigned int cj_pos = 0;
        data.ai32(0);
        auto suite = node->at(2);
        bin_cast(call(&suite));
        data.ai16(MNPyByteCode::POP_BLOCK);
        data.ai16(MNPyByteCode::JUMP);
        unsigned int b_pos = data.get_cursor_pos()+1;
        data.ai32(0);

        data.ai32(data.get_inst_count(), s_pos);

        bool d_except = false;


        if (cont.get_lexeme() == "finally")
        {
           auto f_suite = node->at(5);
           data.ai32(data.get_inst_count(), f_pos);
           bin_cast(call(&f_suite));
        }
        else {
            for (unsigned int i=0; i < cont.get_count(); ++i)
            {
                if (i % 3 == 0)
                {
                    auto it = cont.at(i);
                    auto first = it.at(1);
                    if (cj_pos != 0)
                        data.ai32(data.get_inst_count(), cj_pos);

                    data.ai16(MNPyByteCode::STMT);
                    data.ai32(first.get_row());


                    if (d_except)
                    {
                        error_msg->add_error_text("MN_Error: ",  "default 'except' must be last "
                                  + to_string(it.get_row()) + ":" + to_string(it.get_col()),
                                  source_lexer ? source_lexer->get_line(it.get_row()) : "", 0, it.get_col());
                        break;
                    }


                    if (first.is_found())
                    {
                        data.ai16(MNPyByteCode::DUP_TOP);
                        bin_cast(call(&first));
                        data.ai16(MNPyByteCode::COMPARE_OP_EM);
                        data.ai16(MNPyByteCode::POP_JUMP_IF_FALSE);
                        cj_pos = data.get_cursor_pos()+1;

                        data.ai32(0);
                    }
                    else
                        d_except = true;
                    data.ai16(MNPyByteCode::POP_TOP);

                    std::string s_name = "";
                    if (it.get_count() > 2)
                    {
                        auto c_cont = it.at(2);
                        if (c_cont.get_lexeme() == "as")
                        {
                            auto r_name = it.at(3);
                            s_name = r_name.get_lexeme();
                            _gen_storename(r_name.get_lexeme());
                        }
                    }
                    else
                        data.ai16(MNPyByteCode::POP_TOP);
                    data.ai16(MNPyByteCode::POP_TOP);
                    unsigned int s_finally = 0;
                    if (!s_name.empty())
                    {
                        data.ai16(MNPyByteCode::SETUP_FINALLY);
                        s_finally = data.get_cursor_pos()+1;
                        data.ai32(0);
                    }

                    auto suite = cont.at(i+2);
                    bin_cast(call(&suite));
                    if (!s_name.empty())
                    {
                        data.ai16(MNPyByteCode::POP_BLOCK);
                        data.ai16(MNPyByteCode::POP_EXCEPT);
                        for (unsigned int i=0; i < 2; ++i)
                        {
                            if (i == 1)
                                data.ai32(data.get_inst_count(), s_finally);
                            _gen_str(MNPyByteCode::LOAD_CONST_STR, "None");

                        }
                        _gen_storename(s_name.c_str());
                        _gen_str(MNPyByteCode::DELETE_NAME, s_name);

                        data.ai16(MNPyByteCode::END_FINALLY);
                    }
                    else
                        data.ai16(MNPyByteCode::POP_EXCEPT);
                    data.ai16(MNPyByteCode::JUMP);
                    j_pos.push_back(data.get_cursor_pos()+1);
                    data.ai32(0);
                    if (d_except)
                        data.ai16(END_FINALLY);

                }
            }
            if (cj_pos)
            {
                data.ai32(data.get_inst_count(), cj_pos);
            }

            data.ai32(data.get_inst_count(), b_pos);

            unsigned int c_index = 5;
            auto e_node = node->at(4);
            if (e_node.get_lexeme() == "else")
            {
                c_index += 2;
                auto e_suite = node->at(6);
                bin_cast(call(&e_suite));

            }
            for (auto it : j_pos)
                data.ai32(data.get_inst_count(), it);
            auto f_node = node->at(c_index);
            if (f_node.get_lexeme() == "finally")
            {
                data.ai16(MNPyByteCode::POP_BLOCK);
                _gen_str(MNPyByteCode::LOAD_CONST_STR, "None");

                data.ai32(data.get_inst_count(), f_pos);
                auto f_suite = node->at(c_index + 2);
                bin_cast(call(&f_suite));
            }

        }


        data.ai16(MNPyByteCode::END_FINALLY);
        return create_bin_type(MN_PYBINP_DATA::MN_AND);
    }

    void* MN_MNBin::with_stmt(MNNode *node)
    {
        auto f_withitem = node->at(1);
        unsigned int w_count = 1;
        std::vector<unsigned int> w_pos;
        auto with_e = [&](MNNode& f_withitem)
        {

            if (f_withitem.get_lexeme() != "with_item" && f_withitem.non_terminal())
            {
                bin_cast(call(&f_withitem));
                data.ai16(MNPyByteCode::SETUP_WITH);
                w_pos.push_back(data.get_cursor_pos()+1);
                data.ai32(0);
            }
            else
            {
                auto first = f_withitem.at(0);
                bin_cast(call(&first));
                data.ai16(MNPyByteCode::SETUP_WITH);
                w_pos.push_back(data.get_cursor_pos()+1);

                data.ai32(1);
                auto as_node = f_withitem.at(2);


                store_flag = true;
                unpack_flag = true;
                build_lt_flag = false;
                store_sscr_load = 0;

                bin_cast(call(&as_node));
                store_sscr_load = -1;
                store_flag = false;
                unpack_flag = false;
                build_lt_flag = true;
            }
        };
        with_e(f_withitem);
        auto loop = node->at(2);
        for (auto it : loop)
        {
            if (it.non_terminal())
            {
                with_e(it);
                w_count++;
            }
        }
        auto suite = node->at(4);
        if (suite.non_terminal())
            bin_cast(call(&suite));
        for (unsigned int i=0; i < w_count; ++i)
        {
            data.ai16(MNPyByteCode::POP_BLOCK);
            _gen_str(MNPyByteCode::LOAD_CONST_STR, "None");

            data.ai32(data.get_inst_count(), w_pos[w_count-i-1]);
            data.ai16(MNPyByteCode::WITH_CLEANUP_START);
            data.ai16(MNPyByteCode::WITH_CLEANUP_FINISH);
            data.ai16(MNPyByteCode::END_FINALLY);
        }
        _gen_str(MNPyByteCode::LOAD_CONST_STR, "None");

        data.ai16(MNPyByteCode::RETURN_VALUE);
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::with_item(MNNode *node)
    {
        auto first = node->at(0);
        bin_cast(call(&first));
        data.ai16(MNPyByteCode::SETUP_WITH);
        data.ai32(1);
        auto as_node = node->at(2);


        store_flag = true;
        unpack_flag = true;
        build_lt_flag = false;
        store_sscr_load = 0;

        bin_cast(call(&as_node));
        store_sscr_load = -1;
        store_flag = false;
        unpack_flag = false;
        build_lt_flag = true;


        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::except_clause(MNNode */*node*/)
    {
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    long int MN_MNBin::_process_if_stmt(MNNode test, MNNode suite, unsigned int offset)
    {
        long int jump_pos = -1;
        //data.ai16(MNPyByteCode::IF);

        if_flag = true;
        jump_flag = true;
        if_stmt_flag = true;
        //auto test = cond;
        auto if_expr = bin_cast( call(&test) );

        data.ai16(MNPyByteCode::POP_JUMP_IF_FALSE);
        jump_pos = data.get_cursor_pos()+1;
        data.ai32(0);

        if_stmt_flag = false;
        if_flag = false;
        //data.ai16(MNPyByteCode::END_IF);

        //AND JUMP Transformations

        _process_jumps(1, true, if_expr);
        if (suite.non_terminal())
            bin_cast(call(&suite));

        _process_jumps(0, true, if_expr, nullptr, offset == 1 ? 3 : 0);
        return jump_pos;
    }

    void* MN_MNBin::_process_type(MNNode *node)
    {
        if (node->get_lexeme() == "atom_expr")
        {
            auto fp = node->at(1).at(0);
            auto trailer = node->at(2);
            bool t_exist = trailer.get_lexeme() == "trailer";
            if (t_exist)
                data.ai16(MNPyByteCode::BEGIN_TYPEDEF);
            if (fp.get_meta() == 7)
            {
                _gen_str(MNPyByteCode::LOAD_NAME, fp.get_lexeme());
                //if (!t_exist)
                //    data.ai16(MNPyByteCode::SINGLE_TYPEDEF);
            }
            else
            {
                error_msg->add_error_text("MN_Error: ",  "type expected "
                          + to_string(fp.get_row()) + ":" + to_string(fp.get_col()),
                          source_lexer ? source_lexer->get_line(fp.get_row()) : "", 0, fp.get_col());

            }


            if (trailer.is_list() && trailer.get_count() > 0)
            {
                error_msg->add_error_text("MN_Error: ",  "Too many trailers at line "
                          + to_string(trailer.get_row()) + " column " + to_string(trailer.get_col()),
                          source_lexer ? source_lexer->get_line(trailer.get_row()) : "", 0, trailer.get_col());
            }
            else
            if (t_exist)
            {
                auto t_type = trailer.at(0);
                if (t_type.get_lexeme() == "[")
                {
                    unsigned p_count = 0;
                    auto types = trailer.at(1);
                    if (types.get_lexeme() == "atom_expr")
                        _process_type(&types);
                    else if (types.get_lexeme() == "subscriptlist")
                    {
                        auto first = types.at(0);
                        _process_type(&first);
                        auto iter = types.at(1);
                        unsigned int slice = 0;
                        for (auto &it : iter)
                        {
                            if (it.get_lexeme() == ",")
                            {
                                auto e = iter.at(slice+1);
                                _process_type(&e);
                                p_count++;
                            }
                            slice++;
                        }

                    }

                }
                else
                {
                    error_msg->add_error_text("MN_Error: ",  "Unexpected '(' at line "
                              + to_string(t_type.get_row()) + " column " + to_string(t_type.get_col()),
                              source_lexer ? source_lexer->get_line(t_type.get_row()) : "", 0, t_type.get_col());

                }
            }
            if (t_exist)
                data.ai16(MNPyByteCode::END_TYPEDEF);

        }
        else
            error_msg->add_error_text("MN_Error: ",  "Type expected at line "
                      + to_string(node->get_row()) + " column " + to_string(node->get_col()),
                      source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());

        return nullptr;
    }

    void MN_MNBin::_gen_str(MNPyByteCode type, std::string value)
    {
        data.ai16(type);
        data.ai32(value.size());
        data.astr(value.c_str());
        data.dec_inst_count();
    }

    void MN_MNBin::_gen_loadname(std::string value)
    {
        data.ai16(MNPyByteCode::LOAD_NAME);
        data.ai32(value.size());
        data.astr(value.c_str());
        data.dec_inst_count();
    }

    void MN_MNBin::_gen_storename(std::string value)
    {
        data.ai16(MNPyByteCode::STORE_NAME);
        data.ai32(value.size());
        data.astr(value.c_str());
        data.dec_inst_count();
    }

    void MN_MNBin::_gen_container_type()
    {
        if (build_list.back()==0)
            data.ai16(MNPyByteCode::BUILD_LIST);
        else if (build_list.back()==1)
            data.ai16(MNPyByteCode::BUILD_TUPLE);
        else if (build_list.back()==2)
            data.ai16(MNPyByteCode::BUILD_SET);
        else if (build_list.back()==3)
            data.ai16(MNPyByteCode::BUILD_MAP);

    }

    bool MN_MNBin::_stpack(MNNode *node)
    {
       if ( (node->get_lexeme() == "(" || node->get_lexeme() == "["
             || node->get_lexeme() == "{") && node->get_meta() == 2)
           return true;
       return false;
    }

    void MN_MNBin::_r_add(std::shared_ptr<MN_PyBinType>& /*node*/, bool /*reduce*/)
    {
//        if (node->type == MN_PYBINP_DATA::MN_AND
//                || node->type == MN_PYBINP_DATA::MN_OR)
//        {
//            unsigned int pos = data.get_inst_count();
//            auto meta = reinterpret_cast<std::vector<MN_Jump_Data>*>(node->meta);
//            for (auto &it : *meta)
//                 data.ai32((!reduce ? pos : pos-2), it.pos);
//        }
    }

    void MN_MNBin::_v_add(std::shared_ptr<MN_PyBinType> &node, std::vector<MN_Jump_Data>* values)
    {
        if (node->type == MN_PYBINP_DATA::MN_AND
                || node->type == MN_PYBINP_DATA::MN_OR
                || node->type == MN_PYBINP_DATA::MN_TEST)
        {
            auto meta = reinterpret_cast<std::vector<MN_Jump_Data>*>(node->meta);
            for (auto &it : *meta)
            {
                values->push_back(it);
            }
        }
    }

    bool MN_MNBin::_test_if_list(MNNode& node, MNNode& second)
    {
        if (node.get_lexeme() == "(" && node.get_meta() == 2)
        {
           auto first = second.at(0);
           if (first.non_terminal())
           {
               auto sec = second.at(1);
               if ((sec.is_list() && sec.get_count() > 0) || second.get_count() > 2)
                   return true;
           }
        }
        return false;
    }

    bool MN_MNBin::_is_and_or(std::shared_ptr<MN_PyBinType> type)
    {
        if (type->type == MN_PYBINP_DATA::MN_AND || type->type == MN_PYBINP_DATA::MN_OR
                || type->type == MN_PYBINP_DATA::MN_TEST)
            return true;
        return false;
    }

    void MN_MNBin::_process_jumps(unsigned int type, bool cond, std::shared_ptr<MN_PyBinType> expr,
                                  std::vector<MN_Jump_Data>* values, int meta_val)
    {
        if (expr->type == MN_PYBINP_DATA::MN_AND
                || expr->type == MN_PYBINP_DATA::MN_OR
                || expr->type == MN_PYBINP_DATA::MN_TEST)
        {
            unsigned int pos = data.get_inst_count();
            auto meta = reinterpret_cast<std::vector<MN_Jump_Data>*>(expr->meta);
            for (auto &it : *meta)
            {
                if (it.jump_type == type)
                {
                    data.ai32(meta_val == 3 ? pos+2 : pos, it.pos);
                    if (cond && it.j_pos != 1)
                    {
                        unsigned int c_pos = it.pos - 2;
                        data.ai16(type == 1 ? MNPyByteCode::POP_JUMP_IF_TRUE : MNPyByteCode::POP_JUMP_IF_FALSE, c_pos);
                    }
                    if (values)
                        values->push_back(create_jump_data(it.pos, it.jump_type,
                                                           it.level, meta_val == 2 ? 1 : it.j_pos));
                }
            }
        }
    }

   void* MN_MNBin::_and_or_test(unsigned int type, MNNode *node)
   {
       ++and_or_level;
       std::vector<MN_Jump_Data>* values = new std::vector<MN_Jump_Data>();
       //std::vector<MN_Jump_Data>* v_local = new std::vector<MN_Jump_Data>();

       auto c_test = node->at(0);
       auto c_loop = node->at(1);
       auto v_type = bin_cast(call(&c_test));
       c_loop.start_cache();

       data.ai16(type == 0 ? MNPyByteCode::JUMP_IF_FALSE_OR_POP : MNPyByteCode::JUMP_IF_TRUE_OR_POP);
       values->push_back(create_jump_data(data.get_cursor_pos()+1, type, and_or_level, 0));
       data.ai32(0);


       auto fill_jumps = [&](bool add=false) {
           if (_is_and_or(v_type))
           {
               auto meta = reinterpret_cast<std::vector<MN_Jump_Data>*>(v_type->meta);
               for (auto it : *meta)
               {
                   if (it.jump_type == type )
                   {
                       values->push_back(it);
                       unsigned int c_pos = it.pos - 2;
                       data.ai16(type == 0 ? MNPyByteCode::JUMP_IF_FALSE_OR_POP : MNPyByteCode::JUMP_IF_TRUE_OR_POP, c_pos);

                   }
                   else
                   {
                       data.ai32(data.get_inst_count(), it.pos);
                       if (!add)
                       {
                           unsigned int c_pos = it.pos - 2;
                           data.ai16(type == 0 ? MNPyByteCode::POP_JUMP_IF_TRUE : MNPyByteCode::POP_JUMP_IF_FALSE, c_pos);
                       }
                       if (add)
                           values->push_back(it);
                   }
               }
           }
       };
       fill_jumps();
       for (unsigned int i=0; i < c_loop.get_count(); ++i)
       {
           if (i % 2 == 0)
           {
               auto c_test = c_loop.at(i+1);
               v_type = bin_cast(call(&c_test));

               if (i < c_loop.get_count()-2)
               {
                   data.ai16(type == 0 ? MNPyByteCode::JUMP_IF_FALSE_OR_POP :  MNPyByteCode::JUMP_IF_TRUE_OR_POP);
                   values->push_back(create_jump_data(data.get_cursor_pos()+1, type, and_or_level, 0));
                   data.ai32(0);
               }
               fill_jumps(i >= c_loop.get_count()-2 ? true : false);
           }
       }
       for (auto it : *values)
           data.ai32(data.get_inst_count(), it.pos);
       --and_or_level;
       return create_bin_type(type == 0 ? MN_PYBINP_DATA::MN_AND : MN_PYBINP_DATA::MN_OR, reinterpret_cast<void*>(values));
   }

   void MN_MNBin::_op_err(MNNode *node, std::string type)
   {
       if (trailer_level == 0 && store_flag)
       {
           error_msg->add_error_text("SyntaxError: ",  "can't assign to " + type + " "
                     + to_string(node->get_row()) + ":" + to_string(node->get_col()),
                     source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
       }
   }

   void MN_MNBin::_add_line(MNNode *node)
   {
       data.ai16(MNPyByteCode::STMT);
       data.ai32(node->get_row());
   }


    void* MN_MNBin::suite(MNNode *node)
    {

        if (node->get_lexeme() != "simple_stmt")
        {

            auto c_t = node->at(2);
            if (c_t.get_meta()!=31)
            {
//                data.ai16(MNPyByteCode::STMT);
//                data.ai32(c_t.get_row());
                bin_cast(call(&c_t));
            }
            else
            {
                for (auto &it : c_t)
                {
//                    data.ai16(MNPyByteCode::STMT);
//                    data.ai32(it.get_row());
                    bin_cast(call(&it));
                }
            }

        }
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::f_string(MNNode *node)
    {
        auto iter = node->at(1);
        for (auto it : iter)
        {
            if (it.non_terminal())
            {
                bin_cast(call(&it));
            }
            else
            {
                _gen_str(MNPyByteCode::LOAD_CONST_STR, it.get_lexeme());
            }
        }
        data.ai16(MNPyByteCode::BUILD_STRING);
        data.ai32(iter.get_count());
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::replacement_field(MNNode *node)
    {
        auto f_expr = node->at(1);
        bin_cast(call(&f_expr));
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::f_expression(MNNode *node)
    {
        auto first = node->at(0);
        bin_cast(call(&first));
        auto loop = node->at(1);
        for (auto it : loop)
            if (it.non_terminal())
                bin_cast(call(&it));
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    void* MN_MNBin::format_spec(MNNode *node)
    {
        return not_impl(node);
    }

    void* MN_MNBin::conditional_expression(MNNode *node)
    {
        return test(node);
    }

    void* MN_MNBin::expression(MNNode *node)
    {
        return not_impl(node);
    }

    void* MN_MNBin::lambda_expr(MNNode *node)
    {
        return not_impl(node);
    }

    void* MN_MNBin::yield_expression(MNNode *node)
    {
        return not_impl(node);
    }


    void* MN_MNBin::expression_list(MNNode *node)
    {
        return not_impl(node);
    }

    void* MN_MNBin::not_impl(MNNode* node)
    {
        error_msg->add_error_text("MN_Error: ",  "Not implemented \"" + node->get_lexeme() + "\" at line "
                  + to_string(node->get_row()) + " column " + to_string(node->get_col()),
                  source_lexer ? source_lexer->get_line(node->get_row()) : "", 0, node->get_col());
        return create_bin_type(MN_PYBINP_DATA::MN_END);
    }

    std::string MN_MNBin::space_gen(unsigned int count)
    {
        std::string res="";
        for (unsigned int i=0; i < count+depth;++i)
            res += "    ";
        return res;
    }

    void* MN_MNBin::create_bin_type(MN_PYBINP_DATA type, void *meta)
    {
        auto res = new MN_PyBinType();
        res->type = type; res->meta = meta;
        return reinterpret_cast<void*>(res);
    }


    std::shared_ptr<MN_PyBinType> MN_MNBin::bin_cast(void *value)
    {
        if (value == nullptr)
            return nullptr;

        return std::shared_ptr<MN_PyBinType>(reinterpret_cast<MN_PyBinType*>(value));
    }

    MN_Jump_Data MN_MNBin::create_jump_data(unsigned int pos, int jump_type, unsigned int level, unsigned int j_pos)
    {
        auto ret = MN_Jump_Data();
        ret.pos = pos;
        ret.jump_type = jump_type;
        ret.level = level;
        ret.j_pos = j_pos;
        return ret;
    }
    std::string MN_MNBinRead::_gen_opcode_spaces(std::string opcode)
    {
          unsigned int spaces = 30 - opcode.size();
          std::string result = "";
          for (unsigned int i=0; i < spaces; ++i) result += " ";
          return result;
    }
    void MN_MNBinRead::show_bincode()
    {

        MN_DataStream data_in;
        data_in.open(name.c_str(), 1);
        int16_t current = data_in.ri16();

        unsigned int current_pos = 0;
        long int current_inst_pos = 0;
        std::cout << std::endl;
        std::string tab = "        ";
        //unsigned int stmt_count = 1;

        //std::cout << stmt_count << " -> Line: 1 " << std::endl << std::endl;
        while (current != MNPyByteCode::HALT)
        {

            switch (current) {
            case ARGS_PARAM_TYPE:
                std::cout << tab << current_inst_pos << " -- ARGS_PARAM_TYPE" << std::endl;
                break;
            case BEGIN_FUNCTION_DEC:
                std::cout << tab << current_inst_pos << " -- BEGIN_FUNCTION_DEC" << std::endl;
                break;
            case BEGIN_FUNC_PARAM:
                std::cout << tab << current_inst_pos << " -- BEGIN_FUNC_PARAM" << std::endl;
                break;
            case BEGIN_TYPEDEF:
                std::cout << tab << current_inst_pos << " -- BEGIN_TYPEDEF" << std::endl;
                break;
            case BINARY_ADD:
                std::cout << tab << current_inst_pos << " -- BINARY_ADD" << std::endl;
                break;
            case BINARY_AND:
                std::cout << tab << current_inst_pos << " -- BINARY_AND" << std::endl;
                break;
            case BINARY_XOR:
                std::cout << tab << current_inst_pos << " -- BINARY_XOR" << std::endl;
                break;
            case BINARY_OR:
                std::cout << tab << current_inst_pos << " -- BINARY_OR" << std::endl;
                break;
            case BINARY_LSHIFT:
                std::cout << tab << current_inst_pos << " -- BINARY_LSHIFT" << std::endl;
                break;
            case BINARY_RSHIFT:
                std::cout << tab << current_inst_pos << " -- BINARY_RSHIFT" << std::endl;
                break;
            case BINARY_SUBSTRACT:
                std::cout << tab << current_inst_pos << " -- BINARY_SUBSTRACT" << std::endl;
                break;
            case BINARY_SUBSCR:
                std::cout << tab << current_inst_pos << " -- BINARY_SUBSCR" << std::endl;
                break;
            case BINARY_POWER:
                std::cout << tab << current_inst_pos << " -- BINARY_POWER" << std::endl;
                break;
            case BINARY_MULTIPLY:
                std::cout << tab << current_inst_pos << " -- BINARY_MULTIPLY" << std::endl;
                break;
            case BINARY_TRUE_DIVIDE:
                std::cout << tab << current_inst_pos << " -- BINARY_TRUE_DIVIDE" << std::endl;
                break;
            case BINARY_MATRIX_MULTIPLY:
                std::cout << tab << current_inst_pos << " -- BINARY_MATRIX_MULTIPLY" << std::endl;
                break;
            case BINARY_FLOOR_DIVIDE:
                std::cout << tab << current_inst_pos << " -- BINARY_FLOOR_DIVIDE" << std::endl;
                break;
            case BINARY_MODULO:
                std::cout << tab << current_inst_pos << " -- BINARY_MODULO" << std::endl;
                break;
            case BUILD_TUPLE:
                {
                    std::string spaces = _gen_opcode_spaces("BUILD_TUPLE");
                    std::cout << tab << current_inst_pos << " -- BUILD_TUPLE " << spaces << " ("
                              << data_in.ri32() << ")" << std::endl;
                    current_pos += 4;
                    current_inst_pos ++;
                }
                break;
            case BUILD_LIST:

                {
                    std::string spaces = _gen_opcode_spaces("BUILD_LIST");
                    std::cout << tab << current_inst_pos << " -- BUILD_LIST " << spaces << " ("
                              << data_in.ri32() << ")" << std::endl;
                    current_pos += 4;
                    current_inst_pos ++;
                }
                break;
            case BUILD_LIST_UNPACK:
            {
                std::string spaces = _gen_opcode_spaces("BUILD_LIST_UNPACK");
                std::cout << tab << current_inst_pos << " -- BUILD_LIST_UNPACK " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case BUILD_TUPLE_UNPACK:
            {
                std::string spaces = _gen_opcode_spaces("BUILD_TUPLE_UNPACK");
                std::cout << tab << current_inst_pos << " -- BUILD_TUPLE_UNPACK " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case BUILD_SET_UNPACK:
            {
                std::string spaces = _gen_opcode_spaces("BUILD_SET_UNPACK");
                std::cout << tab << current_inst_pos << " -- BUILD_SET_UNPACK " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case BUILD_SLICE:
            {
                std::string spaces = _gen_opcode_spaces("BUILD_SLICE");
                std::cout << tab << current_inst_pos << " -- BUILD_SLICE " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case BUILD_SET:
            {
                {
                    std::string spaces = _gen_opcode_spaces("BUILD_SET");
                    std::cout << tab << current_inst_pos << " -- BUILD_SET " << spaces << " ("
                              << data_in.ri32() << ")" << std::endl;
                    current_pos += 4;
                }
                current_inst_pos ++;
            }
                break;
            case BUILD_STRING:
            {
                {
                    std::string spaces = _gen_opcode_spaces("BUILD_STRING");
                    std::cout << tab << current_inst_pos << " -- BUILD_STRING " << spaces << " ("
                              << data_in.ri32() << ")" << std::endl;
                    current_pos += 4;
                }
                current_inst_pos ++;
            }
                break;
            case BUILD_MAP:
            {
                std::string spaces = _gen_opcode_spaces("BUILD_MAP");
                std::cout << tab << current_inst_pos << " -- BUILD_MAP " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case BUILD_MAP_UNPACK_WITH_CALL:
            {
                std::string spaces = _gen_opcode_spaces("BUILD_MAP_UNPACK_WITH_CALL");
                std::cout << tab << current_inst_pos << " -- BUILD_MAP_UNPACK_WITH_CALL " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case BREAK_LOOP:
                std::cout << tab << current_inst_pos << " -- BREAK_LOOP" << std::endl;
                break;
            case CALL_FUNCTION:
                {
                    std::string spaces = _gen_opcode_spaces("CALL_FUNCTION");
                    int16_t pos = data_in.ri16();
                    int16_t key = data_in.ri16();
                    std::cout << tab << current_inst_pos << " -- CALL_FUNCTION " << spaces << " ("
                              << pos << " positional) -- ("
                              << key << " keyword pair)" << std::endl;
                    current_pos += 4;
                    current_inst_pos += 2;
                }
                break;
            case CALL_FUNCTION_VAR:
                {
                    std::string spaces = _gen_opcode_spaces("CALL_FUNCTION_VAR");
                    int16_t pos = data_in.ri16();
                    int16_t key = data_in.ri16();
                    std::cout << tab << current_inst_pos << " -- CALL_FUNCTION_VAR " << spaces << " ("
                              << pos << " positional) -- ("
                              << key << " keyword pair)" << std::endl;
                    current_pos += 4;
                    current_inst_pos += 2;
                }
                break;
            case CALL_FUNCTION_VAR_KW:
                {
                    std::string spaces = _gen_opcode_spaces("CALL_FUNCTION_VAR_KW");
                    int16_t pos = data_in.ri16();
                    int16_t key = data_in.ri16();
                    std::cout << tab << current_inst_pos << " -- CALL_FUNCTION_VAR_KW " << spaces << " ("
                              << pos << " positional) -- ("
                              << key << " keyword pair)" << std::endl;
                    current_pos += 4;
                    current_inst_pos += 2;
                }
                break;
            case CALL_FUNCTION_KW:
                {
                    std::string spaces = _gen_opcode_spaces("CALL_FUNCTION_KW");
                    int16_t pos = data_in.ri16();
                    int16_t key = data_in.ri16();
                    std::cout << tab << current_inst_pos << " -- CALL_FUNCTION_KW " << spaces << " ("
                              << pos << " positional) -- ("
                              << key << " keyword pair)" << std::endl;
                    current_pos += 4;
                    current_inst_pos += 2;
                }
                break;
            case COND_OP_AND:
                std::cout << tab << current_inst_pos << " -- COND_OP_AND" << std::endl;
                break;
            case COMPARE_OP_E:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_E" << std::endl;
                break;
            case COMPARE_OP_EM:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_EM" << std::endl;
                break;
            case COMPARE_OP_B:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_B" << std::endl;
                break;
            case COMPARE_OP_A:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_A" << std::endl;
                break;
            case COMPARE_OP_AE:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_AE" << std::endl;
                break;
            case COMPARE_OP_BE:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_BE" << std::endl;
                break;
            case COMPARE_OP_NE:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_NE" << std::endl;
                break;
            case COMPARE_OP_NOT_IN:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_NOT_IN" << std::endl;
                break;
            case COMPARE_OP_IN:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_IN" << std::endl;
                break;
            case COMPARE_OP_IS:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_IS" << std::endl;
                break;
            case COMPARE_OP_IS_NOT:
                std::cout << tab << current_inst_pos << " -- COMPARE_OP_IS_NOT" << std::endl;
                break;
            case DEFAULT_FUNC_PARAM_VALUE:
                std::cout << tab << current_inst_pos << " -- DEFAULT_FUNC_PARAM_VALUE" << std::endl;
                break;
            case DELETE_NAME:
                {
                    std::string spaces = _gen_opcode_spaces("DELETE_NAME");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- DELETE_NAME " << spaces << " ("
                              << data_in.rstr(str_count).c_str()
                              << ")" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;

                }
                break;
            case DELETE_ATTR:
                {
                    std::string spaces = _gen_opcode_spaces("DELETE_ATTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- DELETE_ATTR " << spaces << " ("
                              << data_in.rstr(str_count).c_str()
                              << ")" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;

                }
                break;
            case DELETE_SUBSCR:
                std::cout << tab << current_inst_pos << " -- DELETE_SUBSCR" << std::endl;
                break;
            case DUP_TOP:
                std::cout << tab << current_inst_pos << " -- DUP_TOP" << std::endl;
                break;
            case END_FINALLY:
                std::cout << tab << current_inst_pos << " -- END_FINALLY" << std::endl;
                break;
            case FOR_ITER:
            {
                std::string spaces = _gen_opcode_spaces("FOR_ITER");
                std::cout << tab << current_inst_pos << " -- FOR_ITER " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
               break;
            case GET_ITER:
                std::cout << tab << current_inst_pos << " -- GET_ITER" << std::endl;
                break;
            case GET_YIELD_FROM_ITER:
                std::cout << tab << current_inst_pos << " -- GET_YIELD_FROM_ITER" << std::endl;
                break;
            case IF:
                std::cout << tab << current_inst_pos << " -- BEGIN_IF" << std::endl;
                break;
            case INPLACE_ADD:
                std::cout << tab << current_inst_pos << " -- INPLACE_ADD" << std::endl;
                break;
            case INPLACE_SUBSTRACT:
                std::cout << tab << current_inst_pos << " -- INPLACE_SUBSTRACT" << std::endl;
                break;
            case INPLACE_XOR:
                std::cout << tab << current_inst_pos << " -- INPLACE_XOR" << std::endl;
                break;
            case INPLACE_AND:
                std::cout << tab << current_inst_pos << " -- INPLACE_AND" << std::endl;
                break;
            case INPLACE_OR:
                std::cout << tab << current_inst_pos << " -- INPLACE_OR" << std::endl;
                break;
            case INPLACE_POWER:
                std::cout << tab << current_inst_pos << " -- INPLACE_POWER" << std::endl;
                break;
            case INPLACE_MULTIPLY:
                std::cout << tab << current_inst_pos << " -- INPLACE_MULTIPLY" << std::endl;
                break;
            case INPLACE_TRUE_DIVIDE:
                std::cout << tab << current_inst_pos << " -- INPLACE_TRUE_DIVIDE" << std::endl;
                break;
            case INPLACE_MATRIX_MULTIPLY:
                std::cout << tab << current_inst_pos << " -- INPLACE_MATRIX_MULTIPLY" << std::endl;
                break;
            case INPLACE_FLOOR_DIVIDE:
                std::cout << tab << current_inst_pos << " -- INPLACE_FLOOR_DIVIDE" << std::endl;
                break;
            case INPLACE_MODULO:
                std::cout << tab << current_inst_pos << " -- INPLACE_MODULO" << std::endl;
                break;
            case IMPORT_NAME:
                {
                    std::string spaces = _gen_opcode_spaces("IMPORT_NAME");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- IMPORT_NAME " << spaces << " ("
                              << data_in.rstr(str_count).c_str()
                              << ")" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;

                }
                break;
            case IMPORT_FROM:
                {
                    std::string spaces = _gen_opcode_spaces("IMPORT_FROM");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- IMPORT_FROM " << spaces << " ("
                              << data_in.rstr(str_count).c_str()
                              << ")" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;

                }
                break;
            case KWARGS_PARAM_TYPE:
                std::cout << tab << current_inst_pos << " -- KWARGS_PARAM_TYPE" << std::endl;
                break;
            case END_FUNCTION_DEC:
                std::cout << tab << current_inst_pos << " -- END_FUNCTION_DEC" << std::endl;
                break;
            case END_FUNC_PARAM:
                std::cout << tab << current_inst_pos << " -- END_FUNC_PARAM" << std::endl;
                break;
            case END_TYPEDEF:
                std::cout << tab << current_inst_pos << " -- END_TYPEDEF" << std::endl;
                break;
            case END_IF:
                std::cout << tab << current_inst_pos << " -- END_IF" << std::endl;
                break;
            case LOAD_CONST_STR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_STR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_STR " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_HEX:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_HEX");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_HEX " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_BIN:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_BIN");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_BIN " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_USTR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_USTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_USTR " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_BSTR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_BSTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_BSTR " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_RSTR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_RSTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_RSTR " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_EXP:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_EXP");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_EXP " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_MSTR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_MSTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_MSTR " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_FSTR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_FSTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_FSTR " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_BRSTR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_BRSTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_BRSTR " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_RBSTR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_RBSTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_RBSTR " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_CONST_INT:
            {
                std::string spaces = _gen_opcode_spaces("LOAD_CONST_INT");
                std::cout << tab << current_inst_pos << " -- LOAD_CONST_INT " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case LOAD_CONST_FLOAT:
            {
                std::string spaces = _gen_opcode_spaces("LOAD_CONST_32F");
                std::cout << tab << current_inst_pos << " -- LOAD_CONST_32F " << spaces << " ("
                          << data_in.rfloat() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case LOAD_CONST_IMG:
            {
                std::string spaces = _gen_opcode_spaces("LOAD_CONST_IMG");
                std::cout << tab << current_inst_pos << " -- LOAD_CONST_IMG " << spaces << " ("
                          << data_in.rfloat() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case LOAD_CONST_OCT:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_CONST_OCT");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_CONST_OCT " << spaces << " ('"
                              << data_in.rstr(str_count).c_str()
                              << "')" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;
                }
                break;
            case LOAD_ATTR:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_ATTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_ATTR " << spaces << " ("
                              << data_in.rstr(str_count).c_str()
                              << ")" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;

                }
                break;
            case LOAD_NAME:
                {
                    std::string spaces = _gen_opcode_spaces("LOAD_NAME");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- LOAD_NAME " << spaces << " ("
                              << data_in.rstr(str_count).c_str()
                              << ")" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;

                }
                break;
            case LOAD_BUILD_CLASS:
                std::cout << tab << current_inst_pos << " -- LOAD_BUILD_CLASS" << std::endl;
                break;
            case MAKE_CLASS:
                std::cout << tab << current_inst_pos << " -- MAKE_CLASS" << std::endl;
                break;
            case RETURN_VALUE:
                std::cout << tab << current_inst_pos << " -- RETURN_VALUE" << std::endl;
                break;
            case ROT_TWO:
                std::cout << tab << current_inst_pos << " -- ROT_TWO" << std::endl;
                break;
            case ROT_THREE:
                std::cout << tab << current_inst_pos << " -- ROT_THREE" << std::endl;
                break;
            case STORE_NAME:
                {
                    std::string spaces = _gen_opcode_spaces("STORE_NAME");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- STORE_NAME " << spaces << " ("
                              << data_in.rstr(str_count).c_str()
                              << ")" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;

                }
                break;
            case POP_BLOCK:
                std::cout << tab << current_inst_pos << " -- POP_BLOCK" << std::endl;
                break;

            case POP_JUMP_IF_FALSE:
            {
                std::string spaces = _gen_opcode_spaces("POP_JUMP_IF_FALSE");
                std::cout << tab << current_inst_pos << " -- POP_JUMP_IF_FALSE " << spaces << " "
                          << data_in.ri32() << "" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }

                break;
            case POP_JUMP_IF_TRUE:
            {
                std::string spaces = _gen_opcode_spaces("POP_JUMP_IF_TRUE");
                std::cout << tab << current_inst_pos << " -- POP_JUMP_IF_TRUE " << spaces << " "
                          << data_in.ri32() << "" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }

                break;
            case POP_TOP:
                std::cout << tab << current_inst_pos << " -- POP_TOP" << std::endl;
                break;
            case POP_EXCEPT:
                std::cout << tab << current_inst_pos << " -- POP_EXCEPT" << std::endl;
                break;
            case JUMP:
            {
                std::string spaces = _gen_opcode_spaces("JUMP");
                std::cout << tab << current_inst_pos << " -- JUMP " << spaces << " "
                          << data_in.ri32() << "" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }

                break;
            case JUMP_IF_TRUE_OR_POP:
            {
                std::string spaces = _gen_opcode_spaces("JUMP_IF_TRUE_OR_POP");
                std::cout << tab << current_inst_pos << " -- JUMP_IF_TRUE_OR_POP " << spaces << " "
                          << data_in.ri32() << "" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }

                break;
            case JUMP_IF_FALSE_OR_POP:
            {
                std::string spaces = _gen_opcode_spaces("JUMP_IF_FALSE_OR_POP");
                std::cout << tab << current_inst_pos << " -- JUMP_IF_FALSE_OR_POP " << spaces << " "
                          << data_in.ri32() << "" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }

                break;
            case RAISE_VARARGS:
            {
                std::string spaces = _gen_opcode_spaces("RAISE_VARARGS");
                std::cout << tab << current_inst_pos << " -- RAISE_VARARGS " << spaces << " ("
                          << data_in.ri16() << ")" << std::endl;
                current_pos += 2;
                current_inst_pos ++;
            }
                break;
            case STMT:
                std::cout << /*++stmt_count <<*/ " Line: " << data_in.ri32()
                          << std::endl << std::endl;
                //data_in.ri16();
                current_pos += 4;
                current_inst_pos ++;

                break;
            case STORE_SUBSCR:
                std::cout << tab << current_inst_pos << " -- STORE_SUBSCR" << std::endl;
                break;
            case STORE_ATTR:
                {
                    std::string spaces = _gen_opcode_spaces("STORE_ATTR");
                    int32_t str_count = data_in.ri32();
                    std::cout << tab << current_inst_pos << " -- STORE_ATTR " << spaces << " ("
                              << data_in.rstr(str_count).c_str()
                              << ")" << std::endl;
                    current_pos += 4 + str_count;
                    current_inst_pos += 2;

                }
                break;
            case SINGLE_TYPEDEF:
                std::cout << tab << current_inst_pos << " -- SINGLE_TYPEDEF" << std::endl;
                break;
            case SETUP_EXCEPT:
            {
                uint32_t from = data_in.ri32();
                std::string spaces = _gen_opcode_spaces("SETUP_EXCEPT");
                std::cout << tab << current_inst_pos << " -- SETUP_EXCEPT   " << spaces
                          << from  << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case SETUP_FINALLY:
            {
                uint32_t from = data_in.ri32();
                std::string spaces = _gen_opcode_spaces("SETUP_FINALLY");
                std::cout << tab << current_inst_pos << " -- SETUP_FINALLY   " << spaces
                          << from << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case SETUP_LOOP:
            {
                std::string spaces = _gen_opcode_spaces("SETUP_LOOP");
                std::cout << tab << current_inst_pos << " -- SETUP_LOOP " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case SETUP_LOOP_FOR:
            {
                std::string spaces = _gen_opcode_spaces("SETUP_LOOP_FOR");
                std::cout << tab << current_inst_pos << " -- SETUP_LOOP_FOR " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case SETUP_LOOP_WHILE:
            {
                std::string spaces = _gen_opcode_spaces("SETUP_LOOP_WHILE");
                std::cout << tab << current_inst_pos << " -- SETUP_LOOP_WHILE " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case SETUP_WITH:
            {
                uint32_t from = data_in.ri32();
                std::string spaces = _gen_opcode_spaces("SETUP_WITH");
                std::cout << tab << current_inst_pos << " -- SETUP_WITH   " << spaces
                          << from << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
                break;
            case UNPACK_SEQUENCE:
            {
                std::string spaces = _gen_opcode_spaces("UNPACK_SEQUENCE");
                std::cout << tab << current_inst_pos << " -- UNPACK_SEQUENCE " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
               break;
            case UNPACK_EX:
            {
                std::string spaces = _gen_opcode_spaces("UNPACK_EX");
                std::cout << tab << current_inst_pos << " -- UNPACK_EX " << spaces << " ("
                          << data_in.ri32() << ")" << std::endl;
                current_pos += 4;
                current_inst_pos ++;
            }
               break;
            case UNARY_NOT:
                std::cout << tab << current_inst_pos << " -- UNARY NOT" << std::endl;
                break;
            case UNARY_NEGATIVE:
                std::cout << tab << current_inst_pos << " -- UNARY_NEGATIVE" << std::endl;
                break;
            case UNARY_POSITIVE:
                std::cout << tab << current_inst_pos << " -- UNARY_POSITIVE" << std::endl;
                break;
            case UNARY_INVERT:
                std::cout << tab << current_inst_pos << " -- UNARY_INVERT" << std::endl;
                break;
            case WITH_CLEANUP_START:
                std::cout << tab << current_inst_pos << " -- WITH_CLEANUP_START" << std::endl;
                break;
            case WITH_CLEANUP_FINISH:
                std::cout << tab << current_inst_pos << " -- WITH_CLEANUP_FINISH" << std::endl;
                break;
            case YIELD_VALUE:
                std::cout << tab << current_inst_pos << " -- YIELD_VALUE" << std::endl;
                break;
            case YIELD_FROM:
                std::cout << tab << current_inst_pos << " -- YIELD_FROM" << std::endl;
                break;
            default:
                break;
            }
            data_in >> current;
            current_pos += 2;
            current_inst_pos++;
        }
        std::cout << tab << current_inst_pos << " -- HALT" << std::endl;
        std::cout <<  " ____________________ " << std::endl << std::endl;
        std::cout <<  "Total: " << current_pos+2 << " bytes" << std::endl;
        std::cout <<  " ____________________ " << std::endl << std::endl;
    }
}

