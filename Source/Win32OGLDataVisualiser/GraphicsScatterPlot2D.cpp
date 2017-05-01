#include "GraphicsBase.h"

GraphicsScatterPlot2D::GraphicsScatterPlot2D(int width, int height, Data* theData) : GraphicsBarGraph(width, height, theData)
{
	tallied = false;
}


GraphicsScatterPlot2D::~GraphicsScatterPlot2D()
{
}

void GraphicsScatterPlot2D::LoadInData(std::vector<std::string> givenData, int numberOfAttributes)
{
	//clear previous storage
	dataPairs.erase(dataPairs.begin(), dataPairs.end());
	uniqueContent1.erase(uniqueContent1.begin(), uniqueContent1.end());
	uniqueContent2.erase(uniqueContent2.begin(), uniqueContent2.end());

	tallied = false;

	
	if (numberOfAttributes == 2)
	{
		std::vector<std::string> tempAtt1, tempAtt2;

		//split the provided data into two lists
		SplitIntoTwo(givenData, &tempAtt1, &tempAtt2);

		//shorten these lists to contain only unique values
		GetUniqueValues(&uniqueContent1, tempAtt1);
		GetUniqueValues(&uniqueContent2, tempAtt2);

		//translate the provided data into a list of value pairs linked to the unique data lists.
		FormatDataAndStore(givenData);

		GraphicsBarGraph::tallyLength = givenData.size();
	}
	else if(numberOfAttributes != 0)
	{
		tallied = true;
		GraphicsBarGraph::LoadInData(givenData, 0);
	}
	else
	{
		GraphicsBarGraph::tallyLength = 0;
	}
}

void GraphicsScatterPlot2D::SplitIntoTwo(std::vector<std::string> source, std::vector<std::string>* result1, std::vector<std::string>* result2)
{
	std::vector<std::string> temp;
	//grab all first attribute values
	for (int i = 0; i < source.size()/2; i++)
	{
		temp.push_back(source[i]);
	}
	//sort the first attribute
	std::sort(temp.begin(), temp.end());

	*result1 = temp;

	temp.erase(temp.begin(), temp.end());

	//grab all second attribute values
	for (int i = source.size() / 2; i < source.size(); i++)
	{
		temp.push_back(source[i]);
	}
	//sort the second attribute
	std::sort(temp.begin(), temp.end());

	*result2 = temp;
}

void GraphicsScatterPlot2D::GetUniqueValues(std::vector<std::string>* goal, std::vector<std::string> source)
{
	std::vector<std::string> temp;
	//remove duplicate values
	temp.push_back(source[0]);
	for (int i = 1; i < source.size(); i++)
	{
		if (temp[temp.size() - 1] != source[i])
		{
			temp.push_back(source[i]);
		}
	}

	*goal = temp;
}

void GraphicsScatterPlot2D::FormatDataAndStore(std::vector<std::string> source)
{
	bool contained;
	int cont1Location, cont2Location;
	int entryQuantity = source.size() / 2;

	for (int i = 0; i < entryQuantity; i++)
	{
		//reset variables
		cont1Location = -1;
		cont2Location = -1;		

		//locate the first selected data within the first unique content list
		for (int j = 0; j < uniqueContent1.size(); j++)
		{
			if (uniqueContent1[j] == source[i])
			{
				cont1Location = j;
				j = uniqueContent1.size();
			}
		}

		//locate the second selected data within the second unique content list
		for (int j = 0; j < uniqueContent2.size(); j++)
		{
			if (uniqueContent2[j] == source[i+entryQuantity])
			{
				cont2Location = j;
				j = uniqueContent2.size();
			}
		}

		contained = false;
		for (int j = 0; j < dataPairs.size(); j++)
		{
			if ((dataPairs[j].contentID1 == cont1Location) && (dataPairs[j].contentID2 == cont2Location))
			{
				dataPairs[j].quantity++;
				contained = true;
				j = dataPairs.size();
			}
		}
		if (!contained)
		{
			dataPairs.push_back({ 1, cont1Location, cont2Location, GetRandomColours() });
		}
	}	
}

int GraphicsScatterPlot2D::GetDisplayType()
{
	return ScatterPlot2D;
}

