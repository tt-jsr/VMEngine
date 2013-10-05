#include "stdafx.h"
#include "Assembler.h"
#include "Machine.h"
#include "Instructions.h"
#include "Data.h"
#include <iostream>

namespace
{
    struct ParseContext;
    void Throw(ParseContext& ctx, const char *msg);

    struct ParseContext
    {
        std::string line;
        int pos;
        int lineno;
        vm::Machine& machine;

        ParseContext(vm::Machine& mach)
			:pos(0)
            ,lineno(0)
            ,machine(mach)
		{}

        char Nextc()
        {
            if (line[pos] == '\0')
                return line[pos];
            ++pos;
            return line[pos];
        }

        bool IsQuoted() {
			return line[pos] == '\"';
        }

        bool IsDigit()
        {
            if (line[pos] >= '0' && line[pos] <= '9')
                return true;
            return false;
        }

        bool IsChar()
        {
            if (line[pos] == '\0' || line[pos] == ' ' || line[pos] == '\t')
                return false;
            return true;
        }

        bool IsWS()
        {
            if (line[pos] == ' ' || line[pos] == '\t')
                return true;
            return false;
        }

        char Getc() {return line[pos];}
    };

    void Throw(ParseContext& ctx, const char *msg)
    {
        std::stringstream strm;
        strm << msg << " at line " << ctx.lineno;
        throw std::exception(strm.str().c_str());
    }

    std::string CollectWord(ParseContext&);
    std::string CollectQuoted(ParseContext&);

    void SkipWS(ParseContext& ctx)
    {
        while (ctx.IsWS())
        {
            ctx.Nextc();
        }
    }

    bool CollectInteger(ParseContext& ctx, int& n)
    {
        SkipWS(ctx);
        if (ctx.IsDigit() == false)
        {
            return false;
        }

        std::string s = CollectWord(ctx);
        if (s.size() == 0)
        {
            return false;
        }

        n = strtol(s.c_str(), nullptr, 0);
        return true;
    }

    vm::Data CollectData(ParseContext& ctx)
    {
        SkipWS(ctx);
        if (ctx.IsQuoted())
        {
            std::string s = CollectQuoted(ctx);
            return vm::DataObj::Create(s);
        }
        if (ctx.IsDigit())
        {
            int n = 0;
            if (CollectInteger(ctx, n) == false)
            {
                return nullptr;
            }
            return vm::DataObj::Create(n);
        }
        std::string s = CollectWord(ctx);
        if (s == "true")
        {
            return vm::DataObj::Create(1);
        }
        if (s == "false")
        {
            return vm::DataObj::Create(0);
        }

        return vm::DataObj::CreateVariable(s);
    }


    std::string CollectLabel(ParseContext& ctx)
    {
        SkipWS(ctx);
        return CollectWord(ctx);
    }

    std::string CollectWord(ParseContext& ctx)
    {
        std::stringstream strm;
        SkipWS(ctx);
        while (ctx.IsChar())
        {
            strm << ctx.Getc();
            ctx.Nextc();
        }
        return strm.str();
    }

    std::string CollectQuoted(ParseContext& ctx)
    {
        std::stringstream strm;
        ctx.Nextc();  // consume the "
        while (ctx.Getc() != '\0' && ctx.Getc() != '"')
        {
            strm << ctx.Getc();
            ctx.Nextc();
        }
        ctx.Nextc(); // consume the "
        return strm.str();
    }

