#pragma once
#include <windows.h>
#include <string>
#include <vector>

#include "FileInterface.h"

//Used to control access to the chosen CSV file
class Data
{
	//stores the fInterface class which holds the CSV information
	FileInterface fInterface;
	//stores a list of attributes chosen by the user to display
	std::vector<int> attributes;
	//stores the number of attributes the user has chosen
	int chosenAttNum;

public:
	Data();
	~Data();

	//resets all chosen attribute information to 0
	void ResetAttributeSelectors();
	//change one of the chosen attribute values to a user specified value
	void SetAttribute(int attributeNum, int newValue);
	//opens the load file dialogue and loads the chosen file into fInterface
	bool LoadCSV();
	//Get the names of all the headings of the current file
	inline std::vector<std::string> GetHeadings(){ return fInterface.GetHeadings(); }
	//Get the name of a selected heading of the current file
	inline std::string GetHeading(int chosenHeading) { return fInterface.GetHeading(chosenHeading); }
	//Returns the heading represented by a selected line
	std::string GetHeadingForLine(int chosenLine);

	//Returns the number of attributes whose data currently makes up the displayed graph
	int GetNumOfChosenAtts();

	//The following methods all returns data formatted for different types of display
	std::vector<std::string> GetPieFormat();
	std::vector<std::string> GetBarFormat();
	std::vector<std::string> GetScat2DFormat();
	std::vector<std::string> GetScat3DFormat();
	std::vector<std::string> GetLineFormat();
	std::vector<std::string> GetSpiderFormat();

private:
	//returns data in the standard format
	std::vector<std::string> GetFormat();

	//Create the load file dialogue and return the result
	std::wstring InitLpofn();	
	//Updates the chosenAttNum to accurately represent the number of chosen attributes
	void UpdateAttNum();
};

