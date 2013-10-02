#pragma once

namespace vm
{
    template <class T>
    class Allocator
    {
    public:
        Allocator()
        {

        }
        T *Allocate()
        {
            Block *p = getBlock();
            return new (p->data) T();
        }

        void Free(T *p)
        {
            returnBlock(reinterpret_cast<Block *>(p));
        }
    private:
        struct Block
        {
            char data[sizeof(T)];
        };

        Block * getBlock()
        {
            if (available.empty())
            {
                blocks.push_back(Block());
                available.push_back(&blocks[blocks.size()-1]);
            }
            Block *p = available.back();
            available.pop_back();
            allocated.insert(p);
            return p;
        }

        void returnBlock(Block *b)
        {
            // I guess I don't really care if it is found or not
            allocated.erase(b);
            available.push_back(b);
        }

        std::vector<Block> blocks;
        typedef std::set<Block *> blockset_t;
        blockset_t allocated;
        std::vector<Block *> available;
    };

}
