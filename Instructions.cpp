#include "stdafx.h"
#include "Instructions.h"
#include "machine.h"
#include "stdafx.h"
#include "Data.h"

namespace
{
    const vm::Data ResolveVariable(vm::Machine& machine, vm::Data pData)
    {
        if (pData.get() == nullptr)
        {
            return pData;
        }
        if (pData->IsVariable())
        {
            std::string name;
            pData->GetVariable(name);
            vm::Data p;
            machine.GetVariable(name, p);
            // Fall through, it might be a field
        }
        if (pData->IsField())
        {
            std::string tag;
            pData->GetField(tag);
            std::string value = machine.registers.input.GetFieldValue(tag);
            return vm::DataObj::CreateString(value);
        }
        return pData;
    }

    void Throw(vm::Instruction *pInst, const char *msg)
    {
        std::stringstream strm;
        strm << msg << " at line " << pInst->lineno;
        throw std::exception(strm.str().c_str());
    }
}

namespace vm
{
    Instruction::Instruction()
        :lineno(0)
    {}

    Instruction::~Instruction()
    {}

    /***********************************************/
    void Test::Execute(Machine& machine)
    {
        const Data d1 = ResolveVariable(machine, machine.stack.Peek(0));
        const Data d2 = ResolveVariable(machine, machine.stack.Peek(1));
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        if (d1->type != d2->type)
        {
            Throw(this, "Cannot compare types");
        }
        int result = 0;
        if (d1->IsInt())
        {
            int n1, n2;
            d1->GetInt(n1);
            d2->GetInt(n2);
            result = n1 - n2 ;
        }
        if (d1->IsString())
        {
            std::string s1, s2;
            d1->GetString(s1);
            d1->GetString(s2);
            result = s1.compare(s2);
        }
        if (result < 0)
            machine.registers.SetLT();
        if (result == 0)
            machine.registers.SetEQ();
        if (result > 0)
            machine.registers.SetGT();
    }

