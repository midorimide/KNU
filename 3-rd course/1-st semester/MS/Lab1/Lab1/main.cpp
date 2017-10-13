#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

const double pi = 3.14;
const int N = 501;

typedef vector<vector<double>> Matrix;

Matrix getMinor(Matrix matrix, int row, int col) {
	matrix.erase(matrix.begin() + row);
	for (auto &vec : matrix) {
		vec.erase(vec.begin() + col);
	}
	return matrix;
}

double det(Matrix matrix) {
	if (matrix.size() == 1) return matrix[0][0];
	double res = 0;
	for (size_t i = 0; i < matrix.size(); ++i) {
		res += matrix[i][0] * det(getMinor(matrix, i, 0)) * (i % 2 == 0 ? 1 : -1);
	}
	return res;
}

Matrix transpose(Matrix matrix) {
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < i; ++j) {
			swap(matrix[i][j], matrix[j][i]);
		}
	}
	return matrix;
}

Matrix inverse(Matrix matrix) {
	Matrix inverse_matrix = matrix;
	double determinant = det(matrix);
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix.size(); ++j) {
			inverse_matrix[i][j] = det(getMinor(matrix, j, i)) / determinant * ((i + j) % 2 == 0 ? 1 : -1);
		}
	}
	return inverse_matrix;
}

vector<double> multiply(Matrix matrix, vector<double> vec) {
	vector<double> res(vec.size());
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[i].size(); ++j) {
			res[i] += matrix[i][j] * vec[j];
		}
	}
	return res;
}

double func(vector<double> coef, double t, double f) {
	return coef[0] * pow(t, 3) + coef[1] * pow(t, 2) + coef[2] * pow(t, 1) + coef[3] * sin(2 * pi * f * t) + coef[4];
}

int main() {

	// Set file with data
	ifstream in("data/f1.txt");
	vector<double> x(N), c(N / 2), t(N);

	// Set t[0..5] with step = 0.01
	iota(t.begin(), t.end(), 0);
	transform(t.begin(), t.end(), t.begin(), [](double val) {return val /= 100; });

	// Input x[]
	for (double &val : x) {
		in >> val;
	}

	// Fourier; find c[]
	for (int i = 0; i < N / 2; ++i) {
		double cos_sum = 0, sin_sum = 0;
		for (int j = 0; j < N; ++j) {
			cos_sum += x[j] * cos(2 * pi * i * j / N);
			sin_sum += x[j] * sin(2 * pi * i * j / N);
		}
		c[i] = sqrt(pow(cos_sum, 2) + pow(sin_sum, 2)) / N;
	}

	// Find local max f in c[]
	double f;
	for (int i = 0; i < N / 2 - 2; ++i) {
		if (c[i] < c[i + 1] && c[i + 1] > c[i + 2]) {
			f = (i + 1.) / 5;
			break;
		}
	}

	// A * a = b

	// Set b
	vector<double> b(5);
	for (int i = 0; i < N; ++i) {
		b[0] += x[i] * pow(t[i], 3);
		b[1] += x[i] * pow(t[i], 2);
		b[2] += x[i] * pow(t[i], 1);
		b[3] += x[i] * sin(2 * pi * f * t[i]);
		b[4] += x[i];
	}

	// Set A
	Matrix A(5);
	for (auto &vec : A) {
		vec.resize(5);
	}
	for (double val : t) {
		double sin_val = sin(2 * pi * f * val);
		A[0][0] += pow(val, 6), A[0][1] += pow(val, 5), A[0][2] += pow(val, 4), A[0][3] += pow(val, 3) * sin_val, A[0][4] += pow(val, 3),
		A[1][0] += pow(val, 5), A[1][1] += pow(val, 4), A[1][2] += pow(val, 3), A[1][3] += pow(val, 2) * sin_val, A[1][4] += pow(val, 2),
		A[2][0] += pow(val, 4), A[2][1] += pow(val, 3), A[2][2] += pow(val, 2), A[2][3] += pow(val, 1) * sin_val, A[2][4] += pow(val, 1),
		A[3][0] += pow(val, 3) * sin_val, A[3][1] += pow(val, 2) * sin_val, A[3][2] += pow(val, 1) * sin_val, A[3][3] += pow(sin_val, 2), A[3][4] += sin_val,
		A[4][0] += pow(val, 3), A[4][1] += pow(val, 2), A[4][2] += pow(val, 1), A[4][3] += pow(val, 0) * sin_val, A[4][4] += pow(val, 0);
	}

	// Find a
	vector<double> a = multiply(inverse(A), b);
	cout << "coefs:" << endl;
	for (double val : a) {
		cout << val << ' ';
	}
	cout << endl;

	system("pause");
}