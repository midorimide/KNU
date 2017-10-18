#include <iostream>
#include <SFML\Graphics.hpp>

const int x_size = 10;
const int y_size = 50;

double f(double x) {
	return x * x + 4. * sin(x) - 1.;
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
	std::cout << std::endl << "Result = " << x << std::endl;
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
	//double q = ()
	//int n = log(x_cur - x0) / (1 / )
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
	for (int i = -y_size; i <= y_size; i++) {
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
	double	eps, a, b, c, q, Max, Min;

	std::cout << "Enter accuracy:" << std::endl;
	std::cin >> eps;

	while (true) {
		std::cout << std::endl << "Relax method:" << std::endl;
		std::cout << "Enter range:" << std::endl;
		std::cin >> a >> b;
		Max = std::max(abs(derivF(a)), abs(derivF(b)));
		Min = std::min(abs(derivF(a)), abs(derivF(b)));
		c = 2 / (Max + Min);
		q = (Max - Min) / (Max + Min);
		if (q < 1) {
			findResultsWithRelax((a + b) / 2, c * (derivF((a + b) / 2) < 0 ? 1 : -1), eps);
		} else {
			std::cout << "Wrong range!" << std::endl;
		}

		std::cout << std::endl << "Newton method:" << std::endl;
		std::cout << "Enter value:" << std::endl;
		std::cin >> a;
		findResultsWithNewton(a, eps);

		std::cout << std::endl;
	}

	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close();
			}
		}
	}

	return 0;
}

