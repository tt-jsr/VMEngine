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

		int TotalInstructions() {return totalInstructions;}
    private:
        int ip;
        bool lt;
        bool gt;
        bool eq;
        bool halt;
        int totalInstructions;
    };
}
