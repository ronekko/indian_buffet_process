#include <iostream>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "IBP.h"


#ifdef _DEBUG
	#pragma comment(lib, "opencv_core231d.lib")
	#pragma comment(lib, "opencv_imgproc231d.lib")
	#pragma comment(lib, "opencv_highgui231d.lib")
	#pragma comment(lib, "opencv_features2d231d.lib")
#else
	#pragma comment(lib, "opencv_core231.lib")
	#pragma comment(lib, "opencv_imgproc231.lib")
	#pragma comment(lib, "opencv_highgui231.lib")
	#pragma comment(lib, "opencv_features2d231.lib")
#endif


template<typename T>
cv::Mat naiveResample(const cv::Mat &src, const double &scale)
{
	using namespace cv;
	int rows = src.rows * scale;
	int cols = src.cols * scale;
	Mat dst(rows, cols, src.type());

	for(int i=0; i<rows; ++i){
		for(int j=0; j<cols; ++j){
			dst.at<T>(i,j) = src.at<T>(i/scale,j/scale);
		}
	}

	return dst;
}


std::vector<std::vector<unsigned char>> lof(std::vector<std::vector<unsigned char>> mat) // left-ordered form
{
	using namespace std;

	int N = mat.size();
	int K = mat[0].size();

	vector<vector<unsigned char>> matT(K); // mat[n][k] <=> matT[k][n]
	for(int k=0; k<K; ++k){
		matT[k].resize(N);
		for(int n=0; n<N; ++n){
			matT[k][n] = mat[n][k];
		}
	}

	sort(matT.begin(), matT.end(), [](vector<unsigned char> v1, vector<unsigned char> v2) -> bool{
		for(auto it1=v1.begin(), it2=v2.begin(); it1!=v1.end(); ++it1, ++it2){
			if(*it1 == *it2){
				continue;
			}
			return *it1 > *it2;
		}
		return 0;
	});

	vector<vector<unsigned char>> result(N);
	for(int n=0; n<N; ++n){
		result[n].resize(K);
		for(int k=0; k<K; ++k){
			result[n][k] = matT[k][n];
		}
	}

	return result;
}


int main(void)
{
	using namespace std;
	using namespace boost;

	const int N = 200;
	const double ALPHA = 11.7;
	const double BETA = 1.0;
	boost::mt19937 engine(0);
//	IBP ibp(N, ALPHA, static_cast<unsigned long>(time(0)));
	IBP ibp(engine, N, ALPHA, BETA);

	double H_N = 0;
	for(int n=1; n<=N; ++n){ H_N += BETA/double(n + BETA - 1);}
	//	cout << "K ` Poisson(ƒ¿ƒ°(1/n)): ƒ¿ƒ°(1/n) = " << ALPHA * H_N << endl; // 1-parameter IBP
	cout << "K ` Poisson(ƒ¿ƒ°(1/n)): ƒ¿ƒ°(ƒÀ/(n+ƒÀ-1)) = " << ALPHA * H_N << endl; // 2-parameter IBP

	for(int i=0; i<100; ++i){
		vector<vector<unsigned char>> mat = ibp.sample();

		int K = mat[0].size();
		cout << "K: " << K << endl;
	
		cv::Mat result = cv::Mat::zeros(N, K, CV_32FC1);
		vector<vector<unsigned char>> matLof = lof(mat);
		cv::Mat resultLof = cv::Mat::zeros(N, K, CV_32FC1);
		for(int n=0; n<N; ++n){
			for(int k=0; k<mat[n].size(); ++k){
				result.at<float>(n, k) = mat[n][k];
				resultLof.at<float>(n, k) = matLof[n][k];
			}
		}


		cv::imshow("result", naiveResample<float>(result, 3));
		cv::imshow("lof", naiveResample<float>(resultLof, 3));
		cv::waitKey(0);
	}
	
	return 0;
}