#include "GUIText.h"


GUIText::GUIText()
{
}

GUIText::GUIText(const char* font)
{
	font1 = new GLFont();
	font1->Create(font);
	
	SetScale(8);
	SetPosition(0,0,0);
}

GUIText::~GUIText()
{
}

void GUIText::SetText(string text)
{
	string tempString;

	textToDisplay.erase(textToDisplay.begin(), textToDisplay.end());

 	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] != '\n')
		{
			tempString += text[i];
		}
		else
		{
			textToDisplay.push_back(tempString);
			tempString = "";
		}
	}
	textToDisplay.push_back(tempString);
}

void GUIText::SetPosition(float newX, float newY)
{
	SetPosition(newX, newY, 0);
}

void GUIText::SetPosition(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

void GUIText::SetScale(float newScale)
{
	font1->SetScale(newScale);	
}

void GUIText::Render()
{
	Render(x, y, z);
}

void GUIText::Render(float newX, float newY, float newZ)
{
	font1->Begin();
	for (int i = 0; i < textToDisplay.size(); i++)
	{
		font1->TextOut(textToDisplay[i].c_str(), newX, newY - 2 * font1->GetScale() * i, newZ);
	}
	font1->End();
}