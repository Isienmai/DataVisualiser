#include "GraphicsBase.h"

GraphicsLineGraph::GraphicsLineGraph(int width, int height, Data* theData) : GraphicsBarGraph(width, height, theData)
{
}

GraphicsLineGraph::~GraphicsLineGraph()
{
}

void GraphicsLineGraph::Render()
{
	glTranslatef(0.0f, 0.0f, -6.0f);	
	DrawGraph();
	DrawAxis();
	selectedText.Render();
}


void GraphicsLineGraph::DrawGraph()
{
	RGBColour tempColour;
	int startingIndex = 0;
	double segmentWidth, segmentHeight;

	for (int i = 0; i < lineEndPoints.size(); i++)
	{

		//set line 'i's colour
		if (i == selectedContent)
		{
			glColor3f(selectedColour.red / 255, selectedColour.green / 255, selectedColour.blue / 255);
		}
		else
		{
			tempColour = lines[lineEndPoints[i] - 1].colour;
			glColor3f(tempColour.red / 255, tempColour.green / 255, tempColour.blue / 255);
		}		

		//draw the line
		glBegin(GL_LINE_STRIP);

		for (int j = startingIndex; j < lineEndPoints[i]; j++)
		{
			//draw point j in the line
			glVertex3f(sourceX + scale * points[j].x, sourceY + scale * points[j].y, 0);
		}

		glEnd();

		//draw points in the line
		for (int j = startingIndex; j < lineEndPoints[i]; j++)
		{			
			DrawPoint(sourceX + scale * points[j].x, sourceY + scale * points[j].y);
		}

		startingIndex = lineEndPoints[i] + 1;
	}
}

void GraphicsLineGraph::DrawPoint(double centreX, double centreY)
{
	glBegin(GL_TRIANGLE_STRIP);

	glVertex3f(centreX, centreY + 5, 0);
	glVertex3f(centreX - 5, centreY, 0);
	glVertex3f(centreX, centreY - 5, 0);
	glVertex3f(centreX + 5, centreY, 0);
	glVertex3f(centreX, centreY + 5, 0);

	glEnd();
}

void GraphicsLineGraph::LoadInData(std::vector<std::string> givenData, int numberOfAttributes)
{
	//clear data storage
	lines.erase(lines.begin(), lines.end());
	lineEndPoints.erase(lineEndPoints.begin(), lineEndPoints.end());
	points.erase(points.begin(), points.end());
	maxHeight = 0;

	SplitDataIntoAttributes(givenData, numberOfAttributes);

	OrganiseLines(numberOfAttributes);

	TurnLinesIntoPoints(numberOfAttributes);
}

bool GraphicsLineGraph::CompareByContent(const TallyValue &a, const TallyValue &b)
{
	return a.content < b.content;
}

void GraphicsLineGraph::SplitDataIntoAttributes(std::vector<std::string> givenData, int numOfAtts)
{
	if (givenData.size() > 0)
	{
		int lineLength = givenData.size() / numOfAtts;
		int givenDataIndex = 0;
		RGBColour tempColour;

		for (int i = 0; i < numOfAtts; i++)
		{
			//get a random colour for the next line
			tempColour = GetRandomColours();
			for (int j = 0; givenDataIndex < lineLength * (i + 1); givenDataIndex++)
			{
				lines.push_back({ 1, givenData[givenDataIndex], tempColour });
			}
			lines.push_back({ -1, "", { 0, 0, 0 } });
		}
	}	
}

void GraphicsLineGraph::OrganiseLines(int numOfAtts)
{
	std::vector<TallyValue> uniqueTallies;
	int listIndex = 0, uniqueIndex = 0, lastEndPoint = 0;
	bool contained;

	for (int i = 0; i < numOfAtts; i++)
	{		
		while (lines[listIndex].quantity != -1)
		{
			contained = false;
			for (int j = uniqueIndex; j < uniqueTallies.size(); j++)
			{
				if (uniqueTallies[j].content == lines[listIndex].content)
				{
					uniqueTallies[j].quantity++;
					if (uniqueTallies[j].quantity > maxHeight)
					{
						maxHeight = uniqueTallies[j].quantity;
					}
					j = uniqueTallies.size();
					contained = true;
				}
			}
			if (!contained)
			{
				uniqueTallies.push_back({ 1, lines[listIndex].content, lines[listIndex].colour });
			}
			listIndex++;
		}		
		std::sort(uniqueTallies.begin() + lastEndPoint, uniqueTallies.end(), CompareByContent);
		uniqueTallies.push_back({ -1, "", {0,0,0} });
		lineEndPoints.push_back(uniqueTallies.size() - 1);
		uniqueIndex = uniqueTallies.size();
		lastEndPoint = uniqueIndex - 1;
		listIndex++;
	}

	lines.erase(lines.begin(), lines.end());
	lines.insert(lines.begin(), uniqueTallies.begin(), uniqueTallies.end());
}

