/*************************************************************************/
/*  mn_parser.cpp                                                        */
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
#include "mn_parser.h"

namespace mn
{
    MNParser::MNParser(MNLexer* lexer, MNLexer* p_lexer, MNErrorMsg* error_msg)
    {
        this->lexer = lexer;
        this->p_lexer = p_lexer;
        this->error_msg = error_msg;
        current_token = nullptr;
        consume();
    }

    MNParser::~MNParser()
    {
        delete current_token;
    }

    void MNParser::match(TokenType type)
    {
        if (current_tk_is(type))
        {
            consume();
        }
        else
        {
            error_msg->add_error_text("MNParser: ", "expected '" + MNToken::to_str(type) + "' token at "
                                      + current_token->get_char_position(),
                                      lexer->get_line(current_token->get_row()), 0, current_token->get_col());
            recuperate();
        }
    }

    void MNParser::consume()
    {
        if (current_token != nullptr)
            delete current_token;
        current_token = lexer->next_token();
    }

    bool MNParser::current_tk_is(TokenType type)
    {
        if (current_token->get_type() == type)
            return true;
        return false;
    }

    MNNode* MNParser::gen_node_from_token()
    {
        MNNode* node = new MNNode(mem_pool, gen_struct_node_from_token());
        return node;
    }

    MNDTNode* MNParser::gen_struct_node_from_token()
    {
        MNDTNode* struct_node = mem_pool->allocate();
        struct_node->last_node = -1;
        struct_node->next_node = -1;
        struct_node->children_last_node = -1;
        struct_node->children_next_node = -1;

        struct_node->max_element = 0;
        struct_node->children_max_element = 0;

        struct_node->meta = gen_meta();

        //struct_node->type = current_token->get_type();
        mem_pool->set_lexeme(struct_node, current_token->get_lexeme());
        struct_node->row = current_token->get_row();
        struct_node->col = current_token->get_col();

        return struct_node;
    }

    void MNParser::recuperate()
    {
        while ( !current_tk_is(TK_EOL) && !current_tk_is(TK_END) && !current_tk_is(TK_RULE_DEC) ) {
            consume();
        }
        if (current_tk_is(TK_EOL))
        {
            consume();
            //std::cout << current_token->get_lexeme() << std::endl;
        }
    }

    unsigned int MNParser::gen_meta()
    {
        if ( current_tk_is(TK_RULE) || current_tk_is(TK_RULE_DEC))
            return 10;
        if ( current_tk_is(TK_CHARACTER) )
            return 21;
        if ( current_tk_is(TK_STRING) )
            return 80;
        if ( current_tk_is(TK_ID) )
            return 81;
        return 90;
    }

    namespace parser
    {

        MNGrammarParser::MNGrammarParser(MNLexer *lexer, MNLexer *p_lexer, MNErrorMsg *error_msg)
            :MNParser(lexer, p_lexer, error_msg)
        {
            mem_pool = new MemoryManager();

            this->current_or_count = 0;

            or_table_ptr = nullptr;
            keywords = nullptr;
        }

        MNGrammarParser::~MNGrammarParser()
        {
            for (auto it : rules)
            {
                delete it.second;
            }
            delete mem_pool;
        }

        MNGrammarItem* MNGrammarParser::parse()
        {
            _init_table();
            MNNode* root = new MNNode(mem_pool, 100, "root");
            if (!error_msg->has_errors())
            {
                while (current_tk_is(TK_RULE_DEC))
                {
                    root->add_struct_node(rule());

                }
                while (!current_tk_is(TK_END))
                {
                    if (!current_tk_is(TK_EOL))
                    {
                        error_msg->add_error_text("MNParser: ", "unexpected '" + current_token->get_lexeme() + "' at "
                                              + current_token->get_char_position(),
                                              lexer->get_line(current_token->get_row()), 0, current_token->get_col());
                        recuperate();
                    }
                    consume();

                }
                if (!error_msg->has_errors())
                    _flatten_or(root);
                //std_map_output();
            }
            return new MNGrammarItem(root, or_table_ptr, keywords);
        }

