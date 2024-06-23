#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utilities.h"

using namespace std;

class Player {
private:
	struct vec2 {
		float x;
		float y;
	};


public:
	sf::Vector2f position;
	float moveSpeed;

	sf::CircleShape body;
	/*sf::Vector2i windowSize;
	int tileSize;
	int gridSize;*/
	Utilities::DisplaySettings settings;
	sf::RenderWindow* window;

	float rotation;
	float rotationSpeed;

	//constructor
	Player(Utilities::DisplaySettings settings, float rotationSpeed, float moveSpeed, sf::RenderWindow* window);

	void UpdateRotation(float deltaTime);
	void UpdatePosition(float deltaTime);

	//transforms world coordinates into screen coords
	void UpdateBodyDisplay();

	sf::Vector2<float> GetFirstHorizontalIntersection(std::vector<std::vector<int>> map);

	string DebugStatistics();
};