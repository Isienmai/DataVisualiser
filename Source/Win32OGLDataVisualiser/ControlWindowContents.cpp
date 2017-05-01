#include "ControlWindowContents.h"

ControlWindowContents::ControlWindowContents()
{
}

ControlWindowContents::ControlWindowContents(HWND* controlWindow)
{
	primaryWindow = controlWindow;
	CreateDisplayButtons();
	CreateAttributeSelectors(0);
	CreateDetailDisplay();
	UpdateForPie();
}


ControlWindowContents::~ControlWindowContents()
{
}

void ControlWindowContents::UpdateSelectedData(std::string newData)
{
	std::wstring tempString;
	tempString += CA2CT(newData.c_str());

	SetWindowText(selectedDetails, tempString.c_str());
}

void ControlWindowContents::UpdateForGraph(int graphType)
{
	switch (graphType)
	{
	case PieChart:
		UpdateForPie();
		break;
	case BarGraph:
		UpdateForBar();
		break;
	case ScatterPlot2D:
		UpdateForScat2D();
		break;
	case ScatterPlot3D:
		UpdateForScat3D();
		break;
	case LineGraph:
		UpdateForLine();
		break;
	case SpiderChart:
		UpdateForSpider();
		break;
	default:
		break;
	}
}

void ControlWindowContents::UpdateForPie()
{
	Button_Enable(pieButton, false);	
	Button_Enable(barButton, true);
	Button_Enable(scat2DButton, true);
	Button_Enable(scat3DButton, true);
	Button_Enable(lineButton, true);
	Button_Enable(spiderButton, true);

	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		if (i < 1)
		{
			ShowWindow(attributeSelectors[i], SW_SHOW);
			ShowWindow(attributeLabels[i], SW_SHOW);
		}
		else
		{
			ShowWindow(attributeSelectors[i], SW_HIDE);
			ShowWindow(attributeLabels[i], SW_HIDE);
		}		
	}

	ResetAttributeSelectors();
}

void ControlWindowContents::UpdateForBar()
{
	Button_Enable(pieButton, true);
	Button_Enable(barButton, false);
	Button_Enable(scat2DButton, true);
	Button_Enable(scat3DButton, true);
	Button_Enable(lineButton, true);
	Button_Enable(spiderButton, true);

	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		if (i < 1)
		{
			ShowWindow(attributeSelectors[i], SW_SHOW);
			ShowWindow(attributeLabels[i], SW_SHOW);
		}
		else
		{
			ShowWindow(attributeSelectors[i], SW_HIDE);
			ShowWindow(attributeLabels[i], SW_HIDE);
		}
	}

	ResetAttributeSelectors();
}

void ControlWindowContents::UpdateForScat2D()
{
	Button_Enable(pieButton, true);
	Button_Enable(barButton, true);
	Button_Enable(scat2DButton, false);
	Button_Enable(scat3DButton, true);
	Button_Enable(lineButton, true);
	Button_Enable(spiderButton, true);

	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		if (i < 2)
		{
			ShowWindow(attributeSelectors[i], SW_SHOW);
			ShowWindow(attributeLabels[i], SW_SHOW);
		}
		else
		{
			ShowWindow(attributeSelectors[i], SW_HIDE);
			ShowWindow(attributeLabels[i], SW_HIDE);
		}
	}

	ResetAttributeSelectors();
}

void ControlWindowContents::UpdateForScat3D()
{
	Button_Enable(pieButton, true);
	Button_Enable(barButton, true);
	Button_Enable(scat2DButton, true);
	Button_Enable(scat3DButton, false);
	Button_Enable(lineButton, true);
	Button_Enable(spiderButton, true);

	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		if (i < 3)
		{
			ShowWindow(attributeSelectors[i], SW_SHOW);
			ShowWindow(attributeLabels[i], SW_SHOW);
		}
		else
		{
			ShowWindow(attributeSelectors[i], SW_HIDE);
			ShowWindow(attributeLabels[i], SW_HIDE);
		}
	}

	ResetAttributeSelectors();
}

void ControlWindowContents::UpdateForLine()
{
	Button_Enable(pieButton, true);
	Button_Enable(barButton, true);
	Button_Enable(scat2DButton, true);
	Button_Enable(scat3DButton, true);
	Button_Enable(lineButton, false);
	Button_Enable(spiderButton, true);

	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		if (i < 3)
		{
			ShowWindow(attributeSelectors[i], SW_SHOW);
			ShowWindow(attributeLabels[i], SW_SHOW);
		}
		else
		{
			ShowWindow(attributeSelectors[i], SW_HIDE);
			ShowWindow(attributeLabels[i], SW_HIDE);
		}
	}

	ResetAttributeSelectors();
}

void ControlWindowContents::UpdateForSpider()
{
	Button_Enable(pieButton, true);
	Button_Enable(barButton, true);
	Button_Enable(scat2DButton, true);
	Button_Enable(scat3DButton, true);
	Button_Enable(lineButton, true);
	Button_Enable(spiderButton, false);

	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		if (i < 1)
		{
			ShowWindow(attributeSelectors[i], SW_SHOW);
			ShowWindow(attributeLabels[i], SW_SHOW);
		}
		else
		{
			ShowWindow(attributeSelectors[i], SW_HIDE);
			ShowWindow(attributeLabels[i], SW_HIDE);
		}
	}

	ResetAttributeSelectors();
}

