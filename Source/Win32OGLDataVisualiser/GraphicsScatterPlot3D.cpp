#include "GraphicsBase.h"

GraphicsScatterPlot3D::GraphicsScatterPlot3D(int width, int height, Data* theData) : GraphicsScatterPlot2D(width, height, theData)
{
	sourceX = 0;
	sourceY = 0;
	scale = 10;
	nearPointt = glm::vec3(0, 0, 0);
	farPointt = glm::vec3(0, 0, 1);
	//set selected coords
	selectedX = 0; selectedY = 0; selectedZ = 0;
	selectedText.SetScale(0.015*scale);
}


GraphicsScatterPlot3D::~GraphicsScatterPlot3D()
{
}

void GraphicsScatterPlot3D::Render()
{
	glTranslatef(sourceX / 100, sourceY / 100, -1 * scale);

	glRotatef(XRotation, 1.0f, 0.0f, 0.0f);
	glRotatef(-1*YRotation, 0.0f, 1.0f, 0.0f);

	DrawGraph();
	DrawAxis();
	selectedText.Render();
}

void GraphicsScatterPlot3D::LoadInData(std::vector<std::string> givenData, int numberOfAttributes)
{
	//clear previous storage
	dataTrios.erase(dataTrios.begin(), dataTrios.end());
	uniqueContent1.erase(uniqueContent1.begin(), uniqueContent1.end());
	uniqueContent2.erase(uniqueContent2.begin(), uniqueContent2.end());
	uniqueContent3.erase(uniqueContent3.begin(), uniqueContent3.end());

	tallied = false;

	if (numberOfAttributes >= 3)
	{
		//make sure the provided data is in threes
		if (givenData.size() % 3 != 0)
		{
			for (int i = 0; i < givenData.size() % 3; i++)
			{
				givenData.erase(givenData.end() - 1);
			}
		}

		std::vector<std::string> tempAtt1, tempAtt2, tempAtt3;

		//split the provided data into two lists
		SplitIntoThree(givenData, &tempAtt1, &tempAtt2, &tempAtt3);

		//shorten these lists to contain only unique values
		GetUniqueValues(&uniqueContent1, tempAtt1);
		GetUniqueValues(&uniqueContent2, tempAtt2);
		GetUniqueValues(&uniqueContent3, tempAtt3);

		//translate the provided data into a list of value pairs linked to the unique data lists.
		FormatDataAndStore(givenData);

		tallyLength = givenData.size() / numberOfAttributes;
	}
	else if (numberOfAttributes == 2)
	{
		std::vector<std::string> tempAtt1, tempAtt2;

		//split the provided data into two lists
		SplitIntoTwo(givenData, &tempAtt1, &tempAtt2);

		//shorten these lists to contain only unique values
		GetUniqueValues(&uniqueContent1, tempAtt1);
		GetUniqueValues(&uniqueContent2, tempAtt2);

		//translate the provided data into a list of value pairs linked to the unique data lists.
		FormatDataAndStoreTally(givenData);		

		tallyLength = givenData.size() / numberOfAttributes;
	}
}

void GraphicsScatterPlot3D::SplitIntoThree(std::vector<std::string> source, std::vector<std::string>* result1, std::vector<std::string>* result2, std::vector<std::string>* result3)
{
	std::vector<std::string> temp;
	int index = 0;

	//grab all first attribute values
	while (index < source.size()/3)
	{
		temp.push_back(source[index++]);
	}
	//sort the first attribute
	std::sort(temp.begin(), temp.end());

	*result1 = temp;

	temp.erase(temp.begin(), temp.end());

	//grab all second attribute values
	while (index < 2 * source.size() / 3)
	{
		temp.push_back(source[index++]);
	}
	//sort the second attribute
	std::sort(temp.begin(), temp.end());

	*result2 = temp;

	temp.erase(temp.begin(), temp.end());

	//grab all third attribute values
	while (index < source.size())
	{
		temp.push_back(source[index++]);
	}
	//sort the third attribute
	std::sort(temp.begin(), temp.end());

	*result3 = temp;
}

