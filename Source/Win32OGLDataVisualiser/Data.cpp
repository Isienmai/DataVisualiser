#include "Data.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "Menu.h"

Data::Data()
{
}

Data::~Data()
{
}


//Set chosenAttNum to 0, empty the chosen attributes vector and then refill it with -1s
void Data::ResetAttributeSelectors()
{
	chosenAttNum = 0;
	attributes.erase(attributes.begin(), attributes.end());
	//the attributes vector always holds the same number of values as there are attributes.
	//This way it will be easier to expand the program to allow the user to select more than 3 attributes at once
	for (int i = 0; i < fInterface.GetFieldNum(); i++)
	{
		attributes.push_back(-1);
	}
}

void Data::SetAttribute(int attributeNum, int newValue)
{
	//the probided attribute num and new value are not 0 based, so they need to be converted
	attributeNum--;
	newValue--;

	//make sure the chosen attribute is valid
	if (attributeNum < attributes.size())
	{
		attributes[attributeNum] = newValue;
		UpdateAttNum();
	}	
}

bool Data::LoadCSV()
{
	std::wstring filePath = InitLpofn();
	//If the load dialogue returns a valid file then load that file in and reset the chosen attributes
	if (filePath != L"")
	{
		fInterface.LoadCSV(filePath);
		ResetAttributeSelectors();
		return true;
	}
	else
	{
		return false;
	}
}

std::string Data::GetHeadingForLine(int chosenLine)
{
	//get a list of chosen values, removing all placeholder -1s
	std::vector<int> tempAtts;
	for each (int index in attributes)
	{
		if (index != -1)
		{
			tempAtts.push_back(index);
		}
	}

	//If the user has not chosen any attributes then the chosen line number IS the attribute number
	//If the user has chosen an attribute then the line number is an index for the list of chosen attributes
	if (tempAtts.size() != 0)
	{
		return fInterface.GetHeading(tempAtts[chosenLine]);
	}
	else
	{
		return fInterface.GetHeading(chosenLine);
	}
}

int Data::GetNumOfChosenAtts()
{
	if (chosenAttNum == 0)
	{
		//if the user has not chosen any attributes then all the attributes are used to produce the data
		return fInterface.GetFieldNum();
	}
	else
	{
		return chosenAttNum;
	}
}

//All following methods just link to GetFormat() but if different graphs need slightly different formats later then these methods will facilitate this change
std::vector<std::string> Data::GetPieFormat()
{
	return GetFormat();
}

std::vector<std::string> Data::GetBarFormat()
{
	return GetFormat();
}

std::vector<std::string> Data::GetScat2DFormat()
{
	return GetFormat();
}

std::vector<std::string> Data::GetScat3DFormat()
{
	return GetFormat();
}

std::vector<std::string> Data::GetLineFormat()
{
	return GetFormat();
}

std::vector<std::string> Data::GetSpiderFormat()
{
	return GetFormat();
}

//Returns the data from all of the selected attributes placed end to end
std::vector<std::string> Data::GetFormat()
{
	std::vector<std::string> toReturn, temp;
	if (chosenAttNum != 0)
	{
		for (int i = 0; i < attributes.size(); i++)
		{
			if (attributes[i] != -1)
			{
				temp = fInterface.GetAttributeData(attributes[i]);
				toReturn.insert(toReturn.end(), temp.begin(), temp.end());
			}
		}
	}
	else
	{
		toReturn = fInterface.GetAllData();
	}	
	return toReturn;
}

std::wstring Data::InitLpofn()
{
	OPENFILENAME lpofn;
	WCHAR szFile[2560] = TEXT("");

	// Initialize OPENFILENAME
	ZeroMemory(&lpofn, sizeof(lpofn));
	lpofn.lStructSize = sizeof(lpofn);
	//lpofn.hwndOwner = hWnd;
	lpofn.lpstrFile = szFile;
	//lpofn.lpstrFile[0] = NULL;
	lpofn.nMaxFile = sizeof(szFile);
	lpofn.lpstrFilter = TEXT("CSV\0*.CSV\0");
	lpofn.nFilterIndex = 1;
	lpofn.lpstrFileTitle = NULL;
	lpofn.nMaxFileTitle = 0;
	lpofn.lpstrInitialDir = NULL;
	lpofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	//If a valid file was chosen then returns it's path, if not post an error in the debug window and return and empty string
	if (GetOpenFileName(&lpofn))
	{
		std::printf("\nGetOpenFileName returned true.\n");
		return lpofn.lpstrFile;
	}
	else
	{
		DWORD error = CommDlgExtendedError();
		std::printf("\nGetOpenFileName returned FALSE! FALSE! AT FOUR IN THE AFTERNOON! Error code: %ld\n", error);
		return L"";
	}
}

void Data::UpdateAttNum()
{
	int newAttNum = 0;
	//loop through the list of chosen attributes, counting the number of attributes that have been chosen
	for (int i = 0; i < attributes.size(); i++)
	{
		if (attributes[i] != -1)
		{
			newAttNum++;
		}
	}
	chosenAttNum = newAttNum;
}
