#include "Utilities.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

sf::Vector2<float> Utilities::TransformWorldSpaceToScreenSpace(sf::Vector2<float> worldCoord, Utilities::DisplaySettings s) {
    //                        move 0, 0 to window center       put grid center at center of screen
    float x = worldCoord.x  +   (s.windowSize.x / 2)       -    (s.gridSize / 2 * s.tileSize);
    float y = worldCoord.y  +   (s.windowSize.y / 2)       -    (s.gridSize / 2 * s.tileSize);

    return sf::Vector2f(x, y);
}
