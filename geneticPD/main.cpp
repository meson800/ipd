#include "GeneticAlgorithm.h"
#include "Fitness.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include "Helpers.h"

int main()
{
	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();

	// obtain a seed from the timer
	myclock::duration d = myclock::now() - beginning;
	unsigned seed2 = d.count();
	Helpers::generator.seed(seed2);

	std::cout << "Pick which prisoner's dilemma you want:\n1) Normal\n2) Death after Three Defects\n3) Death after Three Suckers\n4)Human normal\n5)Human Defect Death\n6)Human Sucker Death\n";
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
	case 3:
		fitnessFunction = FitnessFunctions::suckerPrisonersDilemma;
		break;
	case 4:
	{
		std::cout << "Enter organism data:";
		std::getline(std::cin, input);
		unsigned int humanFitness = 0;
		unsigned int aiFitness = 0;
		Genome organism(Helpers::stringToBytes(input));
		std::cout << organism.printGenome() << "\n";
		FitnessFunctions::humanPrisonersDilemma(organism, humanFitness, aiFitness);
		std::cout << "Human fitness: " << humanFitness << " AI fitness: " << aiFitness << "\n";
		system("pause");
		return 0;
		break;
	}
	case 5:
	{
		std::cout << "Enter organism data:";
		std::getline(std::cin, input);
		unsigned int humanFitness = 0;
		unsigned int aiFitness = 0;
		Genome organism(Helpers::stringToBytes(input));
		std::cout << organism.printGenome() << "\n";
		FitnessFunctions::humanDeathPrisonersDilemma(organism, humanFitness, aiFitness);
		std::cout << "Human fitness: " << humanFitness << " AI fitness: " << aiFitness << "\n";
		system("pause");
		return 0;
		break;
	}
	case 6:
	{
		std::cout << "Enter organism data:";
		std::getline(std::cin, input);
		unsigned int humanFitness = 0;
		unsigned int aiFitness = 0;
		Genome organism(Helpers::stringToBytes(input));
		std::cout << organism.printGenome() << "\n";
		FitnessFunctions::humanSuckerPrisonersDilemma(organism, humanFitness, aiFitness);
		std::cout << "Human fitness: " << humanFitness << " AI fitness: " << aiFitness << "\n";
		system("pause");
		return 0;
		break;
	}
	}

	//generate a file with data to plot
	std::cout << "Enter file name to save generation data:";
	std::getline(std::cin, input);
	std::ofstream file;
	file.open(input, std::iostream::app);
	file << "Generation,Best,Mean,Mean STD, Mean Cooperation, Mean Cooperation STD, Mean Game Coop\n";
	file.close();

	GeneticAlgorithm currentRun = GeneticAlgorithm(.25, .15, .5, .0, 1000, 4);
	for (unsigned int i = 0; i < 100; ++i)
	{
		currentRun.runGeneration(fitnessFunction);
		std::cout << "Generation " << i << ": Best: " << currentRun.maxFitness() << " Mean: " << currentRun.meanFitness()
			<< " std: " << currentRun.fitnessDeviation() << " meanCoop: " << currentRun.meanCoop() << 
			" meanCoopSTD: " << currentRun.meanCoopDeviation() << " meanGameCoop: " << currentRun.meanGameCoop() << "\n";
		file.open(input, std::iostream::app);
		file << i << "," << currentRun.maxFitness() << "," << currentRun.meanFitness() << "," << currentRun.fitnessDeviation() 
			<< "," << currentRun.meanCoop() << "," << currentRun.meanCoopDeviation() << "," << currentRun.meanGameCoop() << "\n";
		file.close();
		
	}
	system("pause");
	return 0;
}