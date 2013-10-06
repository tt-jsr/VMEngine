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
        enum Type {STRING, INT, VARIABLE, ARRAY, FIELD};
        DataObj(Type t);
        Type type;

        virtual Data Clone() = 0;
		virtual std::ostream& Dump(std::ostream&) = 0;
        bool GetInt(int&);
        bool GetString(std::string&);
        bool GetVariable(std::string&);
        bool GetArray(std::vector<Data>&);
        bool GetField(std::string& tag);
        bool IsString() const {return type == STRING;}
        bool IsInt() const {return type == INT;}
        bool IsVariable() const {return type == VARIABLE;}
        bool IsArray() const {return type == ARRAY;}
        bool IsField() const {return type == FIELD;}
        static Data CreateBool(bool b);
        static Data CreateInt(int n);
        static Data CreateString(const std::string&);
        static Data CreateVariable(const std::string&);
        static Data CreateArray(const std::vector<Data>&);
        static Data CreateField(const std::string& tag);
    };

    class String : public DataObj
    {
    public:
        String();
        String(std::string s);
        std::string str;

        Data Clone();
        std::ostream& Dump(std::ostream& strm);
    };

    class Int : public DataObj
    {
    public:
        Int();
        Int(int n);
        int n;

        Data Clone();
        std::ostream& Dump(std::ostream& strm);
    };

    class Variable : public DataObj
    {
    public:
        Variable();
        Variable(const std::string& name);
        std::string name;

        Data Clone();
        std::ostream& Dump(std::ostream& strm);
    };

    class Array : public DataObj
    {
    public:
        Array();
        Array(const std::vector<Data>& v);
        Data Clone();
        std::ostream& Dump(std::ostream& strm);
        std::vector<Data> items;
    };

    class Field : public DataObj
    {
    public:
        Field();
        Field(const std::string& tag);
        Data Clone();
        std::ostream& Dump(std::ostream& strm);
        std::string tag;
    };
}

