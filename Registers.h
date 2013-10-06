#pragma once

namespace vm
{
    class FixMessage // Place holder
	{
    public:
		std::string GetFieldValue(const std::string& tag) 
        {
            if (tag == "35")
                return "8";
            if (tag == "55")
                return "6E";
            if (tag == "200")
                return "CME";
            if (tag == "150")
                return "1";
            return "";
        }
    };

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

        FixMessage input;
        FixMessage output;
    private:
        int ip;
        bool lt;
        bool gt;
        bool eq;
        bool halt;
        int totalInstructions;
    };
}
