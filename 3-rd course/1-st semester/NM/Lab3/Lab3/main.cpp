#include <iostream>
#include "SFML\Graphics.hpp"
#include <map>
#include <vector>

constexpr int x_size = 10;
constexpr int y_size = 10;
constexpr double step = 0.001;
double min_x;
double max_x;
std::map<std::pair<int, int>, double> sep_difs;
std::vector<double> vals;

double f(double x) {
	return log10(abs(x - 2));
}

void setSepDifs(double(*pf)(double)) {
	for (int i = 0; i < vals.size(); ++i) {
		for (int j = 0; j < vals.size() - i; ++j) {
			if (i == 0) {
				sep_difs[std::make_pair(j, i)] = (pf(vals[j]));
			} else {
				sep_difs[std::make_pair(j, i)] = (sep_difs[std::make_pair(j + 1, i - 1)] - sep_difs[std::make_pair(j, i - 1)]) / (vals[j + i] - vals[j]);
			}
		}
	}
}

double L(double x) {
	double res = 0;
	double w = 1;
	for (int i = 0; i < vals.size(); ++i) {
		res += sep_difs[std::make_pair(0, i)] * w;
		w *= x - vals[i];
	}
	return res;
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
	for (int i = -y_size; i <= y_size; i += 5) {
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

void drawPlot(sf::RenderWindow *window, double(*pf)(double), double step, sf::Color color) {
	sf::Vertex line[2];
	line[0].color = color;
	line[1].color = color;
	line[0].position.x = 0;
	line[0].position.y = 760 - (pf(-x_size) + y_size) * 760. / (y_size * 2);
	if (pf == L) {
		line[1].position.x = (min_x + x_size) * 1366. / (x_size * 2);
		line[1].position.y = 760 - (pf(min_x) + y_size) * 760. / (y_size * 2);
	}
	for (double i = -x_size; i <= x_size; i += step) {
		if (!((i < min_x || i > max_x) && (pf == L))) {
			line[1].position.x = (i + x_size) * 1366. / (x_size * 2);
			line[1].position.y = 760 - (pf(i) + y_size) * 760. / (y_size * 2);
			window->draw(line, 2, sf::Lines);
		}
		line[0].position = line[1].position;
	}
}

int main() {
	std::cout << "Enter n:" << std::endl;
	int n;
	std::cin >> n;
	std::cout << "Enter a, b:" << std::endl;
	std::cin >> min_x >> max_x;
	vals.resize(n);

	sf::ContextSettings settings;
	sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1366, 768), "Plot", sf::Style::Fullscreen, settings);
	window->clear(sf::Color::White);
	drawGrid(window);
	drawPlot(window, f, step, sf::Color::Blue);

	for (int i = 0; i < n; ++i) {
		vals[i] = min_x + i * (max_x - min_x) / ( i == 0 ? 1 : n - 1);
	}
	setSepDifs(f);
	drawPlot(window, L, step, sf::Color::Red);

	for (int i = 0; i < n; ++i) {
		vals[i] = ((max_x - min_x) * cos(3.14 * (2 * i + 1) / (2 * n + 2)) + (max_x + min_x)) / 2;
	}
	setSepDifs(f);
	drawPlot(window, L, step, sf::Color::Green);

	window->display();
	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
		}
	}
	return 0;
}