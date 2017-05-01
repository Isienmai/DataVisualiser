#include "GraphicsBase.h"

GraphicsBarGraph::GraphicsBarGraph(int width, int height, Data* theData) : GraphicsBase(width, height, theData)
{
	sourceX = 100 - windowWidth / 2;
	sourceY = 100 - windowHeight / 2;
	scale = 1;
	maxHeight = 0;
}


GraphicsBarGraph::~GraphicsBarGraph()
{
}

void GraphicsBarGraph::Render()
{
	glTranslatef(0.0f, 0.0f, -6.0f);
	DrawGraph();
	DrawAxis();
	selectedText.Render();
}

void GraphicsBarGraph::LoadInData(std::vector<std::string> givenData, int numberOfAttributes)
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


	heights.erase(heights.begin(), heights.end());

	maxHeight = 0;
	for (int i = 0; i < tallies.size(); i++)
	{
		if (tallies[i].quantity > maxHeight)
		{
			maxHeight = tallies[i].quantity;
		}
	}

	float nextHeight;
	for (int i = 0; i < tallies.size(); i++)
	{
		nextHeight = (float)tallies[i].quantity / (float)maxHeight;

		nextHeight *= windowHeight - 200;

		heights.push_back(nextHeight + 1);
	}
}

int GraphicsBarGraph::GetDisplayType()
{
	return BarGraph;
}

void GraphicsBarGraph::UpdateWindowSize(int newWidth, int newHeight)
{
	GraphicsBase::UpdateWindowSize(newWidth, newHeight);
	sourceX = 100 - newWidth / 2;
	sourceY = 100 - newHeight / 2;
}

void GraphicsBarGraph::DrawGraph()
{		
	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i < tallies.size(); i++)
	{
		if (i == selectedContent)
		{
			glColor3f(selectedColour.red / 255, selectedColour.green / 255, selectedColour.blue / 255);
		}
		else
		{
			glColor3f(tallies[i].colour.red / 255, tallies[i].colour.green / 255, tallies[i].colour.blue / 255);
		}
		
		DrawBar(i, heights[i]);
	}

	glEnd();	
}

void GraphicsBarGraph::DrawBar(int barNumber, int barHeight)
{
	double barWidth = scale*(windowWidth - 100) / tallies.size();
	double newX, newY;	

	newX = sourceX + barWidth*barNumber;
	newY = sourceY;
	glVertex3f(newX, newY, 0.0f);

	newY = sourceY + (barHeight*scale);
	glVertex3f(newX, newY, 0.0f);

	newX = sourceX + barWidth*(barNumber + 1);
	newY = sourceY;
	glVertex3f(newX, newY, 0.0f);

	newY = sourceY + (barHeight*scale);
	glVertex3f(newX, newY, 0.0f);
}

std::string GraphicsBarGraph::SelectData(float pointX, float pointY)
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

int GraphicsBarGraph::GetSelectedSegment(float pointX, float pointY)
{
	int relativeX = pointX - sourceX;
	int relativeY = pointY - sourceY;
	double barWidth = scale*(windowWidth - 100) / tallies.size();
	
	for (int i = 0; i < tallies.size(); i++)
	{
		if ((relativeX <= barWidth * (i + 1)) && (relativeX >= barWidth * i))
		{
			if ((relativeY >= 0) && (relativeY <= scale*heights[i]))
			{
				return i;
			}
			return -1;
		}
	}

	return -1;
}


