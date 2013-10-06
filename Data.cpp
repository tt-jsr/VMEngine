#include "stdafx.h"
#include "Data.h"

namespace vm
{
    DataObj::DataObj(Type t)
        :type(t)
    {}

    Data DataObj::CreateBool(bool b)
    {
        int n = b ? 1 : 0;
        return Data(new Int(n));
    }

    Data DataObj::CreateInt(int n)
    {
        return Data(new Int(n));
    }

    Data DataObj::CreateString(const std::string& s)
    {
        return Data(new String(s));
    }

    Data DataObj::CreateVariable(const std::string& s)
    {
        return Data(new Variable(s));
    }

    Data DataObj::CreateArray(const std::vector<Data>& d)
    {
        return Data(new Array(d));
    }

    Data DataObj::CreateField(const std::string& d)
    {
        return Data(new Field(d));
    }

    bool DataObj::GetInt(int& n)
    {
        if (type != DataObj::INT)
        {
            return false;
        }
        n = ((Int *)this)->n;
        return true;
    }

    bool DataObj::GetString(std::string& s)
    {
        if (type != DataObj::STRING)
        {
            return false;
        }
        s = ((String *)this)->str;
        return true;
    }

    bool DataObj::GetVariable(std::string& s)
    {
        if (type != DataObj::VARIABLE)
        {
            return false;
        }
        s = ((Variable *)this)->name;
        return true;
    }

    bool DataObj::GetArray(std::vector<Data>& vec)
    {
        if (type != DataObj::ARRAY)
        {
            return false;
        }
        vec = ((Array *)this)->items;
        return true;
    }

    bool DataObj::GetField(std::string& tag)
    {
        if (type != DataObj::FIELD)
        {
            return false;
        }
        tag = ((Field *)this)->tag;
        return true;
    }
    /**********************************************************/

    String::String()
    :DataObj(DataObj::STRING)
    {}

    String::String(std::string s)
    :DataObj(DataObj::STRING)
     , str(s)
    {}

    Data String::Clone()
    {
        return Data(new String(str));
    }

    std::ostream& String::Dump(std::ostream& strm)
    {
        strm << "string: " << str;
        return strm;
    }

    /**********************************************************/

    Int::Int()
    :DataObj(DataObj::INT)
     , n(0)
    {}

    Int::Int(int n_)
    :DataObj(DataObj::INT)
     , n(n_)
    {}


    Data Int::Clone()
    {
        return Data(new Int(n));
    }

    std::ostream& Int::Dump(std::ostream& strm)
    {
        strm << "int: " << n;
        return strm;
    }


    /**********************************************************/
    Variable::Variable()
    :DataObj(DataObj::VARIABLE)
    {}

    Variable::Variable(const std::string& s)
    :DataObj(DataObj::VARIABLE)
     , name(s)
    {}


    Data Variable::Clone()
    {
        return Data(new Variable(name));
    }

    std::ostream& Variable::Dump(std::ostream& strm)
    {
        strm << "variable: " << name;
        return strm;
    }

    /**********************************************************/
    Array::Array()
        :DataObj(DataObj::ARRAY)
    {}

    Array::Array(const std::vector<Data>& v)
        :DataObj(DataObj::ARRAY)
         , items(v)
    {}

    Data Array::Clone()
    {
        return Data(new Array(items));
    }

    std::ostream& Array::Dump(std::ostream& strm)
    {
        strm << "Array";
        return strm;
    }

    /**********************************************************/
    Field::Field()
    :DataObj(DataObj::FIELD)
    {}

    Field::Field(const std::string& t)
    :DataObj(DataObj::FIELD)
     , tag(t)
    {}


    Data Field::Clone()
    {
        return Data(new Field(tag));
    }

    std::ostream& Field::Dump(std::ostream& strm)
    {
        strm << "field: " << tag;
        return strm;
    }
}

