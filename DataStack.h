#pragma once
#include "data.h"

namespace vm
{
    class DataStack
    {
    public:
        DataStack(void);
        ~DataStack(void);

        void Dump(std::ostream&);

        Data Peek(int n);
        void Push(Data );
        void Push(int n);
        void Push(const std::string&);
        Data Pop();
        void Clear();
        int Tos();
        int Size();

    private:
        std::vector<Data> stack;
    };
}

