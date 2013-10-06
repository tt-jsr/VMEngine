#pragma once
#include "Data.h"

namespace vm
{
    class Machine;

    class Instruction
    {
    public:
        Instruction();
        virtual ~Instruction();
        virtual void Execute(Machine&) = 0;
        virtual void SetLabelTarget(int) {}
        virtual std::ostream& Dump(std::ostream& strm) = 0;
    public:
        int lineno;
    };

    /********************************************/
    // Test and compare
    // Does not alter the stack
    class Test : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    // Does not alter the stack
    class TestIm : public Instruction
    {
	public:
        TestIm(int n);
        TestIm(const std::string&);
        TestIm(Data p);
        ~TestIm();
      
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        Data pData;
    };

    /*********************************************/
    // Control
    class Except : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);

        std::string msg;
    };

    class Halt : public Instruction
	{
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
	};

    class JumpEQ : public Instruction
    {
	public:
        JumpEQ();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class JumpNEQ : public Instruction
    {
	public:
        JumpNEQ();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class JumpGTEQ : public Instruction
    {
	public:
        JumpGTEQ();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class JumpGT : public Instruction
    {
	public:
        JumpGT();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class JumpLTEQ : public Instruction
    {
	public:
        JumpLTEQ();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class JumpLT : public Instruction
    {
	public:
        JumpLT();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class Jump : public Instruction
    {
	public:
        Jump();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class ContEQ : public Instruction
    {
	public:
        ContEQ();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class ContNEQ : public Instruction
    {
	public:
        ContNEQ();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class ContGTEQ : public Instruction
    {
	public:
        ContGTEQ();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class ContGT : public Instruction
    {
	public:
        ContGT();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class ContLTEQ : public Instruction
    {
	public:
        ContLTEQ();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class ContLT : public Instruction
    {
	public:
        ContLT();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ipTarget = n;}
        int ipTarget;
    };

    class CallLibrary : public Instruction
    {
	public:
        CallLibrary();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        std::string funcname;
    };

    class Call : public Instruction
    {
	public:
        Call();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        void SetLabelTarget(int n) {ip = n;}
        int ip;
        std::string funcname;
    };
        
    class Return : public Instruction
    {
	public:
        Return();
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    class Break : public Instruction
    {
    public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    /**********************************************************/
    // Arithmetic
    class Inc : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    class Dec : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    class Add : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    class Subtract : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    class Multiply : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    class IntDivide : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    /*********************************************************/
    // Logic
    class And : public Instruction
    {
    public:
        void Execute(Machine&);
        std::ostream& Dump(std::ostream&);
    };

    class Or : public Instruction
    {
    public:
        void Execute(Machine&);
        std::ostream& Dump(std::ostream&);
    };

    class Equal : public Instruction
    {
    public:
        void Execute(Machine&);
        std::ostream& Dump(std::ostream&);
    };

    class NotEqual : public Instruction
    {
    public:
        void Execute(Machine&);
        std::ostream& Dump(std::ostream&);
    };

    /*********************************************************/
    // Variables and data
    class LoadVariable : public Instruction
    {
	public:
        LoadVariable(const std::string& s);
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        std::string name;
    };

    class StoreVariable : public Instruction
    {
	public:
        StoreVariable(const std::string& s);
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        std::string name;
    };

    class Push : public Instruction
    {
	public:
        Push(int n);
        Push(const std::string&);
        Push(Data p);
        ~Push();

        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
        Data pData;
    };

    class Pop : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    class Dup : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };

    class Swap : public Instruction
    {
	public:
        void Execute(Machine& machine);
        std::ostream& Dump(std::ostream&);
    };
}

