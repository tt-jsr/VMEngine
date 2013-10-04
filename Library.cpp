#include "stdafx.h"
#include "Library.h"
#include "Machine.h"
#include "Data.h"
#include "DataStack.h"
#include <iostream>

namespace vm
{
    std::string PopString(Machine& machine)
    {
        Data d = machine.stack.Pop();
        if (d == nullptr)
        {
            throw std::exception("Stack underflow");
        }
        if (d->IsVariable())
        {
            std::string name;
            d->GetVariable(name);
            machine.GetVariable(name, d);
        }
        if (d->IsString() == false)
        {
            throw std::exception("Argument not a string");
        }
        std::string s;
        d->GetString(s);
        return s;
    }

    int PopInt(Machine& machine)
    {
        Data d = machine.stack.Pop();
        if (d == nullptr)
        {
            throw std::exception("Stack underflow");
        }
        if (d->IsVariable())
        {
            std::string name;
            d->GetVariable(name);
            machine.GetVariable(name, d);
        }
        if (d->IsInt() == false)
        {
            throw std::exception("Argument not a int");
        }
        int n;
        d->GetInt(n);
        return n;
    }

    Data Pop(Machine& machine)
    {
        Data d = machine.stack.Pop();
        if (d == nullptr)
        {
            throw std::exception("Stack underflow");
        }
        if (d->IsVariable())
        {
            std::string name;
            d->GetVariable(name);
            machine.GetVariable(name, d);
        }
        return d;
    }

    /*****************************************************************************/
    void lib_strcmp(Machine& machine)
    {
        int ignoreCase = PopInt(machine);
        std::string a2 = PopString(machine);
        std::string a1 = PopString(machine);
        int n = 0;
        if (ignoreCase)
        {
            n = _stricmp(a1.c_str(), a2.c_str());
        }
        else
        {
            n = ::strcmp(a1.c_str(), a2.c_str());
        }
        machine.stack.Push(n);
    }

    void lib_substr(Machine& machine)
    {
        int len = PopInt(machine);
        int startPos = PopInt(machine);
        std::string s = PopString(machine);
        if (len == 0)
        {
            len = std::string::npos;
        }
        std::string r = s.substr(startPos, len);
        machine.stack.Push(r);
    }

    void lib_strcat(Machine& machine)
    {
        std::string s2 = PopString(machine);
        std::string s1 = PopString(machine);
        std::string r = s1 + s2;
        machine.stack.Push(r);
    }

    void lib_print(Machine& machine)
    {
        Data d = Pop(machine);
        if (d->IsInt())
        {
            int n;
            d->GetInt(n);
            std::cout << n;
        }
        if (d->IsString())
        {
            std::string s;
            d->GetString(s);
            std::cout << s;
        }
    }

    void lib_println(Machine& machine)
    {
        Data d = Pop(machine);
        if (d->IsInt())
        {
            int n;
            d->GetInt(n);
            std::cout << n << std::endl;
        }
        if (d->IsString())
        {
            std::string s;
            d->GetString(s);
            std::cout << s << std::endl;
        }
    }

    void InitLibrary(Machine& machine)
    {
        machine.RegisterLibraryFunction("strcmp", std::function<void (Machine&)>(lib_strcmp));
        machine.RegisterLibraryFunction("substr", std::function<void (Machine&)>(lib_substr));
        machine.RegisterLibraryFunction("strcat", std::function<void (Machine&)>(lib_strcat));
        machine.RegisterLibraryFunction("print", std::function<void (Machine&)>(lib_print));
        machine.RegisterLibraryFunction("println", std::function<void (Machine&)>(lib_println));
    }

}
