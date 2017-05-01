#pragma once
#include <string>
#include <vector>
#include "UsefulStructs.h"

class DataModerator
{
public:
	DataModerator();
	~DataModerator();

	std::vector<TallyValue> ModeratePieChart(std::vector<std::string> toMod);
};

