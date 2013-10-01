#pragma once

namespace vm
{
    class Machine;

    class Function
	{
	public:
        virtual void OnExecute(Machine& machine) = 0;
	};
}