        MNDTNode* MNGrammarParser::rule()
        {
            MNNode _rule(mem_pool, 100, current_token->get_lexeme(), 10);

            consume();
            //match(TK_COLON);
            if (_is_expression())
            {
                while (!current_tk_is(TK_RULE_DEC)) {

                    consume_cl();
                    if (current_tk_is(TK_END) || current_tk_is(TK_RULE_DEC))
                        break;

                    if (_is_expression())
                    {
                        _rule.add_struct_node( expression());
                    }
                    else
                    {
                        error_msg->add_error_text("MNParser: ", "'expression' expected at "
                                              + current_token->get_char_position(),
                                              lexer->get_line(current_token->get_row()), 0, current_token->get_col());
                        recuperate();
                        break;
                    }
                }
            }
            else
                error_msg->add_error_text("MNParser: ", "expected 'an expression' at "
                                          + current_token->get_char_position(),
                                          lexer->get_line(current_token->get_row()), 0, current_token->get_col());

            return _rule.get_struct_node();
        }

        MNDTNode* MNGrammarParser::expression()
        {
            MNNode expr(mem_pool, 100, MN_VECTOR_NAME, 1);

            if (_is_expression())
            {
                while (_is_expression() && !current_tk_is(TK_RULE_DEC))
                {
                    if (current_tk_is(TK_ID) || current_tk_is(TK_RULE) || current_tk_is(TK_STRING) || current_tk_is(TK_RULE_DEC))
                    {
                        if (current_tk_is(TK_STRING))
                            keywords->insert(std::make_pair<std::string, bool>(current_token->get_lexeme(), true));
                        expr.add_struct_node(gen_struct_node_from_token());
                        consume();
                    }
                    else
                    if (current_tk_is(TK_CHARACTER))
                    {

                        MNDTNode* character = gen_struct_node_from_token();
                        consume();
                        if (current_tk_is(TK_DDOTS))
                        {
                            consume();
                            MNNode char_range(mem_pool, 100, "CHAR_RANGE", 20);
                            if (current_tk_is(TK_CHARACTER))
                            {
                                char_range.add_struct_node(character);
                                char_range.add_struct_node(gen_struct_node_from_token());
                                expr.add_node(&char_range);
                                consume();
                            }
                            else
                                error_msg->add_error_text("MNParser: ", "expected 'a caracter' at "
                                                      + current_token->get_char_position(),
                                                      lexer->get_line(current_token->get_row()), 0, current_token->get_col());

                        }
                        else
                            expr.add_struct_node(character);

                    }
                    else
                    if (current_tk_is(TK_PARENTHESIS_OPEN))
                    {
                       consume();
                       auto p_expr = MNNode(mem_pool, expression());
                       expr.add_struct_node(p_expr.get_struct_node());
                       match(TK_PARENTHESIS_CLOSE);
                       if (current_tk_is(TK_ASTERISK))
                       {
                           consume();
                           p_expr.set_meta(31);
                       }
                       else
                       if (current_tk_is(TK_PLUS))
                       {
                           consume();
                           p_expr.set_meta(32);
                       }
                       else
                           p_expr.set_meta(30);
                    }
                    else
                    if (current_tk_is(TK_CURLY_BRACKET_OPEN))
                    {
                        consume();
                        auto p_expr = MNNode(mem_pool, expression());
                        expr.add_struct_node(p_expr.get_struct_node());
                        match(TK_CURLY_BRACKET_CLOSE);
                        if (current_tk_is(TK_ASTERISK))
                        {
                            consume();
                            p_expr.set_meta(41);
                        }
                        else
                        if (current_tk_is(TK_PLUS))
                        {
                            consume();
                            p_expr.set_meta(42);
                        }
                        else
                            p_expr.set_meta(40);

                    }
                    else
                    if (current_tk_is(TK_SQUARE_BRACKET_OPEN))
                    {
                       consume();
                       auto p_expr = MNNode(mem_pool, expression());
                       expr.add_struct_node(p_expr.get_struct_node());
                       match(TK_SQUARE_BRACKET_CLOSE);
                       p_expr.set_meta(50);
                    }
                    else
                    if (current_tk_is(TK_OR))
                    {
                        MNNode rule_op(mem_pool, 100, "OR", 60);
                        consume();
                        rule_op.add_node(&expr);
                        if (_is_expression())
                        {
                            rule_op.add_struct_node(expression());
                        }
                        else
                            error_msg->add_error_text("MNParser: ", "expected 'an expression' at "
                                                  + current_token->get_char_position(),
                                                  lexer->get_line(current_token->get_row()), 0, current_token->get_col());

                        MNNode expr_container(mem_pool, 100, MN_VECTOR_NAME, 1);
                        MNNode t_e(mem_pool, 100, "OR", 60);

                        for (unsigned int i=0; i < rule_op.get_count(); ++i)
                        {
                            if (rule_op.at(i).get_count() > 0 && rule_op.at(i).at(0).get_meta() == 60)
                            {
                                for (auto it: rule_op.at(i).at(0))
                                {
                                    t_e.add_struct_node(it.get_struct_node());
                                }
                            }
                            else
                                t_e.add_struct_node(rule_op.at(i).get_struct_node());
                        }
                        expr_container.add_struct_node(t_e.get_struct_node());
                        return expr_container.get_struct_node();
                    }
                    else
                    if (current_tk_is(TK_AT))
                    {
                        consume();
                        MNNode rule_op(mem_pool, 100, "AT", 70);
                        rule_op.add_struct_node(gen_struct_node_from_token());
                        match(TK_ID);
                        match(TK_COLON);
                        rule_op.add_struct_node(gen_struct_node_from_token());
                        match(TK_STRING);

                        rule_op.add_node(&expr);
                        if (_is_expression())
                            rule_op.add_struct_node(expression());
                        else
                            error_msg->add_error_text("MNParser: ", "expected 'an expression' at "
                                                  + current_token->get_char_position(),
                                                  lexer->get_line(current_token->get_row()), 0, current_token->get_col());


                        MNNode expr_container(mem_pool, 100, MN_VECTOR_NAME, 1);
                        expr_container.add_struct_node(rule_op.get_struct_node());
                        return expr_container.get_struct_node();
                    }
                }
            }
            else
                consume();

           return expr.get_struct_node();
        }
        void MNGrammarParser::std_map_output()
        {
            for (auto it : *or_table_ptr)
            {
                std::cout << ">N<: " << it.first << std::endl;
                for (auto it2 : *it.second)
                {
                    std::cout << "    >TID<: " << it2.first << std::endl;
                    for (auto it3 : *it2.second)
                    {
                         for (auto it4 : *it3.second)
                            std::cout << "        >LEX<: " << it3.first << " > POS <: " << it4 << std::endl;
                    }
                }
                std::cout << ">E<: " <<  std::endl;
            }
        }
        void MNGrammarParser::_init_table()
        {
             or_table_ptr = new table_map();
             keywords = new std::unordered_map<std::string, bool>();
        }

