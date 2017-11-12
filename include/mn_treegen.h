/*************************************************************************/
/*  mn_treegen.h                                                         */
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

#ifndef MNTREEGEN_H
#define MNTREEGEN_H

#include <unordered_map>
#include <mn_data_struct.h>
#include <mn_mn_lexer.h>
#include <mn_config.h>

struct rule_info
{
    long int t_count;
};


namespace mn
{
    enum MN_ParseMode
    {
         MN_PARSEMODE_VERBOSE,
         MN_PARSEMODE_MVERBOSE,
         MN_PARSEMODE_LVERBOSE,
    };

    class MNTreeGen
    {
    public:
        MNTreeGen(MNGrammarItem* grammar_tree, MNLexer* lexer,
                  MNErrorMsg* error_msg, MN_ParseMode mode = MN_PARSEMODE_MVERBOSE,
                  bool minify=true, long int depth=2);
        ~MNTreeGen();
        MNNode* generate(std::string entry);
        MNNode* mn_at(MNNode* node, bool required = true);
        MNNode* mn_or(MNNode* node, bool required = true);
        MNNode* mn_id(MNNode* node, bool required = true);
        MNNode* mn_rule(MNNode* node, bool required = true, bool is_rule=false) ;
        MNNode* mn_string(MNNode* node, bool required = true);
        MNNode* mn_vector(MNNode* node, bool required = true);
        MNNode* mn_brackets(MNNode* node, bool required = true);
        MNNode* mn_asterisk(MNNode* node, bool required = true);
        MNNode* mn_character(MNNode* node, bool required = true);
        MNNode* mn_parenthesis(MNNode* node, bool required = true);
        MNNode* mn_parenthesis_many(MNNode* node, bool required = true);
        MNNode* mn_parenthesis_req_many(MNNode* node, bool required = true);
        MNNode* process(MNNode* node, bool required = true);
        void consume();
        MNNode* break_node();
    protected:
        mn::MNLexer* lexer;
        mn::MNNode* grammar_tree;
        MNGrammarItem* grammar_item;
        mn::MemoryManager* mem_pool;
        MNErrorMsg* error_msg;
        std::unordered_map<std::string, MNNode*> rules;
        MNToken* current_token;
        mn::MNNode* _break_node;
        unsigned int token_consumed;
        std::stack<rule_info> rules_info;
        std::vector<MNToken*> tokens;
        long int g_pos;
        MN_ParseMode p_mode;
        bool minify;
        bool agressive_remove;
        long int depth;
    protected:
        bool is_required();
        long int _current_gpos();
        void _restore_gpos(long int c_pos);
        bool is_special_id();
    private:
        bool _is_valid(MNNode* node);
        bool _is_break_node(MNNode* node);
    };
}

#endif // MNTREEGEN_H
