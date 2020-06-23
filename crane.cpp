#include "crane.hpp"

int curr;

Crane::Crane(const int width, const int height, const string title) : crane_path("txt/crane.png"), hook_path("txt/hook.png"), control_path("txt/control.png"), font_path("txt/font.ttf") {
	c_width = width;
	c_height = height;
	c_zero = height - 10;

	window.create(sf::VideoMode(c_width, c_height), title, sf::Style::Close);
	window.setFramerateLimit(120);
	if (!loadtxt()) {
		cout << "Nie wczytalem textur :cccc !!!";
		window.close();
	}
	setup();
}

void Crane::run() {
	while (window.isOpen()) {
		update();
		refresh();
	}
}

bool Crane::loadtxt() {
	return (craneText.loadFromFile(crane_path) && hookText.loadFromFile(hook_path) && controlText.loadFromFile(control_path) && font.loadFromFile(font_path));
}

void Crane::setup() {
	craneSprite.setPosition(c_width - craneText.getSize().x - 600, c_zero - craneText.getSize().y + 10);
	craneSprite.setTexture(craneText);

	controlSprite.setPosition(craneSprite.getPosition().x + 550, craneSprite.getPosition().y + 170);
	controlSprite.setTexture(controlText);

	hookSprite.setPosition(controlSprite.getPosition().x + 9, craneSprite.getPosition().y + 200);
	hookSprite.setTexture(hookText);

	rope.setPosition(controlSprite.getPosition().x + controlText.getSize().x / 2 - 1, controlSprite.getPosition().y + controlText.getSize().y);
	rope.setFillColor(sf::Color::Black);


	for (int i = 0; i < 3; i++) {
		blocks[i].shape.setSize(sf::Vector2f(40, 40));
		blocks[i].shape.setPosition(500 + i * 60, c_zero - blocks[i].shape.getSize().y);
		blocks[i].shape.setFillColor(sf::Color::Black);
		blocks[i].mass = 50;
		blocks[i].accel = 9.81;
		blocks[i].velocity = 0;
		blocks[i].onHook = false;
	}

	circle.shape.setRadius(20);
	circle.shape.setPosition(700, c_zero- 2*circle.shape.getRadius());
	circle.shape.setFillColor(sf::Color::Yellow);

	wrongShape.setFont(font);
	wrongShape.setCharacterSize(40);
	wrongShape.setPosition(sf::Vector2f(600, 100));
	wrongShape.setFillColor(sf::Color::Red);
	wrongShape.setString("To nie jest kwadrat!");
}

void Crane::refresh() {
	window.clear(sf::Color::Blue);

	window.draw(craneSprite);
	window.draw(hookSprite);
	window.draw(controlSprite);
	window.draw(rope);
	for (int i = 0; i < 3; i++) {
		window.draw(blocks[i].shape);
	}
	window.draw(circle.shape);
	if (circle.flaga) window.draw(wrongShape);
	window.display();
}

void Crane::update() {
	int dX = 0;
	int dY = 0;

	movement(dX, dY);
	sf::FloatRect hookRect = hookSprite.getGlobalBounds();
	holdMe(hookRect);
	hookRect.left += dX;
	hookRect.top += dY;

	sf::FloatRect ropeRect = rope.getGlobalBounds();
	ropeRect.left += dX;
	ropeRect.top += dY;

	do_movement(dX, dY, hookRect, ropeRect);

	bool boink=false;

	if (circle.collide(hookRect)) {
		
	}

	if (!blocks[curr].onHook) {
		if ((blocks[curr].shape.getPosition().y + blocks[curr].shape.getSize().y + blocks[curr].velocity) < c_zero) {
			for (int i = 0; i < 3; i++) {
				if (i != curr) {
					boink = blocks[i].shape.getGlobalBounds().intersects(blocks[curr].shape.getGlobalBounds());// true jesli collide
					if (boink) {
						blocks[curr].velocity = 0;
						//blocks[curr].accel = 0;
						break;
					}
				}
			}
			blocks[curr].velocity += blocks[curr].accel / 120.0;
			if (!boink) {			
				
				blocks[curr].shape.move(0, blocks[curr].velocity);
			}

		}
	}
}

void Crane::movement(int& dX, int& dY) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		if (controlSprite.getPosition().x > craneSprite.getPosition().x + 350) {
			dX -= 1;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if (controlSprite.getPosition().x < craneSprite.getPosition().x + 620) {
			dX += 1;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		if ((!blocks[curr].onHook && hookSprite.getPosition().y < c_zero) || (blocks[curr].onHook && (blocks[curr].shape.getPosition().y + blocks[curr].shape.getSize().y) < c_zero)) {
			dY += 2;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		if (hookSprite.getPosition().y > controlSprite.getPosition().y + controlText.getSize().y) {
			dY -= 2;
		}
	}
}

void Crane::do_movement(const int dX, const int dY, const sf::FloatRect& hookRect, const sf::FloatRect& ropeRect) {
	if (dX != 0 && (!(blocks[curr].collide(ropeRect) || blocks[curr].collide(hookRect)) || blocks[curr].onHook)) {
		controlSprite.move(dX, 0);
		hookSprite.move(dX, 0);
		rope.move(dX, 0);
		if (blocks[curr].onHook) {
			blocks[curr].shape.move(dX, 0);
		}
	}
	if (dY != 0 && (!blocks[curr].collide(ropeRect) || blocks[curr].onHook)) {
		hookSprite.move(0, dY);
		rope.setSize(sf::Vector2f(2.0, hookSprite.getPosition().y - controlSprite.getPosition().y - controlText.getSize().y));
		if (blocks[curr].onHook) {
			blocks[curr].shape.move(0, dY);
		}
	}
}

void Crane::holdMe(sf::FloatRect& hookRect) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::H));
		for (int i = 0; i < 3; i++) {
			if (blocks[i].onHook) {
				blocks[i].onHook = false;
			}
			else if (blocks[i].collide(hookRect)) {
				blocks[i].onHook = true;
				curr = i;
			}
		}
	}
}	