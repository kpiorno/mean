/*************************************************************************/
/*  mn_parser.h                                                          */
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
#ifndef MNPARSER_H
#define MNPARSER_H

#include <unordered_map>
#include <functional>
#include <mn_config.h>
#include <mn_lexer.h>
#include <mn_error_msg.h>
#include <mn_data_struct.h>

namespace mn
{
    class MNParser
    {
    public:
        MNParser(MNLexer* lexer, MNLexer* p_lexer, MNErrorMsg* error_msg);
        virtual ~MNParser();
        void match(TokenType type);
        void consume();
        bool current_tk_is(TokenType type);
        MNNode* gen_node_from_token();
        MNDTNode* gen_struct_node_from_token();
        void recuperate();
        virtual MNGrammarItem* parse() = 0;
    private:
        MNLexer* lexer;
    private:
        unsigned int gen_meta();
    protected:
        MNToken* current_token;
        MNErrorMsg* error_msg;
        MemoryManager* mem_pool;
        MNLexer* p_lexer;
    };


    namespace parser
    {
        class MNGrammarParser: public MNParser
        {
        public:
            MNGrammarParser(MNLexer* lexer, MNLexer* p_lexer, MNErrorMsg* error_msg);
            ~MNGrammarParser();
            MNGrammarItem* parse();

        protected:
            MNDTNode* rule();
            MNDTNode* expression();
            void std_map_output();
            void _init_table();
            bool _is_expression();
            inline void consume_cl();
            MNNode* get_rule(std::string rule, int row);
            //void _or_mapper(MNNode* node);
            void _or_mapper(MNNode* node, MNNode* flatten=NULL);
            void _flatten_or(MNNode* node);
            void _flatten_rule(MNNode* node);
            void _flatten_all_rule(MNNode* node);
            void _flatten_current_or(MNNode* node, MNNode* flatten, unsigned int level = 0);
            void _flatten_current_child(MNNode &node, MNNode* flatten, unsigned int level = 0);

        protected:
            std::unordered_map<std::string, MNNode*> rules;
            unsigned int current_or_count;
            //std::vector<std::vector<unsigned int>> or_table;
            table_map* or_table_ptr;
            std::unordered_map<std::string, bool>* keywords;

        };
    }
}


#endif // MNPARSER_H
