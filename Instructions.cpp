#include "stdafx.h"
#include "Instructions.h"
#include "machine.h"
#include "stdafx.h"
#include "Data.h"
#include "Function.h"

namespace
{
    const vm::Data *ResolveVariable(vm::Machine& machine, vm::Data *pData)
    {
        if (pData == nullptr)
        {
            return pData;
        }
        if (pData->type == vm::Data::INT)
        {
            return pData;
        }
        if (pData->type == vm::Data::STRING)
        {
            return pData;
        }
        if (pData->type == vm::Data::VARIABLE)
        {
            vm::Variable *pVar = (vm::Variable *)pData;
            vm::Data *p = nullptr;
            machine.GetVariable(pVar->name, p);
            return p;
        }
        return pData;
    }
}

namespace vm
{
    Instruction::Instruction()
    {}

    Instruction::~Instruction()
    {}

    /***********************************************/
    void Test::Execute(Machine& machine)
    {
        const Data *d1 = ResolveVariable(machine, machine.stack.Peek(0));
        const Data *d2 = ResolveVariable(machine, machine.stack.Peek(1));
        if (d1 == nullptr || d2 == nullptr)
        {
            throw std::exception("Stack underflow");
        }
        if (d1->type != d2->type)
        {
            throw std::exception("Cannot compare types");
        }
        int result = 0;
        if (d1->type == Data::INT)
        {
            result = ((Int *)d1)->n - ((Int *)d2)->n;
        }
        if (d1->type == Data::STRING)
        {
            result = ((String *)d1)->str.compare(((String *)d2)->str);
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
        strm << "test: " << std::endl;
    }

    /*****************************************************/
    TestIm::TestIm(int n)
    {
        pData = new Int(n);
    }

    TestIm::TestIm(const std::string& s)
    {
        pData = new String(s);
    }

    TestIm::TestIm(Data *p)
    {
        pData = p;
	}

    TestIm::~TestIm()
    {
        delete pData;
    }

    void TestIm::Execute(Machine& machine)
    {
        const Data *d1 = ResolveVariable(machine, machine.stack.Peek(0));
        if (d1 == nullptr)
        {
            throw std::exception("Stack underflow");
        }
        const Data *pConstant = ResolveVariable(machine, pData);
        if (d1->type != pConstant->type)
        {
            throw std::exception("Cannot compare types");
        }
        int result = 0;
        if (d1->type == Data::INT)
        {
            result = ((Int *)d1)->n - ((Int *)pConstant)->n;
        }
        if (d1->type == Data::STRING)
        {
            result = ((String *)d1)->str.compare(((String *)pConstant)->str);
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
        strm << "testim: ";
        pData->Dump(strm);
        strm << std::endl;
    }

    /*****************************************************/
    void Inc::Execute(Machine& machine)
    {
        Data *d1 = machine.stack.Peek(0);
        if (d1 == nullptr)
        {
            throw std::exception("Stack underflow");
        }

        if (d1->type != Data::INT)
        {
            throw std::exception("Cannot increment non-int");
        }
        ++((Int *)d1)->n;
    }

    void Inc::Dump(std::ostream& strm)
    {
        strm << "inc: " << std::endl;
    }

    /*****************************************************/
    void Dec::Execute(Machine& machine)
    {
        Data *d1 = machine.stack.Peek(0);
        if (d1 == nullptr)
        {
            throw std::exception("Stack underflow");
        }

        if (d1->type != Data::INT)
        {
            throw std::exception("Cannot decrement non-int");
        }
        --((Int *)d1)->n;
    }

    void Dec::Dump(std::ostream& strm)
    {
        strm << "dec: " << std::endl;
    }

    /*****************************************************/
    void Add::Execute(Machine& machine)
    {
        Data *d1 = machine.stack.Pop();
        Data *d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            delete d1;
            delete d2;
            throw std::exception("Stack underflow");
        }
        const Data *a1 = ResolveVariable(machine, d1);
        const Data *a2 = ResolveVariable(machine, d2);

        if (a1->type != a2->type)
        {
            delete d1;
            delete d2;
            throw std::exception("Cannot add types");
        }

        if (a1->type == Data::INT)
        {
            int n = ((const Int *)a1)->n + ((const Int *)a2)->n;
            machine.stack.Push(n);
        }
        delete d1;
        delete d2;
    }

    void Add::Dump(std::ostream& strm)
    {
        strm << "add: " << std::endl;
    }

    /*****************************************************/
    void Subtract::Execute(Machine& machine)
    {
        Data *d1 = machine.stack.Pop();
        Data *d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            delete d1;
            delete d2;
            throw std::exception("Stack underflow");
        }
        const Data *a1 = ResolveVariable(machine, d1);
        const Data *a2 = ResolveVariable(machine, d2);
        if (a1->type != a2->type)
        {
            delete d1;
            delete d2;
            throw std::exception("Cannot subtract types");
        }
        if (a1->type == Data::INT)
        {
            int n = ((const Int *)a1)->n - ((const Int *)a2)->n;
            machine.stack.Push(n);
        }
        delete d1;
        delete d2;
    }

