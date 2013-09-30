#include "stdafx.h"
#include "Code.h"
#include <exception>

namespace vm
{
    Code::Code(void)
    {
    }


    Code::~Code(void)
    {
    }

    void Code::Dump(std::ostream& strm)
    {
        strm << "#instr: " << instructions.size() << std::endl;
    }

    Instruction *Code::Get(int idx)
    {
        if (idx < 0 || idx > (int)instructions.size())
        {
            throw std::exception("Instruction address out of range");
        }
        return instructions[idx];
    }
        
    int Code::AddInstruction(Instruction *pInst)
    {
        instructions.push_back(pInst);
        return instructions.size()-1;
    }
}