void ControlWindowContents::CreateDisplayButtons()
{
	pieButton = CreateWindow(
		TEXT("BUTTON"), 
		TEXT("Pie Chart"), 
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		10, 10, 
		130, 30, 
		*primaryWindow, 
		(HMENU)ID_GRAPH_PIE, 
		NULL, 
		NULL);

	barButton = CreateWindow(
		TEXT("BUTTON"),
		TEXT("Bar Graph"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		150, 10,
		130, 30,
		*primaryWindow,
		(HMENU)ID_GRAPH_BAR,
		NULL,
		NULL);

	scat2DButton = CreateWindow(
		TEXT("BUTTON"),
		TEXT("Scatter Plot 2D"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		290, 10,
		130, 30,
		*primaryWindow,
		(HMENU)ID_GRAPH_SCAT2D,
		NULL,
		NULL);

	scat3DButton = CreateWindow(
		TEXT("BUTTON"),
		TEXT("Scatter Plot 3D"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		430, 10,
		130, 30,
		*primaryWindow,
		(HMENU)ID_GRAPH_SCAT3D,
		NULL,
		NULL);

	lineButton = CreateWindow(
		TEXT("BUTTON"),
		TEXT("Line"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		570, 10,
		130, 30,
		*primaryWindow,
		(HMENU)ID_GRAPH_LINE,
		NULL,
		NULL);

	spiderButton = CreateWindow(
		TEXT("BUTTON"),
		TEXT("Spider"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		710, 10,
		130, 30,
		*primaryWindow,
		(HMENU)ID_GRAPH_SPIDER,
		NULL,
		NULL);
}

void ControlWindowContents::CreateAttributeSelectors(int numberOfAttributes)
{
	void EmptyAttributeSelectors();
	HWND tempSelector;
	HWND tempLabel;
	int tempLastItem;

	int xPos, yPos;

	for (int i = 0; i < numberOfAttributes; i++)
	{
		xPos = 10;
		yPos = 50 + (30 * i);

		if (yPos > 450)
		{
			xPos = 310;
			yPos = 50 + (30 * (i-14));
		}

		//create the label
		tempLabel = CreateWindow(
			TEXT("STATIC"),
			TEXT(""),
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			xPos, yPos,
			200, 28,
			*primaryWindow,
			NULL,
			NULL,
			NULL);

		SetWindowText(tempLabel, TEXT("Attribute:"));

		//create the selector
		tempSelector = CreateWindow(
			TEXT("COMBOBOX"),
			TEXT(""),
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			xPos + 100, yPos,
			200, 500,
			*primaryWindow,
			NULL,
			NULL,
			NULL);

		SendMessage(tempSelector, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("All"));

		//create the previous item
		tempLastItem = ComboBox_GetCurSel(tempSelector);

		//update the lists
		attributeSelectors.push_back(tempSelector);
		previousItems.push_back(tempLastItem);
		attributeLabels.push_back(tempLabel);
	}
}

void ControlWindowContents::CreateDetailDisplay()
{
	//create the detail display
	selectedDetails = CreateWindow(
		TEXT("STATIC"),
		TEXT(""),
		WS_CHILD | WS_TABSTOP | WS_VISIBLE,
		540, 270,
		400, 60,
		*primaryWindow,
		NULL,
		NULL,
		NULL);

	SetWindowText(selectedDetails, TEXT(""));
}

void ControlWindowContents::EmptyAttributeSelectors()
{
	attributeSelectors.erase(attributeSelectors.begin(), attributeSelectors.end());
	previousItems.erase(previousItems.begin(), previousItems.end());
	attributeLabels.erase(attributeLabels.begin(), attributeLabels.end());
}

void ControlWindowContents::SetAttributes(std::vector<std::string> newHeadings)
{
	CreateAttributeSelectors(newHeadings.size());
	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		for (int j = ComboBox_GetCount(attributeSelectors[i]) - 1; j > 0; j--)
		{
			ComboBox_DeleteString(attributeSelectors[i], j);
		}
	}

	std::wstring tempHeader;	

	for each (std::string heading in newHeadings)
	{
		tempHeader = CA2CT(heading.c_str());
		for (int i = 0; i < attributeSelectors.size(); i++)
		{
			SendMessage(attributeSelectors[i], (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)((LPCSTR)tempHeader.c_str()));
		}
	}
}

int ControlWindowContents::GetChosenItem(int chosenBox)
{
	int toReturn = 0;

	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		if (chosenBox == i + 1)
		{
			toReturn = ComboBox_GetCurSel(attributeSelectors[i]);
			if (toReturn == CB_ERR)
			{
				toReturn = 0;
			}
			i = attributeSelectors.size();
		}
	}

	return toReturn;
}

int ControlWindowContents::GetChangedBox()
{
	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		if (ComboBox_GetCurSel(attributeSelectors[i]) != previousItems[i])
		{
			previousItems[i] = ComboBox_GetCurSel(attributeSelectors[i]);
			return i + 1;
		}
	}
	return 0;
}

void ControlWindowContents::ResetAttributeSelectors()
{
	for (int i = 0; i < attributeSelectors.size(); i++)
	{
		ComboBox_SetCurSel(attributeSelectors[i], -1);
		previousItems[i] = -1;
	}
}
