#pragma once

#include "DataStack.h"
#include "Code.h"
#include "Registers.h"

namespace vm
{
    class Data;

    class Machine
    {
    public:
        Machine(void);
        ~Machine(void);

        void Restart();
        bool IsHalted();
        void Clock();

        DataStack stack;
        Registers registers;
        Code code;
        std::stack<int> callstack;

        // Get a variable using scoping rules
        bool GetVariable(const std::string& name, Data *&);

        // Store a variable using scoping rules
        bool StoreVariable(const std::string& name, Data *);

        bool GetGlobalVariable(const std::string& name, Data *&);
        bool StoreGlobalVariable(const std::string& name, Data *);
        bool GlobalVariableExists(const std::string& name);

        void PushLocalScope();
        void PopLocalScope();
        bool LocalVariableExists(const std::string& name);
        bool GetLocalVariable(const std::string& name, Data *&);
        bool StoreLocalVariable(const std::string& name, Data *);
    public:
        void DumpMachine(std::ostream&);
    private:
        typedef std::map<std::string, Data *> variablemap_t;
        typedef std::vector<variablemap_t> variablescope_t;
        variablemap_t global_variables;
        variablescope_t local_variables;
    };
}

