#include "Utilities.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

sf::Vector2<float> Utilities::TransformWorldSpaceToScreenSpace(sf::Vector2<float> worldCoord, int tileSize, int gridSize, sf::Vector2<int> windowSize) {
    float x = worldCoord.x  +   (windowSize.x / 2)       -    (gridSize / 2 * tileSize);
    float y = worldCoord.y  +   (windowSize.y / 2)       -    (gridSize / 2 * tileSize);

    return sf::Vector2f(x, y);
}
