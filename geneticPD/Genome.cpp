#include "Genome.h"
#include "Exceptions.h"

#include <random>

Genome::Genome(unsigned int genomeSize)
{
	genome.reserve(genomeSize);
	//init with random values
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 255);
	for (unsigned int i = 0; i < genomeSize; ++i)
		genome.push_back((unsigned char)distribution(generator));

}

Genome & Genome::operator=(const Genome & rhs)
{
	if (this != &rhs)
	{
		//clear our genome, and copy the other persons.
		genome.clear();
		genome.reserve(rhs.genomeLength());
		for (unsigned int i = 0; i < rhs.genomeLength(); ++i)
			genome.push_back(rhs.getByte(i));
	}
	return *this;
}

Genome & Genome::operator*=(const Genome & rhs)
{
	if (this != &rhs)
	{
		//average the genes between us and the only genome, only if we are the same size.
		if (genomeLength() != rhs.genomeLength())
			throw GenomeException("Cannot mix genomes of different sizes");
		for (unsigned int i = 0; i < genomeLength(); ++i)
			genome[i] = (unsigned char)(((int)genome[i] + (int)rhs.getByte(i)) / 2);
	}

	return *this;
}

Genome Genome::operator*(const Genome & other) const
{
	Genome result = *this;
	result *= other;
	return result;
}

unsigned int Genome::genomeLength(void) const
{
	return genome.size();
}

unsigned char Genome::getByte(unsigned int idx) const
{
	if (idx >= genomeLength())
		throw GenomeException("Requested byte out of genome range");
	return genome[idx];
}
