#ifndef MNTRAVELER_H
#define MNTRAVELER_H
#include <unordered_map>
/*************************************************************************/
/*  mn_traveler.h                                                        */
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
#include <mn_data_struct.h>
#include <delegate/delegate.h>

template <class T, class R>
class MN_FuncCall
{
public:
    typedef R (T::*fn)( mn::MNNode* );

    MN_FuncCall(T& trg, fn op) : mn_dest(trg), mn_op(op)
    {
    }

    R exec( mn::MNNode* in )
    {
        return (mn_dest.*mn_op)( in );
    }

private:
    MN_FuncCall();
    MN_FuncCall( const MN_FuncCall& );

    T& mn_dest;
    fn mn_op;
};

namespace mn
{
    template <typename T, class R>
    class MNTraveler
    {
       typedef MN_FuncCall<T, R> MNFunc;
    public:
        MNTraveler()
        {
            //this->tree = tree;
        }
        virtual ~MNTraveler()
        {
            for (auto it : funcs)
                delete it.second;
        }
        template <typename F>
        void bind(std::string func_name, F func, T* obj)
        {
            auto result = new MNFunc(*obj, func);
            funcs[func_name] = result;
        }

        void pre_order()
        {

        }

        void post_order()
        {

        }

        R call(MNNode *node)
        {
            R result;
            if (node->get_meta() == MN_NT_VALUE)
            {
                if (funcs.find(node->get_lexeme())!=funcs.end())
                {
                    auto f = funcs[node->get_lexeme()];
                    result = f->exec(node);
                }
            }
            return result;
        }

        virtual R walk(MNNode* ) {return *(new R);}


        bool is_terminal(MNNode& node)
        {
            if (node.get_meta() == MN_NT_VALUE)
                return true;
            return false;
        }

    protected:
        void _pre_order(MNNode& node);

    protected:
        //MNNode* tree;
        std::unordered_map<std::string, MNFunc*> funcs;
    };



}


#endif // MNTRAVELER_H
