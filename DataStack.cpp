#include "stdafx.h"
#include "DataStack.h"
#include "Data.h"

namespace vm
{
    DataStack::DataStack(void)
        :tos(0)
    {
    }


    DataStack::~DataStack(void)
    {
    }

    void DataStack::Dump(std::ostream& strm)
    {
        if (stack.size() == 0)
		{
            strm << "Empty" << std::endl;
            return;
		}
        for (int idx = tos-1; idx >= 0; --idx)
        {
            stack[idx]->Dump(strm);
			strm << std::endl;
        }
    }

    void DataStack::Clear()
	{
        stack.clear();
	}

    void DataStack::Tos()
    {
        return tos;
    }

    void DataStack::Push(Data *d)
    {
        if (tos == (stack.size()))
        {
            stack.push_back(d);
        }
        else
        {
            stack[tos] = d;
        }
        ++tos;
    }

    void DataStack::Push(const std::string& s)
    {
        if (tos == (stack.size()))
        {
            stack.push_back(new String(s));
        }
        else
        {
            stack[tos] = new String(s);
        }
        ++tos;
    }

    void DataStack::Push(int n)
    {
        if (tos == stack.size())
        {
            stack.push_back(new Int(n));
        }
        else
        {
            stack[tos] = new Int(n);
        }
        ++tos;
    }

    Data *DataStack::Pop()
    {
        if (tos == 0)
        {
            return nullptr;
        }
        --tos;
        return stack[tos];
    }

    Data *DataStack::Peek(int n)
    {
        int idx = tos-n-1;
        if (idx < 0)
        {
            return nullptr;
        }
        return stack[idx];
    }
}
