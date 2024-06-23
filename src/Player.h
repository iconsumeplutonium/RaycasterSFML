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
	vec2 position;
	float moveSpeed;

	sf::CircleShape body;
	vec2 windowSize;

	float rotation;
	float rotationSpeed;

	//constructor
	Player(int windowSizeX, int windowSizeY, float rotationSpeed, float moveSpeed);

	void UpdateRotation(float deltaTime);
	void UpdatePosition(float deltaTime);

	//transforms world coordinates into screen coords
	void UpdateBodyDisplay();

	sf::Vector2<float> GetFirstHorizontalIntersection(int tileSize);

	string DebugStatistics();
};