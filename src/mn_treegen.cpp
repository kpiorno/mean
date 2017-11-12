/*************************************************************************/
/*  mn_treegen.cpp                                                       */
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
#include "mn_treegen.h"

namespace mn
{
    MNTreeGen::MNTreeGen(MNGrammarItem* grammar_tree, MNLexer* lexer,
                         MNErrorMsg* error_msg, MN_ParseMode mode,
                         bool minify, long int depth)
    {
        this->lexer = lexer;
        this->grammar_tree = grammar_tree->get_node();
        this->grammar_item = grammar_tree;
        this->depth = depth;
        mem_pool = new MemoryManager();
        this->error_msg = error_msg;
        current_token = nullptr;
        g_pos = -1;
        tokens.clear();
        tokens.reserve(100);

        consume();
        _break_node = new MNNode(mem_pool, 0, "MN_BREAK", MN_BREAK_NODE);
        token_consumed = 0;
        p_mode = mode;
        this->minify = minify;
        agressive_remove = true;

    }

    MNTreeGen::~MNTreeGen()
    {
        delete mem_pool;
        delete _break_node;
        for (auto &it : rules)
        {
            delete it.second;
        }
        for (auto &it : tokens)
        {
            delete it;
        }
    }

    MNNode* MNTreeGen::generate(std::string entry)
    {
        for (auto &it : *grammar_tree)
        {
            rules.insert(std::make_pair(it.get_lexeme(),
                                        it.ptr_copy()));
        }
        if (rules.find(entry) == rules.end())
        {
            error_msg->add_error_text("TreeGen: ", "The rule '"
                                  + entry + "' doesn't exist" );
            return new MNNode(mem_pool, 0, "[...]", 0);
        }

        auto rule = mn_rule(rules[entry], true, true);
        while (current_token->get_type() != TK_END)
        {
            if (current_token->get_type() != TK_EOL)
                error_msg->add_error_text("TreeGen: ", "unexpected '" + current_token->get_lexeme() + "' at "
                                      + current_token->get_char_position());
            consume();

        }
        /*for (unsigned int i=0; i < tokens.size();++i)
        {
            std::cout << "TK ORD - " << i << " : " << tokens[i]->get_lexeme() << std::endl;
        }*/
        return rule;

    }

    inline MNNode* MNTreeGen::mn_at(MNNode *node, bool required)
    {
        MNNode* at_result = new MNNode(mem_pool, 100, node->get_lexeme(), 0);

        node->start_cache();
        auto ptr1 = node->at(2);
        auto left = process(&ptr1, required );
        if (!required && _is_break_node(left))
        {
            delete at_result;
            return break_node();
        }
        if (_is_valid(left))
        {
            at_result->add_node(left);
            if (!_is_break_node(left))
                delete left;
        }

        auto ptr2 = node->at(1);
        auto at_op = process(&ptr2);
        auto ptr3 = node->at(3);
        auto right = process(&ptr3);

        if (_is_valid(right))
        {
            at_result->add_node(right);
            if (!_is_break_node(right))
                delete right;

        }
        if (!_is_break_node(at_op) && _is_valid(at_op))
            delete at_op;
        return at_result;

    }

