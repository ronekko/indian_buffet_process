#pragma once;
#include <vector>
#include <boost/random.hpp>

class IBP
{
public:
	// BETA‚ğÈ—ª‚µ‚½‚ç•’Ê‚ÌIBPABETA‚ğw’è‚µ‚½‚ç2-parameter IBP
	IBP(unsigned long seed, const int &N, const double &ALPHA, const double &BETA = 1.0);
	~IBP();
	std::vector<std::vector<unsigned char>> sample(void);
//	std::vector<std::vector<unsigned char>> lof(std::vector<std::vector<unsigned char>> mat); // left-ordered form

	const int N;
	const double ALPHA;
	const double BETA;
	boost::random::mt19937 engine;
};