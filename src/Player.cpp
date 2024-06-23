#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iomanip>
#include <sstream>
#include <SFML/System/Vector2.hpp>
#include "Utilities.cpp"

using namespace std;

Player::Player(int windowSizeX, int windowSizeY, float rotationSpeed, float moveSpeed, int tileSize, int gridSize) {
    position.x = (gridSize / 2) * tileSize;
    position.y = (gridSize / 2) * tileSize;

    windowSize.x = windowSizeX;
    windowSize.y = windowSizeY;
    this->tileSize = tileSize;
    this->gridSize = gridSize;

    this->rotationSpeed = rotationSpeed;
    this->moveSpeed = moveSpeed;

    body = sf::CircleShape(10);
    body.setFillColor(sf::Color::Red);
    body.setOrigin(body.getRadius(), body.getRadius());
    UpdateBodyDisplay();

    rotation = 270;
}

void Player::UpdateRotation(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        rotation -= 10 * rotationSpeed * deltaTime;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        rotation += 10 * rotationSpeed * deltaTime;

    if (rotation < 0)
        rotation = 359;

    if (rotation >= 360)
        rotation = 0;
}

void Player::UpdatePosition(float deltaTime) {
    sf::Vector2 moveDelta(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        moveDelta.x += 1;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        moveDelta.x -= 1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        moveDelta.y -= 1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        moveDelta.y += 1;

    //calculate rotation matrix
    // cos -sin
    // sin cos
    float radians = rotation * (M_PI / 180.0);
    float x = cos(radians) * moveDelta.x - sin(radians) * moveDelta.y;
    float y = sin(radians) * moveDelta.x + cos(radians) * moveDelta.y;

    //std::cout << "(" << moveDelta.x << ", " << moveDelta.y << ")" << std::endl;

    position.x += x * deltaTime * moveSpeed;
    position.y += y * deltaTime * moveSpeed;

    if (position.x < 0)
        position.x = 0;
    
    if (position.y < 0)
        position.y = 0;

    UpdateBodyDisplay();
}

void Player::UpdateBodyDisplay() {
    sf::Vector2 screenSpacePos = TransformWorldSpaceToScreenSpace(sf::Vector2f((float)position.x, (float)position.y), tileSize, gridSize, sf::Vector2i(windowSize.x, windowSize.y));
    body.setPosition(screenSpacePos.x, screenSpacePos.y);
}

string Player::DebugStatistics() {
    stringstream stats;
    stats << fixed << setprecision(3); 
    stats << "Position (World Space): (" << position.x << ", " << position.y << ")\n";
    stats << "Position (Screen Space): (" << position.x + windowSize.x / 2 << ", " << position.y + windowSize.y / 2 << ")\n";
    stats << "Rotation: " << rotation << "\n";

    return stats.str();
}

sf::Vector2<float> Player::GetFirstHorizontalIntersection(int tileSize) {
    //Note: positive y axis is towards the bottom of the window for some reason 
    float radians = rotation * (M_PI / 180.0);

    int tileX = position.x / tileSize;
    int tileY = position.y / tileSize;

    //position within this cell
    float dx = position.x - (tileX * tileSize);
    float dy = position.y - (tileY * tileSize);
    
    if (dx < 0)
        dx = tileSize - abs(dx);

    if (dy < 0)
        dy = tileSize - abs(dy);

    float xStep;
    float firstX, firstY;
    if (rotation > 0 && rotation <= 90) {
        dy = ((tileY + 1) * tileSize) - position.y;
        xStep = dy / tan(radians + 0.000f);
        
        firstX = position.x + xStep;
        firstY = ((tileY + 1) * tileSize);
    }
    else if (rotation > 90 && rotation <= 180) {
        dy = ((tileY + 1) * tileSize) - position.y;
        xStep = -dy / tan(radians + 0.000f);

        firstX = position.x - xStep;
        firstY = ((tileY + 1) * tileSize);
    }
    else if (rotation > 180 && rotation <= 270) {
        dy = position.y - (tileY * tileSize);
        xStep = dy / tan(radians + 0.000f);

        firstX = position.x - xStep;
        firstY = position.y - dy;
    }
    else {
        dy = position.y - (tileY * tileSize);
        xStep = -dy / tan(radians + 0.000f);

        firstX = position.x + xStep;
        firstY = position.y - dy;
    }

    cout << firstX << ", " << firstY << endl;

    return sf::Vector2(firstX, firstY);

}