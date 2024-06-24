#define _USE_MATH_DEFINES
#include <math.h>
#include "Utilities.h"
#include "Player.h"
#include <SFML/System/Vector2.hpp>
#include <iomanip>
#include <sstream>

using namespace std;

Player::Player(Utilities::DisplaySettings settings, float rotationSpeed, float moveSpeed, int FOV, sf::RenderWindow* window) {
    position.x = (settings.gridSize / 2) * settings.tileSize;
    position.y = (settings.gridSize / 2) * settings.tileSize;

    this->settings = settings;
    this->rotationSpeed = rotationSpeed;
    this->moveSpeed = moveSpeed;
    this->window = window;
    this->FOV = FOV;

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

    if (position.x > (settings.tileSize * settings.gridSize))
        position.x = (settings.tileSize * settings.gridSize);
    
    if (position.y > (settings.tileSize * settings.gridSize))
        position.y = (settings.tileSize * settings.gridSize);

    UpdateBodyDisplay();
}

void Player::UpdateBodyDisplay() {
    sf::Vector2 screenSpacePos = Utilities::TransformWorldSpaceToScreenSpace(position, settings);
    body.setPosition(screenSpacePos);
    window->draw(body);
}

void Player::Update(float deltaTime) {
    UpdatePosition(deltaTime);
    UpdateRotation(deltaTime);

    //update FOV
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        FOV++;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        FOV--;

    if (FOV < 1) FOV = 1;
    if (FOV > 360) FOV = 360;

}

string Player::DebugStatistics() {
    sf::Vector2f screenSpacePos = Utilities::TransformWorldSpaceToScreenSpace(position, settings);

    stringstream stats;
    stats << fixed << setprecision(3) << setw(6);
    stats << "Position (World Space):  (" << position.x << ", " << position.y << ")\n";
    stats << "Position (Screen Space): (" << screenSpacePos.x << ", " << screenSpacePos.y << ")\n";
    stats << "Rotation: " << rotation << "\n";
    stats << "FOV: " << FOV << "\n";

    return stats.str();
}

//@param map: of size settings.gridSize by settings.gridSize
sf::Vector2<float> Player::GetFirstHorizontalIntersection(std::vector<std::vector<int>> map, float rotation) {
    //Note: positive y axis is towards the bottom of the window for some reason 
    float radians = rotation * (M_PI / 180.0);

    int tileX = position.x / settings.tileSize;
    int tileY = position.y / settings.tileSize;

    //position within this cell
    float dy = position.y - (tileY * settings.tileSize);
    if (dy < 0)
        dy = settings.tileSize - abs(dy);

    //if looking down (towards positive Y), calculate distance to lower line (y coord of next horizontal line minus current y coord)
    if (rotation <= 180) {
        dy = ((tileY + 1) * settings.tileSize) - position.y;
    }
    //if looking up (towards negative y), calculate distance to upper line (y coord of previous horizontal line minus current y coord)
    //needs to be negative so the math works out
    else {
        dy = -1 * (position.y - (tileY * settings.tileSize));
    }
    
    float dx = dy / tan(radians);

    /*float firstileCoordX = position.x + dx;
    float firstileCoordY = position.y + dy;
    Utilities::DrawCircle(sf::Vector2f(firstileCoordX, firstileCoordY), sf::Color::Magenta, settings, window);*/

    float tileStep = settings.tileSize * (rotation <= 180 ? 1 : -1);
    float xStep = (1.0f / tan(radians + 0.0001f)) * tileStep;
    float yStep = tileStep;

    /*cout << "xStep: " << xStep << endl;*/
    sf::Vector2f rayPos(position.x, position.y);
    bool firstIter = true;

    int numIntersections = 0;
    bool intersectedWall = false;

    while (true) {
        /*if (!Utilities::IsInBounds(sf::Vector2f(rayPos.x + xStep, rayPos.y + yStep), settings))
            break;*/

        if (firstIter) {
            rayPos.x += dx;
            rayPos.y += dy;
            firstIter = false;
        }
        else {
            rayPos.x += xStep;
            rayPos.y += yStep;
        }

        int tX = (((int) rayPos.x / settings.tileSize)) * settings.tileSize + (settings.tileSize / 2.0f);
        int tY = (((int) rayPos.y / settings.tileSize)) * settings.tileSize + (settings.tileSize / 2.0f);

        int tileCoordX = (((int)rayPos.x / settings.tileSize));
        int tileCoordY = (((int)rayPos.y / settings.tileSize));

        //if we are looking up (
        if (rotation >= 180 && tileCoordY + 1 < settings.gridSize) {
            tileCoordY --;
            tY -= settings.tileSize * 1;
        }


        //Utilities::PrintVector(sf::Vector2i(tileCoordX, tileCoordY));
        /*Utilities::DrawCircle(sf::Vector2f(tX, tY), sf::Color::Green, settings, window);
        Utilities::DrawCircle(rayPos, sf::Color::Magenta, settings, window);*/

        if (tileCoordX > settings.gridSize - 1 || tileCoordY > settings.gridSize - 1 || tileCoordX < 0 || tileCoordY < 0)
            break;

        if (map[tileCoordX][tileCoordY] != 0) {
            intersectedWall = true;
            break;
        }
        
        numIntersections++;
    }


    //Utilities::DrawCircle(rayPos, sf::Color::Magenta, settings, window);

    //cout << numIntersections << ", " << intersectedWall << endl;

    //return -1 if ray never intersected a wall
    return (intersectedWall) ? rayPos : sf::Vector2f(-1.0f, -1.0f);

}

