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
	static unsigned int log2(unsigned int x);
	static void incrementCoop();
	static unsigned int resetCoop();
	static std::vector<unsigned char> stringToBytes(const std::string& input);

private:
	static unsigned int numCoops;
};