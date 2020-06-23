#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;


struct Square {
	double mass = 50;
	double velocity = 0;
	double accel = 9.81;
	bool onHook = false;
	sf::RectangleShape shape;

	bool collide(const sf::FloatRect &entity) {
		sf::FloatRect rect = shape.getGlobalBounds();
		return rect.intersects(entity);
	}
};

struct Circle {
	sf::CircleShape shape;

	bool flaga = false;

	bool collide(const sf::FloatRect& entity) {
		sf::FloatRect rect = shape.getGlobalBounds();
		return flaga = rect.intersects(entity);
	}
};

class Crane {

public:
	Crane(const int width, const int height, const string title);
	void run();

private:
	sf::RenderWindow window;
	int c_width;
	int c_height;
	int c_zero;
	// úcieøki do textur
	const string crane_path;
	const string control_path;
	const string hook_path;
	const string font_path;

	// texturki
	sf::Texture craneText;
	sf::Texture hookText;
	sf::Texture controlText;

	// sprites 
	sf::Sprite craneSprite;
	sf::Sprite hookSprite;
	sf::Sprite controlSprite;

	// blok
	Square blocks[3];

	//kolko
	Circle circle;

	//teskt bledu
	sf::Font font;
	sf::Text wrongShape;

	// lina düwigu
	sf::RectangleShape rope;

	void setup();
	void refresh();
	void update();
	bool loadtxt();
	void movement(int &dX, int &dY);
	void do_movement(const int dX, const int dY, const sf::FloatRect &hookRect, const sf::FloatRect &ropeRect);
	void holdMe(sf::FloatRect &hookRect);

};