//@param map: of size settings.gridSize by settings.gridSize
sf::Vector2<float> Player::GetFirstVerticalIntersection(std::vector<std::vector<int>> map, float rotation) {
    //Note: positive y axis is towards the bottom of the window for some reason 
    float radians = rotation * (M_PI / 180.0);

    int tileX = position.x / settings.tileSize;
    int tileY = position.y / settings.tileSize;

    //position within this cell
    float dx;

    //if facing east (towards positive X) dx is x-coord of the next vertical line minus the current x coordinate
    if (rotation <= 90 || rotation >= 270) {
        dx = ((tileX + 1) * settings.tileSize) - position.x;
    }
    //if facing west (towards negative X) dx is the x-coord of the previous vertical line minus the corrent x coordinate
    //needs to be negative so the math works out
    else {
        dx = -1 * (position.x - (tileX * settings.tileSize));
    }

    float dy = dx * tan(radians + 0.0001f);
    float firstTileCoordX = position.x + dx;
    float firstTileCoordY = position.y + dy;
    //Utilities::DrawCircle(sf::Vector2f(firstTileCoordX, firstTileCoordY), sf::Color::Magenta, settings, window);

   
    float tileStep = settings.tileSize * (rotation <= 90 || rotation >= 270 ? 1 : -1);
    float xStep = tileStep;
    float yStep = tan(radians) * tileStep;

    sf::Vector2f rayPos(position.x, position.y);
    bool firstIter = true;

    //int numIntersections = 0;
    bool intersectedWall = false;

    while (true) {
        /*if (!Utilities::IsInBounds(sf::Vector2f(rayPos.x + xStep, rayPos.y + yStep), settings))
            break;*/

        if (firstIter) {
            rayPos.x += dx;
            rayPos.y += dy;
            firstIter = false;
        }
        else {
            rayPos.x += xStep;
            rayPos.y += yStep;
        }

        int tX = (((int)rayPos.x / settings.tileSize)) * settings.tileSize + (settings.tileSize / 2.0f);
        int tY = (((int)rayPos.y / settings.tileSize)) * settings.tileSize + (settings.tileSize / 2.0f);

        int tileCoordX = (((int)rayPos.x / settings.tileSize));
        int tileCoordY = (((int)rayPos.y / settings.tileSize));

        //if we are looking left
        if ((rotation >= 90 && rotation <= 270) && tileCoordX + 1 < settings.gridSize) {
            tileCoordX--;
            tX -= settings.tileSize * 1;
        }


        //Utilities::PrintVector(sf::Vector2i(tileCoordX, tileCoordY));
        //Utilities::DrawCircle(sf::Vector2f(tX, tY), sf::Color::Green, settings, window);
        //Utilities::DrawCircle(rayPos, sf::Color::Magenta, settings, window);

        if (tileCoordX > settings.gridSize - 1 || tileCoordY > settings.gridSize - 1 || tileCoordX < 0 || tileCoordY < 0)
            break;

        if (map[tileCoordX][tileCoordY] != 0) {
            intersectedWall = true;
            break;
        }

        //numIntersections++;
    }


    //Utilities::DrawCircle(rayPos, sf::Color::Magenta, settings, window);

    //cout << numIntersections << ", " << intersectedWall << endl;

    //return -1 if ray never intersected a wall
    return (intersectedWall) ? rayPos : sf::Vector2f(-1.0f, -1.0f);
}

sf::Vector2<float> Player::GetFirstIntersection(std::vector<std::vector<int>> mapVector, float rotation) {
    sf::Vector2f closestHorizIntersect = GetFirstHorizontalIntersection(mapVector, rotation);
    sf::Vector2f closestVertIntersect = GetFirstVerticalIntersection(mapVector, rotation);

    if (closestVertIntersect.x < 0 && closestHorizIntersect.x < 0)
        return sf::Vector2f(-1.0f, -1.0f);

    //if no intersection, set it to be a massive number so its magnitude is always larger than the other vector, and thus the other vector gets chosen
    if (closestHorizIntersect.x < 0)
        closestHorizIntersect = sf::Vector2f(9999, 9999);

    if (closestVertIntersect.x < 0)
        closestVertIntersect = sf::Vector2f(9999, 9999);

    sf::Vector2f closestPoint = sf::Magnitude(closestHorizIntersect - position) < sf::Magnitude(closestVertIntersect - position) ? closestHorizIntersect : closestVertIntersect;

    
    return closestPoint;
}
