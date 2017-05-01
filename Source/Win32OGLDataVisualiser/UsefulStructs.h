#pragma once
#include <string>

struct RGBColour
{
	float red;
	float green;
	float blue;
};

//stores a value, the quantity of that value and associate a colour with it
struct TallyValue
{
	int quantity;
	std::string content;
	RGBColour colour;	
};

//tallyvalue but with two values
struct DataPair
{
	int quantity;
	int contentID1, contentID2;
	RGBColour colour;
};

//tallyvalue but with three values
struct DataTrio
{
	int quantity;
	int contentID1, contentID2, contentID3;
	RGBColour colour;
};

struct Point
{
	float x;
	float y;
};