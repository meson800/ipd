#include "Fitness.h"

#include <deque>
#include <random>
#include <iostream>
#include <sstream>
#include <bitset>
#include "Helpers.h"

#define REWARD_PAYOFF 5
#define TEMPTATION_PAYOFF 10
#define PUNISHMENT_PAYOFF 1


std::vector<double> FitnessFunctions::normalPrisonersDilemma(const std::vector<Genome>& organisms)
{
	return iteratedPrisonersDilemma(organisms, normalRulePD);
}

std::vector<double> FitnessFunctions::deathPrisonersDilemma(const std::vector<Genome>& organisms)
{
	return iteratedPrisonersDilemma(organisms, dieAfterThreeDefects);
}

std::vector<double> FitnessFunctions::suckerPrisonersDilemma(const std::vector<Genome>& organisms)
{
	return iteratedPrisonersDilemma(organisms, dieAfterThreeSuckers);
}

void FitnessFunctions::humanPrisonersDilemma(Genome organism, unsigned int& firstFitness, unsigned int& secondFitness)
{
	//find the size of the genome, and generate a history stack
	unsigned int genomeSize = organism.genomeLength();
	std::deque<bool> firstHistory;
	//fill history with random data
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 1);

	for (unsigned int i = 0; i < Helpers::log2(genomeSize * 8); ++i)
		firstHistory.push_front((bool)(distribution(generator)));
	std::deque<bool> secondHistory = firstHistory;

	//now run the PD
	for (unsigned int i = 0; i < 100; ++i)
	{
		//human is the first player
		//display the results

		for (unsigned int currentHistory = 0; currentHistory < secondHistory.size(); ++currentHistory)
		{
			if (secondHistory[currentHistory])
				std::cout << "C";
			else
				std::cout << "D";
		}
		std::cout << "\n";

		std::string input;
		std::getline(std::cin, input);
		std::istringstream ss(input);
		int choice = 0;
		ss >> choice;

		if (choice == 2)
		{
			//exit early.
			return;
		}

		bool firstResult = (choice == 1);
		bool secondResult = organism.getBit(Helpers::dequeToInt(firstHistory));
		if (firstResult)
			Helpers::incrementCoop();
		if (secondResult)
			Helpers::incrementCoop();
		std::cout << "\n";

		if (firstResult && secondResult)
		{
			//both cooperated
			firstFitness += REWARD_PAYOFF;
			secondFitness += REWARD_PAYOFF;
			firstHistory.push_front(1);
			secondHistory.push_front(1);
			std::cout << "Reward\n";
		}
		if (!firstResult && secondResult)
		{
			//first defected, second cooperated
			firstFitness += TEMPTATION_PAYOFF;
			firstHistory.push_front(0);
			secondHistory.push_front(1);
			std::cout << "Temptation\n";
		}
		if (firstResult && !secondResult)
		{
			//first cooperated, second defected
			secondFitness += TEMPTATION_PAYOFF;
			firstHistory.push_front(1);
			secondHistory.push_front(0);
			std::cout << "Sucker\n";
		}
		if (!firstResult && !secondResult)
		{
			//both defected
			firstFitness += PUNISHMENT_PAYOFF;
			secondFitness += PUNISHMENT_PAYOFF;
			firstHistory.push_front(0);
			secondHistory.push_front(0);
			std::cout << "Punishment\n";
		}
		firstHistory.pop_back();
		secondHistory.pop_back();
	}
}

void FitnessFunctions::humanDeathPrisonersDilemma(Genome organism, unsigned int& firstFitness, unsigned int& secondFitness)
{
	return;
}

void FitnessFunctions::humanSuckerPrisonersDilemma(Genome organism, unsigned int& firstFitness, unsigned int& secondFitness)
{
	return;
}

