#pragma once
#include <string>
#include <vector>

//Acts as an interface between the core CSV file and the rest of the program
class FileInterface
{
	//Stores a list of the headings read from the CSV file
	std::vector<std::string> fieldHeadings;
	//Stores all of the data that was in the CSV file
	std::vector<std::string> newData;
	//Stores the number of attributes
	int fieldNum;

public:
	FileInterface();
	~FileInterface();

	//takes in the file path of a CSV file and loads that CSV file's data in
	void LoadCSV(std::wstring filePath);
	//returns the data found at a chosen row and column
	std::string GetDataPoint(int row, int column) const;
	//returns all the data that is within a single attribute
	std::vector<std::string> GetAttributeData(int column) const;
	//returns all the data
	std::vector<std::string> GetAllData() const;
	//returns the name of a chosen heading number
	std::string GetHeading(int colNum);
	//returns all the heading names
	std::vector<std::string> GetHeadings();
	//returns the size of the data
	int GetSize() const;
	//returns the number of rows of data
	int GetTupleNum() const;
	//returns the number of attributes
	inline int GetFieldNum() const{ return fieldNum; }

	//outputs all the stored data into the debug window
	void PrintData() const;	
};

