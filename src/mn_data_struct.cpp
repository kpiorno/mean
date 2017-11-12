/*************************************************************************/
/*  mn_data_struct.cpp                                                   */
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
#include "mn_data_struct.h"
#include <fstream>

namespace mn
{
    MNDTNode* MNNode::get_struct_node()
    {
        return this->struct_node;
    }

    inline MNDTNode* MNNode::struct_at(const unsigned int pos)
    {
        if (pos >= this->struct_node->max_element /*&& false*/)
        {
            //FIXME: THIS BEHAVIOUR;
            return nullptr;
        }
        if (!cached_nodes_flag)
        {
            MNDTNode* node = mem->get_child_next_node(this->struct_node);
            for (unsigned int i=0; i < pos;++i)
                 node = mem->get_next_node(node);
            return node;
        }
        else
        {
            return cached_nodes[pos];
        }
        return mem->get_child_next_node(this->struct_node);
    }

    MemoryManager::MemoryManager()
    {
        mem = new MemoryPool<MNDTNode, DT_MEMPOOL_SIZE>();
        lexemes.reserve(200);
        lexemes.push_back(MN_VECTOR_NAME);
        nodes.reserve(200);
        pos = 0;
    }

    MemoryManager::~MemoryManager()
    {
        delete mem;
    }

    MNDTNode* MemoryManager::allocate()
    {
        MNDTNode* node = mem->allocate();
        if (free_pos.empty())
        {
            if (pos >= lexemes.size() )
            {
                lexemes.reserve(200);
                nodes.reserve(200);
            }
            nodes.push_back(node);
            lexemes.push_back(MN_VECTOR_NAME);
            node->pos = nodes.size()-1;
            pos = nodes.size()-1;
        }
        else
        {
            unsigned int c_pos = free_pos.top();
            free_pos.pop();
            nodes[c_pos] = node;
            lexemes[c_pos] = MN_VECTOR_NAME;
        }
        return node;
    }

    void MemoryManager::deallocate(unsigned int pos)
    {
        free_pos.push(pos);
        auto node = nodes[pos];
        mem->deallocate(node);
    }

    inline std::string& MemoryManager::get_lexeme(MNDTNode *node)
    {
        return lexemes[node->pos];
    }

    void MemoryManager::set_lexeme(MNDTNode *node, const std::string lexeme)
    {
        lexemes[node->pos] = lexeme;
    }

    inline MNDTNode* MemoryManager::get_by_pos(const unsigned int pos)
    {
        return nodes[pos];
    }

    inline MNDTNode* MemoryManager::get_next_node(MNDTNode *node)
    {
        if (node->next_node < 0 || node->next_node >= (long int)nodes.size())
            return nullptr;
        return nodes[node->next_node];
    }

    inline MNDTNode* MemoryManager::get_last_node(MNDTNode *node)
    {
        if (node->last_node < 0 || node->last_node >= (long int)nodes.size())
            return nullptr;
        return nodes[node->last_node];
    }

    inline MNDTNode* MemoryManager::get_child_next_node(MNDTNode *node)
    {
        if (node->children_next_node < 0 || node->children_next_node >= (long int)nodes.size())
            return nullptr;
        return nodes[node->children_next_node];
    }

    inline MNDTNode* MemoryManager::get_child_last_node(MNDTNode *node)
    {
        if (node->children_last_node < 0 || node->children_last_node >= (long int)nodes.size())
            return nullptr;
        return nodes[node->children_last_node];
    }


    MNNode::MNNode(MemoryManager *mem, const unsigned int /*children*/,
                       const std::string lexeme,
                       const unsigned int meta)
    {
        this->mem = mem;
        this->struct_node = mem->allocate();
        mem->set_lexeme(this->struct_node, lexeme);


        this->struct_node->meta = meta;
        this->struct_node->meta_aux = -1;

        this->struct_node->max_element = 0;
        this->struct_node->children_max_element = 0;
        this->struct_node->next_node = -1;
        this->struct_node->last_node = -1;
        this->struct_node->children_next_node = -1;
        this->struct_node->children_last_node = -1;

        current_pos = 0;
        this->cached_nodes_flag = false;
        c_iter_pos = 0;
    }

    MNNode::MNNode(MemoryManager *mem, MNDTNode *struct_node)
    {
        this->mem = mem;
        this->struct_node = struct_node;
        current_pos = this->struct_node->max_element;
        this->cached_nodes_flag = false;
        c_iter_pos = 0;
    }