        bool MNGrammarParser::_is_expression()
        {
            if (current_tk_is(TK_ID) || current_tk_is(TK_RULE) || current_tk_is(TK_STRING) /*|| current_tk_is(TK_RULE_DEC)*/ ||
                    current_tk_is(TK_CURLY_BRACKET_OPEN) || current_tk_is(TK_CHARACTER) ||
                    current_tk_is(TK_PARENTHESIS_OPEN) || current_tk_is(TK_SQUARE_BRACKET_OPEN) ||
                    current_tk_is(TK_OR) || current_tk_is(TK_AT))
                return true;
            return false;
        }

        void MNGrammarParser::consume_cl()
        {
            while (current_tk_is(TK_EOL))
                consume();
        }

        MNNode* MNGrammarParser::get_rule(std::string rule, int row)
        {
            if (rules.find(rule) == rules.end())
            {
                error_msg->add_error_text("MNParser: ", "Undefined referenced rule '"
                                      + rule + "' at row: " + to_string(row),
                                      lexer->get_line(row), 0, current_token->get_col());
                return nullptr;
            }
            return rules[rule];
        }

        void MNGrammarParser::_or_mapper(MNNode *node, MNNode* flatten)
        {
            unsigned int current_id = current_or_count++;
            node->set_meta_aux(current_id);
            MNNode or_list(mem_pool, 100, MN_VECTOR_NAME, 1);
            _flatten_current_or(node, &or_list);
            unsigned int c_pos = 0;
            for (unsigned int i=0; i < or_list.get_count();++i )
            {
                if (or_list.at(i).get_meta() == 109771)
                {
                    c_pos++;
                }

                /*if (c_pos >= node->get_count())
                {
                    //c_pos = it.get_count()-1;
                }*/

                if (or_list.at(i).get_meta() != 109771)
                {
                    unsigned int c_meta = or_list.at(i).get_meta() == 80 ? 81 : or_list.at(i).get_meta();
                    c_meta = or_list.at(i).get_meta() == 81 && or_list.at(i).get_lexeme() == "STR" ? 82 : c_meta;
                    c_meta = or_list.at(i).get_meta() == 81 && or_list.at(i).get_lexeme() == "NEWLINE" ? 83 : c_meta;
                    c_meta = or_list.at(i).get_meta() == 81 && or_list.at(i).get_lexeme() == "INDENT" ? 84 : c_meta;
                    c_meta = or_list.at(i).get_meta() == 81 && or_list.at(i).get_lexeme() == "DEDENT" ? 85 : c_meta;
                    c_meta = or_list.at(i).get_meta() == 81 && or_list.at(i).get_lexeme() == "ENDMARKER" ? 86 : c_meta;
                    c_meta = or_list.at(i).get_meta() == 81 && or_list.at(i).get_lexeme() == "NUMBER" ? 87 : c_meta;

                    if (or_list.at(i).get_meta() == 81 &&  c_meta == 81)
                    {
                          c_token res = p_lexer->get_custom_token(or_list.at(i).get_lexeme());
                          if (res.exist_custom_token)
                              c_meta = res.type;
                    }

                    auto pair = std::make_pair<unsigned int, table_meta_table*>(0, new table_meta_table());
                    pair.first = current_id;
                    if (or_table_ptr->find(current_id) == or_table_ptr->end())
                        or_table_ptr->insert(pair);
                    else
                        delete pair.second;

                    auto pair2 = std::make_pair<unsigned int, table_lexeme_index*>(0, new table_lexeme_index());
                    pair2.first = c_meta;

                    if (or_table_ptr->at(current_id)->find(c_meta) == or_table_ptr->at(current_id)->end())
                        or_table_ptr->at(current_id)->insert(pair2);
                    else
                        delete pair2.second;

                    if (or_list.at(i).get_meta() == 81)
                    {
                        if (or_table_ptr->at(current_id)->at(c_meta)->find("default")
                                != or_table_ptr->at(current_id)->at(c_meta)->end())
                        {
                            bool has_pos = true;
                            for (auto it : *or_table_ptr->at(current_id)->at(c_meta)->at("default"))
                                if (it == c_pos) {has_pos = false; break; }

                            if (has_pos)
                                or_table_ptr->at(current_id)->at(c_meta)->at("default")->push_back(c_pos);

                        }
                        else
                        {
                            auto pair3 = std::make_pair<std::string, std::vector<unsigned int>* >("default",
                                                                                                  new std::vector<unsigned int>());

                            or_table_ptr->at(current_id)->at(c_meta)->insert(pair3);
                            or_table_ptr->at(current_id)->at(c_meta)->at("default")->push_back(c_pos);

                        }

                    }
                    else
                    {
                        if (or_table_ptr->at(current_id)->at(c_meta)->find(or_list.at(i).get_lexeme().c_str())
                                != or_table_ptr->at(current_id)->at(c_meta)->end())
                        {
                            bool has_pos = true;
                            for (auto it : *or_table_ptr->at(current_id)->at(c_meta)->at(or_list.at(i).get_lexeme().c_str()))
                                if (it == c_pos) {has_pos = false; break; }

                            if (has_pos)
                                or_table_ptr->at(current_id)->at(c_meta)->at(or_list.at(i).get_lexeme().c_str())->push_back(c_pos);
                        }
                        else
                        {
                            auto pair3 = std::make_pair<std::string, std::vector<unsigned int>* >(or_list.at(i).get_lexeme().c_str(),
                                                                                                  new std::vector<unsigned int>());
                            or_table_ptr->at(current_id)->at(c_meta)->insert(pair3);
                            or_table_ptr->at(current_id)->at(c_meta)->at(or_list.at(i).get_lexeme().c_str())->push_back(c_pos);
                        }
                    }
                }
            }
            if (flatten == nullptr)
            {
                node->add_node(&or_list);
            }
            else
            {
                auto ptr_clone = or_list.at(0);
                flatten->add_node(&ptr_clone);
            }
        }

