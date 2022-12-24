#include "header.h"

#define W 1000

struct scene
{
	double x;
	double y;
	double w;
};
std::vector<scene> lastScenes;

sf::Image image;
int iterations = 5000;
bool rendered = false;
bool fileRendered = true;

double drawX = -2;
double drawY = 2;
double drawWidth = 4;

void draw();
void renderFile();
void calculatePoints(double xStart, double yStart, double w, int picLen, sf::Image* img);

int main()
{
	sf::RenderWindow window(sf::VideoMode(W, W), "Mandelbrot");
	window.setFramerateLimit(150);
	window.setMouseCursorVisible(false);

	image.create(W, W);

	sf::Sprite sprite;
	sf::Texture texture;

	sf::Thread renderFileTh(renderFile);
	sf::Thread drawTh(draw);
	drawTh.launch();

	int len = 300;
	int numberOfZooms = 0;

	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			else if (event.type == sf::Event::MouseWheelMoved) {
				if (len - event.mouseWheel.delta < 20) len = 20;
				else if (len - event.mouseWheel.delta > 900) len = 900;
				len += event.mouseWheel.delta;
			}
		}
		window.clear();

		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);

		if (rendered && window.hasFocus()) {
			sf::Vector2i pos = sf::Mouse::getPosition(window);
			int x = sf::Mouse::getPosition(window).x, y;
			sf::Vertex rec[] = {
				sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Black),
				sf::Vertex(sf::Vector2f(pos.x + len, pos.y), sf::Color::Black),
				sf::Vertex(sf::Vector2f(pos.x + len, pos.y + len), sf::Color::Black),
				sf::Vertex(sf::Vector2f(pos.x, pos.y + len), sf::Color::Black),
				sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Black)
			};
			window.draw(rec, 5, sf::LinesStrip);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				scene scene = { drawX, drawY, drawWidth };
				lastScenes.push_back(scene);

				double pixLen = (drawWidth / (double)W);
				drawWidth = (((double)pos.x + len)* pixLen + drawX) - (pos.x * pixLen + drawX);
				drawX = drawX + pixLen * pos.x;
				drawY = -((pixLen * pos.y) - drawY);

				drawTh.launch();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && lastScenes.size() != 0) {
				drawWidth = lastScenes[lastScenes.size() - 1].w;
				drawX = lastScenes[lastScenes.size() - 1].x;
				drawY = lastScenes[lastScenes.size() - 1].y;
				lastScenes.pop_back();
				drawTh.launch();
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1) && fileRendered) {
				renderFileTh.launch();
			}
		}

		window.display();
	}

	return EXIT_SUCCESS;
}

void draw()
{
	rendered = false;
	image.create(W, W);

	system("cls");
	std::cout << "Upper left corner X: " << drawX << '\n';
	std::cout << "Upper left corner Y: " << drawY << '\n';
	std::cout << "Width: " << drawWidth << '\n';

	calculatePoints(drawX, drawY, drawWidth, W, &image);
	rendered = true;
}

void renderFile()
{
	std::cout << "Render started\n";
	fileRendered = false;
	sf::Image imgFile;
	imgFile.create(4000, 4000);
	calculatePoints(drawX, drawY, drawWidth, imgFile.getSize().x, &imgFile);
	imgFile.saveToFile("screen.jpg");
	std::cout << "Render finished\n";
	fileRendered = true;
}

void calculatePoints(double xStart, double yStart, double w, int picLen, sf::Image* img)
{
	Complex C(0, 0);
	Complex Z(0, 0);
	int i = 0;
	double pixLen = (double)picLen / w;
	for (int x = 0; x < picLen; x++) {
		for (int y = 0; y < picLen; y++) {

			Z.real = 0;
			Z.imaginary = 0;
			C.real = (double)(x + xStart * pixLen) / pixLen;
			C.imaginary = (double)(y - yStart * pixLen) / pixLen;
			i = 0;

			do {
				i++;
				Z.sqr();
				Z += C;
				if (Z.pointNotFits()) {
					sf::Uint8 r = 255 * i + 100;
					sf::Uint8 g = 255 * i + 80;
					sf::Uint8 b = 255 * i + 150;
					(*img).setPixel(x, y, sf::Color(r, g, b));
					break;
				}
			} while (i < iterations);
		}
	}
}