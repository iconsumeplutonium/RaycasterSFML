#include <iostream>
#include <SFML/Graphics.hpp>

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
	sf::Vector2i windowSize;
	int tileSize;
	int gridSize;

	float rotation;
	float rotationSpeed;

	//constructor
	Player(sf::Vector2i windowSize, float rotationSpeed, float moveSpeed, int tileSize, int gridSize);

	void UpdateRotation(float deltaTime);
	void UpdatePosition(float deltaTime);

	//transforms world coordinates into screen coords
	void UpdateBodyDisplay();

	sf::Vector2<float> GetFirstHorizontalIntersection(int tileSize);

	string DebugStatistics();
};