std::vector<double> FitnessFunctions::iteratedPrisonersDilemma(const std::vector<Genome>& organisms,
	void(*pdFunction)(const Genome &, const Genome &, unsigned int &, unsigned int &))
{
	std::vector<double> fitness;
	for (unsigned int i = 0; i < organisms.size(); ++i)
		fitness.push_back(0);
	for (unsigned int i = 0; i < organisms.size(); ++i)
	{
		for (unsigned int j = i + 1; j < organisms.size(); ++j)
		{
			unsigned int firstFitness, secondFitness;
			firstFitness = 0;
			secondFitness = 0;
			pdFunction(organisms[i], organisms[j], firstFitness, secondFitness);
			fitness[i] += firstFitness;
			fitness[j] += secondFitness;
		}
	}
	for (unsigned int i = 0; i < organisms.size(); ++i)
	{
		fitness[i] /= (organisms.size());
	}
	return fitness;
}

void FitnessFunctions::normalRulePD(const Genome & first, const Genome & second,
	unsigned int& firstFitness, unsigned int& secondFitness)
{
	//find the size of the genome, and generate a history stack
	unsigned int genomeSize = first.genomeLength();
	std::deque<bool> firstHistory;
	//fill history with random data
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 1);

	for (unsigned int i = 0; i < Helpers::log2(genomeSize * 8); ++i)
		firstHistory.push_front((bool)(distribution(generator)));
	std::deque<bool> secondHistory = firstHistory;

	//now run the PD
	for (unsigned int i = 0; i < 100; ++i)
	{
		bool firstResult = first.getBit(Helpers::dequeToInt(secondHistory));
		bool secondResult = second.getBit(Helpers::dequeToInt(firstHistory));
		if (firstResult)
			Helpers::incrementCoop();
		if (secondResult)
			Helpers::incrementCoop();

		if (firstResult && secondResult)
		{
			//both cooperated
			firstFitness += REWARD_PAYOFF;
			secondFitness += REWARD_PAYOFF;
			firstHistory.push_front(1);
			secondHistory.push_front(1);
		}
		if (!firstResult && secondResult)
		{
			//first defected, second cooperated
			firstFitness += TEMPTATION_PAYOFF;
			firstHistory.push_front(0);
			secondHistory.push_front(1);
		}
		if (firstResult && !secondResult)
		{
			//first cooperated, second defected
			secondFitness += TEMPTATION_PAYOFF;
			firstHistory.push_front(1);
			secondHistory.push_front(0);
		}
		if (!firstResult && !secondResult)
		{
			//both defected
			firstFitness += PUNISHMENT_PAYOFF;
			secondFitness += PUNISHMENT_PAYOFF;
			firstHistory.push_front(0);
			secondHistory.push_front(0);
		}
		firstHistory.pop_back();
		secondHistory.pop_back();
	}
}

