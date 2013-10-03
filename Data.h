#pragma once
#include <memory>

namespace vm
{
    class DataObj;
    class Int;
    class String;
    class Variable;

    typedef std::shared_ptr<DataObj> Data;

    class DataObj
    {
	public:
        enum Type {STRING, INT, VARIABLE};
        DataObj(Type t);
        Type type;

        virtual Data Clone() = 0;
		virtual void Dump(std::ostream&) = 0;
        static Data Create(bool b);
        static Data Create(int n);
        static Data Create(const std::string&);
        static Data CreateVariable(const std::string&);
        Int * GetInt() {return type==INT ? (Int *)this : nullptr;}
        String * GetString() {return type==STRING ? (String *)this : nullptr;}
        Variable * GetVariable() {return type==VARIABLE ? (Variable *)this : nullptr;}
        bool IsString() const {return type == STRING;}
        bool IsInt() const {return type == INT;}
        bool IsVariable() const {return type == VARIABLE;}
    };

    class String : public DataObj
    {
    public:
        String();
        String(std::string s);
        std::string str;

        Data Clone();
        void Dump(std::ostream& strm);
    };

    class Int : public DataObj
    {
    public:
        Int();
        Int(int n);
        int n;

        Data Clone();
        void Dump(std::ostream& strm);
    };

    class Variable : public DataObj
    {
    public:
        Variable();
        Variable(const std::string& name);
        std::string name;

        Data Clone();
        void Dump(std::ostream& strm);
    };

}

