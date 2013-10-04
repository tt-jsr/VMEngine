#pragma once

#include "DataStack.h"
#include "Code.h"
#include "Registers.h"
#include "Allocator.h"
#include <functional>

namespace vm
{
    class UserFunction;
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

        // Call a script function
        void Call(const std::string& fname, std::vector<Data >& args, std::vector<Data > ret);

        // Register a library function
        bool RegisterLibraryFunction(const std::string& name, std::function<void (Machine&)> &);

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
        typedef std::vector<variablemap_t> variablescope_t;
        typedef std::map<std::string, int> scriptfuncs_t;
        typedef std::map<std::string, std::function<void (Machine&)> > libfuncs_t;
        // Global variables
        variablemap_t global_variables;

        // Local variables
        variablescope_t local_variables;

        // Script functions that C programs can call
        scriptfuncs_t scriptfuncs;

        // Library functions
        libfuncs_t library;
        Allocator<Int> intAllocator;
        Allocator<String> stringAllocator;
        Allocator<Variable> variableAllocator;

        friend class Call;
        friend class CallLibrary;
        friend class Return;
        friend class Assembler;
    };
}

