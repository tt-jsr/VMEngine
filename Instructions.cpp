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
            return p;
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

    void Test::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "test: " << std::endl;
    }

    /*****************************************************/
    TestIm::TestIm(int n)
    {
        pData = DataObj::Create(n);
    }

    TestIm::TestIm(const std::string& s)
    {
        pData = DataObj::Create(s);
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

    void TestIm::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "testim: ";
        pData->Dump(strm);
        strm << std::endl;
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

    void Inc::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "inc: " << std::endl;
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

    void Dec::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "dec: " << std::endl;
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

    void Add::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "add: " << std::endl;
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

    void Subtract::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "sub: " << std::endl;
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

    void Multiply::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "mult: " << std::endl;
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

    void IntDivide::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "idiv: " << std::endl;
    }

    /*****************************************************/
    void Except::Execute(Machine& machine)
    {
        Throw(this, msg.c_str());
    }

    void Except::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "except: " << msg << std::endl;
    }

    /*****************************************************/
    void Halt::Execute (Machine& machine)
    {
        machine.registers.SetHalt();
    }

    void Halt::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "halt: " << std::endl;
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

    void JumpEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpe: " << ipTarget << std::endl;
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

    void JumpNEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpne: " << ipTarget << std::endl;
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

    void JumpGTEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpgte: " << ipTarget << std::endl;
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

    void JumpGT::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpgte: " << ipTarget << std::endl;
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

    void JumpLTEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpgte: " << ipTarget << std::endl;
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

    void JumpLT::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jumpgte: " << ipTarget << std::endl;
    }

    /*****************************************************/
    Jump::Jump()
    :ipTarget(0)
    {}

    void Jump::Execute (Machine& machine)
    {
        machine.registers.IP(ipTarget);
    }

    void Jump::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "jump: " << ipTarget << std::endl;
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

    void ContEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "conte: " << ipTarget << std::endl;
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

    void ContNEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contne: " << ipTarget << std::endl;
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

    void ContGTEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contlt: " << ipTarget << std::endl;
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

    void ContGT::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contgt: " << ipTarget << std::endl;
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

    void ContLTEQ::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contlteq: " << ipTarget << std::endl;
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

    void ContLT::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "contlt: " << ipTarget << std::endl;
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

    void CallLibrary::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "calllib: " << funcname << std::endl;
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

    void Call::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "call: " << ip << std::endl;
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

    void Return::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "return: " << std::endl;
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

    void And::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "And" << std::endl;
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

    void Or::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "Or" << std::endl;
    }

    /*****************************************************/
    void Break::Execute (Machine& machine)
    {
        return;
    }

    void Break::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "break" << std::endl;
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

    void LoadVariable::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "load: " << name << std::endl;
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

    void StoreVariable::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "store: " << name << std::endl;
    }

    /*****************************************************/
    Push::Push(int n)
    {
        pData = DataObj::Create(n);
    }

    Push::Push(const std::string& s)
    {
        pData = DataObj::Create(s);
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

    void Push::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "push: ";
		pData->Dump(strm);
		strm << std::endl;
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

    void Pop::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "pop " << std::endl;
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

    void Dup::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "dup " << std::endl;
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

    void Swap::Dump(std::ostream& strm)
    {
        strm << lineno << ":" << "swap " << std::endl;
    }
 }
