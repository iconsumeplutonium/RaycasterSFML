#include "Utilities.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

sf::Vector2<float> Utilities::TransformWorldSpaceToScreenSpace(sf::Vector2<float> worldCoord, Utilities::DisplaySettings s) {
    //                        move 0, 0 to window center       put grid center at center of screen
    float x = worldCoord.x  +   (s.windowSize.x / 2)       -    (s.gridSize / 2 * s.tileSize);
    float y = worldCoord.y  +   (s.windowSize.y / 2)       -    (s.gridSize / 2 * s.tileSize);

    return sf::Vector2f(x, y);
}

void Utilities::DrawLine(sf::Vector2f start, sf::Vector2f end, sf::Color color, sf::RenderWindow& window, Utilities::DisplaySettings s) {
    sf::Vertex line[2];

    //draw view ray
    line[0].position = start;
    line[1].position = end;

    for (int i = 0; i <= 1; i++) {
        line[i].color = color;
        line[i].position = Utilities::TransformWorldSpaceToScreenSpace(line[i].position, s);
    }
    window.draw(line, 2, sf::Lines);
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

void Utilities::DrawColumn(float xCoord, int height, int columnWidth, sf::Color c, Utilities::DisplaySettings s, sf::RenderWindow& window) {
    sf::Vector2f topLeft     = sf::Vector2f(xCoord - (columnWidth / 2), (s.windowSize.y / 2) + height / 2);
    sf::Vector2f topRight    = sf::Vector2f(xCoord + (columnWidth / 2), (s.windowSize.y / 2) + height / 2);
    sf::Vector2f bottomLeft  = sf::Vector2f(xCoord - (columnWidth / 2), (s.windowSize.y / 2) - height / 2);
    sf::Vector2f bottomRight = sf::Vector2f(xCoord + (columnWidth / 2), (s.windowSize.y / 2) - height / 2);

    sf::ConvexShape quad;
    quad.setPointCount(4);

    quad.setPoint(0, topLeft);
    quad.setPoint(1, topRight);
    quad.setPoint(2, bottomRight);
    quad.setPoint(3, bottomLeft);

    quad.setFillColor(c);

    window.draw(quad);
}