    inline MNNode* MNTreeGen::mn_or(MNNode *node, bool required)
    {
         std::vector<unsigned int>* c_pos;

         unsigned int c_meta = -1;
         if ( current_token->get_type() == TK_RULE )
             c_meta = 10;
         else
         if ( current_token->get_type() == TK_CHARACTER )
             c_meta = 21;
         else
         if ( current_token->get_type() == TK_STRING )
             c_meta = 82;
         else
         if ( current_token->get_type() == TK_ID )
             c_meta = 81;
         else
         if ( current_token->get_type() == TK_EOL )
             c_meta = 83;
         else
         if ( current_token->get_type() == TK_INDENT )
             c_meta = 84;
         else
         if ( current_token->get_type() == TK_DEDENT )
             c_meta = 85;
         else
         if ( current_token->get_type() == TK_NUMBER )
             c_meta = 87;
         else
         {
              c_token res = lexer->get_custom_token(current_token->get_type());
              if (res.exist_custom_token)
                  c_meta = res.type;
         }

         if (current_token->get_type() == TK_ID || current_token->get_type() == TK_CHARACTER   )
         {
            c_pos = grammar_item->get_or_pos_string(node->get_meta_aux(), c_meta, current_token->get_lexeme());
            if (c_pos->size() == 0)
            {
                delete c_pos;
                c_pos = grammar_item->get_or_pos(node->get_meta_aux(), c_meta);
            }
         }
         else
         {
                c_pos = grammar_item->get_or_pos(node->get_meta_aux(), c_meta);
         }

         //c_pos = grammar_item->get_or_pos_string(node->get_meta_aux(), 80, current_token->get_lexeme());
         bool force_required = true;
         if (c_pos->size() != 0 )
         {

             if (c_pos->size() == 1)
             {
                 auto node_ptr = node->ptr_at(c_pos->at(0));
                 auto result = process(node_ptr, required);
                 delete node_ptr;

                 if ((result->get_meta() == MN_NT_VALUE || result->get_meta() == 0) && result->get_count() == 1)
                 {
                     auto res = result->ptr_at(0);
                     if (agressive_remove) result->deallocate();
                     delete result;
                     return res;
                 }
                 return result;

             }
             else
             {
                 for (unsigned int i=0; i < c_pos->size();++i)
                 {
                     long int r_pos = _current_gpos();
                     auto node_ptr = node->ptr_at(c_pos->at(i));
                     auto result = process(node_ptr, false);
                     delete node_ptr;

                     //TO DO: Checking depth
                     if (_is_break_node(result) /*&& (g_pos - r_pos < depth)*/)
                     {
                         _restore_gpos(r_pos);
                         continue;
                     }

                     if ((result->get_meta() == MN_NT_VALUE || result->get_meta() == 0) && result->get_count() == 1)
                     {
                         auto res = result->ptr_at(0);
                         if (agressive_remove) result->deallocate();
                         delete result;
                         return res;
                     }
                     return result;

                 }
             }

         }
         else
             delete c_pos;

         if (required && !is_required() && force_required)
         {
             //return break_node();
             error_msg->add_error_text("TreeGen: ", "unexpected '" + current_token->get_lexeme() + "' at "
                                      + current_token->get_char_position());
             auto result = new MNNode(mem_pool, 100, current_token->get_lexeme(),
                               7);
             result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
             consume();
             return result;
         }
         else
         {
             return break_node();
         }
    }