        void MNGrammarParser::_flatten_or(MNNode *node)
        {
            for (auto &it : *node)
            {
                std::string rrr = it.get_lexeme();
                if (rules.find(it.get_lexeme()) != rules.end())
                    error_msg->add_error_text("MNParser: ", "Duplicated rule name: '"
                                          + it.get_lexeme() + "'",
                                          lexer->get_line(it.get_row()), 0, it.get_col());

                rules.insert(std::make_pair(it.get_lexeme(),
                                            it.ptr_copy()));
            }

            for (auto &it: *node)
            {
                _flatten_all_rule(&it);
            }

        }

        void MNGrammarParser::_flatten_rule(MNNode *node)
        {
            for (auto &it : node->at(0))
            {
                if (((it.get_meta() == 30 || it.get_meta() == 1) && it.at(0).get_meta() == 60))
                {
                    _or_mapper(it.ptr_at(0));
                }
                else
                if (it.get_meta() == 60 )
                {
                    _or_mapper(&it);
                }
            }
        }

        void MNGrammarParser::_flatten_all_rule(MNNode *node)
        {
            for (auto &it : *node)
            {
                if (it.get_meta() == 60 && it.get_meta_aux()==-1)
                    _or_mapper(&it);
                _flatten_all_rule(&it);
                if (it.get_meta() == 10 && rules.find(it.get_lexeme()) == rules.end())
                {
                    error_msg->add_error_text("MNParser: ", "Undefined referenced rule '"
                                          + it.get_lexeme() + "' at row: " + to_string(it.get_row()),
                                          lexer->get_line(it.get_row()), 0, it.get_col());
                }
            }
        }

