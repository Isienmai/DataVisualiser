#include "GraphicsBase.h"


GraphicsSpiderChart::GraphicsSpiderChart(int width, int height, Data* theData) : GraphicsBase(width, height, theData)
{
	sourceX = 0; sourceY = 0;
	scale = 1;
	radius = 200;
	maxSize = 1;
}


GraphicsSpiderChart::~GraphicsSpiderChart()
{
}

void GraphicsSpiderChart::Render()
{
	DrawChart();
	DrawAxis();
	selectedText.Render();
}

void GraphicsSpiderChart::LoadInData(std::vector<std::string> givenData, int numberOfAttributes)
{
	std::sort(givenData.begin(), givenData.end());

	tallies.erase(tallies.begin(), tallies.end());

	maxSize = 1;

	for (int i = 0; i < givenData.size(); i++)
	{
		tallies.push_back({ 1, givenData[i], GetRandomColours() });
		while ((i < givenData.size()) && (tallies[tallies.size() - 1].content == givenData[i]))
		{
			tallies[tallies.size() - 1].quantity++;
			if (tallies[tallies.size() - 1].quantity > maxSize)
			{
				maxSize = tallies[tallies.size() - 1].quantity;
			}
			i++;
		}
	}

	while (tallies.size() < 3)
	{
		tallies.push_back({ 0, "", { 0, 0, 0 } });
	}

	tallyLength = givenData.size();
}

int GraphicsSpiderChart::GetDisplayType()
{
	return SpiderChart;
}

void GraphicsSpiderChart::DrawChart()
{
	RGBColour tempCol = tallies[0].colour;
	double angle = M_PI * 2 / tallies.size();
	double newX, newY;

	glColor3f(tempCol.red / 255, tempCol.green / 255, tempCol.blue / 255);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < tallies.size(); i++)
	{		
		newX = sourceX + radius * scale * cos(angle * i)*((double)tallies[i].quantity / (double)maxSize);
		newY = sourceY + radius * scale * sin(angle * i)*((double)tallies[i].quantity / (double)maxSize);
		glVertex3f(newX, newY, 0);
	}
	newX = sourceX + radius * scale * cos(angle * 0)*((double)tallies[0].quantity / (double)maxSize);
	newY = sourceY + radius * scale * sin(angle * 0)*((double)tallies[0].quantity / (double)maxSize);
	glVertex3f(newX, newY, 0);

	glEnd();
}

void GraphicsSpiderChart::DrawAxis()
{
	double angle = M_PI * 2 / tallies.size();
	double newX, newY;	

	for (int i = 0; i < tallies.size(); i++)
	{
		newX = sourceX + radius * scale * cos(angle * i);
		newY = sourceY + radius * scale * sin(angle * i);

		if (i == selectedContent)
		{
			glColor3f(1, 0, 1);
		}
		else
		{
			glColor3f(1, 1, 1);
		}

		glBegin(GL_LINES);
		glVertex3f(sourceX, sourceY, 0);
		glVertex3f(newX, newY, 0);
		glEnd();
	}

}

bool GraphicsSpiderChart::PointInChart(float pointX, float pointY)
{
	int relativeX = pointX - sourceX;
	int relativeY = pointY - sourceY;
	if (sqrt(relativeX*relativeX + relativeY*relativeY) <= radius*scale)
	{
		return true;
	}
	return false;
}


std::string GraphicsSpiderChart::SelectData(float pointX, float pointY)
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

int GraphicsSpiderChart::GetSelectedSegment(float pointX, float pointY)
{
	int toReturn = -1, bestRange = 100, newRange;
	bool bestRangeFound = false;
	if (PointInChart(pointX, pointY))
	{
		int relativeX = pointX - sourceX;
		int relativeY = pointY - sourceY;

		double angle = M_PI * 2 / tallies.size();
		double newX, newY;
		for (int i = 0; i < tallies.size(); i++)
		{
			newX = radius * scale * cos(angle * i);
			newY = radius * scale * sin(angle * i);
			newRange = CheckLineIntersection(relativeX, relativeY, newX, newY);
			if ((newRange != -1)&&(newRange < bestRange))
			{
				bestRange = newRange;
				toReturn = i;
			}
		}
	}

	return toReturn;
}

int GraphicsSpiderChart::CheckLineIntersection(float pointX, float pointY, float lineX, float lineY)
{


	//if the mouse is on the correct side of the x and y axes
	if ((pointY * lineY >= 0) && (pointX * lineX >= 0))
	{
		//limit the possible siutations to only involve positive gradients
		pointY = abs(pointY);
		lineY = abs(lineY);
		pointX = abs(pointX);
		lineX = abs(lineX);
		
		float tempX = lineX, tempY = lineY;
		float oldSqrt = sqrt(((tempX - pointX) * (tempX - pointX)) + ((tempY - pointY) * (tempY - pointY))) + 1, newSqrt = sqrt(((tempX - pointX) * (tempX - pointX)) + ((tempY - pointY) * (tempY - pointY)));
		float divisor = 100;
		bool goinLeft = true;

		//as long as the distance between the mouse and the point on the line is reducing, move the point along the line
		while (newSqrt != oldSqrt)
		{
			if (goinLeft)
			{
				oldSqrt = newSqrt;
				tempX -= (float)lineX / divisor;
				tempY -= (float)lineY / divisor;
				newSqrt = sqrt(((tempX - pointX) * (tempX - pointX)) + ((tempY - pointY) * (tempY - pointY)));

				if (newSqrt > oldSqrt)
				{
					goinLeft = false;
					divisor *= -100;
				}

				if (tempY < 0)
				{
					newSqrt = oldSqrt;
				}
			}
			else
			{
				oldSqrt = newSqrt;
				tempX -= (float)lineX / divisor;
				tempY -= (float)lineY / divisor;
				newSqrt = sqrt(((tempX - pointX) * (tempX - pointX)) + ((tempY - pointY) * (tempY - pointY)));

				if (newSqrt > oldSqrt)
				{
					goinLeft = true;
					divisor *= -100;
				}

				if (tempY > lineY)
				{
					newSqrt = oldSqrt;
				}
			}
		}

		if (oldSqrt < 10)
		{
			return oldSqrt;
		}
	}
	return -1;
}

void GraphicsSpiderChart::MaintainBounds()
{
	if (GetLeft() > windowWidth / 2)
	{
		SetLeft(windowWidth / 2);
	}
	else if (GetRight() < windowWidth / -2)
	{
		SetRight(windowWidth / -2);
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

float GraphicsSpiderChart::GetLeft()
{
	return sourceX - radius * scale;
}

void GraphicsSpiderChart::SetLeft(float newLeft)
{
	sourceX = newLeft + radius * scale;
}

float GraphicsSpiderChart::GetRight()
{
	return sourceX + radius * scale;
}

void GraphicsSpiderChart::SetRight(float newRight)
{
	sourceX = newRight - radius * scale;
}

float GraphicsSpiderChart::GetTop()
{
	return sourceY + radius * scale;
}

void GraphicsSpiderChart::SetTop(float newTop)
{
	sourceY = newTop - radius * scale;
}

float GraphicsSpiderChart::GetBottom()
{
	return sourceY - radius * scale;
}

void GraphicsSpiderChart::SetBottom(float newBottom)
{
	sourceY = newBottom + radius * scale;
}