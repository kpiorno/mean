/*************************************************************************/
/*  mn_data_struct.h                                                     */
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
#ifndef MN_DATA_STRUCT
#define MN_DATA_STRUCT

#include <mn_token.h>
#include <unordered_map>
#include <mem_pool/MemoryPool.h>
#include <vector>
#include <iterator>
#include <stack>
#include <mn_config.h>


namespace mn
{


    typedef std::unordered_map<std::string, std::vector<unsigned int>*> table_lexeme_index;
    typedef std::unordered_map<unsigned int, table_lexeme_index*> table_meta_table ;
    typedef std::unordered_map<unsigned int, table_meta_table*> table_map ;


    struct MNDTNode
    {
         //TokenType type;
         unsigned int row;
         unsigned int col;
         unsigned int meta;
         long int meta_aux;

         unsigned int pos;

         long int last_node;
         long int next_node;

         long int children_last_node;
         long int children_next_node;

         unsigned int max_element;
         unsigned int children_max_element;
    };

    const size_t DT_MEMPOOL_SIZE = sizeof(MNDTNode)*64;

    class MemoryManager
    {
    public:
        MemoryManager();
        ~MemoryManager();
        MNDTNode* allocate();
        void deallocate(unsigned int pos);

        std::string& get_lexeme(MNDTNode* node);
        void set_lexeme(MNDTNode* node, const std::string lexeme);
        MNDTNode* get_by_pos(const unsigned int pos);
        MNDTNode* get_next_node(MNDTNode* node);
        MNDTNode* get_last_node(MNDTNode* node);
        MNDTNode* get_child_next_node(MNDTNode* node);
        MNDTNode* get_child_last_node(MNDTNode* node);

    private:
        std::vector<std::string> lexemes;
        std::vector<MNDTNode*> nodes;
        MemoryPool<MNDTNode, DT_MEMPOOL_SIZE>* mem;
        std::stack<unsigned int> free_pos;
        unsigned int pos;
    };

    class MNNode
    {
    public:
        MNNode(MemoryManager* mem,
               const unsigned int children = 0,
               const std::string lexeme = MN_VECTOR_NAME,
               const unsigned int meta = 0
               );
        MNNode(MemoryManager* mem,
               MNDTNode* struct_node);
        MNDTNode* get_struct_node();
        MNDTNode* struct_at(const unsigned int pos);
        MNNode* ptr_at(const unsigned int pos);
        MNNode at(const unsigned int pos);
        MNNode* ptr_copy();

        MNNode* ptr_clone();

        void add_node(MNNode* node);
        void add_struct_node(MNDTNode* struct_node);

        unsigned int get_row();
        unsigned int get_col();
        unsigned int get_meta();
        long int get_meta_aux();
        unsigned int get_child_count();
        unsigned int get_count();
        const std::string& get_lexeme();

        void set_meta(const unsigned int meta);
        void set_meta_aux(long int meta);
        void set_lexeme(const std::string& lexeme);
        void std_output(const unsigned int level=0, std::string filename="");
        void start_cache();

        MNNode* get_next_node();
        MNNode* get_last_node();
        MNNode* get_child_next_node();
        MNNode* get_child_last_node();

        void deallocate();
        void break_by_max_count();

        void set_row(unsigned int row);
        void set_col(unsigned int col);

        bool is_found();
        bool is_break_node();
        bool is_list();
        bool is_req_list();
        bool non_terminal();

        class iterator
        {
            public:
                typedef iterator self_type;
                typedef MNNode value_type;
                typedef MNNode& reference;
                typedef MNNode* pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;
                iterator(pointer ptr) : ptr_(ptr) { }
                self_type operator++() { self_type i = *this; auto t = ptr_;
                                         ptr_ = ptr_->get_next_node(); delete t; return i; }
                self_type operator++(int /*junk*/) { ptr_++; return *this; }
                reference operator*() { return *ptr_; }
                pointer operator->() { return ptr_; }
                bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
            private:
                pointer ptr_;
        };

        class const_iterator
        {
            public:
                typedef const_iterator self_type;
                typedef MNNode value_type;
                typedef MNNode& reference;
                typedef MNNode* pointer;
                typedef int difference_type;
                typedef std::forward_iterator_tag iterator_category;
                const_iterator(pointer ptr) : ptr_(ptr) { }
                self_type operator++() { self_type i = *this; ptr_++; return i; }
                self_type operator++(int /*junk*/) { ptr_++; return *this; }
                //const reference operator*() { return *ptr_; }
                //const pointer operator->() { return ptr_; }
                bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
            private:
                pointer ptr_;
        };

        iterator begin()
        {
            return iterator(this->struct_node->children_next_node != -1
                    ? get_child_next_node() : NULL);
        }

        iterator end()
        {
            return iterator(NULL);
        }

        /*const_iterator begin() const
        {
            return const_iterator(get_child_next_node());
        }

        const_iterator end() const
        {
            return const_iterator(NULL);
        }*/

    private:
        MNDTNode* struct_node;
        unsigned int current_pos;
        MemoryManager* mem;
        bool cached_nodes_flag;
        std::vector<MNDTNode*> cached_nodes;
        unsigned int c_iter_pos;
        //std::string lexeme;

    };

    class MNGrammarItem
    {
        public:
            MNGrammarItem(MNNode* node, table_map* table, std::unordered_map<std::string, bool>* keywords);
            ~MNGrammarItem();
            MNNode* get_node();
            table_map* get_table();
            bool exist_keyword(std::string lexeme);
            std::vector<unsigned int>* get_or_pos(unsigned int current, unsigned int meta);
            std::vector<unsigned int>* get_or_pos_string(unsigned int current,
                                                         unsigned int meta, std::string lexeme);
        protected:
            MNNode* node;
            table_map* table;
            std::unordered_map<std::string, bool>* keywords;
    };

}

#endif // MN_DATA_STRUCT

