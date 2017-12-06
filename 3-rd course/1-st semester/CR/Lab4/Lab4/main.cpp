#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

bool isPrime(const int& x) {
	if (x == 1) {
		return false;
	}
	for (int i = 2; i < (int)sqrt(x) + 1; ++i) {
		if (x % i == 0) {
			return false;
		}
	}
	return true;
}

int nextPrime(const int& x) {
	int a = x;
	while (!isPrime(a++));
	return --a;
}

vector<pair<int, int>> getFactorization(const int& x) {
	int a = x;
	int p = 2;
	int r = 0;
	vector<pair<int, int>> res;
	while (a != 1) {
		if (a % p == 0) {
			a /= p;
			++r;
		} else {
			if (r != 0) {
				res.push_back(std::make_pair(p, r));
			}
			r = 0;
			p = nextPrime(p + 1);
		}
	}
	res.push_back(std::make_pair(p, r));
	return res;
}

vector<vector<int>> makeTable(const int& a, const int& p, const vector<pair<int, int>>& factorization) {
	vector<vector<int>> res;
	for (int i = 0; i < factorization.size(); ++i) {
		res.push_back(vector<int>::vector(factorization[i].first));
		for (int j = 0; j < res[i].size(); ++j) {
			res[i][j] = (unsigned int)pow(a, j * ((p - 1) / factorization[i].first)) % p;
		}
	}
	return res;
}

int findPos(const vector<int>& vec, const int& value) {
	int i = 0;
	while (i < vec.size() && vec[i++] != value) {
		if (i == vec.size()) {
			return -1;
		}
	}
	return --i;
}

int binPow(const int&a, const int&b, const int& m) {
	int res = 1;
	int x = a % m;
	int n = b;
	while (n != 0) {
		if (n & 1 == 1) {
			res = (res * x) % m;
		}
		x = (x * x) % m;
		n >>= 1;
	}
	return res;
}

int pohligHellman(const int& a, const int& b, const int& p) {
	vector<pair<int, int>> factorization = getFactorization(p - 1);
	vector<vector<int>> table = makeTable(a, p, factorization);
	vector<int> x(factorization.size());
	vector<int> x_i;
	for (int i = 0; i < factorization.size(); ++i) {
		x_i.resize(factorization[i].second);
		x_i[0] = findPos(table[i], binPow(b, (p - 1) / factorization[i].first, p));
		if (x_i[0] < 0) {
			return -1;
		}
		int sub = x_i[0];
		for (int j = 1; j < factorization[i].second; ++j) {
			int k = (p - 1) / (int)pow(factorization[i].first, j + 1);
			int w = binPow(b, k, p) * binPow(a, p - 1 - (sub * k) % (p - 1), p) % p;
			x_i[j] = findPos(table[i], w);
			if (x_i[j] < 0) {
				return -1;
			}
			sub += x_i[j] * pow(factorization[i].first, j);
		}
		x[i] = sub % (int)pow(factorization[i].first, factorization[i].second);
	}
	int m = 1;
	for (int i = 0; i < factorization.size(); ++i) {
		m *= pow(factorization[i].first, factorization[i].second);
	}
	int res = 0;
	for (int i = 0; i < factorization.size(); ++i) {
		int m_i = m / pow(factorization[i].first, factorization[i].second);
		int rev_m_i = binPow(m_i, pow(factorization[i].first, factorization[i].second) - pow(factorization[i].first, factorization[i].second - 1) - 1, pow(factorization[i].first, factorization[i].second));
		res += x[i] * m_i * rev_m_i;
	}
	return res %= p - 1;
}

const int rand(int l, int r) {
	if (l == r) {
		return l;
	}
	int m = (l + r) / 2;
	if (rand() & 1) {
		return rand(l, m);
	} else {
		return rand(m + 1, r);
	}
}

void testDiskrete() {
	vector<int> pr;
	for (int i = 4; i < 100; ++i) {
		if (isPrime(i)) {
			pr.push_back(i);
		}
	}
	int a, b, p, x, x1;
	for (int i = 0; i < 100; ++i) {
		int ind = rand() % pr.size();
		p = pr[ind];
		a = rand(2, p - 1);
		x = rand(1, p - 2);
		b = binPow(a, x, p);
		x1 = pohligHellman(a, b, p);
		if (x1 < 0) {
			continue;
		}
		cout << "(" << a << " ^ " << x << ")" << " % " << p << " = " << b << endl;
		cout << "x = " << x1 << " -> (a ^ x) % p = " << binPow(a, x1, p) << endl;
	}
	while (cin >> a >> b >> p) {
		cout << pohligHellman(a, b, p) << endl;
	}
}

int main() {
	srand(time(NULL));
	testDiskrete();
	system("pause");
	return 0;
}