#include <iostream>

using namespace std;

double f(double x) {
	return x * x + 4. * sin(x) - 1.;
}

double fRelax(double x, double c) {
	return x - c * f(x);
}

double fNewton(double x) {
	return x - f(x) / (2 * x + 4 * cos(x));
}

void printResult(double x, int counter) {
	cout << endl << "Result = " << x << endl;
	cout << "f(Result) = " << f(x) << endl;
	cout << "Steps count = " << counter << endl;
}

void findResultsWithRelax(double x0, double c, double eps) {
	int counter = 1;
	double x_prev = x0, x_cur = fRelax(x_prev, c);
	while (abs(x_prev - x_cur) >= eps)
	{
		x_prev = x_cur;
		x_cur = fRelax(x_cur, c);
		++counter;
	}
	printResult(x_cur, counter);
}

void findResultsWithNewton(double x0, double eps) {
	int counter = 1;
	double x_prev = x0, x_cur = fNewton(x_prev);
	while (abs(x_prev - x_cur) >= eps) {
		x_prev = x_cur;
		x_cur = fNewton(x_prev);
		++counter;
	}
	printResult(x_cur, counter);
}

int main() {
	double	eps;

	cout << "Enter accuracy:" << endl;
	cin >> eps;

	cout << endl << "Relax method:" << endl;
	findResultsWithRelax(-2, -0.2, eps);
	findResultsWithRelax(1, 0.3, eps);

	cout << endl << "Newton method:" << endl;
	findResultsWithNewton(-2, eps);
	findResultsWithNewton(1, eps);

	cout << endl;
	system("pause");
	return 0;
}

