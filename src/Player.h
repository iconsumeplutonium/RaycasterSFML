#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utilities.h"

using namespace std;

class Player {
private:
	Utilities::DisplaySettings settings;
	sf::RenderWindow* window;

	void UpdateRotation(float deltaTime);
	void UpdatePosition(float deltaTime);

public:
	sf::Vector2f position;
	float rotation;
	float moveSpeed;
	float rotationSpeed;
	int FOV;
	sf::CircleShape body;

	//constructor
	Player(Utilities::DisplaySettings settings, float rotationSpeed, float moveSpeed, int FOV, sf::RenderWindow* window);

	void UpdateBodyDisplay();
	void Update(float deltaTime);

	sf::Vector2<float> GetFirstHorizontalIntersection(std::vector<std::vector<int>> map, float rotation);
	sf::Vector2<float> GetFirstVerticalIntersection(std::vector<std::vector<int>> map, float rotation);
	sf::Vector2<float> GetFirstIntersection(std::vector<std::vector<int>> map, float rotation, bool& wallWasHorizontal); //extra boolean to determine if closest wall is horizontal or vertical (used for fake lighting)

	string DebugStatistics();
};