void GraphicsScatterPlot2D::DrawGraph()
{
	if (tallied)
	{
		GraphicsBarGraph::DrawGraph();
	}	
	else
	{
		double barWidth = scale*(windowWidth - 100) / uniqueContent1.size();
		double barHeight = scale*(windowHeight - 100) / uniqueContent2.size();

		for (int i = 0; i < dataPairs.size(); i++)
		{

			if (i == selectedContent)
			{
				glColor3f(selectedColour.red / 255, selectedColour.green / 255, selectedColour.blue / 255);
			}
			else
			{
				glColor3f(dataPairs[i].colour.red / 255, dataPairs[i].colour.green / 255, dataPairs[i].colour.blue / 255);
			}
			DrawPoint(sourceX + (barWidth*dataPairs[i].contentID1) + (barWidth / 2), sourceY + (barHeight*dataPairs[i].contentID2) + (barHeight / 2));
		}
	}
}

void GraphicsScatterPlot2D::DrawBar(int barNumber, int barHeight)
{
	double barWidth = scale*(windowWidth - 100) / GraphicsBarGraph::tallies.size();
	DrawPoint(sourceX + (barWidth * barNumber) + (barWidth / 2), sourceY + barHeight*scale);
}

void GraphicsScatterPlot2D::DrawPoint(double centreX, double centreY)
{	
	glBegin(GL_TRIANGLE_STRIP);

	glVertex3f(centreX, centreY + 5, 0);
	glVertex3f(centreX - 5, centreY, 0);
	glVertex3f(centreX, centreY - 5, 0);
	glVertex3f(centreX + 5, centreY, 0);
	glVertex3f(centreX, centreY + 5, 0);

	glEnd();
}

std::string GraphicsScatterPlot2D::SelectData(float pointX, float pointY)
{
	if ((tallies.size() == 0)&&(dataPairs.size() == 0)) return "";
	int selectedSegment = GetSelectedSegment(pointX, pointY);
	std::string toReturn;

	if (selectedSegment != -1)
	{
		selectedContent = selectedSegment;
		if (tallied)
		{
			toReturn += "Selected Value: ";
			toReturn += tallies[selectedSegment].content.c_str();
			toReturn += "\nQuantity: ";
			toReturn += std::to_string(tallies[selectedSegment].quantity);
		}
		else
		{
			toReturn += "Selected Value Pair: ";
			toReturn += uniqueContent1[dataPairs[selectedSegment].contentID1].c_str();
			toReturn += ", ";
			toReturn += uniqueContent2[dataPairs[selectedSegment].contentID2].c_str();
			toReturn += "\nQuantity: ";
			toReturn += std::to_string(dataPairs[selectedSegment].quantity);
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

int GraphicsScatterPlot2D::GetSelectedSegment(float pointX, float pointY)
{
	double barWidth, barHeight;

	double centralX, centralY;

	int relativeX;
	int relativeY;	

	if (tallied)
	{
		barWidth = scale*(windowWidth - 100) / GraphicsBarGraph::tallies.size();

		for (int i = 0; i < GraphicsBarGraph::tallies.size(); i++)
		{
			centralX = sourceX + (barWidth * i) + (barWidth / 2);
			centralY = sourceY + GraphicsBarGraph::heights[i]*scale;

			relativeX = pointX - centralX;
			relativeY = pointY - centralY;

			if (sqrt((relativeX * relativeX) + (relativeY * relativeY)) < 5)
			{
				return i;
			}
		}
	}
	else
	{
		barWidth = scale*(windowWidth - 100) / uniqueContent1.size();
		barHeight = scale*(windowHeight - 100) / uniqueContent2.size();

		for (int i = 0; i < dataPairs.size(); i++)
		{
			centralX = sourceX + (barWidth*dataPairs[i].contentID1) + (barWidth / 2);
			centralY = sourceY + (barHeight*dataPairs[i].contentID2) + (barHeight / 2);

			relativeX = pointX - centralX;
			relativeY = pointY - centralY;

			if (sqrt((relativeX * relativeX) + (relativeY * relativeY)) < 5)
			{
				return i;
			}
		}
	}
	return -1;
}
