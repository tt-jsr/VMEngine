#include "stdafx.h"
#include "machine.h"
#include "Instructions.h"
#include "Data.h"

namespace vm
{
    Machine::Machine(void)
    {
        PushLocalScope();
    }


    Machine::~Machine(void)
    {
    }

    void Machine::Restart()
    {
        registers.Restart();
        local_variables.clear();
        PushLocalScope();
        global_variables.clear();
        stack.Clear();
        while (callstack.size())
		{
            callstack.pop();
		}
    }

    bool Machine::IsHalted()
    {
        return registers.IsHalted();
    }

    void Machine::DumpMachine(std::ostream& strm)
    {
        strm << "**************************************" << std::endl;
        strm << "Next Instruction" << std::endl;
        code.Get(registers.IP())->Dump(strm);
        strm << std::endl << "Registers" << std::endl;
        registers.Dump(strm);
        strm << std::endl << "Data stack" << std::endl;
        stack.Dump(strm);
        strm << std::endl << "Code" << std::endl;
        code.Dump(strm);

        strm << "Variables" << std::endl;
        auto it = global_variables.begin();
        for (; it != global_variables.end(); ++it)
        {
            strm << it->first << "=";
            it->second->Dump(strm);
            strm << std::endl;
        }
    }

    void Machine::Clock()
    {
        if (registers.IsHalted())
        {
            return;
        }
        Instruction *inst = code.Get(registers.IP());
        registers.IncIP();
        inst->Execute(*this);
        if (registers.IP() == code.Size()-1)
        {
            registers.SetHalt();
        }
    }

    bool Machine::GetVariable(const std::string& name, Data *& pData)
    {
        bool b = GetLocalVariable(name, pData);
        if (b == false)
        {
            b = GetGlobalVariable(name, pData);
        }
        return b;
    }

    bool Machine::StoreVariable(const std::string& name, Data *pData)
    {
        if (LocalVariableExists(name))
        {
            return StoreLocalVariable(name, pData);
        }
        if (GlobalVariableExists(name))
        {
            return StoreGlobalVariable(name, pData);
        }
        return StoreLocalVariable(name, pData);
    }

    bool Machine::GetGlobalVariable(const std::string& name, Data *& pData)
    {
        auto it = global_variables.find(name);
        if (it == global_variables.end())
        {
            return false;
        }
        pData = it->second;
        return true;
    }

    bool Machine::StoreGlobalVariable(const std::string& name, Data *pData)
    {
        auto it = global_variables.find(name);
        if (it != global_variables.end())
        {
            delete it->second;
        }
        global_variables[name] = pData;
        return true;
    }

    bool Machine::GlobalVariableExists(const std::string& name)
    {
        auto it = global_variables.find(name);
        if (it == global_variables.end())
        {
            return false;
        }
        return true;
    }

    void Machine::PushLocalScope()
    {
        local_variables.push_back(variablemap_t());
    }

    void Machine::PopLocalScope()
    {
        local_variables.pop_back();
    }

    bool Machine::LocalVariableExists(const std::string& name)
    {
        auto it = local_variables.back().find(name);
        if (it == local_variables.back().end())
        {
            return false;
        }
        return true;
    }
    
    bool Machine::GetLocalVariable(const std::string& name, Data *& pData)
    {
        auto it = local_variables.back().find(name);
        if (it == local_variables.back().end())
        {
            return false;
        }
        pData = it->second;
        return true;
    }

    bool Machine::StoreLocalVariable(const std::string& name, Data *pData)
    {
        auto it = local_variables.back().find(name);
        if (it != local_variables.back().end())
        {
            delete it->second;
        }
        local_variables.back()[name] = pData;
        return true;
    }
}
