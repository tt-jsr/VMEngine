#include "stdafx.h"
#include "Library.h"
#include "Machine.h"
#include "Data.h"
#include "DataStack.h"
#include <iostream>

namespace vm
{
    std::string PeekString(Machine& machine, int n)
    {
        Data d = machine.stack.Peek(n);
        if (d == nullptr)
        {
            throw std::exception("Stack underflow");
        }
        if (d->IsVariable())
        {
            std::string name;
            d->GetVariable(name);
            machine.GetVariable(name, d); // Fall through in case it's a field
        }
        if (d->IsField())
        {
            std::string tag;
            d->GetField(tag);
            std::string value = machine.registers.input.GetFieldValue(tag);
            return value;
        }
        if (d->IsString() == false)
        {
            throw std::exception("Argument not a string");
        }
        std::string s;
        d->GetString(s);
        return s;
    }

    std::string PopString(Machine& machine)
    {
        std::string s = PeekString(machine, 0);
        machine.stack.Pop();
        return s;
    }

    int PeekInt(Machine& machine, int pos)
    {
        Data d = machine.stack.Peek(pos);
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
    
    int PopInt(Machine& machine)
    {
        int n = PeekInt(machine, 0);
        machine.stack.Pop();
        return n;
    }

    std::vector<Data> PeekArray(Machine& machine, int n)
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
        if (d->IsArray() == false)
        {
            throw std::exception("Argument not an array");
        }
        return std::move(((Array *)d.get())->items);
    }

    std::vector<Data> PopArray(Machine& machine)
    {
        std::vector<Data> v = PeekArray(machine, 0);
        machine.stack.Pop();
        return v;
    }

    Data Peek(Machine& machine, int n)
    {
        Data d = machine.stack.Peek(n);
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

    Data Pop(Machine& machine)
    {
        Data d = Peek(machine, 0);
        machine.stack.Pop();
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

    void lib_strsplit(Machine& machine)
    {
        std::string delim = PopString(machine);
        std::string s = PopString(machine);
        const char *p = s.c_str();
        std::stringstream strm;
        std::vector<Data> items;
        while (*p)
        {
            if (*p == delim[0])
            {
                items.push_back(DataObj::CreateString(strm.str()));
                strm.str("");
            }
            else
            {
                strm << *p;
            }
            ++p;
        }
        if (strm.str().size() > 0)
        {
            items.push_back(DataObj::CreateString(strm.str()));
            strm.str();
        }
        machine.stack.Push(items);
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

    /************************************************************************/
    // Array methods
    void lib_arr_append(Machine& machine)
    {
        Data d = Pop(machine);
        std::vector<Data> vec = PopArray(machine);
        vec.push_back(d);
        machine.stack.Push(vec);
    }

    void lib_arr_get(Machine& machine)
    {
        int idx = PopInt(machine);
        std::vector<Data> vec = PopArray(machine);
        if (idx < 0 || idx >= (int)vec.size())
        {
            std::stringstream strm;
            strm << "Index out of range";
            throw std::exception(strm.str().c_str());
        }
        Data d = vec[idx];
        machine.stack.Push(d);
    }

    void lib_arr_size(Machine& machine)
    {
        std::vector<Data> vec = PopArray(machine);
        machine.stack.Push(vec.size());
    }

    /************************************************************************/
    void InitLibrary(Machine& machine)
    {
        machine.RegisterLibraryFunction("strcmp", std::function<void (Machine&)>(lib_strcmp));
        machine.RegisterLibraryFunction("substr", std::function<void (Machine&)>(lib_substr));
        machine.RegisterLibraryFunction("strcat", std::function<void (Machine&)>(lib_strcat));
        machine.RegisterLibraryFunction("strsplit", std::function<void (Machine&)>(lib_strsplit));
        machine.RegisterLibraryFunction("print", std::function<void (Machine&)>(lib_print));
        machine.RegisterLibraryFunction("println", std::function<void (Machine&)>(lib_println));
        machine.RegisterLibraryFunction("arr_append", std::function<void (Machine&)>(lib_arr_append));
        machine.RegisterLibraryFunction("arr_get", std::function<void (Machine&)>(lib_arr_get));
        machine.RegisterLibraryFunction("arr_size", std::function<void (Machine&)>(lib_arr_size));
    }

}
