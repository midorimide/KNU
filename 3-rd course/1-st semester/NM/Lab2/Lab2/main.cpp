#include <vector>
#include <iostream>

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

vector<double> squareMethod(Matrix A, vector<double> b) {
	Matrix S(A.size());
	Matrix d(A.size());
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
	}
	return multiply(inverse(S), multiply(inverse(multiply(transpose(S), d)), b));
}

vector<double> JacobiMethod(Matrix A, vector<double> b, double eps, vector<double> x)
{
	vector<double> temp_x(A.size());
	double norm;
	do {
		for (size_t i = 0; i < A.size(); i++) {
			temp_x[i] = b[i];
			for (int j = 0; j < A.size(); j++) {
				if (i != j)
					temp_x[i] -= A[i][j] * x[j];
			}
			temp_x[i] /= A[i][i];
		}
		norm = abs(x[0] - temp_x[0]);
		for (int h = 0; h < A.size(); h++) {
			if (abs(x[h] - temp_x[h]) > norm)
				norm = abs(x[h] - temp_x[h]);
			x[h] = temp_x[h];
		}
	} while (norm > eps);
	return x;
}

int main() {
	Matrix A = {
		{ 2, 1, -1, 0},
		{ 1, 3,  0, 1},
		{-1, 0,  2, 1},
		{ 0, 1,  1, 4}
	};
	vector<double> b = { 1, -3, -2, -5 };

	if (A == transpose(A) && det(A) != 0) {
		vector<double> x = squareMethod(A, b);
		cout << "Square root method ? :" << endl;
		for (double val : x) {
			cout << val << ' ';
		}
		cout << endl;
	}

	vector<double> x{ 0, 0, 0, 0 };
	x = JacobiMethod(A, b, 0.0001, x);
	cout << "Jacobi method:" << endl;
	for (double val : x) {
		cout << val << ' ';
	}
	cout << endl;

	system("pause");
	return 0;
}