    std::ostream& Test::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "test";
        return strm;
    }

    /*****************************************************/
    TestIm::TestIm(int n)
    {
        pData = DataObj::CreateInt(n);
    }

    TestIm::TestIm(const std::string& s)
    {
        pData = DataObj::CreateString(s);
    }

    TestIm::TestIm(Data p)
    {
        pData = p;
	}

    TestIm::~TestIm()
    {
    }

    void TestIm::Execute(Machine& machine)
    {
        const Data d1 = ResolveVariable(machine, machine.stack.Peek(0));
        if (d1 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data pConstant = ResolveVariable(machine, pData);
        if (d1->type != pConstant->type)
        {
            Throw(this, "Cannot compare types");
        }
        int result = 0;
        if (d1->IsInt())
        {
            int n1, n2;
            d1->GetInt(n1);
            pConstant->GetInt(n2);
            result = n1 - n2;
        }
        if (d1->IsString())
        {
            std::string s1, s2;
            d1->GetString(s1);
            pConstant->GetString(s2);
            result = s1.compare(s2);
        }
        if (result < 0)
            machine.registers.SetLT();
        if (result == 0)
            machine.registers.SetEQ();
        if (result > 0)
            machine.registers.SetGT();
    }

    std::ostream& TestIm::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "testim" << pData->Dump(strm);
        return strm;
    }

    /*****************************************************/
    void Inc::Execute(Machine& machine)
    {
        Data d1 = machine.stack.Peek(0);
        if (d1 == nullptr)
        {
            Throw(this, "Stack underflow");
        }

        if (d1->IsInt() == false)
        {
            Throw(this, "Cannot increment non-int");
        }
        ++((Int *)d1.get())->n;
    }

    std::ostream& Inc::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "inc";
        return strm;
    }

    /*****************************************************/
    void Dec::Execute(Machine& machine)
    {
        Data d1 = machine.stack.Peek(0);
        if (d1 == nullptr)
        {
            Throw(this, "Stack underflow");
        }

        if (d1->IsInt() == false)
        {
            Throw(this, "Cannot decrement non-int");
        }
        --((Int *)d1.get())->n;
    }

    std::ostream& Dec::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "dec";
        return strm;
    }

    /*****************************************************/
    void Add::Execute(Machine& machine)
    {
        Data d1 = machine.stack.Pop();
        Data d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data a1 = ResolveVariable(machine, d1);
        const Data a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            Throw(this, "Cannot add different types");
        }
        if (a1->IsInt() == false)
        {
            Throw(this, "Cannot add non integers");
        }

        int n1, n2;
        a1->GetInt(n1);
        a2->GetInt(n2);
        int n = n1 + n2;
        machine.stack.Push(n);
    }

    std::ostream& Add::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "add";
        return strm;
    }

    /*****************************************************/
    void Subtract::Execute(Machine& machine)
    {
        Data d2 = machine.stack.Pop();
        Data d1 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data a1 = ResolveVariable(machine, d1);
        const Data a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            Throw(this, "Cannot subtract different types");
        }
        if (a1->IsInt() == false)
        {
            Throw(this, "Cannot subtract non integers");
        }

        int n1, n2;
        a1->GetInt(n1);
        a2->GetInt(n2);
        int n = n1 - n2;
        machine.stack.Push(n);
    }

    std::ostream& Subtract::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "sub";
        return strm;
    }

    /*****************************************************/
    void Multiply::Execute(Machine& machine)
    {
        Data d1 = machine.stack.Pop();
        Data d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data a1 = ResolveVariable(machine, d1);
        const Data a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            Throw(this, "Cannot multiply different types");
        }
        if (a1->IsInt() == false)
        {
            Throw(this, "Cannot multiply non integers");
        }

        int n1, n2;
        a1->GetInt(n1);
        a2->GetInt(n2);
        int n = n1 * n2;
        machine.stack.Push(n);
    }

    std::ostream& Multiply::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "mult";
        return strm;
    }

    /*****************************************************/
    void IntDivide::Execute(Machine& machine)
    {
        Data d2 = machine.stack.Pop();
        Data d1 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data a1 = ResolveVariable(machine, d1);
        const Data a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            Throw(this, "Cannot divide different types");
        }
        if (a1->IsInt() == false)
        {
            Throw(this, "Cannot divide non integers");
        }

        int n1, n2;
        a1->GetInt(n1);
        a2->GetInt(n2);
        int n = n1 / n2;
        machine.stack.Push(n);
    }

    std::ostream& IntDivide::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "idiv";
        return strm;
    }

    /*****************************************************/
    void Except::Execute(Machine& machine)
    {
        Throw(this, msg.c_str());
    }

    std::ostream& Except::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "except: " << msg;
        return strm;
    }

    /*****************************************************/
    void Halt::Execute (Machine& machine)
    {
        machine.registers.SetHalt();
    }

    std::ostream& Halt::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "halt";
        return strm;
    }

    /*****************************************************/
    JumpEQ::JumpEQ()
    :ipTarget(0)
    {}

    void JumpEQ::Execute (Machine& machine)
    {
        if (machine.registers.IsEQ())
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& JumpEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpe: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    JumpNEQ::JumpNEQ()
    :ipTarget(0)
    {}

    void JumpNEQ::Execute (Machine& machine)
    {
        if (machine.registers.IsEQ() == false)
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& JumpNEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpne: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    JumpGTEQ::JumpGTEQ()
    :ipTarget(0)
    {}

    void JumpGTEQ::Execute (Machine& machine)
    {
        if (machine.registers.IsGT() || machine.registers.IsEQ())
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& JumpGTEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpgte: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    JumpGT::JumpGT()
    :ipTarget(0)
    {}

    void JumpGT::Execute (Machine& machine)
    {
        if (machine.registers.IsGT())
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& JumpGT::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpgte: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    JumpLTEQ::JumpLTEQ()
    :ipTarget(0)
    {}

    void JumpLTEQ::Execute (Machine& machine)
    {
        if (machine.registers.IsLT() || machine.registers.IsEQ())
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& JumpLTEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpgte: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    JumpLT::JumpLT()
    :ipTarget(0)
    {}

    void JumpLT::Execute (Machine& machine)
    {
        if (machine.registers.IsLT())
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& JumpLT::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpgte: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    Jump::Jump()
    :ipTarget(0)
    {}

    void Jump::Execute (Machine& machine)
    {
        machine.registers.IP(ipTarget);
    }

    std::ostream& Jump::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jump: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    ContEQ::ContEQ()
    :ipTarget(0)
    {}

    void ContEQ::Execute (Machine& machine)
    {
        if (machine.registers.IsEQ() == false)
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& ContEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "conte: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    ContNEQ::ContNEQ()
    :ipTarget(0)
    {}

    void ContNEQ::Execute (Machine& machine)
    {
        if (machine.registers.IsEQ() == true)
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& ContNEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contne: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    ContGTEQ::ContGTEQ()
    :ipTarget(0)
    {}

    void ContGTEQ::Execute (Machine& machine)
    {
        if (machine.registers.IsLT() == false)
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& ContGTEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contlt: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    ContGT::ContGT()
    :ipTarget(0)
    {}

    void ContGT::Execute (Machine& machine)
    {
        if (machine.registers.IsLT() || machine.registers.IsEQ())
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& ContGT::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contgt: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    ContLTEQ::ContLTEQ()
    :ipTarget(0)
    {}

    void ContLTEQ::Execute (Machine& machine)
    {
        if (machine.registers.IsGT())
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& ContLTEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contlteq: " << ipTarget;
        return strm;
    }

    /*****************************************************/
    ContLT::ContLT()
    :ipTarget(0)
    {}

    void ContLT::Execute (Machine& machine)
    {
        if (machine.registers.IsGT() || machine.registers.IsEQ())
        {
            machine.registers.IP(ipTarget);
        }
    }

    std::ostream& ContLT::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contlt: " << ipTarget;
        return strm;
    }
    /*****************************************************/
    CallLibrary::CallLibrary()
    {}

    void CallLibrary::Execute (Machine& machine)
    {
        auto it = machine.library.find(funcname);
        if (it == machine.library.end())
        {
            std::stringstream strm;
            strm << "Library function " << funcname << " not found";
            Throw(this, strm.str().c_str());
        }
        try
        {
            it->second(machine);
        }
        catch (std::exception& ex)
        {
            Throw(this, ex.what());
        }
    }

    std::ostream& CallLibrary::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "calllib: " << funcname;
        return strm;
    }

    /*****************************************************/
    Call::Call()
    :ip(0)
    {}

    void Call::Execute (Machine& machine)
    {
        machine.PushLocalScope();
        machine.callstack.push(machine.registers.IP());
        machine.registers.IP(ip);
    }

    std::ostream& Call::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "call: " << ip;
        return strm;
    }

    /*****************************************************/
    Return::Return()
    {}

    void Return::Execute (Machine& machine)
    {
        if (machine.callstack.empty())
        {
            Throw(this, "Callstack underflow");
        }
        int n = machine.callstack.top();
        machine.callstack.pop();
        machine.registers.IP(n);
        machine.PopLocalScope();

    }

    std::ostream& Return::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "return";
        return strm;
    }

    /*****************************************************/
    void And::Execute(Machine& machine)
    {
        Data d1 = machine.stack.Pop();
        Data d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data a1 = ResolveVariable(machine, d1);
        const Data a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            Throw(this, "Cannot And different types");
        }
        if (a1->IsInt() == false)
        {
            Throw(this, "Cannot And non integers");
        }

        int n1, n2;
        a1->GetInt(n1);
        a2->GetInt(n2);
        int n = n1 && n2;
        machine.stack.Push(n);
    }

    std::ostream& And::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "and";
        return strm;
    }

    /*****************************************************/
    void Or::Execute(Machine& machine)
    {
        Data d1 = machine.stack.Pop();
        Data d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data a1 = ResolveVariable(machine, d1);
        const Data a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            Throw(this, "Cannot Or different types");
        }
        if (a1->IsInt() == false)
        {
            Throw(this, "Cannot Or non integers");
        }

        int n1, n2;
        a1->GetInt(n1);
        a2->GetInt(n2);
        int n = n1 || n2;
        machine.stack.Push(n);
    }

    std::ostream& Or::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "or";
        return strm;
    }

    /*****************************************************/
    void Equal::Execute(Machine& machine)
    {
        Data d1 = machine.stack.Pop();
        Data d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data a1 = ResolveVariable(machine, d1);
        const Data a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            Throw(this, "Cannot test equality of different types");
        }
        if (a1->IsInt() == true)
        {
            int n1, n2;
            a1->GetInt(n1);
            a2->GetInt(n2);
            bool b = n1 == n2;
            machine.stack.Push(b);
        }
        else if (a1->IsString() == true)
        {
            std::string s1, s2;
            a1->GetString(s1);
            a2->GetString(s2);
            bool b = s1 == s2;
            machine.stack.Push(b);
        }
        else if (a1->IsArray() == true)
        {
            bool b = ((Array *)a1.get())->items == ((Array *)a2.get())->items;
            machine.stack.Push(b);
        }
        else
        {
            Throw(this, "Unknown data type");
        }
    }

    std::ostream& Equal::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "eq";
        return strm;
    }
    /*****************************************************/
    void NotEqual::Execute(Machine& machine)
    {
        Data d1 = machine.stack.Pop();
        Data d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            Throw(this, "Stack underflow");
        }
        const Data a1 = ResolveVariable(machine, d1);
        const Data a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            Throw(this, "Cannot test equality of different types");
        }
        if (a1->IsInt() == true)
        {
            int n1, n2;
            a1->GetInt(n1);
            a2->GetInt(n2);
            bool b = n1 != n2;
            machine.stack.Push(b);
        }
        else if (a1->IsString() == true)
        {
            std::string s1, s2;
            a1->GetString(s1);
            a2->GetString(s2);
            bool b = s1 != s2;
            machine.stack.Push(b);
        }
        else if (a1->IsArray() == true)
        {
            bool b = ((Array *)a1.get())->items != ((Array *)a2.get())->items;
            machine.stack.Push(b);
        }
        else
        {
            Throw(this, "Unknown data type");
        }
    }

    std::ostream& NotEqual::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "neq";
        return strm;
    }

    /*****************************************************/
    void Break::Execute (Machine& machine)
    {
        return;
    }

    std::ostream& Break::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "break";
        return strm;
    }

    
    /*****************************************************/
    LoadVariable::LoadVariable(const std::string& s)
        :name(s)
    {}

    void LoadVariable::Execute(Machine& machine)
    {
        Data pData;
        bool b = machine.GetVariable(name, pData);
        if (b == false)
        {
            std::stringstream strm;
            strm << "Variable " << name << " does not exist";
            Throw(this, strm.str().c_str());
        }
        machine.stack.Push(pData->Clone());
    }

    std::ostream& LoadVariable::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "load: " << name;
        return strm;
    }

    /*****************************************************/
    StoreVariable::StoreVariable(const std::string& s)
        :name(s)
    {}

    void StoreVariable::Execute(Machine& machine)
    {
        Data pData = machine.stack.Pop();
        if (pData == nullptr)
        {
            Throw(this, "stack underflow");
        }
        machine.StoreVariable(name, pData);
    }

    std::ostream& StoreVariable::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "store: " << name;
        return strm;
    }

    /*****************************************************/
    Push::Push(int n)
    {
        pData = DataObj::CreateInt(n);
    }

    Push::Push(const std::string& s)
    {
        pData = DataObj::CreateString(s);
    }

    Push::Push(Data p)
    {
        pData = p;
    }

    Push::~Push()
    {
    }

    void Push::Execute(Machine& machine)
    {
        if (pData->type == DataObj::VARIABLE)
        {
            const Data p = ResolveVariable(machine, pData);
            if (p == nullptr)
            {
                Throw(this, "Variable not found");
            }
        }
        machine.stack.Push(pData->Clone());
    }

    std::ostream& Push::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "push: " << pData->Dump(strm);
        return strm;
    }

    /*****************************************************/
    void Pop::Execute(Machine& machine)
    {
        Data pData = machine.stack.Pop();
        if (pData == nullptr)
        {
            Throw(this, "stack underflow");
        }
    }

    std::ostream& Pop::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "pop";
        return strm;
    }

    /*****************************************************/
    void Dup::Execute(Machine& machine)
    {
        Data pData = machine.stack.Peek(0);
        if (pData == nullptr)
        {
            Throw(this, "stack underflow");
        }
        Data pNew = pData->Clone();
        machine.stack.Push(pNew);
    }

    std::ostream& Dup::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "dup";
        return strm;
    }

    /*****************************************************/
    void Swap::Execute(Machine& machine)
    {
        Data pData1 = machine.stack.Pop();
        if (pData1 == nullptr)
        {
            Throw(this, "stack underflow");
        }
        Data pData2 = machine.stack.Pop();
        if (pData2 == nullptr)
        {
            Throw(this, "stack underflow");
        }
        machine.stack.Push(pData1);
        machine.stack.Push(pData2);
    }

    std::ostream& Swap::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "swap";
        return strm;
    }
 }
