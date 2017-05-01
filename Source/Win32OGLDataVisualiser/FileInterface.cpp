#include "FileInterface.h"
#include <cstdio>
#include <iostream>
#include <fstream>

FileInterface::FileInterface()
{
}


FileInterface::~FileInterface()
{
}

void FileInterface::LoadCSV(std::wstring filePath)
{
	newData.erase(newData.begin(), newData.end());
	fieldHeadings.erase(fieldHeadings.begin(), fieldHeadings.end());
	std::ifstream inputStream(filePath);
	std::cout << "File Opened\n\n";
	std::string line;
	std::string word;
	std::getline(inputStream, line, '\n');

	int count = 0;
	bool headings = true;
	fieldNum = 0;
	//while the line is valid, extract values from it and get the next line
	while (line.find_first_of(',') != std::string::npos)
	{
		count++;
		//while there is at least one value left extract it and remove it from the string
		while (line.find_first_of(',') != std::string::npos)
		{
			word = line.substr(0, line.find_first_of(','));
			if (headings)
			{
				fieldNum++;
				fieldHeadings.push_back(word);
			}
			else
			{
				newData.push_back(word);
			}
			line.erase(0, line.find_first_of(',') + 1);			
		}
		if (headings)
		{
			fieldNum++;
			fieldHeadings.push_back(line);
			headings = false;
		}
		else
		{
			newData.push_back(line);
		}
		//send a message to the debug window for every 1000 lines that have been read in
		if ((count % 1000) == 0) std::cout << "Line " << count << "Loaded\n";
		std::getline(inputStream, line, '\n');
	}
	inputStream.close();

	std::cout << "File Loaded\n\n";
}

std::string FileInterface::GetDataPoint(int row, int column)const
{
	if ((row*column < newData.size())&&(column < fieldNum))
	{
		return newData[row*fieldNum + column];
	}
	else return NULL;
}

std::vector<std::string> FileInterface::GetAttributeData(int column)const
{
	std::vector<std::string> toReturn;
	for (int i = 0; i < GetTupleNum(); i++)
	{
		toReturn.push_back(GetDataPoint(i, column));
	}
	return toReturn;
}

std::vector<std::string> FileInterface::GetAllData()const
{
	std::vector<std::string> toReturn, tempData;

	for (int i = 0; i < fieldNum; i++)
	{
		tempData = GetAttributeData(i);
		toReturn.insert(toReturn.end(), tempData.begin(), tempData.end());
	}
	return toReturn;
}

std::string FileInterface::GetHeading(int colNum)
{
	return fieldHeadings[colNum];
}

std::vector<std::string> FileInterface::GetHeadings()
{
	return fieldHeadings;
}

int FileInterface::GetSize() const
{
	return newData.size();
}

int FileInterface::GetTupleNum() const
{
	return newData.size() / fieldNum;
}

void FileInterface::PrintData() const
{
	for (int i = 0; i < newData.size() / fieldNum; i++)
	{
		for (int j = 0; j < fieldNum; j++)
		{
			std::cout << newData[i * fieldNum + j] << ", ";
		}
		std::printf("\n");
	}
}

