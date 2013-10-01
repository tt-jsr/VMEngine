#include "stdafx.h"
#include "DataStack.h"
#include "Data.h"

namespace vm
{
    DataStack::DataStack(void)
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
        for (size_t idx = stack.size()-1; idx >= 0; --idx)
        {
            stack[idx]->Dump(strm);
			strm << std::endl;
        }
    }

    int DataStack::Size()
    {
        return (int)stack.size();
    }

    void DataStack::Clear()
	{
        stack.clear();
	}

    int DataStack::Tos()
    {
        return (int)stack.size();
    }

    void DataStack::Push(Data *d)
    {
        stack.push_back(d);
    }

    void DataStack::Push(const std::string& s)
    {
        stack.push_back(new String(s));
    }

    void DataStack::Push(int n)
    {
        stack.push_back(new Int(n));
    }

    Data *DataStack::Pop()
    {
        if (stack.size() == 0)
        {
            return nullptr;
        }
        Data *p = stack[stack.size()-1];
        stack.pop_back();
        return p;
    }

    Data *DataStack::Peek(int n)
    {
        if (n < 0)
        {
            return nullptr;
        }
        int idx = (int)stack.size()-n-1;
        if (idx < 0)
        {
            return nullptr;
        }
        return stack[idx];
    }
}
