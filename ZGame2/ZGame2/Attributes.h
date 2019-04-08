#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H


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

	//Constructor
	Attributes(Options options)
	{
		Options_Holder = options;
	}
	//Default Constructor
	Attributes() {};
};


#endif