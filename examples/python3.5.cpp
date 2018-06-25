#include <iostream>
#include <math.h>
#include <mn_root.h>
#include <lexers/mn_meanlex.h>

int main(int /*argc*/, char **/*argv[]*/)
{
    //Class with shortcuts methods
    auto root = new mn::MNRoot();
    //Class to store the encountered errors
    mn::MNErrorMsg* err = new mn::MNErrorMsg();
    auto parse_lexer = new mn::lexer::MNMeanLex("python3.5.mn", err);

    root->test_parser("python3.5.mng", "file_input",
                                        parse_lexer);
    delete err;
    delete root;
}

