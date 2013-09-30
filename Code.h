#pragma once

namespace vm
{
    class Instruction;

    class Code
    {
    public:
        Code(void);
        ~Code(void);

        void Dump(std::ostream&);

        int Size() {return (int)instructions.size();}
        int AddInstruction(Instruction *pInst);
        Instruction *Get(int idx);
    private:
        std::vector<Instruction *> instructions;
    };
}
