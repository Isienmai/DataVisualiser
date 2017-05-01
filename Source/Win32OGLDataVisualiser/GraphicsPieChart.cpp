#include "GraphicsBase.h"


GraphicsPieChart::GraphicsPieChart(int width, int height, Data* theData) : GraphicsBase(width, height, theData)
{
	sourceX = 0; sourceY = 0;
	scale = 1;
	radius = 200;
}


GraphicsPieChart::~GraphicsPieChart()
{
}

void GraphicsPieChart::Render()
{	
	DrawChart();
	selectedText.Render();
}

void GraphicsPieChart::LoadInData(std::vector<std::string> givenData, int numberOfAttributes)
{
	std::sort(givenData.begin(), givenData.end());

	tallies.erase(tallies.begin(), tallies.end());

	for (int i = 0; i < givenData.size(); i++)
	{
		if ((tallies.size() > 0) && (tallies[tallies.size() - 1].content == givenData[i]))
		{
			tallies[tallies.size() - 1].quantity++;
		}
		else
		{
			tallies.push_back({ 1, givenData[i], GetRandomColours() });
		}
	}

	tallyLength = givenData.size();
}

int GraphicsPieChart::GetDisplayType()
{
	return PieChart;
}

void GraphicsPieChart::DrawChart()
{
	std::vector<RGBColour> colour;

	for (int i = 0; i < tallies.size(); i++)
	{
		for (int j = 0; j < tallies[i].quantity; j++)
		{
			if (i == selectedContent)
			{
				colour.push_back(selectedColour);
			}
			else
			{
				colour.push_back(tallies[i].colour);
			}			
		}
	}

	glBegin(GL_TRIANGLE_FAN);
	
	for (int i = 0; i < tallyLength; i++)
	{		
		glColor3f(colour[i].red / 255, colour[i].green / 255, colour[i].blue / 255);
		DrawSegment(i);
	}

	glEnd();
}

void GraphicsPieChart::DrawSegment(int segmentNumber)
{
	double angle = M_PI * 2 / tallyLength;
	double newX, newY;
		
	glVertex2f(sourceX,sourceY);

	newX = sourceX + radius * scale * cos(angle * segmentNumber);
	newY = sourceY + radius * scale * sin(angle * segmentNumber);
	glVertex2f(newX, newY);

	segmentNumber++;
	newX = sourceX + radius * scale * cos(angle * segmentNumber);
	newY = sourceY + radius * scale * sin(angle * segmentNumber);
	glVertex2f(newX, newY);
}

bool GraphicsPieChart::PointInChart(int pointX, int pointY)
{
	int relativeX = pointX - sourceX;
	int relativeY = pointY - sourceY;
	if (sqrt(relativeX*relativeX + relativeY*relativeY) <= radius*scale)
	{
		return true;
	}
	return false;
}


std::string GraphicsPieChart::SelectData(float pointX, float pointY)
{
	if (tallies.size() == 0) return "";
	int selectedSegment = GetSelectedSegment(pointX, pointY);
	std::string toReturn;

	if (selectedSegment != -1)
	{
		selectedContent = selectedSegment;
		toReturn += "Selected Value: ";
		toReturn += tallies[selectedSegment].content.c_str();
		toReturn += "\nQuantity: ";
		toReturn += std::to_string(tallies[selectedSegment].quantity);
		toReturn += "\nPercentage: ";
		toReturn += std::to_string((float)tallies[selectedSegment].quantity * (float)100 / (float)tallyLength);
		toReturn += "%";

		selectedText.SetText(toReturn);
		selectedText.SetPosition(pointX + 15, pointY);
	}
	else
	{
		selectedText.SetText("");
	}

	return toReturn;
}

int GraphicsPieChart::GetSelectedSegment(float pointX, float pointY)
{
	if (PointInChart(pointX, pointY))
	{
		int toReturn = 0;

		int relativeX = pointX - sourceX;
		int relativeY = pointY - sourceY;
		double angle = M_PI * 2 / tallyLength;	

		double pointAngle = atan2(relativeY, relativeX);
		if (pointAngle < 0)
		{
			pointAngle = 2*M_PI + pointAngle;
		}
		for (int i = 0; i < tallyLength; i++)
		{
			if (pointAngle < angle + angle * i)
			{
				toReturn = i;
				i = tallyLength;
			}
		}	

		for (int i = 0; i < tallies.size(); i++)
		{
			toReturn -= tallies[i].quantity;
			if (toReturn < 0)
			{
				toReturn = i;
				i = tallies.size();
			}
		}

		return toReturn;
	}

	return -1;
}

void GraphicsPieChart::MaintainBounds()
{
	if (GetLeft() > windowWidth / 2)
	{
		SetLeft(windowWidth / 2);
	}
	else if (GetRight() < windowWidth / -2)
	{
		SetRight( windowWidth / -2);
	}

	if (GetTop() < windowHeight / -2)
	{
		SetTop(windowHeight / -2);
	}
	else if (GetBottom() > windowHeight / 2)
	{
		SetBottom(windowHeight / 2);
	}
}

float GraphicsPieChart::GetLeft()
{
	return sourceX - radius * scale;
}

void GraphicsPieChart::SetLeft(float newLeft)
{
	sourceX = newLeft + radius * scale;
}

float GraphicsPieChart::GetRight()
{
	return sourceX + radius * scale;
}

void GraphicsPieChart::SetRight(float newRight)
{
	sourceX = newRight - radius * scale;
}

float GraphicsPieChart::GetTop()
{
	return sourceY + radius * scale;
}

void GraphicsPieChart::SetTop(float newTop)
{
	sourceY = newTop - radius * scale;
}

float GraphicsPieChart::GetBottom()
{
	return sourceY - radius * scale;
}

void GraphicsPieChart::SetBottom(float newBottom)
{
	sourceY = newBottom + radius * scale;
}


