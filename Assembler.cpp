#include "stdafx.h"
#include "Assembler.h"
#include "Machine.h"
#include "Instructions.h"
#include "Data.h"
#include <iostream>

namespace
{
    struct ParseContext
    {
        std::string line;
        int pos;
        int lineno;

        bool IsQuoted() {return line[pos] == '\"';}
        bool IsDigit()
        {
            if (line[0] >= '0' && line[0] <= '9')
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
            return !IsChar();
        }

        char GetChar() {return line[pos];}
    };

    std::string CollectWord(ParseContext&);
    std::string CollectQuoted(ParseContext&);

    void SkipWS(ParseContext& ctx)
    {
        while (ctx.IsWS())
        {
            ++ctx.pos;
        }
    }

    int CollectInteger(ParseContext& ctx)
    {
        SkipWS(ctx);
        std::string s = CollectWord(ctx);
        return strtol(s.c_str(), nullptr, 10);
    }

    vm::Data *CollectData(ParseContext& ctx)
    {
        SkipWS(ctx);
        if (ctx.IsQuoted())
        {
            std::string s = CollectQuoted(ctx);
            return new vm::String(s);
        }
        if (ctx.IsDigit())
        {
            int n = CollectInteger(ctx);
            return new vm::Int(n);
        }
        std::string s = CollectWord(ctx);
        return new vm::Variable(s);
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
            strm << ctx.GetChar();
            ++ctx.pos;
        }
        return strm.str();
    }

    std::string CollectQuoted(ParseContext& ctx)
    {
        std::stringstream strm;
        ++ctx.pos;  // consume the "
        while (ctx.GetChar() != '\0' && ctx.GetChar() != '"')
        {
            strm << ctx.GetChar();
            ++ctx.pos;
        }
        ++ctx.pos; // consume the "
        return strm.str();
    }

    vm::Push *ParsePush(ParseContext& ctx)
    {
        SkipWS(ctx);
        vm::Data *p = CollectData(ctx);
        return new vm::Push(p);
    }

    vm::Pop *ParsePop(ParseContext& ctx)
    {
        return new vm::Pop();
    }

    vm::Test *ParseTest(ParseContext& ctx)
    {
        return new vm::Test();
    }

    vm::TestIm *ParseTestIm(ParseContext& ctx)
    {
        SkipWS(ctx);
        vm::Data *p = CollectData(ctx);
        return new vm::TestIm(p);
    }

    vm::JumpEQ *ParseJumpEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::JumpEQ();
    }

    vm::JumpNEQ *ParseJumpNEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::JumpNEQ();
    }

    vm::JumpGTEQ *ParseJumpGTEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::JumpGTEQ();
    }

    vm::JumpGT *ParseJumpGT(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::JumpGT();
    }

    vm::JumpLTEQ *ParseJumpLTEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::JumpLTEQ();
    }

    vm::JumpLT *ParseJumpLT(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::JumpLT();
    }

    vm::Jump *ParseJump(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::Jump();
    }

    vm::ContEQ *ParseContEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::ContEQ();
    }

    vm::ContNEQ *ParseContNEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::ContNEQ();
    }

    vm::ContGTEQ *ParseContGTEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::ContGTEQ();
    }

    vm::ContGT *ParseContGT(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::ContGT();
    }

    vm::ContLTEQ *ParseContLTEQ(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::ContLTEQ();
    }

    vm::ContLT *ParseContLT(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        targetName = CollectLabel(ctx);
        return new vm::ContLT();
    }

    vm::Call *ParseCall(ParseContext& ctx, std::string& targetName)
    {
        SkipWS(ctx);
        int n = CollectInteger(ctx);
        targetName = CollectLabel(ctx);
        vm::Call *pCall = new vm::Call();
        pCall->funcname = targetName;
        pCall->nargs = n;
        return pCall;
    }

    vm::Halt *ParseHalt(ParseContext& ctx)
    {
        return new vm::Halt();
    }

    vm::Break *ParseBreak(ParseContext& ctx)
    {
        return new vm::Break();
    }

    vm::Return *ParseReturn(ParseContext& ctx)
    {
        return new vm::Return();
    }

    vm::Inc *ParseInc(ParseContext& ctx)
    {
        return new vm::Inc();
    }

    vm::Add *ParseAdd(ParseContext& ctx)
    {
        return new vm::Add();
    }

    vm::Subtract *ParseSubtract(ParseContext& ctx)
    {
        return new vm::Subtract();
    }

    vm::Multiply *ParseMultiply(ParseContext& ctx)
    {
        return new vm::Multiply();
    }

    vm::IntDivide *ParseIntDivide(ParseContext& ctx)
    {
        return new vm::IntDivide();
    }

    vm::Dec *ParseDec(ParseContext& ctx)
    {
        return new vm::Dec();
    }

    vm::LoadVariable *ParseLoadVariable(ParseContext& ctx)
    {
        SkipWS(ctx);
        std::string s = CollectWord(ctx);
        return new vm::LoadVariable(s);
    }

    vm::StoreVariable *ParseStoreVariable(ParseContext& ctx)
    {
        SkipWS(ctx);
        std::string s = CollectWord(ctx);
        return new vm::StoreVariable(s);
    }

    vm::Dup *ParseDup(ParseContext& ctx)
    {
        return new vm::Dup();
    }

    vm::Swap *ParseSwap(ParseContext& ctx)
    {
        return new vm::Swap();
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


    void Assembler::AddLabel(const std::string& name)
    {
        auto it = labels.find(name);
        if (it == labels.end())
        {
            Label label;
            label.name = name;
            label.target = pMachine->code.Size();
            labels.insert(labels_t::value_type(name, label));
        }
        else
        {
            Label& label = it->second;
            label.target = pMachine->code.Size();
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

        ParseContext ctx;

        while (strm.eof() == false)
		{
            std::string line;
            std::getline(strm, line);
            ++ctx.lineno;
            if (strm.eof())
            {
                return;
            }

            ctx.line = line;
            ctx.pos = 0;
            SkipWS(ctx);
            if (ctx.GetChar() == ';' || ctx.GetChar() == '\0')
                continue;
            //std::cout << line << std::endl;
            std::string s = CollectWord(ctx);
            if (s[0] == ':') // label
            {
                std::string label = s.substr(1);
                AddLabel(label);
            }
            else if (s == "var")
            {
                std::string name = CollectWord(ctx);
                SkipWS(ctx);
                Data *pData = nullptr;
                if (ctx.IsQuoted())
                {
                    std::string s = CollectQuoted(ctx);
                    pData = new String(s);
                }
                else
                {
                    std::string s = CollectWord(ctx);
                    int n = strtol(s.c_str(), nullptr, 10);
                    pData = new Int(n);
                }
                machine.StoreGlobalVariable(name, pData);
            }

            else if (s == "test")
                machine.code.AddInstruction( ParseTest(ctx));
            else if (s == "testim")
                machine.code.AddInstruction( ParseTestIm(ctx));
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
                strm << "Unknown instruction " << s;
                throw std::exception (strm.str().c_str());
			}
		}
    }
}
