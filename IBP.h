#pragma once;
#include <vector>
#include <boost/random.hpp>

class IBP
{
public:
	// BETAを省略したら普通のIBP、BETAを指定したら2-parameter IBP
	IBP(unsigned long seed, const int &N, const double &ALPHA, const double &BETA = 1.0);
	~IBP();
	std::vector<std::vector<float>> sample(void);

	const int N;
	const double ALPHA;
	const double BETA;
	boost::random::mt19937 engine;
};