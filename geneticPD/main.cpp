#include "GeneticAlgorithm.h"
#include "Fitness.h"

#include <iostream>
#include <sstream>
#include <fstream>

int main()
{
	
	std::cout << "Pick which prisoner's dilemma you want:\n1) Normal\n2) Death after Three Defects\n";
	std::string input;
	std::getline(std::cin, input);
	std::istringstream ss(input);
	int whichDilemma = 0;
	ss >> whichDilemma;
	std::vector<double>(*fitnessFunction)(const std::vector<Genome>&);
	switch (whichDilemma)
	{
	default:
	case 1:
		fitnessFunction = FitnessFunctions::normalPrisonersDilemma;
		break;
	case 2:
		fitnessFunction = FitnessFunctions::deathPrisonersDilemma;
		break;
	}

	//generate a file with data to plot
	std::cout << "Enter file name to save generation data:";
	std::getline(std::cin, input);
	std::ofstream file;
	file.open(input, std::iostream::app);
	file << "Generation,Best,Mean,Mean STD\n";
	file.close();

	GeneticAlgorithm currentRun = GeneticAlgorithm(.25, .15, .5, .0, 1000, 4);
	for (unsigned int i = 0; i < 100; ++i)
	{
		currentRun.runGeneration(fitnessFunction);
		std::cout << "Generation " << i << ": Best: " << currentRun.maxFitness() << " Mean: " << currentRun.meanFitness()
			<< " std: " << currentRun.fitnessDeviation() << "\n";
		file.open(input, std::iostream::app);
		file << i << "," << currentRun.maxFitness() << "," << currentRun.meanFitness() << "," << currentRun.fitnessDeviation() << "\n";
		file.close();
		
	}
	system("pause");
	return 0;
}