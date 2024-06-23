#include "Utilities.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

sf::Vector2<float> Utilities::TransformWorldSpaceToScreenSpace(sf::Vector2<float> worldCoord, Utilities::DisplaySettings s) {
    //                        move 0, 0 to window center       put grid center at center of screen
    float x = worldCoord.x  +   (s.windowSize.x / 2)       -    (s.gridSize / 2 * s.tileSize);
    float y = worldCoord.y  +   (s.windowSize.y / 2)       -    (s.gridSize / 2 * s.tileSize);

    return sf::Vector2f(x, y);
}


void Utilities::DrawCircle(sf::Vector2f coord, sf::Color c, Utilities::DisplaySettings s, sf::RenderWindow* window) {
    sf::CircleShape circle = sf::CircleShape(5);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(c);
    circle.setOutlineThickness(3.0f);
    circle.setOrigin(circle.getRadius(), circle.getRadius());

    sf::Vector2f screenCoords = Utilities::TransformWorldSpaceToScreenSpace(coord, s);
    circle.setPosition(screenCoords.x, screenCoords.y);

    window->draw(circle);
}

bool Utilities::IsInBounds(sf::Vector2f v, Utilities::DisplaySettings s) {
    return !(v.x < 0 || v.y < 0 || v.x > (s.tileSize * s.gridSize) || v.y > (s.tileSize * s.gridSize));
}