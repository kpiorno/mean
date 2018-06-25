#include <iostream>
#include <mn_root.h>

#include <lexers/mn_meanlex.h>
#include <travelers/mn_mn_bin.h>
#include <vm/mn_vm.h>

int main(int /*argc*/, char */*argv*/[])
{
    auto root = new mn::MNRoot();
    for (unsigned int i=0; i < 1; ++i)
    {
        mn::MNErrorMsg* err = new mn::MNErrorMsg();
        mn::lexer::MNMeanLex* parse_lexer = new mn::lexer::MNMeanLex("code.mn", err);
        auto travel = new mn::MN_MNBin("code.mnc");
        root->run_parser("Python3.6.mng", "file_input", parse_lexer, travel, !true, 10);

        delete err;
        delete travel;
        delete parse_lexer;

        mn::MN_VM p("code.mnc");
        p.process();
    }
}

