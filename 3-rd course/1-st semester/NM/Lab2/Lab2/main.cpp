#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

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

Matrix multiply(Matrix m1, Matrix m2) {
	Matrix res(m1.size());
	for (size_t i = 0; i < m1.size(); i++) {
		res[i].resize(m2[0].size());
		for (size_t j = 0; j < m2[0].size(); j++) {
			for (size_t k = 0; k < m2.size(); k++) {
				res[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	return res;
}

void squareMethod(Matrix A, vector<double> b, double &determinant) {
	if (A != transpose(A)) {
		cout << "Matrix is not symetric!" << endl;
	}
	Matrix S(A.size());
	Matrix d(A.size());
	determinant = 1;
	for (int i = 0; i < A.size(); i++) {
		double temp = 0;
		for (int j = 0; j < i; j++) {
			temp += pow(S[j][i], 2) * d[j][j];
		}
		S[i].resize(A[i].size());

		double q = A[i][i] - temp;
		S[i][i] = sqrt(abs(q));
		d[i].resize(A[i].size());
		d[i][i] = q > 0 ? 1 : (q == 0 ? 0 : -1);
		temp = 0;
		for (int j = i + 1; j < A.size(); j++) {
			for (int k = 0; k < i; k++) {
				temp += S[k][i] * d[k][k] * S[k][j];
			}
			S[i][j] = (A[i][j] - temp) / (d[i][i] * S[i][i]);
		}
		cout << "S[i][i] = " << S[i][i] << endl;
		determinant *= d[i][i] * pow(S[i][i], 2);
	}
	vector<double> x = multiply(inverse(S), multiply(inverse(multiply(transpose(S), d)), b));
	cout << "Square root method:" << endl;
	for (double val : x) {
		cout << val << ' ';
	}
	cout << endl << "Square det = " << determinant << ", normal det = " << det(A) << endl;
}

void JacobiMethod(Matrix A, vector<double> b, double eps, vector<double> x) {
	double sum, q = INT_MAX;
	for (int i = 0; i < A.size(); i++) {
		if (A[i][i] == 0) {
			cout << "Diag element equal 0!" << endl;
		}
		sum = 0;
		for (int j = 0; j < A.size(); j++) {
			if (j != i) {
				sum += abs(A[i][j]);
			}
		}
		if (abs(A[i][i]) < sum) {
			cout << "No diag advantage!" << endl;
		}
		if (sum / abs(A[i][i]) < q) {
			q = sum / abs(A[i][i]);
		}
	}
	vector<double> temp_x(A.size());
	int count = 0;
	do {
		temp_x = b;
		for (size_t i = 0; i < A.size(); i++) {
			for (int j = 0; j < A.size(); j++) {
				if (i != j) {
					temp_x[i] -= A[i][j] * x[j];
				}
			}
			temp_x[i] /= A[i][i];
		}
		x = temp_x;
	} while (pow(q, count++) / (1 - q) >= eps);
	cout << "Jacobi method:" << endl << "Steps count: " << count << endl;
	for (double val : x) {
		cout << val << ' ';
	}
	cout << endl;
}

int main() {
	Matrix A = {
		{ 2, 1, -1, 0},
		{ 1, 3,  0, 1},
		{-1, 0,  2, 1},
		{ 0, 1,  1, 4}
	};
	vector<double> b = { 1, -3, -2, -5 };

	double eps;
	cin >> eps;
	double determinant;

	squareMethod(A, b, determinant);

	cout << endl;

	vector<double> x_0{ 0, 0, 0, 0 };
	JacobiMethod(A, b, eps, x_0);	

	system("pause");
	return 0;
}