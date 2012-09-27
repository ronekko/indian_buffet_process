#pragma once;
#include <vector>
#include <list>
#include <boost/random.hpp>

class IBP
{
public:
	// BETA‚ğÈ—ª‚µ‚½‚ç•’Ê‚ÌIBPABETA‚ğw’è‚µ‚½‚ç2-parameter IBP
	IBP(boost::mt19937 &engine, const int &N, const double &ALPHA, const double &BETA = 1.0);
	~IBP();
	std::vector<std::vector<unsigned char>> sample(void);
//	std::vector<std::vector<unsigned char>> lof(std::vector<std::vector<unsigned char>> mat); // left-ordered form
	std::vector<std::vector<unsigned char>> list2vector(const std::list<std::vector<unsigned char>> &mat);

	const int N;
	const double ALPHA;
	const double BETA;
	boost::mt19937 engine;

	std::list<std::vector<unsigned char>> mat;	
	std::list<int> m;
};