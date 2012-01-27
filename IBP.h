#pragma once;
#include <vector>
#include <boost/random.hpp>

class IBP
{
public:
	IBP(const int &N, const double &ALPHA, unsigned long seed, const double &BETA = 1.0);
	~IBP();
	std::vector<std::vector<float>> sample(void);

	const int N;
	const double ALPHA;
	const double BETA;
	boost::random::mt19937 engine;
};