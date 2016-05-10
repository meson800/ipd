#include "Fitness.h"

#include <deque>
#include <random>
#include "Helpers.h"

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

		if (firstResult && secondResult)
		{
			//both cooperated
			firstFitness += 5;
			secondFitness += 5;
			firstHistory.push_front(1);
			secondHistory.push_front(1);
		}
		if (!firstResult && secondResult)
		{
			//first defected, second cooperated
			firstFitness += 10;
			firstHistory.push_front(0);
			secondHistory.push_front(1);
		}
		if (firstResult && !secondResult)
		{
			//first cooperated, second defected
			secondFitness += 10;
			firstHistory.push_front(1);
			secondHistory.push_front(0);
		}
		if (!firstResult && !secondResult)
		{
			//both defected
			firstFitness += 1;
			secondFitness += 1;
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

		if (firstResult && secondResult)
		{
			//both cooperated
			firstFitness += 5;
			secondFitness += 5;
			firstHistory.push_front(1);
			secondHistory.push_front(1);
		}
		if (!firstResult && secondResult)
		{
			//first defected, second cooperated
			firstFitness += 10;
			firstHistory.push_front(0);
			secondHistory.push_front(1);
		}
		if (firstResult && !secondResult)
		{
			//first cooperated, second defected
			secondFitness += 10;
			firstHistory.push_front(1);
			secondHistory.push_front(0);
		}
		if (!firstResult && !secondResult)
		{
			//both defected
			firstFitness += 1;
			secondFitness += 1;
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

		if (firstResult && secondResult)
		{
			//both cooperated
			firstFitness += 5;
			secondFitness += 5;
			firstHistory.push_front(1);
			secondHistory.push_front(1);
		}
		if (!firstResult && secondResult)
		{
			//first defected, second cooperated
			firstFitness += 10;
			firstHistory.push_front(0);
			secondHistory.push_front(1);
		}
		if (firstResult && !secondResult)
		{
			//first cooperated, second defected
			secondFitness += 10;
			firstHistory.push_front(1);
			secondHistory.push_front(0);
		}
		if (!firstResult && !secondResult)
		{
			//both defected
			firstFitness += 1;
			secondFitness += 1;
			firstHistory.push_front(0);
			secondHistory.push_front(0);
		}
		firstHistory.pop_back();
		secondHistory.pop_back();
	}
}
