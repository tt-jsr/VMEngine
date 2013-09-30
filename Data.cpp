#include "stdafx.h"
#include "Data.h"

namespace vm
{
    Data::Data(Type t)
        :type(t)
    {}

    String::String(std::string s)
    :Data(Data::STRING)
     , str(s)
    {}

    Data *String::Clone()
    {
        return new String(str);
    }

    void String::Dump(std::ostream& strm)
    {
        strm << "string: " << str;
    }

    Int::Int(int n_)
    :Data(Data::INT)
     , n(n_)
    {}


    Data *Int::Clone()
    {
        return new Int(n);
    }

    void Int::Dump(std::ostream& strm)
    {
        strm << "int: " << n;
    }

    Data *Data::Create(int n)
    {
        return new Int(n);
    }

    Data *Data::Create(const std::string& s)
    {
        return new String(s);
    }
}

