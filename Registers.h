#pragma once

namespace vm
{
    class Registers
    {
    public:
        Registers(void);
        ~Registers(void);

        void Dump(std::ostream&);

        void Restart();
        void ClearFlags();

        void SetEQ();
        void UnsetEQ();
        bool IsEQ();

        void SetLT();
        void UnsetLT();
        bool IsLT();

        void SetGT();
        void UnsetGT();
        bool IsGT();

        void SetHalt();
        bool IsHalted();

        void IncIP() {++ip; ++totalInstructions;}
        void IP(int n) {ip = n;}
        int IP() const {return ip;}

        int NArgs() {return nargs;}
        void NArgs(int n) {nargs = n;}

		int TotalInstructions() {return totalInstructions;}
    private:
        int ip;
        bool lt;
        bool gt;
        bool eq;
        int nargs;
        bool halt;
        int totalInstructions;
    };
}
