#include "GraphicsBase.h"


GraphicsBase::GraphicsBase()
{
}

GraphicsBase::GraphicsBase(int width, int height, Data* theData)
{
	//initialise all values
	windowWidth = width;
	windowHeight = height;
	data = theData;
	srand(time(NULL));
	selectedColour = { 200, 0, 255 };
	selectedContent = -1;

	XRotation = 0; 
	YRotation = 0;
	CreateText();
}

GraphicsBase::~GraphicsBase()
{
}

void GraphicsBase::UpdateWindowSize(int newWidth, int newHeight)
{
	windowWidth = newWidth;
	windowHeight = newHeight;

	//update the projection matrix of the current graph
	SetProjection();
}

void GraphicsBase::UpdateData()
{
	switch (GetDisplayType())
	{
	case PieChart:
		LoadInData(data->GetPieFormat(), data->GetNumOfChosenAtts());		
		break;
	case BarGraph:
		LoadInData(data->GetBarFormat(), data->GetNumOfChosenAtts());
		break;
	case ScatterPlot2D:
		LoadInData(data->GetScat2DFormat(), data->GetNumOfChosenAtts());
		break;
	case ScatterPlot3D:
		LoadInData(data->GetScat3DFormat(), data->GetNumOfChosenAtts());
		break;
	case LineGraph:
		LoadInData(data->GetLineFormat(), data->GetNumOfChosenAtts());
		break;
	case SpiderChart:
		LoadInData(data->GetSpiderFormat(), data->GetNumOfChosenAtts());
		break;
	}
}

void GraphicsBase::DeselectData()
{
	selectedContent = -1;
}

void GraphicsBase::Scale(int change, float mouseX, float mouseY)
{
	float oldScale = scale;
	if (change > 0)
	{
		scale = scale * 2.0f;
	}
	else
	{
		scale = scale * 0.5f;
	}
	

	if ((scale < 1)||(scale > 200))
	{
		scale = oldScale;
	}
	else
	{
		//set mouse as centre
		sourceY -= mouseY;
		sourceX -= mouseX;

		if (change > 0)
		{
			//scale around centre
			sourceY = sourceY * 2.0f;
			sourceX = sourceX * 2.0f;
		}
		else
		{
			//scale around centre
			sourceY = sourceY * 0.5f;
			sourceX = sourceX * 0.5f;
		}		

		//unset mouse as centre
		sourceY += mouseY;
		sourceX += mouseX;
	}	

	MaintainBounds();
}

void GraphicsBase::MoveTo(int newX, int newY)
{
	sourceX = newX;
	sourceY = newY;
	MaintainBounds();
}

void GraphicsBase::MoveBy(int dX, int dY)
{
	sourceX += dX;
	sourceY += dY;
	MaintainBounds();
}

void GraphicsBase::Rotate(float XDif, float YDif)
{
	YRotation -= XDif;
	//printf("XDif: %f, XRotation: %f\n", XDif, XRotation);
	XRotation -= YDif;
	//printf("YDif: %f, XRotation: %f\n", YDif, YRotation);

	if (XRotation > 360)
	{
		XRotation -= 360;
	}
	else if (XRotation < -360)
	{
		XRotation += 360;
	}

	if (YRotation > 360)
	{
		YRotation -= 360;
	}
	else if (YRotation < -360)
	{
		YRotation += 360;
	}
}

void GraphicsBase::SetProjection()
{
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//default to orthographic projection
	glOrtho(-0.5*windowWidth, 0.5*windowWidth, -0.5*windowHeight, 0.5*windowHeight, 0.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GraphicsBase::CreateText()
{
	selectedText = GUIText("franklin_gothic.glf");
}

void GraphicsBase::DrawAxis()
{
	//get the coords of the edges of the window
	int top = sourceY + (windowHeight - 100)*scale, bottom = sourceY - 100 * scale;
	int right = sourceX + (windowWidth - 100)*scale, left = sourceX - 100 * scale;

	//draw two white lines producing standard graph axes
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);

	glVertex3f(sourceX, top, 0);
	glVertex3f(sourceX, bottom, 0);

	glVertex3f(left, sourceY, 0);
	glVertex3f(right, sourceY, 0);

	glEnd();
}

void GraphicsBase::MaintainBounds()
{
	//prevent the graph from going off screen
	if (GetLeft() > windowWidth / -2)
	{
		SetLeft(windowWidth / -2);
	}
	else if (GetRight() < windowWidth / 2)
	{
		SetRight(windowWidth / 2);
	}

	if (GetTop() < windowHeight / 2)
	{
		SetTop(windowHeight / 2);
	}
	else if (GetBottom() > windowHeight / -2)
	{
		SetBottom(windowHeight / -2);
	}
}

float GraphicsBase::GetLeft()
{
	return sourceX - 100 * scale;
}

void GraphicsBase::SetLeft(float newLeft)
{
	sourceX = newLeft + 100 * scale;
}

float GraphicsBase::GetRight()
{
	return sourceX + (windowWidth - 100) * scale;
}

void GraphicsBase::SetRight(float newRight)
{
	sourceX = (newRight - windowWidth * scale) + 100 * scale;
}

float GraphicsBase::GetTop()
{
	return sourceY + (windowHeight - 100) * scale;
}

void GraphicsBase::SetTop(float newTop)
{
	sourceY = (newTop - windowHeight * scale) + 100 * scale;
}

float GraphicsBase::GetBottom()
{
	return sourceY - 100 * scale;
}

void GraphicsBase::SetBottom(float newBottom)
{
	sourceY = newBottom + 100 * scale;
}

RGBColour GraphicsBase::GetRandomColours()
{
	//get a random red int between 100 and 200, so the colour isn't too dark or too light
	int red = rand() % 100 + 100;
	//get random green/blue ints between 0 and 200 so the colour isn't too light
	int green = rand() % 200;
	int blue = rand() % 200;

	return{ red, green, blue };
}



