#include <iostream>
#include <numeric>
#include <SFML\Graphics.hpp>

const int x_size = 10;
const int y_size = 50;

double f(double x) {
	return x * x + 4 * sin(x) - 1.;
}

double fRelax(double x, double c) {
	return x + c * f(x);
}

double fNewton(double x) {
	return x - f(x) / (2 * x + 4 * cos(x));
}

double derivF(double x) {
	return 2 * x + 4 * cos(x);
}

double deriv2F(double x) {
	return 2 - 4 * sin(x);
}

void printResult(double x, int counter) {
	std::cout << "Result = " << x << std::endl;
	std::cout << "f(Result) = " << f(x) << std::endl;
	std::cout << "Steps count = " << counter << std::endl;
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

void relaxMethod(double eps) {
	std::cout << std::endl << "Relax method:" << std::endl;
	std::cout << "Enter range:" << std::endl;
	double a, b;
	std::cin >> a >> b;
	if (a > b) {
		std::swap(a, b);
	}
	if (f(a) * f(b) > 0) {
		std::cout << "Wrong value!(no roots)" << std::endl;
		return;
	}
	// find derivate f in range [a, b]
	std::vector<double> derivs1(1000);
	std::iota(derivs1.begin(), derivs1.end(), 0);
	std::transform(derivs1.begin(), derivs1.end(), derivs1.begin(), [a, b](double x) {return abs(derivF((b - a) * x / 1000 + a)); });
	// find Max = max in derivs, Min = min in derivs 
	double Max = *std::max_element(derivs1.begin(), derivs1.end());
	double Min = *std::min_element(derivs1.begin(), derivs1.end());
	// optimal const
	double c = 2 / (Max + Min);
	double q = (Max - Min) / (Max + Min);
	if (q < 1) {
		std::cout << "q = " << q << std::endl;
		std::cout << "Minimum expected n: " << round(log(((b - a) / 2) / eps) / log(1 / q)) + 1 << std::endl;
		findResultsWithRelax((a + b) / 2, c * (derivF(a) > 0 && derivF(b) > 0 ? -1 : 1), eps);
	} else {
		std::cout << "Wrong range!" << std::endl;
		return;
	}
}

void NewtonMethod(double eps) {
	std::cout << std::endl << "Newton method:" << std::endl;
	std::cout << "Enter value:" << std::endl;
	double x0;
	int range = 1;
	std::cin >> x0;
	/*if (f(x0 - range) * f(x0 + range) > 0) {
		std::cout << "Wrong value!(no roots)" << std::endl;
		return;
	}*/
	// find first and second derivate f in range [a - 1, a + 1]
	std::vector<double> derivs1(1000);
	std::vector<double> derivs2(1000);
	std::iota(derivs1.begin(), derivs1.end(), 0);
	std::iota(derivs2.begin(), derivs2.end(), 0);
	std::transform(derivs1.begin(), derivs1.end(), derivs1.begin(), [x0, range](double x) {return abs(derivF(2 * range * x / 1000 + (x0 - range))); });
	std::transform(derivs2.begin(), derivs2.end(), derivs2.begin(), [x0, range](double x) {return abs(deriv2F(2 * range * x / 1000 + (x0 - range))); });
	double Max = *std::max_element(derivs2.begin(), derivs2.end());
	double Min = *std::min_element(derivs1.begin(), derivs1.end());
	bool sign1 = derivs1[0] > 0, sign2 = derivs2[0] > 0;
	for (size_t i = 0; i < derivs1.size(); ++i) {
		if (sign1 != derivs1[i] > 0 || sign2 != derivs2[i] > 0) {
			std::cout << "Wrong value! (sign not const)" << std::endl;
			return;
		}
	}
	/*if (derivF(x0 - range) * deriv2F(x0 - range) > 0) {
		x0 = x0 + range;
	} else {
		x0 = x0 - range;
	}*/
	if (f(x0) * deriv2F(x0) < 0) {
		std::cout << "Wrong value! (f * f'' < 0)" << std::endl;
		return;
	}
	double q = Max * range / (2 * Min);
	if (q < 1) {
		std::cout << "Min expected n: " << round(log2(log(range / eps) / log(1 / q) + 1)) + 1 << std::endl;
		findResultsWithNewton(x0, eps);
	} else {
		std::cout << "Wrong value! (q >= 1)" << std::endl;
		return;
	}
}

void drawGrid(sf::RenderWindow *window) {

	sf::Font font;
	font.loadFromFile("Touch.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(15);

	sf::Vertex line[2];
	line[0].color = sf::Color::Black;
	line[1].color = sf::Color::Black;

	// y-axis
	line[0].position.x = 683;
	line[0].position.y = 0;
	line[1].position.x = 683;
	line[1].position.y = 760;
	window->draw(line, 2, sf::Lines);

	text.setFillColor(sf::Color::Green);
	for (int i = -y_size; i <= y_size; i+=5) {
		text.setString(std::to_string(i));
		text.setPosition(sf::Vector2f(683, 760 - (i + y_size) * 760 / (y_size * 2)));
		window->draw(text);
	}

	// x-axis
	line[0].position.x = 0;
	line[0].position.y = 380;
	line[1].position.x = 1366;
	line[1].position.y = 380;
	window->draw(line, 2, sf::Lines);

	text.setFillColor(sf::Color::Red);
	for (int i = -x_size; i <= x_size; i++) {
		text.setString(std::to_string(i));
		text.setPosition(sf::Vector2f((i + x_size) * 1366 / (x_size * 2), 360));
		window->draw(text);
	}
}

void drawPlot(sf::RenderWindow *window) {
	drawGrid(window);
	sf::Vertex line[2];
	line[0].color = sf::Color::Blue;
	line[1].color = sf::Color::Blue;
	line[0].position.x = 0;
	line[0].position.y = 760 - (f(-x_size) + y_size) * 760. / (y_size * 2);
	for (double i = -x_size; i <= x_size; i += 0.001) {
		line[1].position.x = (i + x_size) * 1366. / (x_size * 2);
		line[1].position.y = 760 - (f(i) + y_size) * 760. / (y_size * 2);
		window->draw(line, 2, sf::Lines);
		line[0].position = line[1].position;
	}
}

int main() {
	// draw plot
	sf::ContextSettings settings;
	sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1366, 768), "Plot", sf::Style::Fullscreen, settings);
	window->clear(sf::Color::White);
	drawPlot(window);
	window->display();

	 // solve
	double	eps;

	std::cout << "Enter accuracy:" << std::endl;
	std::cin >> eps;

	while (true) {
		relaxMethod(eps);
		NewtonMethod(eps);
		std::cout << std::endl;
	}
	return 0;
}

