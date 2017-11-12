#include <iostream>
#include <math.h>
#include <mn_root.h>
#include <lexers/mn_simple.h>


class Expr: public mn::MNTraveler<Expr, float>
{
public:
    Expr(){
        bind("expr", &Expr::expr, this);
        bind("term", &Expr::term, this);
        bind("factor", &Expr::factor, this);
        bind("base", &Expr::base, this);
    }
    float expr(mn::MNNode* node)
    {
        return arith_op(node, 0);
    }

    float term(mn::MNNode* node)
    {
        return arith_op(node, 1);
    }

    float factor(mn::MNNode* node)
    {
        auto base = node->at(0);
        auto exp = node->at(2);
        float base_value = call(&base);
        float exp_value = call(&exp);
        return pow(base_value, exp_value);
    }

    float base(mn::MNNode* node)
    {
        auto n = node->at(0);
        if (n.get_meta() == 2)
        {
            auto expr = node->at(1);
            return call(&expr);
        }
        else
        {
            float res = atof(n.get_lexeme().c_str());
            return res;
        }
    }

    float arith_op(mn::MNNode* node, int op)
    {
        auto term = node->at(0);
        float res = call(&term);
        auto term_list = node->at(1);
        for (unsigned int i=0; i < term_list.get_count(); ++i)
        {
            if ((i+1) % 2 == 0)
            {
                auto res_node = term_list.at(i);
                switch (op) {
                case 0:
                    res += call(&res_node);
                    break;
                case 1:
                    res *= call(&res_node);
                    break;
                default:
                    res -= call(&res_node);
                    break;
                }
            }
        }
        return res;
    }

    float walk(mn::MNNode* node)
    {
        return call(node);
    }
};
int main(int /*argc*/, char **/*argv[]*/)
{
    auto root = new mn::MNRoot();

    mn::MNErrorMsg* err = new mn::MNErrorMsg();
    auto parse_lexer = new mn::lexer::MN_Simple("expr_grammar.mn", err);
    auto traveler = new Expr();
    std::cout << "Result is: " << root->run_parser("expr_grammar.mng", "expr",
                                                   parse_lexer, traveler) << std::endl;
    delete traveler;
    delete err;
    delete root;
}

