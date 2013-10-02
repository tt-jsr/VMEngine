#pragma once

namespace vm
{
    class Machine;

    class Function
	{
	public:
        enum ArgType {INVALID, INT, STRING};
        virtual void OnExecute() = 0;

        void AddReturn(Data p) {ret.push_back(p);}
        int numargs() const {return (int)args.size();}
        ArgType GetType(int idx) const 
        {
            switch (args[idx]->type)
            {
                case DataObj::INT:
                    return Function::INT;
                case DataObj::STRING:
                    return Function::STRING;
            }
            return Function::INVALID;
        }
        
        int GetInt(int idx) const
        {
            return args[idx]->GetInt()->n;
        }

        std::string GetString(int idx) const
        {
            return args[idx]->GetString()->str;
        }
    public:
        std::vector<Data > args;
        std::vector<Data > ret;
        Machine *pMachine;
	};
}
