
#include "Attributes.h"


//Attributes Methods
//--------------------------------
// Position bit operations
void Attributes::Turn_Opt_On(Options option)
{

	Options_Holder |= option;  // This is equivalent to Options = (Options | position)
};

void Attributes::Turn_Opt_Off(Options option)
{
	Options_Holder &= ~option; // This is equivalent to Options = (Options & ~position)
};

void Attributes::Switch_Opt(Options option)
{
	Options_Holder ^= option;
};

// Check if options are on or off
bool Attributes::Is_Opt_On(Options option) const
{

	if (Options_Holder & option)
	{

		//std::cout<< "Option" << " "<< log2(option)+1 <<" "<< "is on" << std::endl;
		return 1;
	}
	else
	{
		//std::cout << "Option" << " "<< log2(option)+1 <<" "<< "is off"<< std::endl;
		return 0;
	}
};