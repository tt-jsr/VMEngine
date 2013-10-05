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
        enum Type {STRING, INT, VARIABLE, ARRAY};
        DataObj(Type t);
        Type type;

        virtual Data Clone() = 0;
		virtual void Dump(std::ostream&) = 0;
        bool GetInt(int&);
        bool GetString(std::string&);
        bool GetVariable(std::string&);
        bool GetArray(std::vector<Data>&);
        bool IsString() const {return type == STRING;}
        bool IsInt() const {return type == INT;}
        bool IsVariable() const {return type == VARIABLE;}
        bool IsArray() const {return type == ARRAY;}
        static Data Create(bool b);
        static Data Create(int n);
        static Data Create(const std::string&);
        static Data CreateVariable(const std::string&);
        static Data Create(const std::vector<Data>&);
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

    class Array : public DataObj
    {
    public:
        Array();
        Array(const std::vector<Data>& v);
        Data Clone();
        void Dump(std::ostream& strm);
        std::vector<Data> items;
    };
}

