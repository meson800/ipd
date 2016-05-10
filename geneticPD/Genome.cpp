#include "Genome.h"
#include "Exceptions.h"
#include "Helpers.h"

#include <random>
#include <sstream>
#include <bitset>

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
			//We want if both bits are 0, we get 0 (same with 1), but
			//we have a 50/50 chance of getting 0 or 1 if those are the parents, as desired

			//the solution, done with the help of Denko and Zach, is
			// (a | b) & (a | r) & (b | r)
			unsigned char first = genome[i];
			unsigned char second = rhs.getByte(i);
			unsigned char rand = distribution(Helpers::generator);

			genome[i] = (first | second) & (first | rand) & (second | rand);
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

std::string Genome::printGenome(void) const
{
	//find number of history steps we went
	unsigned int numHistory = Helpers::log2(genome.size() * 8);
	unsigned int numBits = genome.size() * 8;
	std::ostringstream builder;

	for (unsigned int i = 0; i < genome.size() * 8; ++i)
	{
		//print out the sequence of history. i is the current number we want to give to bitset.
		//The number of bytes required will be numHistory / 8
		unsigned int numBytes = numHistory / 8;
		if (numBytes == 0)
			numBytes = 1;
		for (unsigned int currentByte = 0; currentByte < numBytes; ++currentByte)
		{
			std::bitset<8> outputByte = std::bitset<8>(i >> (8 * currentByte) & 0xff);
			builder << outputByte.to_string('D', 'C');
		}
		//now print out what we would do
		builder << ":" << (getBit(i) ? 'C' : 'D') << "\n";
	}
	//print out raw bytes
	builder << "raw:";
	for (unsigned int i = 0; i < genome.size(); ++i)
		builder << std::hex << (int)(genome[i]);
	builder << "\n";
	return builder.str();
}
