#pragma once
#include <vector>
#include "Genome.h"

namespace FitnessFunctions
{
	std::vector<double> normalPrisonersDilemma(const std::vector<Genome>& organisms);
	std::vector<double> iteratedPrisonersDilemma(const std::vector<Genome>& organisms,
		void(*pdFunction)(const Genome&, const Genome&, unsigned int&, unsigned int&));
	void normalRulePD(const Genome& first, const Genome& second, unsigned int& firstFitness, unsigned int& secondFitness);

}