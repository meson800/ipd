#pragma once
#include "Genome.h"
#include <deque>
#include <random>

class Helpers
{
public:
	static std::default_random_engine generator;
	static double normalCDF(double z);
	static unsigned int dequeToInt(const std::deque<bool>& history);
};