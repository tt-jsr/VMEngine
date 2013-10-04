#include "stdafx.h"
#include "machine.h"
#include "Instructions.h"
#include "Data.h"
#include "Library.h"

namespace vm
{
    Machine::Machine(void)
    {
        InitLibrary(*this);
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
        if (registers.IP() >= code.Size())
        {
            // Ran off the end of the world
            throw std::exception ("Invalid IP address");
        }
        Instruction *inst = code.Get(registers.IP());
        registers.IncIP();
        inst->Execute(*this);
    }

    void Machine::Call(const std::string& fname, std::vector<Data>& args, std::vector<Data> ret)
    {
        auto it = scriptfuncs.find(fname);
        if (it == scriptfuncs.end())
        {
            std::stringstream strm;
            strm << "UserFunction " << fname << " not found";
            throw std::exception(strm.str().c_str());
        }
        // Save the current IP
        int ipSave = registers.IP();
        // set the current ip to -1
        registers.IP(-1);

        // Save the stack position
        int tosSave = stack.Tos();

        // Push our function args on the stack
        for (size_t i = 0; i < args.size(); i++)
        {
            stack.Push(args[i]);
        }

        // We use the call instruction to set everything up
        vm::Call call;
        call.ip = it->second;
        call.Execute(*this);

        // The return statement will set the ip back to -1
        // causing the loop to terminate. Run the clock while 
        // the IP is valid
        while (registers.IP() >= 0)
        {
            Clock();
        }

        // Reset the ip
        registers.IP(ipSave);

        // Pickup the return values
        while (stack.Tos() > tosSave)
        {
            Data p = stack.Pop();
            ret.push_back(p);
        }
    }

    bool Machine::RegisterLibraryFunction(const std::string& name, std::function<void (Machine&)>& f)
    {
        library[name] = f;
        return true;
    }

    bool Machine::GetVariable(const std::string& name, Data & pData)
    {
        bool b = GetLocalVariable(name, pData);
        if (b == false)
        {
            b = GetGlobalVariable(name, pData);
        }
        return b;
    }

    bool Machine::StoreVariable(const std::string& name, Data pData)
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

    bool Machine::GetGlobalVariable(const std::string& name, Data & pData)
    {
        auto it = global_variables.find(name);
        if (it == global_variables.end())
        {
            return false;
        }
        pData = it->second;
        return true;
    }

    bool Machine::StoreGlobalVariable(const std::string& name, Data pData)
    {
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
    
    bool Machine::GetLocalVariable(const std::string& name, Data & pData)
    {
        auto it = local_variables.back().find(name);
        if (it == local_variables.back().end())
        {
            return false;
        }
        pData = it->second;
        return true;
    }

    bool Machine::StoreLocalVariable(const std::string& name, Data pData)
    {
        local_variables.back()[name] = pData;
        return true;
    }
}
