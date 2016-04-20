#pragma once
#include <vector>

class Genome
{
public:
	Genome(unsigned int genomeSize);
	Genome & operator=(const Genome &rhs);
	Genome & operator*=(const Genome &rhs);
	Genome operator*(const Genome &other) const;
	unsigned int genomeLength(void) const;
	unsigned char getByte(unsigned int idx) const;

private:
	std::vector<unsigned char> genome;

};