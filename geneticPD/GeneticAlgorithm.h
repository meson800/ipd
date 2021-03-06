#pragma once
#include "Genome.h"
class GeneticAlgorithm
{
public:
	GeneticAlgorithm(double _mutationRate, double _mutationAmount, double _reproductionCutoff, 
		double _cloneCutoff, unsigned int _numOrganisms, unsigned int genomeSize);
	double maxFitness(void) const;
	double meanFitness(void) const;
	double fitnessDeviation(void) const;
	double meanCoop(void) const;
	double meanCoopDeviation(void) const;
	double meanGameCoop(void) const;
	void runGeneration(std::vector<double>(*fitnessFunction)(const std::vector<Genome>&));

private:
	std::vector<Genome> organisms;
	std::vector<double> fitness;
	unsigned int numOrganisms;
	double bestFitness, fitnessStd, averageFitness, avgCoop, avgCoopStd, avgGameCoop;
	double mutationRate, mutationAmount, reproductionCutoff, cloneCutoff;
	unsigned int genomeSize;
};