        void MNGrammarParser::_flatten_current_or(MNNode *node, MNNode* flatten, unsigned int /*level*/)
        {
            for (auto it : *node)
            {
                 _flatten_current_child(it, flatten);
                 MNNode sep(mem_pool, 100, "OR_SEP", 109771);
                 flatten->add_node(&sep);
            }
        }

        void MNGrammarParser::_flatten_current_child(MNNode &node, MNNode* flatten, unsigned int /*level*/)
        {
            if (node.get_meta() == 1)
            {
                for (unsigned int i=0; i < node.get_count(); ++i)
                {
                    if (node.get_count() > 0 )
                    {
                        MNNode or_node = node.at(i);
                        //TO DO
                        if (or_node.get_meta() == 31 || or_node.get_meta() == 50)
                        {
                            node.start_cache();

                            for (unsigned int i=1; i < node.get_count();++i)
                            {
                                MNNode c_node = node.at(i);
                                _flatten_current_child(c_node, flatten);
                                if (c_node.get_meta() != 31 && c_node.get_meta() != 50)
                                    break;
                            }
                        }
                        if (or_node.get_meta() == 1)
                        {
                            MNNode or_node_2 = or_node.at(0);
                            _flatten_current_child(or_node_2, flatten);
                        }
                        else if (or_node.get_meta() == 60)
                        {
                            MNNode or_list(mem_pool, 100, MN_VECTOR_NAME, 1);
                            _or_mapper(&or_node, &or_list);

                            for (auto it : or_list) {
                                if (it.get_meta() != 109771 )
                                {
                                    MNNode* ptr_clone =  it.ptr_clone();
                                    flatten->add_node(ptr_clone);
                                    delete ptr_clone;
                                }
                            }
                        }
                        else if (or_node.get_meta() == 30
                                 || or_node.get_meta() == 31
                                 || or_node.get_meta() == 32
                                 || or_node.get_meta() == 50)
                        {
                            if (or_node.get_count() > 0)
                            {
                                MNNode or_node_2 = or_node.at(0);
                                _flatten_current_child(or_node_2, flatten);
                            }
                        }
                        else if (or_node.get_meta() == 10)
                        {
                            MNNode* rule = get_rule(or_node.get_lexeme(), or_node.get_row());
                            if (!rule)
                                break;
                            if (rule->get_count() > 0)
                            {
                                MNNode or_node_2 = rule->at(0);
                                _flatten_current_child(or_node_2, flatten);
                            }
                        }
                        else
                        {
                            MNNode* ptr_clone =  or_node.ptr_clone();
                            flatten->add_node(ptr_clone);
                            delete ptr_clone;
                        }

                        if (or_node.get_meta() != 31 && or_node.get_meta() != 50)
                            break;
                    }
                }
            }
            else
            if (node.get_meta() == 60)
            {
                MNNode or_list(mem_pool, 100, MN_VECTOR_NAME, 1);
                _or_mapper(&node, &or_list);

                for (auto it : or_list) {
                    if (it.get_meta() != 109771 )
                    {
                        MNNode* ptr_clone =  it.ptr_clone();
                        flatten->add_node(ptr_clone);
                        delete ptr_clone;
                    }
                }
            }
            else
            if (node.get_meta() == 30
                    || node.get_meta() == 31
                    || node.get_meta() == 32
                    || node.get_meta() == 50)
            {
                if (node.get_count() > 0)
                {
                    MNNode or_node_2 = node.at(0);
                    _flatten_current_child(or_node_2, flatten);

                }
            }
            else if (node.get_meta() == 10)
            {
                MNNode* rule = get_rule(node.get_lexeme(), node.get_row());
                if (rule)
                {
                    MNNode* rule = rules[node.get_lexeme()];
                    if (rule->get_count() > 0)
                    {
                        MNNode or_node_2 = rule->at(0);
                        _flatten_current_child(or_node_2, flatten);

                    }
                }
            }
            else
            {
                MNNode* ptr_clone =  node.ptr_clone();
                flatten->add_node(ptr_clone);
                delete ptr_clone;

            }
        }
    }
}

