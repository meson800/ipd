#pragma once
#include <vector>
#include <random>
#include <string>

class Genome
{
public:
	Genome(unsigned int genomeSize);
	Genome(std::vector<unsigned char> bytes);
	Genome & operator=(const Genome &rhs);
	Genome & operator*=(const Genome &rhs);
	Genome operator*(const Genome &other) const;
	unsigned int genomeLength(void) const;
	unsigned char getByte(unsigned int idx) const;
	bool getBit(unsigned int idx) const;
	void mutateBit(unsigned int idx);
	std::string printGenome(void) const;
	double meanCoop(void) const;

private:
	std::vector<unsigned char> genome;

};