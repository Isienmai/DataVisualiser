#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "GLFont.h"

using namespace std;

//wrapper for GLFont
class GUIText
{
private:
	//store a list of all the pieces of text to display
	vector<string> textToDisplay;
	//store a GLFont
	GLFont* font1;
	//store the coordinates where the text will be displayed
	float x, y, z;

public:
	GUIText();
	//take the filepath of a font and initialise the GLFont with this font
	GUIText(const char* font);
	~GUIText();

	//change the text to display into a given string
	void SetText(string text);
	//change the 2D position of the text
	void SetPosition(float x, float y);
	//change the 3D position of the text
	void SetPosition(float x, float y, float z);
	//change the text size
	void SetScale(float newScale);
	//draw the text at the chosen location
	void Render();
	//draw the text at a given location
	void Render(float newX, float newY, float newZ);
};

