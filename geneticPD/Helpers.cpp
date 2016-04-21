#include "Helpers.h"

#include <cmath>

//taken from http://www.johndcook.com/blog/cpp_phi/
double Helpers::normalCDF(double z)
{

	// constants
	double a1 = 0.254829592;
	double a2 = -0.284496736;
	double a3 = 1.421413741;
	double a4 = -1.453152027;
	double a5 = 1.061405429;
	double p = 0.3275911;

	// Save the sign of x
	int sign = 1;
	if (z < 0)
		sign = -1;
	z = fabs(z) / sqrt(2.0);

	// A&S formula 7.1.26
	double t = 1.0 / (1.0 + p*z);
	double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-z*z);

	return 0.5*(1.0 + sign*y);
}

unsigned int Helpers::dequeToInt(const std::deque<bool>& history)
{
	unsigned int result = 0;
	for (unsigned int i = 0; i < history.size(); ++i)
	{
		result += (((int)history[i]) << i);
	}
	return result;
}
