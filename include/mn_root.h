/*************************************************************************/
/*  mn_root.h                                                            */
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
#ifndef MNROOT_H
#define MNROOT_H
#include <mn_config.h>
#include "mn_mn_lexer.h"
#include "mn_lexer.h"
#include "mn_error_msg.h"
#include <mn_data_struct.h>
#include <mn_parser.h>
#include <mn_treegen.h>
#include <mn_traveler.h>

namespace mn
{
    class MNRoot
    {
    public:
        MNRoot();
        template <typename T, typename F>
        F run_parser(std::string source,
                              std::string start_terminal,
                              MNLexer* target_lexer,
                              MNTraveler<T, F>* traveler,
                              bool log = true,
                              long int cleaner=-1,
                              bool from_file = true,
                              std::string emitter="")
        {

            F result;
            MNErrorMsg* err = target_lexer->get_error_msg();

            MN_MNLexer* lexer = from_file ? new MN_MNLexer(source, err) : new MN_MNLexer(source, err, false);
            mn::MNLexer* parse_lexer = target_lexer;

            traveler->set_error_msg(err);
            traveler->set_source_lexer(target_lexer);

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
                                                                true,
                                                                2,
                                                                cleaner,
                                                                emitter);
                    tree_gen->set_name("SyntaxError: ");
                    if (!err->has_errors() )
                    {
                        MNNode* node = tree_gen->generate(start_terminal);
                        if (log)
                            node->std_output(/*0, "here.txt"*/);

                        if (/*!err->has_errors() &&*/ traveler)
                        {
                            if (!err->has_errors())
                                result = traveler->walk(node);
                            while (tree_gen->is_cleaner()) {
                                delete node;
                                node = tree_gen->generate(start_terminal);
                                if (log)
                                    node->std_output(/*0, "here.txt"*/);
                                if (!err->has_errors())
                                {
                                   result = traveler->walk(node);
                                }
//                                else
//                                    break;
                            }
                            //delete w_res;
                        }
                        else
                            result = *(new F);
                        delete node;
                    }
                    else
                         result = *(new F);
                    delete tree_gen;
                }
                else
                    result = *(new F);
                delete parse;
                delete grammar;
            }
            else
                result = *(new F);
            //if (log)
            err->std_out_errors();
            delete lexer;
            return result;
        }
        void test_parser(std::string parser_file,
                         std::string start_terminal,
                         MNLexer* target_lexer);

    };




}
#endif // MNROOT_H
