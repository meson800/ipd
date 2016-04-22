#include "GeneticAlgorithm.h"
#include "Fitness.h"

#include <iostream>

int main()
{
	GeneticAlgorithm currentRun = GeneticAlgorithm(.25, .15, .5, .1, 1000, 2);
	for (unsigned int i = 0; i < 100; ++i)
	{
		currentRun.runGeneration(FitnessFunctions::normalPrisonersDilemma);
		std::cout << "Generation " << i << ": Best: " << currentRun.maxFitness() << " Mean: " << currentRun.meanFitness()
			<< " std: " << currentRun.fitnessDeviation() << "\n";
	}
	system("pause");
	return 0;
}