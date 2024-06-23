#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

template<typename T>
void DrawLine(sf::Vector2<T> start, sf::Vector2<T> end, sf::Color color, sf::RenderWindow& window) {
    sf::Vertex line[2];

    //draw view ray
    line[0].position = start;
    line[1].position = end;

    for (int i = 0; i <= 1; i++) {
        line[i].color = color;
        //line[i].position.x += windowSize.x / 2;
        //line[i].position.y += windowSize.y / 2;
    }
    window.draw(line, 2, sf::Lines);
}

inline sf::Vector2<float> TransformWorldSpaceToScreenSpace(sf::Vector2<float> worldCoord, int tileSize, int gridSize, sf::Vector2<int> windowSize) {
    float x = worldCoord.x  +   (windowSize.x / 2)       -    (gridSize / 2 * tileSize);
    float y = worldCoord.y  +   (windowSize.y / 2)       -    (gridSize / 2 * tileSize);

    return sf::Vector2(x, y);
}