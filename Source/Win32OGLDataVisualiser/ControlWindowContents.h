#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include <string.h>
#include <vector>
#include <atlstr.h>

#include "Menu.h"
#include "GraphicsBase.h"

//holds the various buttons, labels, etc, that are found in the controls window
class ControlWindowContents
{
	//stores a pointer to the control windows
	HWND* primaryWindow;
	
	//stores one button for each type of graph
	HWND pieButton;
	HWND barButton;
	HWND scat2DButton;
	HWND scat3DButton;
	HWND lineButton;
	HWND spiderButton;

	//stores a list of comboboxes that can be used to select attributes
	std::vector<HWND> attributeSelectors;
	//stores a list of the previous item that each combobox had selected
	std::vector<int> previousItems;
	//stores one label for each attribute combobox to identify it
	std::vector<HWND> attributeLabels;

	//displays the details of the currently selected data
	HWND selectedDetails;
	
public:
	ControlWindowContents();
	ControlWindowContents(HWND* controlWindow);
	~ControlWindowContents();

	//update the contents of each attribute combobox to contain all the headings provided
	void SetAttributes(std::vector<std::string> newHeadings);

	//update the contents of the selectedDetails display
	void UpdateSelectedData(std::string newData);

	//update the controls window based on which graph has been selected
	void UpdateForGraph(int graphType);
	void UpdateForPie();
	void UpdateForBar();
	void UpdateForScat2D();
	void UpdateForScat3D();
	void UpdateForLine();
	void UpdateForSpider();

	//return the selected item within the 
	int GetChosenItem(int chosenBox);
	//return the number of the box whose selected data has changed recently
	int GetChangedBox();
private:
	//create the control buttons
	void CreateDisplayButtons();
	//create the control comboboxes
	void CreateAttributeSelectors(int numberOfAttributes);
	//create the detail display label
	void CreateDetailDisplay();
	//set all attribute selector lists (combobox, label, and previous values)
	void EmptyAttributeSelectors();

	//reset all selected values to -1
	void ResetAttributeSelectors();
};

