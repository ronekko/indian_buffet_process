#include <iostream>
#include <ctime>
#include <opencv2/opencv.hpp>
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
cv::Mat upsample(const cv::Mat &src, const int &scale)
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



int main(void)
{
	using namespace std;
	using namespace boost;

	const int N = 200;
	const double ALPHA = 30.0;
	IBP ibp(N, ALPHA, static_cast<unsigned long>(time(0)));
	vector<vector<float>> mat = ibp.sample();

	int K = mat[0].size();
	cout << "K: " << K << endl;
	double H_N = 0;
	for(int n=1; n<=N; ++n){ H_N += 1.0/double(n);}
	cout << "K ` Poisson(ƒ¿ƒ°(1/n)): ƒ¿ƒ°(1/n) = " << ALPHA * H_N << endl;

	cv::Mat result = cv::Mat::zeros(N, K, CV_32FC1);
	for(int n=0; n<N; ++n){
		for(int k=0; k<mat[n].size(); ++k){
			result.at<float>(n, k) = mat[n][k];
		}
	}

	//
	cv::imshow("result", upsample<float>(result, 5));
	cv::waitKey(0);

	
	return 0;
}