#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "matrix.h"
#include <iostream>

using namespace std;

const double EPS = 0.001;

ml::Matrix<double> ones(const size_t &size) {
	ml::Matrix<double> temp(size, size, 0);
	for (size_t i = 0; i < size; ++i) {
		temp[i][i] = 1;
	}
	return temp;
}

ml::Matrix<double> inv(const ml::Matrix<double> &matrix) {

	ml::Matrix<double> temp(matrix);

	ml::Matrix<double> E(ones(temp.getSize().first));

	for (size_t i = 0; i < temp.getSize().first; ++i) {
		size_t max = i;
		for (size_t j = i + 1; j < temp.getSize().first; ++j) {
			if (abs(temp[max][i]) < abs(temp[j][i])) {
				max = j;
			}
		}
		swap(temp[i], temp[max]);
		swap(E[i], E[max]);
		double d = temp[i][i];
		for (size_t j = 0; j < temp.getSize().second; ++j) {
			temp[i][j] /= d;
			E[i][j] /= d;
		}
		for (size_t j = i + 1; j < temp.getSize().first; ++j) {
			double f = temp[j][i];
			for (size_t k = 0; k < temp.getSize().second; ++k) {
				temp[j][k] -= temp[i][k] * f;
				E[j][k] -= E[i][k] * f;
			}
		}
	}

	for (size_t i = temp.getSize().first - 1; i > 0; --i) {
		for (size_t j = i - 1; j >= 0; --j) {
			double f = temp[j][i];
			temp[j][i] -= temp[i][i] * f;
			for (size_t k = 0; k < E.getSize().second; ++k) {
				E[j][k] -= E[i][k] * f;
			}
			if (j == 0) {
				break;
			}
		}
	}

	return E;
}

ml::Matrix<double> pinvG(const ml::Matrix<double> &matrix) {
	ml::Matrix<double> a = ml::Matrix<double>::Matrix(matrix.getRow(0));
	ml::Matrix<double> A(a.getTranspose());
	ml::Matrix<double> res;
	if ((a.getTranspose() * a)[0][0] == 0) {
		res = ml::Matrix<double>::Matrix(a.getCol(0));
	} else {
		res = ml::Matrix<double>::Matrix((a / (a.getTranspose() * a)[0][0]).getCol(0));
	}
	for (int i = 1; i < matrix.getSize().first; ++i) {
		a = ml::Matrix<double>::Matrix(matrix.getRow(i));
		ml::Matrix<double> t1 = ones(A.getSize().second);
		ml::Matrix<double> t2 = res * A;
		ml::Matrix<double> t3 = t1 - t2;
		ml::Matrix<double> Z(ones(A.getSize().second) - res * A);
		ml::Matrix<double> Z_a(Z * a);
		ml::Matrix<double> aT_r(a.getTranspose() * res);
		double scalar = (a.getTranspose() * Z_a)[0][0];
		A.addRow(a.getCol(0));
		if (abs(scalar) > EPS) {
			res = res - (Z_a * aT_r) / scalar;
			res.addCol(((Z_a) / scalar).getCol(0));
		} else {
			ml::Matrix<double> R(res * res.getTranspose());
			ml::Matrix<double> R_a(R * a);
			scalar = 1 + (a.getTranspose() * R_a)[0][0];
			res = res - (R_a * aT_r) / scalar;
			res.addCol(((R_a) / scalar).getCol(0));
		}
		std::cout << "Round " << i << " complete" << std::endl;
	}
	return res;
}

ml::Matrix<double> pinvMP(const ml::Matrix<double> &matrix) {
	ml::Matrix<double> E(ones(matrix.getSize().first));
	return matrix.getTranspose() * (inv(matrix * matrix.getTranspose() + E * 0.1));
}

int main() {
	BITMAPFILEHEADER bfh_x, bfh_y;
	BITMAPINFOHEADER bih_x, bih_y;

	FILE *x, *y, *e;
	x = fopen("x.bmp", "r + b");
	y = fopen("y.bmp", "r + b");
	e = fopen("e.bmp", "w + b");

	fread(&bfh_x, sizeof(bfh_x), 1, x);
	fread(&bih_x, sizeof(bih_x), 1, x);

	fread(&bfh_y, sizeof(bfh_y), 1, y);
	fread(&bih_y, sizeof(bih_y), 1, y);

	byte val;

	ml::Matrix<double> A(bih_x.biHeight, bih_x.biWidth);
	for (size_t i = 0; i < A.getSize().first; ++i) {
		for (size_t j = 0; j < A.getSize().second; ++j) {
			fread(&val, sizeof(byte), 1, x);
			A[i][j] = val;
		}
	}

	ml::Matrix<double> B(bih_y.biHeight, bih_y.biWidth);
	for (size_t i = 0; i < B.getSize().first; ++i) {
		for (size_t j = 0; j < B.getSize().second; ++j) {
			fread(&val, sizeof(byte), 1, y);
			B[i][j] = val;
		}
	}

	A.addRow(vector<double>::vector(A.getSize().second, 1));

	ml::Matrix<double> C(B * (pinvG(A) * A));

	fwrite(&bfh_y, sizeof(bfh_y), 1, e);
	fwrite(&bih_y, sizeof(bih_y), 1, e);

	for (size_t i = 0; i < C.getSize().first; ++i) {
		for (size_t j = 0; j < C.getSize().second; ++j) {
			val = (byte)C[i][j];
			fwrite(&val, sizeof(byte), 1, e);
		}
	}

	_fcloseall();
	return 0;
}