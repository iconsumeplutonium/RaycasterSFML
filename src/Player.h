#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utilities.h"

using namespace std;

class Player {

public:
	sf::Vector2f position;
	float moveSpeed;

	sf::CircleShape body;

	Utilities::DisplaySettings settings;
	sf::RenderWindow* window;

	float rotation;
	float rotationSpeed;
	int FOV;

	//constructor
	Player(Utilities::DisplaySettings settings, float rotationSpeed, float moveSpeed, int FOV, sf::RenderWindow* window);

	void UpdateRotation(float deltaTime);
	void UpdatePosition(float deltaTime);
	void Update(float deltaTime);

	//transforms world coordinates into screen coords
	void UpdateBodyDisplay();

	sf::Vector2<float> GetFirstHorizontalIntersection(std::vector<std::vector<int>> map, float rotation);
	sf::Vector2<float> GetFirstVerticalIntersection(std::vector<std::vector<int>> map, float rotation);
	sf::Vector2<float> GetFirstIntersection(std::vector<std::vector<int>> map, float rotation);

	string DebugStatistics();
};