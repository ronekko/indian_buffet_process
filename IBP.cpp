#pragma once;
#include <vector>
#include <boost/random.hpp>
#include "IBP.h"


IBP::IBP(unsigned long seed, const int &N, const double &ALPHA, const double &BETA) : N(N), ALPHA(ALPHA), BETA(BETA)
{
	engine.seed(seed);
}

IBP::~IBP(){}


// N*K matrix: mat[n][k]
std::vector<std::vector<unsigned char>> IBP::sample(void)
{
	using namespace std;
	using namespace boost;

	vector<vector<unsigned char>> mat(N);
	uniform_real<> uniformDistribution(0, 1);
	variate_generator<mt19937&, uniform_real<>> uniform(engine, uniformDistribution);

	int K = 0;
	vector<int> m;
	for(int n=1; n<=N; ++n) // n = 1, 2, ... , N
	{
		mat[n-1].resize(K);
		for(int k=0; k<K; ++k){
			double pi = static_cast<double>(m[k]) / static_cast<double>(n + BETA - 1);
			int coinToss = (uniform() < pi) ? 1 : 0;
			mat[n-1][k] = coinToss;
			m[k] += coinToss;
		}

		poisson_distribution<> poissonDistribution( (ALPHA * BETA) / static_cast<double>(n + BETA - 1));
		variate_generator<mt19937&, poisson_distribution<>> poisson(engine, poissonDistribution);
		int numNewDishes = poisson();

		for(int k=K; k<K+numNewDishes; ++k){
			mat[n-1].push_back(1);
			m.push_back(1);
		}

		K += numNewDishes;
	}
	
	vector<vector<unsigned char>> result(N);
	for(int n=0; n<N; ++n){
		result[n].resize(K, 0);
		for(int k=0; k<mat[n].size(); ++k){
			result[n][k] = mat[n][k];
		}
	}

	return result;
}