#pragma once

#include "DataStack.h"
#include "Code.h"
#include "Registers.h"
#include "Allocator.h"

namespace vm
{
    class Function;
    class Int;
    class String;
    class Variable;
    
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

        void Call(const std::string& fname, std::vector<Data >& args, std::vector<Data > ret);

        bool RegisterFunction(const std::string& name, Function *);
        Function *LookupFunction(const std::string& name);

        // Get a variable using scoping rules
        bool GetVariable(const std::string& name, Data &);

        // Store a variable using scoping rules
        bool StoreVariable(const std::string& name, Data );

        bool GetGlobalVariable(const std::string& name, Data &);
        bool StoreGlobalVariable(const std::string& name, Data );
        bool GlobalVariableExists(const std::string& name);

        bool LocalVariableExists(const std::string& name);
        bool GetLocalVariable(const std::string& name, Data &);
        bool StoreLocalVariable(const std::string& name, Data );

    public:
        void DumpMachine(std::ostream&);
    private:
        void PushLocalScope();
        void PopLocalScope();
        typedef std::map<std::string, Data > variablemap_t;
        typedef std::map<std::string, Function *> functionmap_t;
        typedef std::vector<variablemap_t> variablescope_t;
        typedef std::map<std::string, int> scriptfuncs_t;
        variablemap_t global_variables;
        variablescope_t local_variables;
        scriptfuncs_t scriptfuncs;
        functionmap_t functions;
        Allocator<Int> intAllocator;
        Allocator<String> stringAllocator;
        Allocator<Variable> variableAllocator;

        friend class Call;
        friend class Return;
        friend class Assembler;
    };
}

