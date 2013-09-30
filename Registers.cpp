#include "stdafx.h"
#include "Registers.h"

namespace vm
{
    Registers::Registers(void)
    : ip(0)
      ,eq(false)
      ,gt(false)
      ,lt(false)
      ,halt(false)
    {
    }


    Registers::~Registers(void)
    {
    }

    void Registers::Dump(std::ostream& strm)
    {
        strm << "ip: " << ip << std::endl;
        strm << "eq: " << eq << std::endl;
        strm << "lt: " << lt << std::endl;
        strm << "gt: " << gt << std::endl;
        strm << "bp: " << bp << std::endl;
        strm << "halt: " << halt << std::endl;
    }

    void Registers::Restart()
    {
        ClearFlags();
        halt = false;
        ip = 0;
        bp = 0;
    }

    void Registers::ClearFlags()
    {
        eq = false;
        gt = false;
        lt = false;
    }

    void Registers::SetHalt()
    {
        halt = true;
    }

    bool Registers::IsHalted()
    {
        return halt;
    }

    void Registers::SetEQ() 
    {
        eq = true;
        lt = false;
        gt = false;
    }

    void Registers::UnsetEQ() 
    {
        eq = false;
    }

    bool Registers::IsEQ() 
    {
        return eq;
    }

    void Registers::SetLT() 
    {
        lt = true;
        gt = false;
        eq = false;
    }

    void Registers::UnsetLT() 
    {
        lt = false;
    }

    bool Registers::IsLT() 
    {
        return lt;
    }

    void Registers::SetGT() 
    {
        gt = true;
        lt = false;
        eq = false;
    }

    void Registers::UnsetGT() 
    {
        gt = false;
    }

    bool Registers::IsGT() 
    {
        return gt;
    }
}