    inline MNNode* MNTreeGen::mn_id(MNNode *node, bool required)
    {
        if (node->get_lexeme() == "NEWLINE" )
        {
            if (current_token->get_type() != TK_EOL)
            {
                if (required && !is_required())
                {
                    /*error_msg->add_error_text("TreeGen: ", "expected '" + node->get_lexeme() + "' at "
                                              + current_token->get_char_position());*/
                    error_msg->add_error_text("TreeGen: ", "unexpected '" + current_token->get_lexeme() + "' at "
                                              + current_token->get_char_position());



                }
                else
                {
                    return break_node();
                }
            }
            else
            {
                auto result = new MNNode(mem_pool, 100, current_token->get_lexeme(),
                                  4);
                result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
                consume();
                return result;
            }
        }
        else if (node->get_lexeme() == "INDENT" )
        {
            if (current_token->get_type() != TK_ID || current_token->get_lexeme() != "<INDENT>")
            {
                if (required && !is_required())
                {
                    error_msg->add_error_text("TreeGen: ", "expected an indent block at "
                                              + current_token->get_char_position());
                }
                else
                {
                    return break_node();
                }
            }
            else
            {
                auto result = new MNNode(mem_pool, 100, current_token->get_lexeme(),
                                  5);
                result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
                consume();
                return result;
            }
        }
        else if (node->get_lexeme() == "DEDENT" )
        {
            if (current_token->get_type() != TK_ID || current_token->get_lexeme() != "<DEDENT>")
            {
                if (required && !is_required())
                {
                    error_msg->add_error_text("TreeGen: ", "unexpected indent at "
                                              + current_token->get_char_position());
                }
                else
                {
                    return break_node();
                }
            }
            else
            {
                auto result = new MNNode(mem_pool, 100, current_token->get_lexeme(),
                                  6);
                result->set_row(current_token->get_row()); result->set_col(current_token->get_col());

                consume();
                return result;
            }
        }
        else if (node->get_lexeme() == "ENDMARKER" )
        {
            if (current_token->get_type() != TK_END)
            {
                if (required && !is_required())
                {
                    /*error_msg->add_error_text("TreeGen: ", "expected '" + node->get_lexeme() + "' at "
                                              + current_token->get_char_position());*/
                    error_msg->add_error_text("TreeGen: ", "invalid sintax at "
                                              + current_token->get_char_position());


                }
                else
                {
                    return break_node();
                }
            }
            else
            {
                auto result = new MNNode(mem_pool, 1, current_token->get_lexeme(),
                                  TK_END);
                result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
                consume();
                return result;
            }
        }
        else if (node->get_lexeme() == "STR" )
        {
            if (current_token->get_type() != TK_STRING)
            {
                if (required && !is_required())
                {
                    error_msg->add_error_text("TreeGen: ", "expected '" + node->get_lexeme() + "' at "
                                              + current_token->get_char_position());
                }
                else
                {
                    return break_node();
                }
            }
            else
            {
                auto result = new MNNode(mem_pool, 1, current_token->get_lexeme(),
                                  TK_STRING);
                result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
                consume();
                return result;
            }
        }
        else if (node->get_lexeme() == "NUMBER" )
        {
            if (current_token->get_type() != TK_NUMBER)
            {
                if (required && !is_required())
                {
                    error_msg->add_error_text("TreeGen: ", "expected '" + node->get_lexeme() + "' at "
                                              + current_token->get_char_position());
                }
                else
                {
                    return break_node();
                }
            }
            else
            {
                auto result = new MNNode(mem_pool, 1, current_token->get_lexeme(),
                                  TK_NUMBER);
                result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
                consume();
                return result;
            }
        }
        else
        {
            c_token ret = lexer->get_custom_token(node->get_lexeme());
            if (ret.exist_custom_token)
            {
                 if (current_token->get_type() != ret.type)
                 {
                     if (required && !is_required())
                     {
                         error_msg->add_error_text("TreeGen: ", "expected '" + node->get_lexeme() + "' at "
                                                   + current_token->get_char_position());
                     }
                     else
                     {
                         return break_node();
                     }
                 }
                 else
                 {
                     auto result = new MNNode(mem_pool, 1, current_token->get_lexeme(),
                                       ret.type);
                     result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
                     consume();
                     return result;
                 }

            }


            if (current_token->get_type() != TK_ID
                    || (current_token->get_type() == TK_ID
                        && (grammar_item->exist_keyword(current_token->get_lexeme())
                        || is_special_id())))
            {
                if (required && !is_required())
                {
                    error_msg->add_error_text("TreeGen: ", "expected '" + node->get_lexeme() + "' at "
                                              + current_token->get_char_position());
                }
                else
                {
                    return break_node();
                }
            }
            else
            {
                auto result = new MNNode(mem_pool, 1, current_token->get_lexeme(),
                                  7);
                result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
                consume();
                return result;
            }
        }

        auto result = new MNNode(mem_pool, 1, current_token->get_lexeme(),
                          current_token->get_type());
        consume();
        return result;
    }

