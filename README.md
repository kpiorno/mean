# Mean
A LL-N Grammar Parser for C++11. Fun to travel. 

### Motivation
"Mean" is the result of a personal journey to parse and make custom changes to the Python 3 grammar. 
The library uses an EBNF-like syntax, automaticaly generates the node tree and has a funny way to travel it.
There is a complete definition of the Python3.5 grammar at "examples" folder with some modifications according to the original.

### Quickstart
Please check the full example ```simple.cpp ``` at "examples" folder.

Create a grammar file: ```expr_grammar.mng ```
```
expr: term ( ('+' | '-') term )*
term: factor ( ('*' | '/') factor )*
factor: base [ '^' term ]
base: '(' expr ')' | ['-'] NUMBER
```
Create a source code file: ```expr_grammar.mn ```

```2 ^ 6 - 1```

So we need a ```MN_Lexer``` which will read the source code file. We could use built-in lexer "MN_Simple" which generates tokens: ```NAME, NUMBER, HEX, +, -, ^```:

```cpp
#include <mn_root.h>
#include <lexers/mn_simple.h>

...

int main(int /*argc*/, char **/*argv[]*/)
{
    //Class with shortcuts methods
    auto root = new mn::MNRoot();
    //Class to store the encountered errors
    mn::MNErrorMsg* err = new mn::MNErrorMsg();
    
    //Built-in Lexer
    auto parse_lexer = new mn::lexer::MN_Simple("path/to/expr_grammar.mn", err);  
    //Output the tree node
    root->test_parser("expr_grammar.mng", "expr", parse_lexer)   
}    
```
The above code will output the next tree:

```
|expr -- 100
  |factor -- 100
    |base -- 100
      |(?) -- 17
      |2 -- 26
    |^ -- 2
    |base -- 100
      |(?) -- 17
      |6 -- 26
  |[.] -- 30
    |- -- 2
    |base -- 100
      |- -- 2
      |1 -- 26
```

The nodes with the number ```100``` are non-terminals.  

The nodes ```[.] - 30``` are lists.

The nodes ```[?] - 17``` are optionals nodes. 

The rest of the nodes are terminals.

So lets do the right stuff to travel it.
It's possible to use a custom way to do that, but luckily we have the template class ```MN_Traveler``` which help us.

Now lets declare a class which inherit from ```MN_Traveler```:

```cpp
#include <mn_root.h>
#include <lexers/mn_simple.h>

//Class to travel the tree
class Expr: public mn::MNTraveler<Expr, float>
{
public:
    Expr(){
        //Bind the "expr" method with the "expr" grammar's non-terminal
        bind("expr", &Expr::expr, this);    
    }
    float expr(mn::MNNode* node)
    {
        return 1.1; 
    }
    //Entry point to start the travel   
    float walk(mn::MNNode* node)
    {
        return call(node);
    }    
};    

int main(int /*argc*/, char **/*argv[]*/) 
{
    //Class with shortcuts methods
    auto root = new mn::MNRoot();
    //Class to store the encountered errors
    mn::MNErrorMsg* err = new mn::MNErrorMsg();
    auto parse_lexer = new mn::lexer::MN_Simple("expr_grammar.mn", err);

    auto traveler = new Expr();
    //The run_parser method generate the tree and use the traveler class to travel it.
    std::cout << "Result is: " << root->run_parser("expr_grammar.mng", "expr",
                                                   parse_lexer, traveler) << std::endl;
    delete traveler;
    delete err;
    delete root;
}
```

The second parameter of the template class define "float" as the type of all methods bounds to the grammar's non-terminals. Thus we can define specific custom types. 

```bind("expr", &Expr::expr, this);``` bind the "expr" method with the "expr" grammar's non-terminal.

The ```float expr(mn::MNNode* node) ...``` method will handle the output generated by the non-terminal "expr".

The ```float walk(mn::MNNode* node) ...``` is the first method which is invoked. The method ```call(node)``` in ```return call(node)``` will invoke the 
right method bound to the non-terminal ("expr" non-terminal according to the tree generated by the previous example). Use "call" method when you need to travel a non-terminal.

So, executing the last code we got the next output:

```Result is: 1.1```

Which is the value that returns "expr" method.

So, adding the proper funcionalities we got a simple expression evaluator with the next code: 

```cpp
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
        //expr: term ( ('+' | '-') term )*
        return arith_op(node, 0);
    }

    float term(mn::MNNode* node)
    {
        //term: factor ( ('*' | '/') factor )*        
        return arith_op(node, 1);
    }

    float factor(mn::MNNode* node)
    {
        //factor: base [ '^' term ]
        auto base = node->at(0);
        auto exp = node->at(2);
        float base_value = call(&base);
        float exp_value = call(&exp);
        return pow(base_value, exp_value);
    }

    float base(mn::MNNode* node)
    {
        //base: '(' expr ')' |  ['-'] NUMBER        
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
    //Class with shortcuts methods
    auto root = new mn::MNRoot();
    //Class to store the encountered errors
    mn::MNErrorMsg* err = new mn::MNErrorMsg();
    auto parse_lexer = new mn::lexer::MN_Simple("expr_grammar.mn", err);

    auto traveler = new Expr();
    //The run_parser method generate the tree and use the traveler class to travel it.
    std::cout << "Result is: " << root->run_parser("expr_grammar.mng", "expr",
                                                   parse_lexer, traveler) << std::endl;
    delete traveler;
    delete err;
    delete root;
}
};
```
### Designs
In order to reduce the amount of nodes from generated trees, for the rules like this ```A: B [C]```:

```If [C] not encountered then A is replaced by B```. According to the last example if you change the context of file ```expr_grammar.mn ``` to 1 and execute the example, you will got
the next output:

```
|base -- 100
  |(?) -- 17
  |1 -- 26
```

And the tree will be right traveled because the method "walk" from traveler invoke the bound method "base" via the "call" function.
 

### Limitations 
Despite that it's possible to declare terminals tokens like identifiers or numbers via the EBNF, the "mean" library is designed to generate tokens using ```MN_Lexer``` classes. 
So regular expression to declare tokens is not supported at now. The goal is to use anothers tokens sources beyond those generated via regular expressions.

The ```* +``` operators could be used only inmediate next to ```)```. So expression like ```NAME*``` is not supported. Use ```(NAME)*``` instead.

if you have a rule like: ```A: B | D| [C]``` please consider change it to the form: ```A: [B | D| [C]]``` to avoid unexpected behaviours.