    MNNode* MNNode::ptr_at(const unsigned int pos)
    {
        return new MNNode(mem, struct_at(pos));
    }

    MNNode MNNode::at(const unsigned int pos)
    {
        return MNNode(mem, struct_at(pos));
    }

    MNNode* MNNode::ptr_copy()
    {
        return new MNNode(mem, this->struct_node);
    }

    MNNode* MNNode::ptr_clone()
    {
        auto struct_node = mem->allocate();
        mem->set_lexeme(struct_node, get_lexeme());

        struct_node->meta = get_meta();
        struct_node->meta_aux = get_meta_aux();

        struct_node->max_element = this->struct_node->max_element;
        struct_node->children_max_element = this->struct_node->children_max_element;
        struct_node->next_node = this->struct_node->next_node;
        struct_node->last_node = this->struct_node->last_node;
        struct_node->children_next_node = this->struct_node->children_next_node;
        struct_node->children_last_node = this->struct_node->children_last_node;
        struct_node->row = this->struct_node->row;
        struct_node->col= this->struct_node->col;

        return new MNNode(mem, struct_node);

    }

    void MNNode::add_node(MNNode *node)
    {
        this->add_struct_node(node->get_struct_node());
    }

    void MNNode::add_struct_node(MNDTNode *struct_node)
    {
        ++current_pos;
        if (this->struct_node->children_last_node != -1)
        {
            mem->get_child_last_node(this->struct_node)
                    ->next_node = struct_node->pos;
            mem->get_child_last_node(this->struct_node)
                    ->last_node = struct_node->pos;
            this->struct_node
                    ->children_last_node = struct_node->pos;
        }
        else
        if (this->struct_node->children_next_node == -1)
        {
            this->struct_node
                    ->children_next_node = struct_node->pos;
            this->struct_node
                    ->children_last_node = struct_node->pos;
        }

        this->struct_node->max_element = current_pos;
    }

    unsigned int MNNode::get_row()
    {
        return this->struct_node->row;
    }

    unsigned int MNNode::get_col()
    {
        return this->struct_node->col;
    }

    unsigned int MNNode::get_meta()
    {
        return this->struct_node->meta;
    }

    long int MNNode::get_meta_aux()
    {
        return this->struct_node->meta_aux;
    }

    unsigned int MNNode::get_child_count()
    {
        return this->struct_node->children_max_element;
    }

    unsigned int MNNode::get_count()
    {
        return this->struct_node->max_element;
    }


    const std::string& MNNode::get_lexeme()
    {
        return mem->get_lexeme(this->struct_node);
    }

    void MNNode::set_meta(unsigned int meta)
    {
        this->struct_node->meta = meta;
    }

    void MNNode::set_meta_aux(long int meta)
    {
        this->struct_node->meta_aux = meta;
    }


    void MNNode::set_lexeme(const std::string &lexeme)
    {
        mem->set_lexeme(this->struct_node, lexeme);
    }

    void MNNode::std_output(const unsigned int level, std::string filename)
    {
        bool increment = true;
        std::string indent = "";

        for (unsigned int i=0; i < level;++i) indent += " ";

        if (get_lexeme() != "X[<<|MN_VECTOR|>>]X" || true)
            if (filename.empty())
                std::cout << indent << "|" << get_lexeme() << " -- " << get_meta() << std::endl;
            else
            {
                std::ofstream file; file.open(filename, level != 0 ? std::ios_base:: out | std::ios_base::app : std::ios_base:: out );
                file << indent << "|" << get_lexeme() << " -- " << get_meta() << std::endl;
                file.close();

            }
        else
            increment = false;

        for (auto &it: *this)
        //for (unsigned int i=0; i < this->get_count();++i)
        {
            //auto it = this->at(i);
            it.std_output(level+(increment ? 2: 0), filename);
        }
        //if (!increment)
        //    std::cout << indent << " ." << std::endl;
    }

    void MNNode::start_cache()
    {
        if (!cached_nodes_flag && this->struct_node->children_next_node != -1)
        {
            MNDTNode* node = mem->get_child_next_node(this->struct_node);
            cached_nodes.push_back(node);
            cached_nodes.reserve(this->struct_node->max_element);
            if (this->struct_node->max_element > 0)
                for (unsigned int i=0; i < this->struct_node->max_element-1;++i)
                    cached_nodes.push_back(node = mem->get_next_node(node));
            cached_nodes_flag = true;
        }
    }

