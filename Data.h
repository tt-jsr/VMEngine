#pragma once

namespace vm
{
    class Data
    {
	public:
        enum Type {STRING, INT};
        Data(Type t);
        Type type;

        virtual Data *Clone() = 0;
		virtual void Dump(std::ostream&) = 0;
        static Data *Create(int n);
        static Data *Create(const std::string&);
    };

    class String : public Data
    {
    public:
        String(std::string s);
        std::string str;

        Data *Clone();
        void Dump(std::ostream& strm);
    };

    class Int : public Data
    {
    public:
        Int(int n);
        int n;

        Data *Clone();
        void Dump(std::ostream& strm);
    };
}

