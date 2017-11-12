/*************************************************************************/
/*  mn_root.cpp                                                          */
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
#include "mn_root.h"
#include "mn_lexer.h"
#include "mn_mn_lexer.h"
#include "mn_error_msg.h"
#include <mn_data_struct.h>
#include <mn_parser.h>
#include <mn_treegen.h>
namespace mn
{
    MNRoot::MNRoot()
    {

    }

    void MNRoot::test_parser(std::string parser_file, std::string start_terminal, MNLexer *target_lexer)
    {
        MNNode* node = nullptr;
        MNErrorMsg* err = target_lexer->get_error_msg();
        MN_MNLexer* lexer = new MN_MNLexer(parser_file, err);

        mn::MNLexer* parse_lexer = target_lexer;

        if (!err->has_errors())
        {
            mn::parser::MNGrammarParser* grammar = new mn::parser::MNGrammarParser(lexer,
                                                                                   parse_lexer,
                                                                                   err);
            auto parse = grammar->parse();
            //parse->get_node()->std_output(0);
            if (!err->has_errors() )
            {
                mn::MNTreeGen* tree_gen = new mn::MNTreeGen(parse,
                                                            parse_lexer,
                                                            err,
                                                            mn::MN_PARSEMODE_LVERBOSE,
                                                            true);
                if (!err->has_errors() )
                {
                    node = tree_gen->generate(start_terminal);
                    node->std_output();
                }
                delete tree_gen;
            }
            delete parse;
            delete grammar;
        }
        err->std_out_errors();
        delete lexer;
        delete parse_lexer;
        delete node;
    }

}

