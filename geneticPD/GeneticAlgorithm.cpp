#include "GeneticAlgorithm.h"
#include "Helpers.h"
#include <cmath>
#include <map>
#include <random>
#include <algorithm>
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm(double _mutationRate, double _mutationAmount, 
	double _reproductionCutoff, double _cloneCutoff, unsigned int _numOrganisms, unsigned int _genomeSize)
{
	mutationRate = _mutationRate;
	mutationAmount = _mutationAmount;
	reproductionCutoff = _reproductionCutoff;
	cloneCutoff = _cloneCutoff;
	numOrganisms = _numOrganisms;
	genomeSize = _genomeSize;
	for (unsigned int i = 0; i < numOrganisms; ++i)
		organisms.push_back(Genome(genomeSize));

}

double GeneticAlgorithm::maxFitness(void) const
{
	return bestFitness;
}

double GeneticAlgorithm::meanFitness(void) const
{
	return averageFitness;
}

double GeneticAlgorithm::fitnessDeviation(void) const
{
	return fitnessStd;
}

void GeneticAlgorithm::runGeneration(std::vector<double>(*fitnessFunction)(const std::vector<Genome>&))
{
	//pass our organisms to the fitness function, and get the fitnesses back.
	fitness = fitnessFunction(organisms);
	//find the mean and STD of organisms fitnesses.
	double best = -9999999;
	double mean = 0;
	double std = 0;

	//sort the list of organisms by fitness
	std::vector<std::pair<unsigned int, double>> fitnessSort;

	for (unsigned int i = 0; i < numOrganisms; ++i)
	{
		fitnessSort.push_back(std::pair<unsigned int, double>(i, fitness[i]));
		if (fitness[i] > best)
			best = fitness[i];
		mean += fitness[i];
	}
	mean /= numOrganisms;
	for (unsigned int i = 0; i < numOrganisms; ++i)
	{
		std += (fitness[i] - mean)*(fitness[i] - mean);
	}
	std = std::sqrt(std / numOrganisms);

	bestFitness = best;
	averageFitness = mean;
	fitnessStd = std;

	//now sort
	std::sort(fitnessSort.begin(), fitnessSort.end(),
		[](const std::pair<unsigned int, double> &a, const std::pair<unsigned int, double> &b)
	{ return a.second > b.second; });

	std::cout << organisms[fitnessSort[0].first].printGenome();

	//now calculate our cutoff variables - the index below which we should reproduce and clone
	unsigned int cutoffReproduce = (unsigned int)((reproductionCutoff) * (numOrganisms - 1));
	unsigned int cutoffClone = (unsigned int)((cloneCutoff) * (numOrganisms - 1));

	//now create our cloned organisms
	unsigned int numNewOrganisms = 0;
	std::vector<Genome> newOrganisms;
	for (unsigned int i = 0; i < cutoffClone; ++i)
	{
		//clone- create two new organisms in the new population
		newOrganisms.push_back(organisms[fitnessSort[i].first]);
		newOrganisms.push_back(organisms[fitnessSort[i].first]);
		numNewOrganisms += 2;
	}
	//now arbitrarily reproduce two organisms above the reproduction cutoff
	std::uniform_int_distribution<int> distribution(0,cutoffReproduce);
	std::uniform_real_distribution<double> randDistribution(0,1);
	std::uniform_int_distribution<int> bitDistribution(0, (genomeSize * 8) - 1);
	while (numNewOrganisms < numOrganisms)
	{
		newOrganisms.push_back(organisms[fitnessSort[distribution(Helpers::generator)].first]
			* organisms[fitnessSort[distribution(Helpers::generator)].first]);
		++numNewOrganisms;
	}

	//now mutate randomly
	for (unsigned int i = 0; i < numOrganisms; ++i)
	{
		if (randDistribution(Helpers::generator) > mutationRate)
		{
			for (unsigned int j = 0; j < genomeSize*8*mutationAmount; ++j)
				newOrganisms[i].mutateBit(bitDistribution(Helpers::generator));
		}
	}

	organisms = newOrganisms;
}
