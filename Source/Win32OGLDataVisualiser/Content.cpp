#include "Content.h"


Content::Content()
{
	lastMouse = { false, false, 0, 0 };
	currentFile = Data();
	display = new GraphicsPieChart(1000, 800, &currentFile);
}

Content::~Content()
{
}

void Content::SetControlContents(ControlWindowContents* givenCC)
{
	controlContents = givenCC;
}

void Content::CreateText()
{
	display->CreateText();
}

void Content::Render(HDC _hDc)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();	
		
	//render the graph
	display->Render();

	glFlush();

	SwapBuffers(_hDc);	
}

void Content::ReadControlInput(WPARAM wParam)
{
	int changedBox;
	switch (HIWORD(wParam))
	{
	case CBN_SELCHANGE:
		changedBox = controlContents->GetChangedBox();
		currentFile.SetAttribute(changedBox, controlContents->GetChosenItem(changedBox));	
		display->UpdateData();
		break;
	default:
		break;
	}

	GraphicsBase* oldDisplay = display;
	switch (LOWORD(wParam))
	{
	case ID_LOAD_FILE:
		if (currentFile.LoadCSV())
		{
			controlContents->SetAttributes(currentFile.GetHeadings());
			display->UpdateData();
			controlContents->UpdateForGraph(display->GetDisplayType());
		}
		break;
	case ID_GRAPH_PIE:
		currentFile.ResetAttributeSelectors();
		display = new GraphicsPieChart(display->GetWindowWidth(), display->GetWindowHeight(), &currentFile);
		display->SetProjection();
		display->UpdateData();
		controlContents->UpdateForPie();
		delete(oldDisplay);
		break;
	case ID_GRAPH_BAR:
		currentFile.ResetAttributeSelectors();
		display = new GraphicsBarGraph(display->GetWindowWidth(), display->GetWindowHeight(), &currentFile);
		display->SetProjection();
		display->UpdateData();
		controlContents->UpdateForBar();
		delete(oldDisplay);
		break;
	case ID_GRAPH_SCAT2D:
		currentFile.ResetAttributeSelectors();
		display = new GraphicsScatterPlot2D(display->GetWindowWidth(), display->GetWindowHeight(), &currentFile);
		display->SetProjection();
		display->UpdateData();
		controlContents->UpdateForScat2D();
		delete(oldDisplay);
		break;
	case ID_GRAPH_SCAT3D:
		currentFile.ResetAttributeSelectors();
		display = new GraphicsScatterPlot3D(display->GetWindowWidth(), display->GetWindowHeight(), &currentFile);
		display->SetProjection();
		display->UpdateData();
		controlContents->UpdateForScat3D();
		delete(oldDisplay);
		break;
	case ID_GRAPH_LINE:
		currentFile.ResetAttributeSelectors();
		display = new GraphicsLineGraph(display->GetWindowWidth(), display->GetWindowHeight(), &currentFile);
		display->SetProjection();
		display->UpdateData();
		controlContents->UpdateForLine();
		delete(oldDisplay);
		break;
	case ID_GRAPH_SPIDER:
		currentFile.ResetAttributeSelectors();
		display = new GraphicsSpiderChart(display->GetWindowWidth(), display->GetWindowHeight(), &currentFile);
		display->SetProjection();
		display->UpdateData();
		controlContents->UpdateForSpider();
		delete(oldDisplay);
		break;
	default:
		break;
	}
}

void Content::MoveMouse(float newX, float newY)
{
	if (lastMouse.LMB)
	{
		display->MoveBy(newX - lastMouse.x, newY - lastMouse.y);
	}	
	if (lastMouse.RMB)
	{
		display->Rotate(newX - lastMouse.x, newY - lastMouse.y);
	}
	lastMouse.x = newX;
	lastMouse.y = newY;
}

void Content::LeftButtonDown()
{
	if (!lastMouse.LMB)
	{
		controlContents->UpdateSelectedData(display->SelectData(lastMouse.x, lastMouse.y));
	}
	lastMouse.LMB = true;
}

void Content::LeftButtonUp()
{
	if (lastMouse.LMB)
	{
		display->DeselectData();
	}
	lastMouse.LMB = false;
}

void Content::RightButtonDown()
{
	lastMouse.RMB = true;
}

void Content::RightButtonUp()
{
	lastMouse.RMB = false;
}

void Content::MouseScroll(int amount)
{
	if (amount < 60000)
	{
		display->Scale(120, lastMouse.x, lastMouse.y);
	}
	else
	{
		display->Scale(-120, lastMouse.x, lastMouse.y);
	}
}

void Content::ResizeWindow(int newWidth, int newHeight)
{
	display->UpdateWindowSize(newWidth, newHeight);
}


