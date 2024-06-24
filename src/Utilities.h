#ifndef UTILITIES_H
#define UTILITIES_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace Utilities {
    struct DisplaySettings {
        int tileSize;
        int gridSize;
        sf::Vector2i windowSize;
    };

    template<typename T>
    void DrawLine(sf::Vector2<T> start, sf::Vector2<T> end, sf::Color color, sf::RenderWindow& window, DisplaySettings s) {
        sf::Vertex line[2];

        //draw view ray
        line[0].position = start;
        line[1].position = end;

        for (int i = 0; i <= 1; i++) {
            line[i].color = color;
            line[i].position = TransformWorldSpaceToScreenSpace(line[i].position, s);
        }
        window.draw(line, 2, sf::Lines);
    }

	sf::Vector2<float> TransformWorldSpaceToScreenSpace(sf::Vector2<float> worldCoord, Utilities::DisplaySettings s);

    //@brief Takes in a world space coordinate and displays a circle in screen space coords
    void DrawCircle(sf::Vector2f coord, sf::Color c, Utilities::DisplaySettings s, sf::RenderWindow* window);
    
    bool IsInBounds(sf::Vector2f v, Utilities::DisplaySettings s);
}

//overloading SFML's vector2 operators because it just doesnt do this already for some reason
//modifying the namespace directly because screw you
namespace sf {
    template<typename T>
    inline std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T> v) {
        return stream << "(" << v.x << ", " << v.y << ")";
    }

    template<typename T>
    inline float Magnitude(const sf::Vector2<T> v) {
        return sqrt(v.x * v.x + v.y * v.y);
    }
}

#endif