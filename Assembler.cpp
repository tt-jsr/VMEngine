#include "stdafx.h"
#include "Assembler.h"
#include "Machine.h"
#include "Instructions.h"
#include "Data.h"
#include <iostream>

namespace
{
    std::string CollectWord(const std::string& line, int& pos);
    std::string CollectQuoted(const std::string& line, int& pos);

    void SkipWS(const std::string& line, int& pos)
    {
        const char *p = line.c_str();
        while (p[pos] != '\0' && (p[pos] == ' ' || p[pos] == '\t'))
        {
            ++pos;
        }
    }

    vm::Data *CollectData(const std::string& line, int& pos)
    {
        SkipWS(line, pos);
        if (line[pos] == '\"')
        {
            std::string s = CollectQuoted(line, pos);
            return new vm::String(s);
        }
        std::string value = CollectWord(line, pos);
        if (value[0] >= '0' && value[0] <= '9')
        {
            int n = strtol(value.c_str(), nullptr, 10);
            return new vm::Int(n);
        }
        return new vm::Variable(value);
    }

    std::string CollectLabel(const std::string& line, int& pos)
    {
        SkipWS(line, pos);
        return CollectWord(line, pos);
    }

    std::string CollectWord(const std::string& line, int& pos)
    {
        std::stringstream strm;
        SkipWS(line, pos);
        const char *p = line.c_str();
        while (p[pos] != '\0' && p[pos] != ' ' && p[pos] != '\t')
        {
            strm << p[pos];
            ++pos;
        }
        return strm.str();
    }

    std::string CollectQuoted(const std::string& line, int& pos)
    {
        std::stringstream strm;
        const char *p = line.c_str();
        ++pos;  // consume the "
        while (p[pos] != '\0' && p[pos] != '"')
        {
            strm << p[pos];
            ++pos;
        }
        ++pos; // consume the "
        return strm.str();
    }

    vm::Push *ParsePush(const std::string& line, int& pos)
    {
        SkipWS(line, pos);
        vm::Data *p = CollectData(line, pos);
        return new vm::Push(p);
    }

    vm::Pop *ParsePop(const std::string& line, int& pos)
    {
        return new vm::Pop();
    }

    vm::Test *ParseTest(const std::string& line, int& pos)
    {
        return new vm::Test();
    }

    vm::TestIm *ParseTestIm(const std::string& line, int& pos)
    {
        SkipWS(line, pos);
        vm::Data *p = CollectData(line, pos);
        return new vm::TestIm(p);
    }

    vm::JumpEQ *ParseJumpEQ(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::JumpEQ();
    }

    vm::JumpNEQ *ParseJumpNEQ(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::JumpNEQ();
    }

    vm::JumpGTEQ *ParseJumpGTEQ(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::JumpGTEQ();
    }

    vm::JumpGT *ParseJumpGT(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::JumpGT();
    }

    vm::JumpLTEQ *ParseJumpLTEQ(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::JumpLTEQ();
    }

    vm::JumpLT *ParseJumpLT(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::JumpLT();
    }

    vm::Jump *ParseJump(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::Jump();
    }

    vm::ContEQ *ParseContEQ(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::ContEQ();
    }

    vm::ContNEQ *ParseContNEQ(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::ContNEQ();
    }

    vm::ContGTEQ *ParseContGTEQ(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::ContGTEQ();
    }

    vm::ContGT *ParseContGT(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::ContGT();
    }

    vm::ContLTEQ *ParseContLTEQ(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::ContLTEQ();
    }

    vm::ContLT *ParseContLT(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        return new vm::ContLT();
    }

    vm::Call *ParseCall(const std::string& line, int& pos, std::string& targetName)
    {
        SkipWS(line, pos);
        targetName = CollectLabel(line, pos);
        vm::Call *pCall = new vm::Call();
        pCall->funcname = targetName;
        return pCall;
    }

    vm::StoreBP *ParseStoreBP(const std::string& line, int& pos)
    {
        return new vm::StoreBP();
    }

    vm::Halt *ParseHalt(const std::string& line, int& pos)
    {
        return new vm::Halt();
    }

    vm::Break *ParseBreak(const std::string& line, int& pos)
    {
        return new vm::Break();
    }

    vm::Return *ParseReturn(const std::string& line, int& pos)
    {
        return new vm::Return();
    }

    vm::Inc *ParseInc(const std::string& line, int& pos)
    {
        return new vm::Inc();
    }

    vm::Add *ParseAdd(const std::string& line, int& pos)
    {
        return new vm::Add();
    }

    vm::Subtract *ParseSubtract(const std::string& line, int& pos)
    {
        return new vm::Subtract();
    }

    vm::Multiply *ParseMultiply(const std::string& line, int& pos)
    {
        return new vm::Multiply();
    }