void GraphicsLineGraph::TurnLinesIntoPoints(int numOfAtts)
{
	int startingIndex = 0;
	double segmentWidth, segmentHeight;
	for (int i = 0; i < numOfAtts; i++)
	{
		segmentWidth = (windowWidth - 100) / ((lineEndPoints[i] - startingIndex) - 1);
		segmentHeight = (windowHeight - 100) / maxHeight;

		for (int j = startingIndex; j < lineEndPoints[i]; j++)
		{
			//store line point
			points.push_back({ segmentWidth * (j - startingIndex), lines[j].quantity * segmentHeight });
		}

		points.push_back({ -1, -1 });
		startingIndex = lineEndPoints[i] + 1;
	}
}

int GraphicsLineGraph::GetDisplayType()
{
	return LineGraph;
}

std::string GraphicsLineGraph::SelectData(float pointX, float pointY)
{
	if (lines.size() == 0) return "";
	int selectedSegment = GetSelectedLine(pointX, pointY);
	std::string toReturn;

	if (selectedSegment != -1)
	{
		int selectedPoint = GetSelectedPoint(pointX, pointY, selectedSegment);
		selectedContent = selectedSegment;
		toReturn += "Selected Attribute: ";
		toReturn += data->GetHeadingForLine(selectedSegment);
		if (selectedPoint != -1)
		{
			toReturn += "\nSelected Data: ";
			toReturn += lines[selectedPoint].content.c_str();
			toReturn += "\nSelected Quantity: ";
			toReturn += std::to_string(lines[selectedPoint].quantity);
		}

		selectedText.SetText(toReturn);
		selectedText.SetPosition(pointX + 15, pointY);
	}
	else
	{
		selectedText.SetText("");
	}
	
	return toReturn;
}

int GraphicsLineGraph::GetSelectedLine(float pointX, float pointY)
{
	int startingIndex = 0;
	Point botLeft, topRight, mouse = {pointX, pointY};

	for (int i = 0; i < lineEndPoints.size(); i++)
	{
		for (int j = startingIndex + 1; j < lineEndPoints[i]; j++)
		{
			botLeft = { sourceX + scale*points[j - 1].x, sourceY + scale*points[j - 1].y };
			topRight = { sourceX + scale*points[j].x, sourceY + scale*points[j].y };
			if (CheckLineIntersection(mouse, botLeft, topRight))
			{
				return i;
			}
		}

		startingIndex = lineEndPoints[i] + 1;
	}

	return -1;
}

int GraphicsLineGraph::GetSelectedPoint(float pointX, float pointY, int chosenLine)
{
	int relativeX;
	int relativeY;

	pointX -= sourceX;
	pointY -= sourceY;

	int smallestPoint, biggestPoint = lineEndPoints[chosenLine];

	if (chosenLine == 0)
	{
		smallestPoint = 0;
	}
	else
	{
		smallestPoint = chosenLine - 1;
	}

	for (int i = smallestPoint; i < biggestPoint; i++)
	{

		relativeX = pointX - points[i].x*scale;
		relativeY = pointY - points[i].y*scale;

		if (sqrt((relativeX * relativeX) + (relativeY * relativeY)) < 5)
		{
			return i;
		}
	}
	return -1;
}

bool GraphicsLineGraph::CheckLineIntersection(Point mouse, Point line1, Point line2)
{
	//convert the points into 2D vectors
	glm::vec2 linePoint1 = glm::vec2(line1.x, line1.y), linePoint2 = glm::vec2(line2.x, line2.y), vectMouse = glm::vec2(mouse.x, mouse.y);

	//get the mouse coords relative to a chosen endpoint
	glm::vec2 relativeMouse = vectMouse - linePoint1;

	//get the normalised vectore of the line, originating at a chosen endpoint
	glm::vec2 normalisedLine = glm::normalize(linePoint2 - linePoint1);

	//get the length of the line
	float lineLength = glm::distance(linePoint1, linePoint2);

	float dotProduct = glm::dot(relativeMouse, normalisedLine);

	glm::vec2 nearestPoint;

	//find the nearest point on the line to the mouse
	if (dotProduct <= 0)
	{
		nearestPoint = linePoint1;
	}
	else if (dotProduct >= lineLength)
	{
		nearestPoint = linePoint2;
	}
	else
	{
		nearestPoint = linePoint1 + normalisedLine*dotProduct;
	}

	//return true if the nearest point is close enough to the mouse, false otherwise
	if (glm::distance(vectMouse, nearestPoint) <= 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}