    inline MNNode* MNTreeGen:: mn_rule(MNNode *node, bool required, bool is_rule)
    {

        MNNode* rule = new MNNode(mem_pool, 100, node->get_lexeme(), is_rule ? MN_NT_VALUE: MN_L_VALUE);
        rule->set_row(current_token->get_row()); rule->set_col(current_token->get_col());

        rule_info inf;
        inf.t_count = 0;
        auto t_node = node;
        if (is_rule) {
            t_node = node->ptr_at(0);
        }
        t_node->start_cache();
        bool is_sub_rule = true;
        for (unsigned int i=0; i < t_node->get_count();++i)
        {
            auto it = t_node->at(i);

            auto element = process(&it, inf.t_count < depth ? required : true);

            if (/*rule->get_count()<=2*/ inf.t_count < depth && !required && (_is_break_node(element) ) )
            {
                if (agressive_remove) rule->deallocate();
                delete rule;
                //is_sub_rule = false;
                rule = break_node();
                break;
            }

            if ((it.get_meta() != 31 && it.get_meta() != 50) ||
                    (it.get_meta() == 31 && !_is_break_node(element)) ||
                     (it.get_meta() == 50 && element->is_found()))
            {
                //check_required = false;
                if (!required)
                    ++inf.t_count;
            }

            if (_is_valid(element))
            {
                auto e = element;
                bool is_break_node = _is_break_node(element);
                if (p_mode == MN_PARSEMODE_LVERBOSE || p_mode == MN_PARSEMODE_MVERBOSE)
                {
                    if (e->get_meta() == MN_L_VALUE && (p_mode == MN_PARSEMODE_MVERBOSE ? e->get_count() == 1 ? true : false : true))
                    {
                        if (p_mode == MN_PARSEMODE_MVERBOSE)
                        {
                            auto ic = e->ptr_at(0);
                            rule->add_node(ic);

                            if (i == 0)
                            {
                                auto ic = e->ptr_at(0);
                                if (ic->get_meta() != MN_NT_VALUE)
                                    is_sub_rule = false;
                                if(!_is_break_node(ic))
                                   delete ic;
                            }
                            else
                            {
                                auto ic = e->ptr_at(0);

                                if (!(ic->get_meta() == MN_NEXIST_T || (ic->get_meta() == MN_TG_LIST && ic->get_count() == 0 )))
                                    is_sub_rule = false;
                            }

                            if(!_is_break_node(ic))
                               delete ic;

                        }
                        else
                        {
                            for (unsigned int i=0; i < e->get_count(); ++i)
                            {
                               auto it = e->at(i);
                               rule->add_node(&it);
                            }
                            is_sub_rule = false;
                            e->break_by_max_count();
                        }
                        if (agressive_remove)
                        {
                            e->deallocate();
                        }

                    }
                    else
                    {
                         rule->add_node(e);

                         if (minify)
                         {
                             if (i == 0)
                             {
                                 auto first = rule->at(0);
                                 if (first.get_meta() != MN_NT_VALUE) is_sub_rule = false;
                             }
                             else
                             {
                                 auto rest = rule->at(i);
                                 if (!(rest.get_meta() == MN_NEXIST_T || (rest.get_meta() == MN_TG_LIST && rest.get_count() == 0 )))
                                     is_sub_rule = false;
                             }
                         }
                    }
                }
                else
                {
                    rule->add_node(e);


                    if (minify)
                    {
                        if (i == 0)
                        {
                            auto first = rule->at(0);
                            if (first.get_meta() != MN_NT_VALUE) is_sub_rule = false;
                        }
                        else
                        {
                            auto rest = rule->at(i);
                            if (!(rest.get_meta() == MN_NEXIST_T || (rest.get_meta() == MN_TG_LIST && rest.get_count() == 0 )))
                                is_sub_rule = false;
                        }
                    }
                }
                if (!is_break_node)
                    delete element;
            }
        }
        if (is_rule)
        {
            delete t_node;
        }

        if (minify && is_sub_rule && rule->get_count() > 0)
        {
            auto ret = rule->ptr_at(0);
            if (agressive_remove)
                rule->deallocate();
            delete rule;
            return ret;
        }
        return rule;
    }

    inline MNNode* MNTreeGen::mn_string(MNNode *node, bool required)
    {
         if (current_token->get_type() != TK_ID
                 || current_token->get_lexeme() != node->get_lexeme() )
         {
             if (required && !is_required())
             {
                 error_msg->add_error_text("TreeGen: ", "expected '"
                                           + node->get_lexeme() + "' at "
                                           + current_token->get_char_position());
                 /*error_msg->add_error_text("TreeGen: ", "invalid sintax at "
                                           + current_token->get_char_position());*/

             }
             else
             {
                 return break_node();
             }
         }
         auto result = new MNNode(mem_pool, 0, node->get_lexeme(), 1);
         result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
         consume();
         return result;
    }

    inline MNNode* MNTreeGen::mn_vector(MNNode *node, bool required)
    {
         return mn_rule(node, required);
    }