    vm::IntDivide *ParseIntDivide(const std::string& line, int& pos)
    {
        return new vm::IntDivide();
    }

    vm::Dec *ParseDec(const std::string& line, int& pos)
    {
        return new vm::Dec();
    }

    vm::LoadVariable *ParseLoadVariable(const std::string& line, int& pos)
    {
        SkipWS(line, pos);
        std::string s = CollectWord(line, pos);
        return new vm::LoadVariable(s);
    }

    vm::StoreVariable *ParseStoreVariable(const std::string& line, int& pos)
    {
        SkipWS(line, pos);
        std::string s = CollectWord(line, pos);
        return new vm::StoreVariable(s);
    }

    vm::Dup *ParseDup(const std::string& line, int& pos)
    {
        return new vm::Dup();
    }

    vm::Swap *ParseSwap(const std::string& line, int& pos)
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

        while (strm.eof() == false)
		{
            std::string line;
            std::getline(strm, line);
            if (strm.eof())
            {
                return;
            }

            int pos = 0;
            SkipWS(line, pos);
            if (line[pos] == ';' || line[pos] == '\0')
                continue;
            //std::cout << line << std::endl;
            std::string s = CollectWord(line, pos);
            if (s[0] == ':') // label
            {
                std::string label = s.substr(1);
                AddLabel(label);
            }
            else if (s == "var")
            {
                std::string name = CollectWord(line, pos);
                SkipWS(line, pos);
                Data *pData = nullptr;
                if (line[pos] == '\"')
                {
                    std::string s = CollectQuoted(line, pos);
                    pData = new String(s);
                }
                else
                {
                    std::string s = CollectWord(line, pos);
                    int n = strtol(s.c_str(), nullptr, 10);
                    pData = new Int(n);
                }
                machine.StoreGlobalVariable(name, pData);
            }

            else if (s == "test")
                machine.code.AddInstruction( ParseTest(line, pos));
            else if (s == "testim")
                machine.code.AddInstruction( ParseTestIm(line, pos));
            else if (s == "jumpe")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpEQ(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumpne")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpNEQ(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumpgte")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpGTEQ(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumpgt")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpGT(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumplte")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpLTEQ(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jumplt")
            {
                std::string targetName;
                Instruction *pInst = ParseJumpLT(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "jump")
            {
                std::string targetName;
                Instruction *pInst = ParseJump(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "conte")
            {
                std::string targetName;
                Instruction *pInst = ParseContEQ(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contne")
            {
                std::string targetName;
                Instruction *pInst = ParseContNEQ(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contgte")
            {
                std::string targetName;
                Instruction *pInst = ParseContGTEQ(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contgt")
            {
                std::string targetName;
                Instruction *pInst = ParseContGT(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contlte")
            {
                std::string targetName;
                Instruction *pInst = ParseContLTEQ(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "contlt")
            {
                std::string targetName;
                Instruction *pInst = ParseContLT(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "call")
            {
                std::string targetName;
                Instruction *pInst = ParseCall(line, pos, targetName);
                AddLabelTarget(targetName, pInst);
                machine.code.AddInstruction( pInst );
            }
            else if (s == "storebp")
            {
                Instruction *pInst = ParseStoreBP(line, pos);
            }
            else if (s == "halt")
                machine.code.AddInstruction( ParseHalt(line, pos));
            else if (s == "break")
                machine.code.AddInstruction( ParseBreak(line, pos));
            else if (s == "return")
                machine.code.AddInstruction( ParseReturn(line, pos));
            else if (s == "inc")
                machine.code.AddInstruction( ParseInc(line, pos));
            else if (s == "dec")
                machine.code.AddInstruction( ParseDec(line, pos));
            else if (s == "add")
                machine.code.AddInstruction( ParseAdd(line, pos));
            else if (s == "sub")
                machine.code.AddInstruction( ParseSubtract(line, pos));
            else if (s == "mult")
                machine.code.AddInstruction( ParseMultiply(line, pos));
            else if (s == "idiv")
                machine.code.AddInstruction( ParseIntDivide(line, pos));
            else if (s == "load")
                machine.code.AddInstruction( ParseLoadVariable(line, pos));
            else if (s == "store")
                machine.code.AddInstruction( ParseStoreVariable(line, pos));
            else if (s == "push")
                machine.code.AddInstruction( ParsePush(line, pos));
            else if (s == "pop")
                machine.code.AddInstruction( ParsePop(line, pos));
            else if (s == "dup")
                machine.code.AddInstruction( ParseDup(line, pos));
            else if (s == "swap")
                machine.code.AddInstruction( ParseSwap(line, pos));
			else
			{
                std::stringstream strm;
                strm << "Unknown instruction " << s;
                throw std::exception (strm.str().c_str());
			}
		}
    }
}
