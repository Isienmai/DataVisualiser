#include "DataModerator.h"


DataModerator::DataModerator()
{
}


DataModerator::~DataModerator()
{
}

std::vector<TallyValue> DataModerator::ModeratePieChart(std::vector<std::string> toMod)
{
	std::vector<TallyValue> toReturn;
	RGBColour tempColour = {0,0,0};
	bool added;

	for (int i = 0; i < toMod.size(); i++)
	{
		added = false;
		for (int j = 0; j < toReturn.size(); j++)
		{
			if (toMod[i] == toReturn[j].content)
			{
				added = true;
				toReturn[j].quantity++;
				j = toReturn.size();
			}
		}

		if (!added)
		{
			toReturn.push_back({ 1, toMod[i], tempColour });
		}

		if (i % 1000 == 0) std::printf("Loaded %d,", i);
	}

	return toReturn;
}