   inline  MNNode* MNTreeGen::mn_brackets(MNNode *node, bool /*required*/)
    {
        long int c_pos = _current_gpos();
        MNNode* res =  mn_rule(node, false);
        if (_is_break_node(res) && (g_pos - c_pos < depth) )
        {
            MNNode* non = new MNNode(mem_pool, 0, "(?)", MN_NEXIST_T);
            _restore_gpos(c_pos);
            return non;
        }
        return res;
    }

    inline MNNode* MNTreeGen::mn_character(MNNode *node, bool required)
    {
         if (current_token->get_lexeme() != node->get_lexeme()
                 || current_token->get_type() != TK_CHARACTER)
         {
             if (required && !is_required())
             {
                 std::string l1 = node->get_lexeme();
                 std::string l2 = current_token->get_lexeme();
                 error_msg->add_error_text("TreeGen: ", "expected character '"
                                           + node->get_lexeme() + "' at "
                                           + current_token->get_char_position());
                 /*error_msg->add_error_text("TreeGen: ", "invalid sintax at "
                                           + current_token->get_char_position());*/
             }
             else
             {
                 return break_node();
             }
         }
         auto result = new MNNode(mem_pool, 1, node->get_lexeme(), 2);
         result->set_row(current_token->get_row()); result->set_col(current_token->get_col());
         consume();
         return result;
    }


    inline MNNode* MNTreeGen::mn_parenthesis(MNNode *node, bool required)
    {
        return mn_rule(node, required);
    }

    inline MNNode* MNTreeGen::mn_parenthesis_many(MNNode *node, bool /*required*/)
    {
        MNNode* list = new MNNode(mem_pool, 1, MN_VECTOR_NAME, MN_TG_LIST);
        list->set_row(current_token->get_row()); list->set_col(current_token->get_col());


        long int c_pos = _current_gpos();
        //MNNode* res_node = mn_parenthesis(node, false);
        MNNode* res_node = mn_rule(node, false);

        //TO DO: Check this statement commented

        if (_is_valid(res_node) && _is_break_node(res_node) && ((g_pos - c_pos) < depth))
        {
            _restore_gpos(c_pos);
        }


        while (!_is_break_node(res_node))
        {

            auto e = res_node;
            if (p_mode == MN_PARSEMODE_LVERBOSE || p_mode == MN_PARSEMODE_MVERBOSE)
            {
                if (e->get_meta() == MN_L_VALUE && (p_mode == MN_PARSEMODE_MVERBOSE ? e->get_count() == 1 ? true : false : true))
                {
                    if (p_mode == MN_PARSEMODE_MVERBOSE)
                    {
                        auto i = e->ptr_at(0);
                        list->add_node(i);
                        if(!_is_break_node(i))
                           delete i;

                    }
                    else
                    {
                        //e->start_cache();
                        for (unsigned int i=0; i < e->get_count(); ++i)
                        {
                           auto it = e->at(i);
                           list->add_node(&it);

                        }

                    }
                }
                else
                {
                    list->add_node(e);
                }
            }
            else
            {
                list->add_node(e);
            }



            if (_is_valid(res_node))
                delete res_node;
            long int c_pos = _current_gpos();
            res_node = mn_parenthesis(node, false);
            //TO DO: Checking depth
            if (_is_valid(res_node) && _is_break_node(res_node) && ((g_pos - c_pos) < depth))
            {
                _restore_gpos(c_pos);
            }

        }

        if (_is_valid(res_node) && !_is_break_node(res_node))
            delete res_node;


        if (list->get_count() == 1)
        {
            auto res = list->ptr_at(0);
            if (agressive_remove) list->deallocate();
            delete list;
            return res;
        }

        list->break_by_max_count();
        return list;
    }

