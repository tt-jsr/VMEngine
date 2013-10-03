#pragma once

namespace vm
{
    class Machine;
    class Instruction;

    class Assembler
    {
    public:
        Assembler(void);
        ~Assembler(void);

        void Assemble(Machine& machine, std::istream& strm);
    private:
        struct Label
        {
			Label() : target(-1), lineno(-1) {}
            int target;
            std::vector<Instruction *> instructions;
            std::string name;
            int lineno;
        };

        typedef std::map<std::string, Label> labels_t;
        labels_t labels;
        Machine *pMachine;
    private:
        void AddLabel(const std::string& name, int lineno);
        void AddLabelTarget(const std::string& name, Instruction *);
        Label *GetLabel(const std::string&);
    };
}

