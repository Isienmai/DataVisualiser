#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm/glm.hpp>

#include <string>

#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <time.h>
#include <vector>
#include <algorithm>

#include "UsefulStructs.h"
#include "Data.h"
#include "GUIText.h"

enum GraphicsTypes
{
	Base,
	PieChart,
	BarGraph,
	ScatterPlot2D,
	ScatterPlot3D,
	LineGraph,
	SpiderChart
};

//Base graphics class. Is the top of the graphics class inheritance structure
class GraphicsBase
{
protected:
	//stores a pointer to the data 
	Data* data;
	//stores the size of the window
	float windowWidth, windowHeight;

	//stores the course coordinates of the graph
	float sourceX, sourceY;
	//stores the graph's scale
	float scale;
	//stores the graph's rotation
	float XRotation, YRotation;
	
	//stores the id of the selected data
	int selectedContent;
	//stores the colour that selected data is displayed as
	RGBColour selectedColour;

	//stores the GUIText which displays the details of the selected data
	GUIText selectedText;

public:
	GraphicsBase();
	//takes in the size of the window and a pointer to the data as initial values
	GraphicsBase(int width, int height, Data* theData);
	~GraphicsBase();

	//modifies the source and window size values according to the new window size and the current type of graph
	virtual void UpdateWindowSize(int newWidth, int newHeight);
	//returns the current window dimensions
	inline int GetWindowWidth() { return windowWidth; }
	inline int GetWindowHeight() { return windowHeight; }

	//renders the graph
	virtual void Render() = 0;
	//loads data into the graph
	virtual void LoadInData(std::vector<std::string> givenData, int numberOfAttributes) = 0;
	//returns the graph type
	virtual int GetDisplayType() = 0;
	//updates the contents of the current graph
	void UpdateData();

	//gets the data at (pointX,pointY)
	virtual std::string SelectData(float pointX, float pointY) = 0;
	//resets the selected data variables
	void DeselectData();

	//update the scale of the graph
	virtual void Scale(int change, float mouseX, float mouseY);

	//update the source point of the graph based on a change in, or a completely new, (X,Y)
	void MoveTo(int newX, int newY);
	void MoveBy(int dX, int dY);	

	//update the rotation based on a change in X and Y
	void Rotate(float XDif, float YDif);

	//Update the projection matrix
	virtual void SetProjection();

	//load the selected text with a font
	void CreateText();

protected:
	//Draw the axes
	virtual void DrawAxis();
	
	//Keep the graph within reasonable bounds of the screen
	virtual void MaintainBounds();	

	//get the coords of the different edges of the graph
	virtual float GetLeft();
	virtual void SetLeft(float newLeft);
	virtual float GetRight();
	virtual void SetRight(float newLeft);
	virtual float GetTop();
	virtual void SetTop(float newLeft);
	virtual float GetBottom();
	virtual void SetBottom(float newLeft);

	//return a random colour
	RGBColour GetRandomColours();
};


class GraphicsPieChart : public GraphicsBase
{
	//stores a list of values and the quantity of each 
	std::vector<TallyValue> tallies;
	//stores the total number of values (a.k.a a sum of all quantities)
	int tallyLength;

	//stores the radius of the pie
	int radius;

public:
	GraphicsPieChart(int width, int height, Data* theData);
	~GraphicsPieChart();

	void Render();
	void LoadInData(std::vector<std::string> givenData, int numberOfAttributes);
	int GetDisplayType();

private:
	//Draw the pie chart
	void DrawChart();
	//Draw a segment of the pie chart that represents a chosen piece of data
	void DrawSegment(int segmentNumber);

public:
	//return a description of the selected data
	std::string SelectData(float pointX, float pointY);

private:
	//returns the id number of the segment that the given point is pointing at
	int GetSelectedSegment(float pointX, float pointY);
	//returns true if the given point is pointing at the chart
	bool PointInChart(int pointX, int pointY);

	void MaintainBounds();	

	float GetLeft();
	void SetLeft(float newLeft);
	float GetRight();
	void SetRight(float newRight);
	float GetTop();
	void SetTop(float newTop);
	float GetBottom();
	void SetBottom(float newBottom);
};


class GraphicsBarGraph : public GraphicsBase
{
protected:
	std::vector<TallyValue> tallies;
	//stores the heights of the bars that represent the data in tallies
	std::vector<int> heights;
	//max height stores the maximum height
	int tallyLength, maxHeight;

public:
	GraphicsBarGraph(int width, int height, Data* theData);
	~GraphicsBarGraph();

	virtual void Render();
	virtual void LoadInData(std::vector<std::string> givenData, int numberOfAttributes);
	virtual int GetDisplayType();

	virtual void UpdateWindowSize(int newWidth, int newHeight);

protected:
	//draw the bars
	virtual void DrawGraph();
	//draw the bar for a given piece of data with a given height
	virtual void DrawBar(int barNumber, int barHeight);

public:
	virtual std::string SelectData(float pointX, float pointY);

protected:
	virtual int GetSelectedSegment(float pointX, float pointY);
};


class GraphicsScatterPlot2D : public GraphicsBarGraph
{
protected:
	//true if the Y axis represents quantities
	bool tallied;
	//stores a list of unique value pairs and their quantity
	std::vector<DataPair> dataPairs;
	//stores all of the unique values. the values within data pairs reference these vectors
	std::vector<std::string> uniqueContent1, uniqueContent2;

public:
	GraphicsScatterPlot2D(int width, int height, Data* theData);
	~GraphicsScatterPlot2D();