void GraphicsScatterPlot3D::FormatDataAndStore(std::vector<std::string> source)
{
	bool contained;
	int cont1Location, cont2Location, cont3Location;
	int entryQuantity = source.size() / 3;

	for (int i = 0; i < entryQuantity; i++)
	{
		//reset variables
		cont1Location = -1;
		cont2Location = -1;
		cont3Location = -1;
		contained = false;

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
			if (uniqueContent2[j] == source[i + entryQuantity])
			{
				cont2Location = j;
				j = uniqueContent2.size();
			}
		}

		//locate the third selected data within the third unique content list
		for (int j = 0; j < uniqueContent3.size(); j++)
		{
			if (uniqueContent3[j] == source[i + (2 * entryQuantity)])
			{
				cont3Location = j;
				j = uniqueContent3.size();
			}
		}


		for (int j = 0; j < dataTrios.size(); j++)
		{
			if ((dataTrios[j].contentID1 == cont1Location) && (dataTrios[j].contentID2 == cont2Location) && (dataTrios[j].contentID3 == cont3Location))
			{
				dataTrios[j].quantity++;
				contained = true;
				j = dataTrios.size();
			}
		}
		if (!contained)
		{
			dataTrios.push_back({ 1, cont1Location, cont2Location, cont3Location, GetRandomColours() });
		}
	}
}

void GraphicsScatterPlot3D::FormatDataAndStoreTally(std::vector<std::string> source)
{
	bool contained;
	int cont1Location, cont2Location, cont3Location = 1;
	int entryQuantity = source.size() / 2;

	for (int i = 0; i < entryQuantity; i++)
	{
		//reset variables
		cont1Location = -1;
		cont2Location = -1;		
		contained = false;

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
			if (uniqueContent2[j] == source[i + entryQuantity])
			{
				cont2Location = j;
				j = uniqueContent2.size();
			}
		}
		
		//put this data into the core
		for (int j = 0; j < dataTrios.size(); j++)
		{
			if ((dataTrios[j].contentID1 == cont1Location) && (dataTrios[j].contentID2 == cont2Location))
			{
				dataTrios[j].quantity++;

				if (dataTrios[j].quantity > cont3Location) cont3Location = dataTrios[j].quantity;

				contained = true;
				j = dataTrios.size();
			}
		}
		if (!contained)
		{
			dataTrios.push_back({ 1, cont1Location, cont2Location, 0, GetRandomColours() });
		}
	}

	for (int i = 0; i < cont3Location; i++)
	{
		uniqueContent3.push_back(std::to_string(i+1));
	}

	for (int i = 0; i < dataTrios.size(); i++)
	{
		dataTrios[i].contentID3 = dataTrios[i].quantity-1;
	}
}

int GraphicsScatterPlot3D::GetDisplayType()
{
	return ScatterPlot3D;
}

void GraphicsScatterPlot3D::UpdateWindowSize(int newWidth, int newHeight)
{
	GraphicsBase::UpdateWindowSize(newWidth, newHeight);
	sourceX = 0;
	sourceY = 0;
}

void GraphicsScatterPlot3D::DrawGraph()
{
	for (int i = 0; i < dataTrios.size(); i++)
	{
		
		if (i == selectedContent)
		{
			glColor3f(selectedColour.red / 255, selectedColour.green / 255, selectedColour.blue / 255);
		}
		else
		{
			glColor3f(dataTrios[i].colour.red / 255, dataTrios[i].colour.green / 255, dataTrios[i].colour.blue / 255);
		}
		DrawPoint(i);
	}
}

void GraphicsScatterPlot3D::DrawPoint(int barNumber)
{
	float barWidth = 4.0 / uniqueContent1.size();
	float barHeight = 4.0 / uniqueContent2.size();
	float barDepth = 4.0 / uniqueContent3.size();
	float centralX, centralY, centralZ;

	centralX = barWidth*dataTrios[barNumber].contentID1 - 2.0;
	centralY = barHeight*dataTrios[barNumber].contentID2 - 2.0;
	centralZ = barDepth*dataTrios[barNumber].contentID3 *-1 + 2.0;

	DrawCube(centralX, centralY, centralZ);
}

