#pragma once

namespace vm
{
    class Data;

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

        void IncIP() {++ip;}
        void IP(int n) {ip = n;}
        int IP() const {return ip;}

        void BP(int n) {bp = n;}
        int BP() const {return bp;}
    private:
        int ip;
        bool lt;
        bool gt;
        bool eq;
        bool bp;
        bool halt;
    };
}