    vm::Push *ParsePush(ParseContext& ctx)
    {
        SkipWS(ctx);
        vm::Data p = CollectData(ctx);
        vm::Push *pInst = new vm::Push(p);
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Pop *ParsePop(ParseContext& ctx)
    {
        vm::Pop *pInst = new vm::Pop();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Test *ParseTest(ParseContext& ctx)
    {
        vm::Test *pInst = new vm::Test();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::TestIm *ParseTestIm(ParseContext& ctx)
    {
        SkipWS(ctx);
        vm::Data p = CollectData(ctx);
        vm::TestIm *pInst = new vm::TestIm(p);
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Except *ParseExcept(ParseContext& ctx)
    {
        SkipWS(ctx);
        std::string s = CollectQuoted(ctx);
        if (s.empty())
        {
            Throw(ctx, "Expected message");
        }
        vm::Except *pInst = new vm::Except();
        pInst->lineno = ctx.lineno;
        pInst->msg = s;
        return pInst;
    }

    vm::JumpEQ *ParseJumpEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::JumpEQ *pInst = new vm::JumpEQ();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::JumpNEQ *ParseJumpNEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::JumpNEQ *pInst = new vm::JumpNEQ();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::JumpGTEQ *ParseJumpGTEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::JumpGTEQ *pInst = new vm::JumpGTEQ();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::JumpGT *ParseJumpGT(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::JumpGT *pInst = new vm::JumpGT();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::JumpLTEQ *ParseJumpLTEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::JumpLTEQ *pInst = new vm::JumpLTEQ();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::JumpLT *ParseJumpLT(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::JumpLT *pInst = new vm::JumpLT();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Jump *ParseJump(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::Jump *pInst = new vm::Jump();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::ContEQ *ParseContEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::ContEQ *pInst = new vm::ContEQ();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::ContNEQ *ParseContNEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::ContNEQ *pInst = new vm::ContNEQ();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::ContGTEQ *ParseContGTEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::ContGTEQ *pInst = new vm::ContGTEQ();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::ContGT *ParseContGT(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::ContGT *pInst = new vm::ContGT();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::ContLTEQ *ParseContLTEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::ContLTEQ *pInst = new vm::ContLTEQ();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::ContLT *ParseContLT(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::ContLT *pInst = new vm::ContLT();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Halt *ParseHalt(ParseContext& ctx)
    {
        vm::Halt *pInst = new vm::Halt();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Break *ParseBreak(ParseContext& ctx)
    {
        vm::Break *pInst = new vm::Break();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::CallLibrary *ParseCallLib(ParseContext& ctx)
    {
        std::string s = CollectWord(ctx);
        if (s.empty())
        {
            Throw(ctx, "Expected library name");
        }
        vm::CallLibrary *pInst = new vm::CallLibrary();
        pInst->funcname = s;
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Call *ParseCall(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        if (targetName.size() == 0)
        {
            Throw(ctx, "Target name is empty");
        }
        vm::Call *pCall = new vm::Call();
        pCall->lineno = ctx.lineno;
        pCall->funcname = targetName;
        return pCall;
    }

    vm::Return *ParseReturn(ParseContext& ctx)
    {
        vm::Return *pInst = new vm::Return();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Inc *ParseInc(ParseContext& ctx)
    {
        vm::Inc *pInst = new vm::Inc();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Add *ParseAdd(ParseContext& ctx)
    {
        vm::Add *pInst = new vm::Add();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Subtract *ParseSubtract(ParseContext& ctx)
    {
        vm::Subtract *pInst = new vm::Subtract();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Multiply *ParseMultiply(ParseContext& ctx)
    {
        vm::Multiply *pInst = new vm::Multiply();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::IntDivide *ParseIntDivide(ParseContext& ctx)
    {
        vm::IntDivide *pInst = new vm::IntDivide();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Dec *ParseDec(ParseContext& ctx)
    {
        vm::Dec *pInst = new vm::Dec();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::And *ParseAnd(ParseContext& ctx)
    {
        vm::And *pInst = new vm::And();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Or *ParseOr(ParseContext& ctx)
    {
        vm::Or *pInst = new vm::Or();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::LoadVariable *ParseLoadVariable(ParseContext& ctx)
    {
        SkipWS(ctx);
        std::string s = CollectWord(ctx);
        vm::LoadVariable *pInst = new vm::LoadVariable(s);
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::StoreVariable *ParseStoreVariable(ParseContext& ctx)
    {
        SkipWS(ctx);
        std::string s = CollectWord(ctx);
        vm::StoreVariable *pInst = new vm::StoreVariable(s);
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Dup *ParseDup(ParseContext& ctx)
    {
        vm::Dup *pInst = new vm::Dup();
        pInst->lineno = ctx.lineno;
        return pInst;
    }

    vm::Swap *ParseSwap(ParseContext& ctx)
    {
        vm::Swap *pInst = new vm::Swap();
        pInst->lineno = ctx.lineno;
        return pInst;
    }
}

namespace vm
{
    Assembler::Assembler(void)
		:pMachine(nullptr)
    {
    }


    Assembler::~Assembler(void)
    {
    }


    Assembler::Label *Assembler::GetLabel(const std::string& name)
    {
        auto it = labels.find(name);
        if (it == labels.end())
        {
            return nullptr;
        }
        return &(it->second);
    }

    void Assembler::AddLabel(const std::string& name, int lineno)
    {
        auto it = labels.find(name);
        if (it == labels.end())
        {
            Label label;
            label.name = name;
            label.target = pMachine->code.Size();
            label.lineno = lineno;
            labels.insert(labels_t::value_type(name, label));
        }
        else
        {
            Label& label = it->second;
            label.target = pMachine->code.Size();
            label.lineno = lineno;
            for each(Instruction *pInst in label.instructions)
            {
                pInst->SetLabelTarget(label.target);
            }
        }
    }

    void Assembler::AddLabelTarget(const std::string& name, Instruction *pInst)
    {
        auto it = labels.find(name);
        if (it == labels.end())
        {
            Label label;
            label.name = name;
            label.instructions.push_back(pInst);
            labels.insert(labels_t::value_type(name, label));
        }
        else
        {
            Label& label = it->second;
            pInst->SetLabelTarget(label.target);
            label.instructions.push_back(pInst);
        }
    }

    void Assembler::Assemble(Machine& machine, std::istream& strm)
    {
        pMachine = &machine;

        ParseContext ctx(machine);

        while (strm.eof() == false)
		{
            std::string line;
            std::getline(strm, line);
            ++ctx.lineno;
            if (strm.eof())
            {
                break;
            }

            if (line.size() == 0)
            {
                continue;
            }
            ctx.line = line;
            ctx.pos = 0;
            SkipWS(ctx);
            if (ctx.Getc() == ';' || ctx.Getc() == '\0')
                continue;
            //std::cout << line << std::endl;
            std::string s = CollectWord(ctx);
            if (s[0] == ':') // label
            {
                std::string label = s.substr(1);
                Label *pLabel = GetLabel(label);
                if (pLabel && pLabel->lineno >= 0)
                {
                    std::stringstream strm;
                    strm << "Label " << label << " already defined at line " << pLabel->lineno;
                    Throw(ctx, strm.str().c_str());
                }
                AddLabel(label, ctx.lineno);
            }
            else if (s == "function")
            {
                std::string name = CollectWord(ctx);
                if (name.empty())
                {
                    Throw(ctx, "No function name");
                }
                machine.scriptfuncs.insert(Machine::scriptfuncs_t::value_type(name, machine.code.Size()));
                // This means that function names and labels share the same namespace
                Label *pLabel = GetLabel(name);
                if (pLabel && pLabel->lineno >= 0)
                {
                    std::stringstream strm;
                    strm << "Function " << name << " already defined at line " << pLabel->lineno;
                    Throw(ctx, strm.str().c_str());
                }
                AddLabel(name, ctx.lineno);
            }
            else if (s == "var")
            {
                std::string name = CollectWord(ctx);
                SkipWS(ctx);
                Data pData = nullptr;
                if (ctx.IsQuoted())
                {
                    std::string s = CollectQuoted(ctx);
                    pData = DataObj::Create(s);
                }
                else
                {
                    std::string s = CollectWord(ctx);
                    int n = strtol(s.c_str(), nullptr, 0);
                    pData = DataObj::Create(n);
                }
                machine.StoreGlobalVariable(name, pData);
            }

            else if (s == "test")
                machine.code.AddInstruction( ParseTest(ctx));
            else if (s == "testim")
                machine.code.AddInstruction( ParseTestIm(ctx));
            else if(s == "except")
            {
                machine.code.AddInstruction( ParseExcept(ctx) );
            }
            else if (s == "jumpe")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpEQ(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumpne")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpNEQ(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumpgte")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpGTEQ(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumpgt")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpGT(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumplte")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpLTEQ(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumplt")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpLT(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jump")
            {
                std::string targetName;
                Instruction *pInst = ParseJump(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "conte")
            {
                std::string targetName;
                Instruction *pInst = ParseContEQ(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contne")
            {
                std::string targetName;
                Instruction *pInst = ParseContNEQ(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contgte")
            {
                std::string targetName;
                Instruction *pInst = ParseContGTEQ(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contgt")
            {
                std::string targetName;
                Instruction *pInst = ParseContGT(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contlte")
            {
                std::string targetName;
                Instruction *pInst = ParseContLTEQ(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contlt")
            {
                std::string targetName;
                Instruction *pInst = ParseContLT(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "calllib")
            {
                Instruction *pInst = ParseCallLib(ctx);
                machine.code.AddInstruction(pInst);
            }
            else if (s == "call")
            {
                std::string targetName;
                Instruction *pInst = ParseCall(ctx, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "halt")
                machine.code.AddInstruction( ParseHalt(ctx));
            else if (s == "break")
                machine.code.AddInstruction( ParseBreak(ctx));
            else if (s == "return")
                machine.code.AddInstruction( ParseReturn(ctx));
            else if (s == "inc")
                machine.code.AddInstruction( ParseInc(ctx));
            else if (s == "dec")
                machine.code.AddInstruction( ParseDec(ctx));
            else if (s == "add")
                machine.code.AddInstruction( ParseAdd(ctx));
            else if (s == "sub")
                machine.code.AddInstruction( ParseSubtract(ctx));
            else if (s == "mult")
                machine.code.AddInstruction( ParseMultiply(ctx));
            else if (s == "idiv")
                machine.code.AddInstruction( ParseIntDivide(ctx));
            else if (s == "or")
                machine.code.AddInstruction( ParseOr(ctx));
            else if (s == "and")
                machine.code.AddInstruction( ParseAnd(ctx));
            else if (s == "load")
                machine.code.AddInstruction( ParseLoadVariable(ctx));
            else if (s == "store")
                machine.code.AddInstruction( ParseStoreVariable(ctx));
            else if (s == "push")
                machine.code.AddInstruction( ParsePush(ctx));
            else if (s == "pop")
                machine.code.AddInstruction( ParsePop(ctx));
            else if (s == "dup")
                machine.code.AddInstruction( ParseDup(ctx));
            else if (s == "swap")
                machine.code.AddInstruction( ParseSwap(ctx));
			else
			{
                std::stringstream strm;
                strm << "Unknown instruction " << s << " at line " << ctx.lineno;
                throw std::exception (strm.str().c_str());
			}
		}
        auto it = labels.begin();
        for (; it != labels.end(); ++it)
        {
            Label &Label = it->second;
            if (Label.lineno < 0)
            {
                std::stringstream strm;
                strm << "Label " << Label.name << " is not defined";
                throw std::exception (strm.str().c_str());
            }
        }
    }
}
