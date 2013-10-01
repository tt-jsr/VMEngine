#pragma once

namespace vm
{
    class Machine;

    class Function
	{
	public:
        enum ArgType {INVALID, INT, STRING};
        virtual void OnExecute() = 0;

        int numargs() const {return (int)args.size();}
        ArgType GetType(int idx) const 
        {
            switch (args[idx]->type)
            {
                case Data::INT:
                    return Function::INT;
                case Data::STRING:
                    return Function::STRING;
            }
            return Function::INVALID;
        }
        
        int GetInt(int idx) const
        {
            return ((Int *)args[idx])->n;
        }

        std::string GetString(int idx) const
        {
            return ((String *)args[idx])->str;
        }
    public:
        std::vector<Data *> args;
        Machine *pMachine;
	};
}
