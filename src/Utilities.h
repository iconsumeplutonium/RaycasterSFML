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

    typedef enum class RenderMode {
        TOPDOWN, FIRSTPERSON, EMPTYDEBUG
    };

    void DrawLine(sf::Vector2f start, sf::Vector2f end, sf::Color color, sf::RenderWindow& window, DisplaySettings s);

	sf::Vector2<float> TransformWorldSpaceToScreenSpace(sf::Vector2<float> worldCoord, Utilities::DisplaySettings s);

    //@brief Takes in a world space coordinate and displays a circle in screen space coords
    void DrawCircle(sf::Vector2f coord, sf::Color c, Utilities::DisplaySettings s, sf::RenderWindow* window);
    void DrawColumn(float xCoord, int height, int columnWidth, sf::Color c, Utilities::DisplaySettings s, sf::RenderWindow& window);
    
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

    inline sf::Color operator*(const sf::Color& color, const float v) {
        return sf::Color::Color(color.r * v, color.g * v, color.b * v);
    }
}

#endif