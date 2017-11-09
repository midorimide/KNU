#pragma once

#include <vector>
#include <string>
#include <thread>

namespace ml {

	template<typename T>
	class Matrix {

	public:
		Matrix();

		Matrix(const std::pair<size_t, size_t> &size);
		Matrix(const std::pair<size_t, size_t> &size, const T & value);
		Matrix(const size_t &rows, const size_t &cols);
		Matrix(const size_t &rows, const size_t &cols, const T &value);

		Matrix(const std::vector<T> &vector);
		Matrix(const std::vector<std::vector<T>> &matrix);

		Matrix(const Matrix &other);

		void						setRow(const size_t &row, const std::vector<T> &vector);
		void						setRow(const size_t &row, const T &value);
		void						setCol(const size_t &col, const std::vector<T> &vector);
		void						setCol(const size_t &col, const T &value);
		void						addRow(const std::vector<T> &vector);
		void						addCol(const std::vector<T> &vector);

		const std::string			toString();

		std::pair<size_t, size_t>	getSize() const;

		std::vector<T>				getRow(const size_t &row) const;
		std::vector<T>				getCol(const size_t &col) const;

		Matrix<T>					getMinor(const size_t &row, const size_t &col) const;
		Matrix<T>					getInverse() const;
		Matrix<T>					getTranspose() const;

		T							getDeterminant();
		
		std::vector<T>				eraseRow(const size_t &row);
		std::vector<T>				eraseCol(const size_t &col);

		std::vector<T>&				operator[](const size_t &row);

		template<typename T> friend Matrix<T>			operator*(const Matrix<T> &left, const T &right);
		template<typename T> friend Matrix<T>			operator*(const Matrix<T> &left, const Matrix<T> &right);
		template<typename T> friend Matrix<T>			operator*(const Matrix<T> &left, const std::vector<T> &right);
		template<typename T> friend Matrix<T>			operator*(const Matrix<T> &left, const std::vector<std::vector<T>> &right);
		template<typename T> friend Matrix<T>			operator/(const Matrix<T> &left, const T &right);
		template<typename T> friend Matrix<T>			operator/(const Matrix<T> &left, const Matrix<T> &right);
		template<typename T> friend Matrix<T>			operator/(const Matrix<T> &left, const std::vector<std::vector<T>> &right);
		template<typename T> friend Matrix<T>			operator+(const Matrix<T> &left, const T &right);
		template<typename T> friend Matrix<T>			operator+(const Matrix<T> &left, const Matrix<T> &right);
		template<typename T> friend Matrix<T>			operator+(const Matrix<T> &left, const std::vector<std::vector<T>> &right);
		template<typename T> friend Matrix<T>			operator-(const Matrix<T> &left, const T &right);
		template<typename T> friend Matrix<T>			operator-(const Matrix<T> &left, const Matrix<T> &right);
		template<typename T> friend Matrix<T>			operator-(const Matrix<T> &left, const std::vector<std::vector<T>> &right);
		template<typename T> friend bool				operator==(const Matrix<T> &left, const Matrix<T> &right);

	private:
		std::vector<std::vector<T>> matrix;
		std::pair<size_t, size_t>	size;
		bool						is_det;
		T							det;
	};

	template<typename T>
	inline Matrix<T>::Matrix() {
		is_det = false;
		size = std::make_pair((size_t)0, (size_t)0);
	}

	template<typename T>
	inline Matrix<T>::Matrix(const std::pair<size_t, size_t>& size) : Matrix<T>() {
		this->size = size;
		matrix = std::vector<std::vector<T>>::vector(size.first, std::vector<T>::vector(size.second, 0));
	}

	template<typename T>
	inline Matrix<T>::Matrix(const std::pair<size_t, size_t>& size, const T & value) : Matrix<T>() {
		this->size = size;
		matrix = std::vector<std::vector<T>>::vector(size.first, std::vector<T>::vector(size.second, value));
	}

	template<typename T>
	inline Matrix<T>::Matrix(const size_t & rows, const size_t & cols) : Matrix<T>(std::make_pair(rows, cols)) {
	}

	template<typename T>
	inline Matrix<T>::Matrix(const size_t & rows, const size_t & cols, const T & value) : Matrix<T>(std::make_pair(rows, cols), value) {
	}

	template<typename T>
	inline Matrix<T>::Matrix(const std::vector<T>& vector) : Matrix<T>() {
		if (vector.size() != 0) {
			size = std::make_pair((size_t)vector.size(), (size_t)1);
			matrix = std::vector<std::vector<T>>::vector(size.first, std::vector<T>::vector(size.second));
			for (size_t i = 0; i < size.first; ++i) {
				matrix[i][0] = vector[i];
			}
		}
	}