    inline MNNode* MNTreeGen::mn_parenthesis_req_many(MNNode *node, bool required)
    {
        MNNode* list = new MNNode(mem_pool, 100, MN_VECTOR_NAME, MN_TG_REQ_LIST);
        list->set_row(current_token->get_row()); list->set_col(current_token->get_col());

        long int c_pos = _current_gpos();
        MNNode* res_node = mn_parenthesis(node, required);
        //TO DO: Checking depth
        if (!required && _is_valid(res_node) && _is_break_node(res_node) && ((g_pos - c_pos) < depth))
        {
            _restore_gpos(c_pos);
        }
        while (!_is_break_node(res_node))
        {
            //list->add_node(res_node);
            auto e = res_node;
            if (p_mode == MN_PARSEMODE_LVERBOSE || p_mode == MN_PARSEMODE_MVERBOSE)
            {
                if (e->get_meta() == MN_L_VALUE && (p_mode == MN_PARSEMODE_MVERBOSE ? e->get_count() == 1 ? true : false : true))
                {
                    if (p_mode == MN_PARSEMODE_MVERBOSE)
                    {
                        auto i = e->ptr_at(0);
                        list->add_node(i);
                        if(!_is_break_node(i))
                           delete i;

                    }
                    else
                    {
                        for (unsigned int i=0; i < e->get_count(); ++i)
                        {
                           auto it = e->at(i);
                           list->add_node(&it);

                        }
                    }
                }
                else
                {
                    //if (!(e->get_meta() == 30 && e->get_count() == 0))
                        list->add_node(e);
                }
            }
            else
                list->add_node(e);

            if (_is_valid(res_node))
                delete res_node;
            long int c_pos = _current_gpos();
            res_node = mn_parenthesis(node, false);
            //TO DO: Cheking depth
            if (_is_valid(res_node) && _is_break_node(res_node) && ((g_pos - c_pos) < depth))
            {
                _restore_gpos(c_pos);
            }
        }
        if (list->get_count() == 1)
        {
            auto res = list->ptr_at(0);
            if (agressive_remove) list->deallocate();
            delete list;
            return res;
        }
        list->break_by_max_count();
        return list;
    }

    inline MNNode* MNTreeGen::process(MNNode *node, bool required)
    {
        switch (node->get_meta()) {
        case 1:
            return mn_rule(node, required);
            break;
        case 10:
            return mn_rule(rules[node->get_lexeme()], required, true);
            break;
        case 21:
            return mn_character(node, required);
            break;
        case 30:
            return mn_parenthesis(node, required);
            break;
        case 31:
            return mn_parenthesis_many(node, required);
            break;
        case 32:
            return mn_parenthesis_req_many(node, required);
            break;
        case 50:
            return mn_brackets(node, required);
            break;
        case 60:
            return mn_or(node, required);
            break;
        case 70:
            return mn_at(node, required);
            break;
        case 80:
            return mn_string(node, required);
            break;
        case 81:
            return mn_id(node, required);
            break;
        default:
            break;
        }
        return nullptr;
    }

    inline void MNTreeGen::consume()
    {
        //if (current_token != nullptr)
        //    delete current_token;
        token_consumed++;
        if (g_pos == -1 || g_pos >= (long int)tokens.size())
        {
            current_token = lexer->next_token();
            tokens.push_back(current_token);
            g_pos = tokens.size();
        }
        else
            current_token = tokens[g_pos++];
        /*if (current_token->get_type() == TK_END)
            g_pos = tokens.size();*/
    }

    inline bool MNTreeGen::is_required()
    {
        return !true;
        if (!rules_info.empty())
        {
            auto t = rules_info.top();
            if (t.t_count < 2)
                return true;
        }
        return false;
    }

    inline long int MNTreeGen::_current_gpos()
    {
        return g_pos <= 0 ? -2 : g_pos;
    }

    inline void MNTreeGen::_restore_gpos(long int c_pos)
    {
        if (c_pos == -2)
        {
            current_token = tokens[0];
            this->g_pos = 1;
        }
        else
        {
            this->g_pos = c_pos;
            current_token = tokens[this->g_pos-1];
        }

    }

    inline bool MNTreeGen::is_special_id()
    {
        if (current_token->get_lexeme() == "<DEDENT>" ||
            current_token->get_lexeme() == "<INDENT>" )
                return true;
            return false;
    }


    inline MNNode* MNTreeGen::break_node()
    {
        return _break_node;
    }

    inline  bool MNTreeGen::_is_valid(MNNode *node)
    {
        if (node == nullptr)
            return false;
        return true;
    }

    inline bool MNTreeGen::_is_break_node(MNNode *node)
    {
        if (node != nullptr)
        {
            if (node->get_meta() == MN_BREAK_NODE)
                return true;
        }
        return false;
    }

}