    MNNode* MNNode::get_next_node()
    {
        auto res = mem->get_next_node(this->struct_node);
        if (res == nullptr)
            return nullptr;
        return new MNNode(mem, res);
    }

    MNNode* MNNode::get_last_node()
    {
        auto res = mem->get_last_node(this->struct_node);
        if (res == nullptr)
            return nullptr;
        return new MNNode(mem, res);

    }

    MNNode* MNNode::get_child_next_node()
    {
        auto res = mem->get_child_next_node(this->struct_node);
        if (res == nullptr)
            return nullptr;
        return new MNNode(mem, res);

    }

    MNNode* MNNode::get_child_last_node()
    {
        auto res = mem->get_child_last_node(this->struct_node);
        if (res == nullptr)
            return nullptr;
        return new MNNode(mem, res);

    }

    void MNNode::deallocate()
    {
        mem->deallocate(struct_node->pos);
    }

    void MNNode::break_by_max_count()
    {
        if (struct_node->children_last_node != -1)
        {
            auto node = mem->get_by_pos(struct_node->children_last_node);
            auto n_node = node->next_node;
            node->next_node = -1;
            while (n_node != -1)
            {
                auto c_node = mem->get_by_pos(n_node)->next_node;
                mem->deallocate(n_node);
                n_node = c_node;
            }
        }
    }

    void MNNode::set_row(unsigned int row)
    {
        this->struct_node->row = row;
    }

    void MNNode::set_col(unsigned int col)
    {
        this->struct_node->col = col;
    }

    bool MNNode::is_found()
    {
        return !(get_meta() == MN_NEXIST_T);
    }

    bool MNNode::is_break_node()
    {
        return get_meta() == MN_BREAK_NODE;
    }

    bool MNNode::is_list()
    {
        return get_meta() == MN_TG_LIST;
    }

    bool MNNode::is_req_list()
    {
        return get_meta() == MN_TG_REQ_LIST;
    }

    bool MNNode::non_terminal()
    {
        return get_meta() == MN_NT_VALUE;
    }

    MNGrammarItem::MNGrammarItem(MNNode *node, table_map *table, std::unordered_map<std::string, bool>* keywords)
    {
        this->node = node;
        this->table = table;
        this->keywords = keywords;
    }

    MNGrammarItem::~MNGrammarItem()
    {
        delete node;
        auto iter = table->begin();
        for (; iter != table->end(); iter++)
        {
            auto iter2 = (*iter).second->begin();
            for (; iter2 != (*iter).second->end(); iter2++)
            {
                auto iter3 = (*iter2).second->begin();
                for (; iter3 != (*iter2).second->end(); iter3++)
                    delete (*iter3).second;

                 delete (*iter2).second;
            }

            delete (*iter).second;
        }
        delete table;
        delete keywords;
    }

    MNNode* MNGrammarItem::get_node()
    {
        return node;
    }

    table_map* MNGrammarItem::get_table()
    {
        return table;
    }

    bool MNGrammarItem::exist_keyword(std::string lexeme)
    {
          if (keywords->find(lexeme)!=keywords->end())
              return true;
          return false;
    }


    std::vector<unsigned int>* MNGrammarItem::get_or_pos(unsigned int current, unsigned int meta)
    {
        auto c_or = table->find(current);
        if (c_or!=table->end())
        {
            auto meta_f = table->at(current)->find(meta);
            if (meta_f != table->at(current)->end())
            {
               auto c_lexeme = table->at(current)->at(meta)->find("default");
               if (c_lexeme != table->at(current)->at(meta)->end())
               {
                   return table->at(current)->at(meta)->at("default");
               }
            }
        }
        std::vector<unsigned int>* void_v = new std::vector<unsigned int>(); void_v->clear();
        return void_v;
    }

    std::vector<unsigned int>* MNGrammarItem::get_or_pos_string(unsigned int current, unsigned int meta, std::string lexeme)
    {
        auto c_or = table->find(current);
        if (c_or!=table->end())
        {
            auto meta_f = table->at(current)->find(meta);
            if (meta_f != table->at(current)->end())
            {
               auto c_lexeme = table->at(current)->at(meta)->find(lexeme);
               if (c_lexeme != table->at(current)->at(meta)->end())
               {
                   return table->at(current)->at(meta)->at(lexeme);
               }
            }
        }

        std::vector<unsigned int>* void_v = new std::vector<unsigned int>(); void_v->clear();
        return void_v;

    }






}

