#pragma once
#include "Genome.h"
#include <deque>

class Helpers
{
public:
	static double normalCDF(double z);
	static unsigned int dequeToInt(const std::deque<bool>& history);
};