	template<typename T>
	inline Matrix<T>::Matrix(const std::vector<std::vector<T>>& other) : Matrix<T>() {
		if (other.size() != 0) {
			size = std::make_pair(other.size(), other[0].size());
		}
		matrix = std::vector<std::vector<T>>::vector(size.first, std::vector<T>::vector(size.second));
		for (size_t i = 0; i < size.first; ++i) {
			for (size_t j = 0; j < size.second; ++j) {
				matrix[i][j] = other[i][j];
			}
		}
	}

	template<typename T>
	inline Matrix<T>::Matrix(const Matrix & other) : Matrix<T>(other.getSize()) {
		for (size_t i = 0; i < size.first; ++i) {
			for (size_t j = 0; j < size.second; ++j) {
				matrix[i][j] = other.matrix[i][j];
			}
		}
	}

	template<typename T>
	inline void Matrix<T>::setRow(const size_t & row, const std::vector<T> &vector) {
		for (size_t i = 0; i < size.second; ++i) {
			matrix[row][i] = vector[i];
		}
		is_det = false;
	}

	template<typename T>
	inline void Matrix<T>::setRow(const size_t & row, const T & value) {
		setRow(row, std::vector<T>::vector(size.second, value));
	}

	template<typename T>
	inline void Matrix<T>::setCol(const size_t & col, const std::vector<T> &vector) {
		for (size_t i = 0; i < size.first; ++i) {
			matrix[i][col] = vector[i];
		}
		is_det = false;
	}

	template<typename T>
	inline void Matrix<T>::setCol(const size_t & col, const T & value) {
		setCol(col, std::vector<T>::vector(size.first, value));
	}

	template<typename T>
	inline void Matrix<T>::addRow(const std::vector<T>& vector) {
		is_det = false;
		size.first++;
		matrix.push_back(vector);
	}

	template<typename T>
	inline void Matrix<T>::addCol(const std::vector<T>& vector) {
		is_det = false;
		size.second++;
		for (size_t i = 0; i < size.first; ++i) {
			matrix[i].push_back(vector[i]);
		}
	}

	template<typename T>
	inline const std::string Matrix<T>::toString() {
		std::string res;
		res += std::to_string(size.first) + ' ' + std::to_string(size.second) + '\n';
		for (size_t i = 0; i < size.first; ++i) {
			for (size_t j = 0; j < size.second; ++j) {
				res += std::to_string(matrix[i][j]) + ' ';
			}
			res += '\n';
		}
		res += '\n';
		return res;
	}

	template<typename T>
	inline std::pair<size_t, size_t> Matrix<T>::getSize() const {
		return size;
	}

	template<typename T>
	inline std::vector<T> Matrix<T>::getRow(const size_t & row) const {
		return matrix[row];
	}

