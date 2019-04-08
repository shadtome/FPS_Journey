#ifndef IENTITY_H
#define IENTITY_H



class Entity_Manager;

// Entities stuff
//This is the Unique ID for a Entity
class IEntity
{
	friend Entity_Manager;

public:
	unsigned int ID = 0; // This has 32 bits for use in bitmasking
	//Name ID
	unsigned int Name_ID = 0;
	// Position ID
	unsigned int Pos_ID = 0; // For these It might be best to try to use a good has function (use a prime number)
	//Velocity ID
	unsigned int Vel_ID = 0;
	// Model ID
	unsigned int Model_ID = 0;
	// Collision ID
	unsigned int Col_ID = 0;

private:
	//Attributes of this Entity
	Attributes attributes;
public:
	//Default Constructor
	IEntity() :attributes(0) {};
	//Ordinatey constructor
	IEntity(Options options)
	{
		attributes.Switch_Opt(options);
	}
};


#endif