    void Subtract::Dump(std::ostream& strm)
    {
        strm << "sub: " << std::endl;
    }

    /*****************************************************/
    void Multiply::Execute(Machine& machine)
    {
        Data *d1 = machine.stack.Pop();
        Data *d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            delete d1;
            delete d2;
            throw std::exception("Stack underflow");
        }
        const Data *a1 = ResolveVariable(machine, d1);
        const Data *a2 = ResolveVariable(machine, d2);
        if (a1->type != a2->type)
        {
            delete d1;
            delete d2;
            throw std::exception("Cannot add types");
        }
        if (a1->type == Data::STRING)
        {
            delete d1;
            delete d2;
            throw std::exception("Cannot multiply strings");
        }
        if (a1->type == Data::INT)
        {
            int n = ((const Int *)a1)->n * ((const Int *)a2)->n;
            machine.stack.Push(n);
        }
        delete d1;
        delete d2;
    }

    void Multiply::Dump(std::ostream& strm)
    {
        strm << "mult: " << std::endl;
    }

    /*****************************************************/
    void IntDivide::Execute(Machine& machine)
    {
        Data *d1 = machine.stack.Pop();
        Data *d2 = machine.stack.Pop();
        if (d1 == nullptr || d2 == nullptr)
        {
            delete d1;
            delete d2;
            throw std::exception("Stack underflow");
        }
        const Data *a1 = ResolveVariable(machine, d1);
        const Data *a2 = ResolveVariable(machine, d2);
        if (a1->type != a2->type)
        {
            delete d1;
            delete d2;
            throw std::exception("Cannot add types");
        }
        if (a1->type == Data::INT)
        {
            int n = ((const Int *)a1)->n / ((const Int *)a2)->n;
            machine.stack.Push(n);
        }
        delete d1;
        delete d2;
    }

    void IntDivide::Dump(std::ostream& strm)
    {
        strm << "idiv: " << std::endl;
    }

    /*****************************************************/
    void Halt::Execute (Machine& machine)
    {
        machine.registers.SetHalt();
    }

    void Halt::Dump(std::ostream& strm)
    {
        strm << "halt: " << std::endl;
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
        strm << "jumpe: " << ipTarget << std::endl;
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
        strm << "jumpne: " << ipTarget << std::endl;
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
        strm << "jumpgte: " << ipTarget << std::endl;
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
        strm << "jumpgte: " << ipTarget << std::endl;
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
        strm << "jumpgte: " << ipTarget << std::endl;
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
        strm << "jumpgte: " << ipTarget << std::endl;
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
        strm << "jump: " << ipTarget << std::endl;
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
        strm << "conte: " << ipTarget << std::endl;
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
        strm << "contne: " << ipTarget << std::endl;
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
        strm << "contlt: " << ipTarget << std::endl;
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
        strm << "contgt: " << ipTarget << std::endl;
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
        strm << "contlteq: " << ipTarget << std::endl;
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
        strm << "contlt: " << ipTarget << std::endl;
    }

    /*****************************************************/
    Call::Call()
    :ip(0)
     , nargs(0)
    {}

    void Call::Execute (Machine& machine)
    {
        machine.PushLocalScope();
        machine.callstack.push(machine.registers.NArgs());
        machine.registers.NArgs(nargs);
        if (ip <= 0)
        {
            Function *pFunc = machine.LookupFunction(funcname);
            if (pFunc == nullptr)
            {
                std::stringstream strm;
                strm << "Function " << funcname << " not found";
                throw std::exception(strm.str().c_str());
            }
            pFunc->OnExecute(machine);
        }
        else
        {
            machine.callstack.push(machine.registers.IP());
            machine.registers.IP(ip);
        }
    }

    void Call::Dump(std::ostream& strm)
    {
        strm << "call: " << ip << std::endl;
    }

    /*****************************************************/
    Return::Return()
    {}

    void Return::Execute (Machine& machine)
    {
        if (machine.callstack.empty())
        {
            throw std::exception("Callstack underflow");
        }
        int n = machine.callstack.top();
        machine.callstack.pop();
        machine.registers.IP(n);
        n = machine.callstack.top();
        machine.registers.NArgs(n);
        machine.PopLocalScope();

    }

    void Return::Dump(std::ostream& strm)
    {
        strm << "return: " << std::endl;
    }

    /*****************************************************/
    void Break::Execute (Machine& machine)
    {
        return;
    }

    void Break::Dump(std::ostream& strm)
    {
        strm << "break" << std::endl;
    }

    
    /*****************************************************/
    LoadVariable::LoadVariable(const std::string& s)
        :name(s)
    {}

    void LoadVariable::Execute(Machine& machine)
    {
        Data *pData;
        bool b = machine.GetVariable(name, pData);
        if (b == false)
        {
            std::stringstream strm;
            strm << "Variable " << name << " does not exist";
            throw std::exception (strm.str().c_str());
        }
        machine.stack.Push(pData->Clone());
    }

    void LoadVariable::Dump(std::ostream& strm)
    {
        strm << "load: " << name << std::endl;
    }

    /*****************************************************/
    StoreVariable::StoreVariable(const std::string& s)
        :name(s)
    {}

    void StoreVariable::Execute(Machine& machine)
    {
        Data *pData = machine.stack.Pop();
        if (pData == nullptr)
        {
            throw std::exception("stack underflow");
        }
        machine.StoreVariable(name, pData);
    }

    void StoreVariable::Dump(std::ostream& strm)
    {
        strm << "store: " << name << std::endl;
    }

    /*****************************************************/
    Push::Push(int n)
    {
        pData = new Int(n);
    }

    Push::Push(const std::string& s)
    {
        pData = new String(s);
    }

    Push::Push(Data *p)
    {
        pData = p;
    }

    Push::~Push()
    {
        delete pData;
    }

    void Push::Execute(Machine& machine)
    {
        if (pData->type == Data::VARIABLE)
        {
            const Data *p = ResolveVariable(machine, pData);
        }
        machine.stack.Push(pData->Clone());
    }

    void Push::Dump(std::ostream& strm)
    {
        strm << "push: ";
		pData->Dump(strm);
		strm << std::endl;
    }

    /*****************************************************/
    void Pop::Execute(Machine& machine)
    {
        Data *pData = machine.stack.Pop();
        if (pData == nullptr)
        {
            throw std::exception("stack underflow");
        }
        delete pData;
    }

    void Pop::Dump(std::ostream& strm)
    {
        strm << "pop " << std::endl;
    }

    /*****************************************************/
    void Dup::Execute(Machine& machine)
    {
        Data *pData = machine.stack.Peek(0);
        if (pData == nullptr)
        {
            throw std::exception("stack underflow");
        }
        Data *pNew = pData->Clone();
        machine.stack.Push(pNew);
    }

    void Dup::Dump(std::ostream& strm)
    {
        strm << "dup " << std::endl;
    }

    /*****************************************************/
    void Swap::Execute(Machine& machine)
    {
        Data *pData1 = machine.stack.Pop();
        if (pData1 == nullptr)
        {
            throw std::exception("stack underflow");
        }
        Data *pData2 = machine.stack.Pop();
        if (pData2 == nullptr)
        {
            throw std::exception("stack underflow");
        }
        machine.stack.Push(pData1);
        machine.stack.Push(pData2);
    }

    void Swap::Dump(std::ostream& strm)
    {
        strm << "swap " << std::endl;
    }
}