	template<typename T>
	inline std::vector<T> Matrix<T>::getCol(const size_t & col) const {
		std::vector<T> temp(size.first);
		for (size_t i = 0; i < size.first; ++i) {
			temp[i] = matrix[i][col];
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> Matrix<T>::getMinor(const size_t & row, const size_t & col) const {
		Matrix<T> temp(*this);
		temp.eraseRow(row);
		temp.eraseCol(col);
		return temp;
	}

	template<typename T>
	inline Matrix<T> Matrix<T>::getInverse() const {
		Matrix<T> temp(*this);
		double t_det = temp.getDeterminant();
		for (size_t i = 0; i < size.first; ++i) {
			for (size_t j = 0; j < size.second; ++j) {
				temp[i][j] = getMinor(j, i).getDeterminant() / t_det * ((i + j) % 2 == 0 ? 1 : -1);
			}
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> Matrix<T>::getTranspose() const {
		Matrix<T> temp(size.second, size.first);
		for (size_t i = 0; i < size.second; ++i) {
			for (size_t j = 0; j < size.first; ++j) {
				temp[i][j] = matrix[j][i];
			}
		}
		return temp;
	}

	template<typename T>
	inline T Matrix<T>::getDeterminant() {
		if (is_det) {
			return det;
		}
		if (size.first == 1) return matrix[0][0];
		double res = 0;
		for (size_t i = 0; i < size.first; ++i) {
			res += matrix[i][0] * getMinor(i, 0).getDeterminant() * (i % 2 == 0 ? 1 : -1);
		}
		det = res;
		is_det = true;
		return res;
	}

	template<typename T>
	inline std::vector<T> Matrix<T>::eraseRow(const size_t & row) {
		std::vector<T> temp = matrix[row];
		matrix.erase(matrix.begin() + row);
		is_det = false;
		--size.first;
		return temp;
	}

	template<typename T>
	inline std::vector<T> Matrix<T>::eraseCol(const size_t & col) {
		std::vector<T> temp(size.first);
		for (size_t i = 0; i < size.first; ++i) {
			temp[i] = matrix[i][col];
			matrix[i].erase(matrix[i].begin() + col);
		}
		is_det = false;
		--size.second;
		return temp;
	}

	template<typename T>
	inline std::vector<T>& Matrix<T>::operator[](const size_t & row) {
		return matrix[row];
	}

	template<typename T>
	inline Matrix<T> operator*(const Matrix<T> &left, const T &right) {
		Matrix<T> temp(left);
		for (size_t i = 0; i < temp.getSize().first; ++i) {
			for (size_t j = 0; j < temp.getSize().second; ++j) {
				temp[i][j] *= right;
			}
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> operator*(const Matrix<T> &left, const Matrix<T> &right) {
		Matrix<T> temp(left.getSize().first, right.getSize().second);
		for (size_t i = 0; i < temp.getSize().first; ++i) {
			for (size_t j = 0; j < temp.getSize().second; ++j) {
				for (size_t k = 0; k < left.getSize().second; ++k) {
					temp[i][j] += left.matrix[i][k] * right.matrix[k][j];
				}
			}
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> operator*(const Matrix<T> &left, const std::vector<T> &right) {
		return left * Matrix<T>::Matrix(right);
	}

	template<typename T>
	inline Matrix<T> operator*(const Matrix<T> &left, const std::vector<std::vector<T>> &right) {
		return left * Matrix<T>::Matrix(right);
	}

	template<typename T>
	inline Matrix<T> operator/(const Matrix<T> &left, const T &right) {
		Matrix<T> temp(left);
		for (size_t i = 0; i < temp.getSize().first; ++i) {
			for (size_t j = 0; j < temp.getSize().second; ++j) {
				temp[i][j] /= right;
			}
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> operator/(const Matrix<T> &left, const Matrix<T> &right) {
		return left * right.getInverse();
	}

	template<typename T>
	inline Matrix<T> operator/(const Matrix<T> &left, const std::vector<std::vector<T>> &right) {
		return left / Matrix<T>::Matrix(right);
	}

	template<typename T>
	inline Matrix<T> operator+(const Matrix<T> &left, const T &right) {
		Matrix<T> temp(left);
		for (size_t i = 0; i < temp.getSize().first; ++i) {
			for (size_t j = 0; j < temp.getSize().second; ++j) {
				temp[i][j] += right;
			}
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> operator+(const Matrix<T> &left, const Matrix<T> &right) {
		Matrix<T> temp(left);
		for (size_t i = 0; i < temp.getSize().first; ++i) {
			for (size_t j = 0; j < temp.getSize().second; ++j) {
				temp[i][j] += right.matrix[i][j];
			}
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> operator+(const Matrix<T> &left, const std::vector<std::vector<T>> &right) {
		return left + Matrix<T>::Matrix(right);
	}

	template<typename T>
	inline Matrix<T> operator-(const Matrix<T> &left, const T &right) {
		Matrix<T> temp(left);
		for (size_t i = 0; i < temp.getSize().first; ++i) {
			for (size_t j = 0; j < temp.getSize().second; ++j) {
				temp[i][j] -= right;
			}
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> operator-(const Matrix<T> &left, const Matrix<T> &right) {
		Matrix<T> temp(left);
		for (size_t i = 0; i < temp.getSize().first; ++i) {
			for (size_t j = 0; j < temp.getSize().second; ++j) {
				temp[i][j] -= right.matrix[i][j];
			}
		}
		return temp;
	}

	template<typename T>
	inline Matrix<T> operator-(const Matrix<T> &left, const std::vector<std::vector<T>> &right) {
		return left - Matrix<T>::Matrix(right);
	}

	template<typename T>
	inline bool	operator==(const Matrix<T> &left, const Matrix<T> &right) {
		if (left.getSize() != right.getSize()) {
			return false;
		}
		for (size_t i = 0; i < left.getSize().first; ++i) {
			for (size_t j = 0; j < left.getSize().second; ++j) {
				if (left.matrix[i][j] != right.matrix[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

}


