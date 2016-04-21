#include "Genome.h"
#include "Exceptions.h"
#include "Helpers.h"

#include <random>

Genome::Genome(unsigned int genomeSize)
{
	genome.reserve(genomeSize);
	//init with random values
	std::uniform_int_distribution<int> distribution(0, 255);
	for (unsigned int i = 0; i < genomeSize; ++i)
		genome.push_back((unsigned char)distribution(Helpers::generator));

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
		std::uniform_int_distribution<int> distribution(0, 255);
		for (unsigned int i = 0; i < genomeLength(); ++i)
		{
			//OR the first byte with a random byte, then AND that with the RHS
			//this guarantees that if both bits are 0, we get 0 (same with 1), but
			//we have a 50/50 chance of getting 0 or 1 if those are the parents, as desired
			genome[i] = (genome[i] | distribution(Helpers::generator)) & rhs.getByte(i);
		}
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

bool Genome::getBit(unsigned int idx) const
{
	if (idx > genomeLength() * 8)
		throw GenomeException("Bit out of range");

	unsigned int byte = idx / 8;
	unsigned int bit = idx % 8;
	return (genome[byte] & (1 << bit)) != 0;
}

void Genome::mutateBit(unsigned int idx)
{
	if (idx >= genomeLength() * 8)
		throw GenomeException("Requested mutation out of range");

	unsigned int byte = idx / 8;
	unsigned int bit = idx % 8;
	genome[byte]  = genome[byte] ^ (1 << bit);
}