void GraphicsScatterPlot3D::DrawCube(float centreX, float centreY, float centreZ)
{
	float displacement = 0.01;
	glBegin(GL_QUADS);

	//draw top 
	glVertex3f(centreX - displacement, centreY + displacement, centreZ + displacement);
	glVertex3f(centreX + displacement, centreY + displacement, centreZ + displacement);
	glVertex3f(centreX + displacement, centreY + displacement, centreZ - displacement);
	glVertex3f(centreX - displacement, centreY + displacement, centreZ - displacement);

	//draw left
	glVertex3f(centreX - displacement, centreY - displacement, centreZ + displacement);
	glVertex3f(centreX - displacement, centreY + displacement, centreZ + displacement);
	glVertex3f(centreX - displacement, centreY + displacement, centreZ - displacement);
	glVertex3f(centreX - displacement, centreY - displacement, centreZ - displacement);

	//draw front
	glVertex3f(centreX + displacement, centreY - displacement, centreZ + displacement);
	glVertex3f(centreX + displacement, centreY + displacement, centreZ + displacement);
	glVertex3f(centreX - displacement, centreY + displacement, centreZ + displacement);
	glVertex3f(centreX - displacement, centreY - displacement, centreZ + displacement);

	//draw right
	glVertex3f(centreX + displacement, centreY - displacement, centreZ + displacement);
	glVertex3f(centreX + displacement, centreY + displacement, centreZ + displacement);
	glVertex3f(centreX + displacement, centreY + displacement, centreZ - displacement);
	glVertex3f(centreX + displacement, centreY - displacement, centreZ - displacement);

	//draw back
	glVertex3f(centreX + displacement, centreY - displacement, centreZ - displacement);
	glVertex3f(centreX + displacement, centreY + displacement, centreZ - displacement);
	glVertex3f(centreX - displacement, centreY + displacement, centreZ - displacement);
	glVertex3f(centreX - displacement, centreY - displacement, centreZ - displacement);

	//draw bottom
	glVertex3f(centreX - displacement, centreY - displacement, centreZ + displacement);
	glVertex3f(centreX + displacement, centreY - displacement, centreZ + displacement);
	glVertex3f(centreX + displacement, centreY - displacement, centreZ - displacement);
	glVertex3f(centreX - displacement, centreY - displacement, centreZ - displacement);

	glEnd();
}

void GraphicsScatterPlot3D::DrawAxis()
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);

	glVertex3f(-2.0f, -2.0f, 2.0f);
	glVertex3f(-2.0f, 2.0f, 2.0f);

	glVertex3f(-2.0f, -2.0f, 2.0f);
	glVertex3f(2.0f, -2.0f, 2.0f);

	glVertex3f(-2.0f, -2.0f, 2.0f);
	glVertex3f(-2.0f, -2.0f, -2.0f);

	glEnd();

	//Draw the last projection ray from the mouse
	/*glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(nearPointt.x, nearPointt.y, nearPointt.z);
	glVertex3f(farPointt.x, farPointt.y, farPointt.z);
	glEnd();*/
}

std::string GraphicsScatterPlot3D::SelectData(float pointX, float pointY)
{
	if (dataTrios.size() == 0) return "";

	//reset selected coords
	selectedX = 0; selectedY = 0; selectedZ = 0;

	int selectedSegment = GetSelectedSegment(pointX, pointY);
	std::string toReturn;

	if (selectedSegment != -1)
	{
		selectedContent = selectedSegment;
		toReturn += "Selected X Value: ";
		toReturn += uniqueContent1[dataTrios[selectedSegment].contentID1].c_str();
		toReturn += "\nSelected Y Value: ";
		toReturn += uniqueContent2[dataTrios[selectedSegment].contentID2].c_str();
		toReturn += "\nSelected Z Value: ";
		toReturn += uniqueContent3[dataTrios[selectedSegment].contentID3].c_str();
		
		selectedText.SetText(toReturn);
		selectedText.SetPosition(selectedX, selectedY, selectedZ);
	}

	return toReturn;
}

