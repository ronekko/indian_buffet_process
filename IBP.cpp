#pragma once;
#include <vector>
#include <list>
#include <boost/random.hpp>
#include "IBP.h"


IBP::IBP(boost::mt19937 &engine, const int &N, const double &ALPHA, const double &BETA) : engine(engine), N(N), ALPHA(ALPHA), BETA(BETA)
{
	int K;
	double sum = 0.0;
	for(int n=1; n<=N; ++n){
		sum += 1.0 / static_cast<double>(n - 1 + BETA);
	}
	K = ALPHA * BETA * sum;

	mat.resize(K);
	m.resize(K);

	// matをランダムなバイナリ行列に初期化、m[k]に各列kの1の出現回数をカウント
	{
		auto itm=m.begin();
		for(auto itmat=mat.begin(); itmat!=mat.end(); ++itmat, ++itm){
			*itm = 0;
			(*itmat).resize(N, 0);
			for(int n=0; n<N; ++n){
				double u = boost::uniform_01<>()(engine);
				if(u < (ALPHA / static_cast<double>(K))){
					(*itmat)[n] = 1;
					(*itm)++;
				}
			}
		}
	}
}

IBP::~IBP(){}


// N*K matrix: mat[n][k]
std::vector<std::vector<unsigned char>> IBP::sample(void)
{
	using namespace std;
	using namespace boost;
	
	boost::uniform_01<> uniform01;
	boost::poisson_distribution<> poisson( ALPHA * BETA / static_cast<double>(N - 1 + BETA)); // 2-parameter IBP (1-parameter IBP if BETA==1)

	// ギブスサンプリング1サイクル ここから
	for(int i=0; i<N; ++i){
		auto itm=m.begin();
		for(auto itmat=mat.begin(); itmat!=mat.end();){
			int mk = *itm;
			int m_ik = mk - (*itmat)[i];
			if(m_ik == 0){
				// k列目を削除
				itmat = mat.erase(itmat);
				itm = m.erase(itm);
				continue;
			}
			double p = static_cast<double>(m_ik) / static_cast<double>(N - 1 + BETA); // 2-parameter IBP
			double u = boost::uniform_01<>()(engine);
			int coinToss = (p > u) ? 1 : 0;
			(*itmat)[i] = coinToss;
			*itm = m_ik + coinToss;

			++itmat;
			++itm;
		}
		int numNewDishes = poisson(engine);
		for(int k=0; k<numNewDishes; ++k){
			vector<unsigned char> vec(N);
			vec[i] = 1;
			mat.push_back(vec);
			m.push_back(1);
		}
	}
	// ギブスサンプリング1サイクル ここまで

	vector<vector<unsigned char>> matV = list2vector(mat);
	vector<vector<unsigned char>> result(N);
	const int K = mat.size();
	for(int n=0; n<N; ++n){
		result[n].resize(K, 0);
		for(int k=0; k<matV[n].size(); ++k){
			result[n][k] = matV[n][k];
		}
	}

	return result;
}



std::vector<std::vector<unsigned char>> IBP::list2vector(const std::list<std::vector<unsigned char>> &mat)
{
	using namespace std;

	int K = mat.size();
	int N;
	{
		auto it = mat.begin();
		N = (*it).size();
	}

	vector<vector<unsigned char>> result(N);
	for(int n=0; n<N; ++n){
		result[n].resize(K);
	}

	int k = 0;
	for(auto it=mat.begin(); it!=mat.end(); ++it, ++k){
		for(int n=0; n<N; ++n){
			result[n][k] = (*it)[n];
		}
	}

	return result;
}