	virtual void LoadInData(std::vector<std::string> givenData, int numberOfAttributes);
protected:
	//split the provided data into the two component attributes
	void SplitIntoTwo(std::vector<std::string> source, std::vector<std::string>* result1, std::vector<std::string>* result2);
	//extracts the unique values from one list of strings and stores them in another
	void GetUniqueValues(std::vector<std::string>* goal, std::vector<std::string> source);
	//turns the provided data into a list of references to the unique data lists
	//takes the references, tallies up the number of times each unique pair of values appears
	//stores the list of value pair tallies and stores it in dataPairs
	void FormatDataAndStore(std::vector<std::string> source);

public:
	virtual int GetDisplayType();

protected:
	//draws all the points
	virtual void DrawGraph();
	//draws a given piece of data as a point at a given height
	void DrawBar(int barNumber, int barHeight);
	//Draw a point at a given location
	virtual void DrawPoint(double centreX, double centreY);

public:
	virtual std::string SelectData(float pointX, float pointY);

protected:
	//find the point that is pointed to
	virtual int GetSelectedSegment(float pointX, float pointY);
};


class GraphicsScatterPlot3D : public GraphicsScatterPlot2D
{
	//same purpose as in scatter plot 2D, except with a 3rd axis
	std::vector<DataTrio> dataTrios;
	std::vector<std::string> uniqueContent3;
	//stores the near and far points of the last mouse ray
	glm::vec3 nearPointt, farPointt;
	
	//stores the location of the selected point
	float selectedX, selectedY, selectedZ;

public:
	GraphicsScatterPlot3D(int width, int height, Data* theData);
	~GraphicsScatterPlot3D();

	void Render();
	void LoadInData(std::vector<std::string> givenData, int numberOfAttributes);
private:
	//splits given data into three attributes
	void SplitIntoThree(std::vector<std::string> source, std::vector<std::string>* result1, std::vector<std::string>* result2, std::vector<std::string>* result3);
	//stores unique triples of data
	void FormatDataAndStore(std::vector<std::string> source);
	//stores unique triples of data, storing a quantity as the 3rd piece of data
	void FormatDataAndStoreTally(std::vector<std::string> source);

public:
	int GetDisplayType();
	void UpdateWindowSize(int newWidth, int newHeight);

private:
	void DrawGraph();
	//draw a point to represent the data pointed to by the provided index
	void DrawPoint(int barNumber);
	//draw a cube at a given point in 3D space
	void DrawCube(float centreX, float centreY, float centreZ);
	//draw the three axes in 3D space
	void DrawAxis();

public:
	std::string SelectData(float pointX, float pointY);

	//change the projection to be perspective rather than orthographic
	void SetProjection();

	void Scale(int change, float mouseX, float mouseY);

private:
	int GetSelectedSegment(float pointX, float pointY);
	//check to see if a given point is close to a given line
	float CheckLineIntersection(glm::vec3 mouse, glm::vec3 line1, glm::vec3 line2);

	void MaintainBounds();
};

class GraphicsLineGraph : public GraphicsBarGraph
{
private:
	//the list of lines. all lines are separated by a tally value with quantity -1 and colour 0,0,0
	std::vector<TallyValue> lines;
	//a list of point coordinates, each one representing a pice of data stored in lines
	std::vector<Point> points;
	//stores the indexes of the breakpoints between the lines within the line vector
	std::vector<int> lineEndPoints;

public:
	GraphicsLineGraph(int width, int height, Data* theData);
	~GraphicsLineGraph();

	void Render();
private:
	void DrawGraph();
	void DrawPoint(double centreX, double centreY);

public:
	void LoadInData(std::vector<std::string> givenData, int numberOfAttributes);
private:
	//used to sort tally values in alphabetical order of the content
	static bool GraphicsLineGraph::CompareByContent(const TallyValue &a, const TallyValue &b);
	//splits given data into a given number of attributes
	void SplitDataIntoAttributes(std::vector<std::string> givenData, int numOfAtts);
	//sorts each line separately from the others and stores them all end to end within the lines vector
	void OrganiseLines(int numOfAtts);
	//turns each line into a list of points to display
	void TurnLinesIntoPoints(int numOfAtts);

public:
	int GetDisplayType();

	std::string SelectData(float pointX, float pointY);
	
private:
	//get the id of the selected line, if there is one
	int GetSelectedLine(float pointX, float pointY);
	//get the id of the selected point
	int GetSelectedPoint(float pointX, float pointY, int chosenLine);

	//Check to see if a given point is close to a given line
	bool CheckLineIntersection(Point mouse, Point line1, Point line2);
};

class GraphicsSpiderChart : public GraphicsBase
{
	std::vector<TallyValue> tallies;
	//max size stores the largest quantity
	int tallyLength, maxSize;

	int radius;

public:
	GraphicsSpiderChart(int width, int height, Data* theData);
	~GraphicsSpiderChart();

	void Render();
	void LoadInData(std::vector<std::string> givenData, int numberOfAttributes);
	int GetDisplayType();

private:
	//draw the spider chart
	void DrawChart();
	//draw the frame of the web
	void DrawAxis();

public:
	std::string SelectData(float pointX, float pointY);

private:
	int GetSelectedSegment(float pointX, float pointY);
	int CheckLineIntersection(float pointX, float pointY, float lineX, float lineY);

	bool PointInChart(float pointX, float pointY);

	void MaintainBounds();

	float GetLeft();
	void SetLeft(float newLeft);
	float GetRight();
	void SetRight(float newRight);
	float GetTop();
	void SetTop(float newTop);
	float GetBottom();
	void SetBottom(float newBottom);
};