#include <fstream>
#include <iostream>
#include <numeric>
#include <algorithm>
#include "matrix.h"

constexpr double EPS = 0.001;

using namespace std;

ml::Matrix<double> ones(const size_t &size) {
	ml::Matrix<double> temp(size, size, 0);
	for (size_t i = 0; i < size; ++i) {
		temp[i][i] = 1;
	}
	return temp;
}

int main() {
	ifstream in("data/y3.txt");
	vector<double> y(251), t(251);

	for (double &val : y) {
		in >> val;
	}

	iota(t.begin(), t.end(), 0);
	transform(t.begin(), t.end(), t.begin(), [](double val) {return val /= 5; });

	vector<double> beta = { 0.1, 0.1, 9 }; // c1, c3, m1
	double c2 = 0.3, c4 = 0.12, m2 = 28, m3 = 18;
	
	double I = 150, sum1 = 0, sum2 = 0, sumI = 0, count = 0;

	while (I > EPS, count++) {
		ml::Matrix<double> A(6, 6, 0);
		A[0][1] = 1;
		A[1][0] = -(c2 + beta[0]) / beta[2];
		A[1][2] = c2 / beta[2];
		A[2][3] = 1;
		A[3][0] = c2 / m2;
		A[3][2] = -(c2 + beta[1]) / m2;
		A[3][4] = beta[1] / m2;
		A[4][5] = 1;
		A[5][2] = beta[1] / m3;
		A[5][4] = -(c4 + beta[1]) / m3;

		ml::Matrix<double> dA1(6, 6, 0), dA2(6, 6, 0), dA3(6, 6, 0);

		dA1[1][0] = -1 / beta[2];

		dA2[3][2] = -1 / beta[2];
		dA2[3][4] = 1 / beta[2];
		dA2[5][2] = 1 / m3;
		dA2[5][4] = -1 / m3;

		dA3[1][0] = (c2 + beta[0]) / (pow(beta[2], 2));
		dA3[1][2] = -c2 / (pow(beta[2], 2));

		ml::Matrix<double> U(6, 3, 0);

	}

	return 0;
}