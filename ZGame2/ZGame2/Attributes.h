#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "Components.h"

typedef unsigned int Options;

// Attributes managment,
class Attributes
{
public:
	// Holds all the information of the options
	Options Options_Holder = 0;

	// Methods of this Class
	//-----------------------------
	// Options Bit Operations
	// Turn option on or off
	void Turn_Opt_On(Options option);  // This takes in one of the defined consts bit that correspond to the Components
	// Turn Option option off
	void Turn_Opt_Off(Options option);
	// switch on or off for position
	void Switch_Opt(Options option);

	// Check if options are on or off
	bool Is_Opt_On(Options option) const;

};
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

#endif