int GraphicsScatterPlot3D::GetSelectedSegment(float givenX, float givenY)
{
	//the following code obtains the opengl coordinates of the ray from the mouse
	GLint viewport[4];                 
	glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves The Viewport Values (X, Y, Width, Height)

	GLdouble modelview[16];                
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);       // Retrieve The Modelview Matrix

	GLdouble projection[16];               
	glGetDoublev(GL_PROJECTION_MATRIX, projection);     // Retrieve The Projection Matrix
	
	//Get the X an Y coords of the mouse
	float pointX, pointY;
	pointX = givenX + (float)windowWidth / 2.0f;
	pointY = givenY + (float)windowHeight / 2.0f;

	//get the X,Y coords of the mouse within the OGL space
	glm::vec3 nearPoint, farPoint;
	double oglX, oglY, oglZ;
	gluUnProject(pointX, pointY, 0.0f, modelview, projection, viewport, &oglX, &oglY, &oglZ);//get near point of ray
	nearPointt = glm::vec3(oglX, oglY, oglZ);
	gluUnProject(pointX, pointY, 1.0f, modelview, projection, viewport, &oglX, &oglY, &oglZ);//get far point of ray
	farPointt = glm::vec3(oglX, oglY, oglZ);

	//this code is used to turn each piece of data into a point in the coordinate system
	float barWidth = 4.0 / uniqueContent1.size();
	float barHeight = 4.0 / uniqueContent2.size();
	float barDepth = 4.0 / uniqueContent3.size();
	float centralX, centralY, centralZ;
	
	int currentData = -1;
	float currentDataDistance = -1, currentDistance;

	for (int i = 0; i < dataTrios.size(); i++)
	{
		centralX = barWidth*dataTrios[i].contentID1 - 2.0;
		centralY = barHeight*dataTrios[i].contentID2 - 2.0;
		centralZ = barDepth*dataTrios[i].contentID3 *-1 + 2.0;

		currentDistance = CheckLineIntersection(glm::vec3(centralX, centralY, centralZ), nearPointt, farPointt);

		if (currentDistance > currentDataDistance)
		{
			currentDataDistance = currentDistance;
			currentData = i;
			selectedX = centralX;
			selectedY = centralY;
			selectedZ = centralZ;
		}
	}
	return currentData;
}

float GraphicsScatterPlot3D::CheckLineIntersection(glm::vec3 point, glm::vec3 line1, glm::vec3 line2)
{
	//get the mouse coords relative to a chosen endpoint
	glm::vec3 relativeMouse = point - line1;

	//get the normalised vectore of the line, originating at a chosen endpoint
	glm::vec3 normalisedLine = glm::normalize(line2 - line1);

	//get the length of the line
	float lineLength = glm::distance(line1, line2);

	float dotProduct = glm::dot(relativeMouse, normalisedLine);

	glm::vec3 nearestPoint;

	//find the nearest point on the line to the mouse
	if (dotProduct <= 0)
	{
		nearestPoint = line1;
	}
	else if (dotProduct >= lineLength)
	{
		nearestPoint = line2;
	}
	else
	{
		nearestPoint = line1 + normalisedLine*dotProduct;
	}

	//return true if the nearest point is close enough to the mouse, false otherwise
	if (glm::distance(point, nearestPoint) <= 0.025)
	{
		return 0.025 - glm::distance(point, nearestPoint);
	}
	else
	{
		return -1;
	}
}


void GraphicsScatterPlot3D::MaintainBounds()
{
	//No bounds
}

void GraphicsScatterPlot3D::SetProjection()
{
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GraphicsScatterPlot3D::Scale(int change, float mouseX, float mouseY)
{
	float oldScale = scale;
	if (change < 0)
	{
		scale += 1;
	}
	else
	{
		scale -= 1;
	}


	if ((scale < 1) || (scale > 20))
	{
		scale = oldScale;
	}


	selectedText.SetScale(0.010*scale);
}

