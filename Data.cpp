#include "stdafx.h"
#include "Data.h"

namespace vm
{
    DataObj::DataObj(Type t)
        :type(t)
    {}

    Data DataObj::Create(bool b)
    {
        int n = b ? 1 : 0;
        return Data(new Int(n));
    }

    Data DataObj::Create(int n)
    {
        return Data(new Int(n));
    }

    Data DataObj::Create(const std::string& s)
    {
        return Data(new String(s));
    }

    Data DataObj::CreateVariable(const std::string& s)
    {
        return Data(new Variable(s));
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

    void String::Dump(std::ostream& strm)
    {
        strm << "string: " << str;
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

    void Int::Dump(std::ostream& strm)
    {
        strm << "int: " << n;
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

    void Variable::Dump(std::ostream& strm)
    {
        strm << "variable: " << name;
    }
}