void FitnessFunctions::dieAfterThreeDefects(const Genome & first, const Genome & second, 
	unsigned int & firstFitness, unsigned int & secondFitness)
{
	//find the size of the genome, and generate a history stack
	unsigned int genomeSize = first.genomeLength();
	std::deque<bool> firstHistory;
	//fill history with random data
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 1);

	for (unsigned int i = 0; i < Helpers::log2(genomeSize * 8); ++i)
		firstHistory.push_front((bool)(distribution(generator)));
	std::deque<bool> secondHistory = firstHistory;

	//now run the PD
	for (unsigned int i = 0; i < 100; ++i)
	{
		//see if the last three times have been a defect for either party
		//check the first one first
		if (!firstHistory[0] && !firstHistory[1] && !firstHistory[2])
		{
			//we're done. Return 0 fitness for the first person, they lost.
			//For the second one, give it the +10 payoff for the rest of the rounds
			firstFitness = 0;
			secondFitness += (100 - i) * 10;
			return;
		}
		//do the other case
		if (!secondHistory[0] && !secondHistory[1] && !secondHistory[2])
		{
			secondFitness = 0;
			firstFitness += (100 - i) * 10;
			return;
		}
		bool firstResult = first.getBit(Helpers::dequeToInt(secondHistory));
		bool secondResult = second.getBit(Helpers::dequeToInt(firstHistory));

		if (firstResult)
			Helpers::incrementCoop();
		if (secondResult)
			Helpers::incrementCoop();

		if (firstResult && secondResult)
		{
			//both cooperated
			firstFitness += REWARD_PAYOFF;
			secondFitness += REWARD_PAYOFF;
			firstHistory.push_front(1);
			secondHistory.push_front(1);
		}
		if (!firstResult && secondResult)
		{
			//first defected, second cooperated
			firstFitness += TEMPTATION_PAYOFF;
			firstHistory.push_front(0);
			secondHistory.push_front(1);
		}
		if (firstResult && !secondResult)
		{
			//first cooperated, second defected
			secondFitness += TEMPTATION_PAYOFF;
			firstHistory.push_front(1);
			secondHistory.push_front(0);
		}
		if (!firstResult && !secondResult)
		{
			//both defected
			firstFitness += PUNISHMENT_PAYOFF;
			secondFitness += PUNISHMENT_PAYOFF;
			firstHistory.push_front(0);
			secondHistory.push_front(0);
		}
		firstHistory.pop_back();
		secondHistory.pop_back();
	}
}

void FitnessFunctions::dieAfterThreeSuckers(const Genome & first, const Genome & second, 
	unsigned int & firstFitness, unsigned int & secondFitness)
{
	//find the size of the genome, and generate a history stack
	unsigned int genomeSize = first.genomeLength();
	std::deque<bool> firstHistory;
	//fill history with random data
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 1);

	for (unsigned int i = 0; i < Helpers::log2(genomeSize * 8); ++i)
		firstHistory.push_front((bool)(distribution(generator)));
	std::deque<bool> secondHistory = firstHistory;

	//now run the PD
	for (unsigned int i = 0; i < 100; ++i)
	{
		//see if the last three times have been a sucker. That requires US cooperating
		//check the first one first
		if (!firstHistory[0] && secondHistory[0] && !firstHistory[1] && secondHistory[1] && !firstHistory[2] && secondHistory[2])
		{
			//we're done. Return 0 fitness for the first person, they lost.
			//For the second one, give it the +10 payoff for the rest of the rounds
			firstFitness = 0;
			secondFitness += (100 - i) * 10;
			return;
		}
		//do the other case
		if (!secondHistory[0] && firstHistory[0] && !secondHistory[1] && firstHistory[1] && !secondHistory[2] && firstHistory[2])
		{
			secondFitness = 0;
			firstFitness += (100 - i) * 10;
			return;
		}
		bool firstResult = first.getBit(Helpers::dequeToInt(secondHistory));
		bool secondResult = second.getBit(Helpers::dequeToInt(firstHistory));

		if (firstResult)
			Helpers::incrementCoop();
		if (secondResult)
			Helpers::incrementCoop();

		if (firstResult && secondResult)
		{
			//both cooperated
			firstFitness += REWARD_PAYOFF;
			secondFitness += REWARD_PAYOFF;
			firstHistory.push_front(1);
			secondHistory.push_front(1);
		}
		if (!firstResult && secondResult)
		{
			//first defected, second cooperated
			firstFitness += TEMPTATION_PAYOFF;
			firstHistory.push_front(0);
			secondHistory.push_front(1);
		}
		if (firstResult && !secondResult)
		{
			//first cooperated, second defected
			secondFitness += TEMPTATION_PAYOFF;
			firstHistory.push_front(1);
			secondHistory.push_front(0);
		}
		if (!firstResult && !secondResult)
		{
			//both defected
			firstFitness += PUNISHMENT_PAYOFF;
			secondFitness += PUNISHMENT_PAYOFF;
			firstHistory.push_front(0);
			secondHistory.push_front(0);
		}
		firstHistory.pop_back();
		secondHistory.pop_back();
	}
}
