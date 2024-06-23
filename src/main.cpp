#include <SFML/Graphics.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Player.h"
#include <filesystem>
#include "Utilities.h"

using namespace std;

int tileSize = 50;
int gridSize = 10;
sf::Vector2i windowSize(1280, 720);
Utilities::displaySettings settings;
sf::Vector2 playerPos(0, 0);
bool isFocus = true;

int map[10][10] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

void GenerateGrid(sf::RenderWindow& window);
void DrawViews(Player player, sf::RenderWindow& window);

int main() {
    //settings.tileSize = 50;
    //settings.gridSize = 10;
    //settings.windowSize = sf::Vector2i(1280, 720);

    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Raycaster");
    Player player(windowSize, 20.0f, 100.0f, tileSize, gridSize);
    
    sf::Clock clock;
    sf::Font font;
    if (!font.loadFromFile("retro_computer.ttf")) {
        cout << "font not loaded" << endl;
        std::string currentDir = std::filesystem::current_path().string();
        std::cout << "Current working directory: " << currentDir << std::endl;
    }
    sf::Text debugText;
    debugText.setFont(font);
    debugText.setCharacterSize(15);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition(sf::Vector2(10.0f, 10.0f));


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::GainedFocus)
                isFocus = true;
            
            if (event.type == sf::Event::LostFocus)
                isFocus = false;

            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Time deltaTime = clock.restart();
        window.clear();
        GenerateGrid(window);
        window.draw(player.body);

        if (isFocus) {
            player.UpdateRotation(deltaTime.asSeconds());
            player.UpdatePosition(deltaTime.asSeconds());
            DrawViews(player, window);
        }

        debugText.setString(player.DebugStatistics());
        window.draw(debugText);
        window.display();
    }                    

    return 0;
}

void GenerateGrid(sf::RenderWindow& window) {
    //sf::Vector2 startCoord(-(gridSize / 2.0f) * tileSize, -(gridSize / 2.0f) * tileSize);
    sf::Vector2 startCoord(0, 0);
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f(tileSize, tileSize));
    tile.setFillColor(sf::Color::Transparent);
    tile.setOutlineColor(sf::Color::White);
    tile.setOutlineThickness(1.0f);

    for (int tileY = 0; tileY < gridSize; tileY++) {
        for (int tileX = 0; tileX < gridSize; tileX++) {
            
            if (::map[tileX][tileY] == 0) {
                tile.setFillColor(sf::Color::Transparent);
                //tile.setOutlineColor(sf::Color::White);
            } else {
                tile.setFillColor(sf::Color::Blue);
                //tile.setOutlineColor(sf::Color::Blue);
            }

            //tile coord -> world coord   move 0,0 to window center       put grid center at center of screen
            //int x = (tileX * tileSize);//  +   (windowSize.x / 2)       -    (gridSize / 2 * tileSize);
            //int y = (tileY * tileSize);//  +   (windowSize.y / 2)       -    (gridSize / 2 * tileSize);
            sf::Vector2 a = Utilities::TransformWorldSpaceToScreenSpace(sf::Vector2((float)tileX * tileSize, (float)tileY * tileSize), tileSize, gridSize, windowSize);

            tile.setPosition(a.x, a.y);
            window.draw(tile);
        }
    }
}

void DrawViews(Player player, sf::RenderWindow& window) {
    sf::Vertex line[2];
    float radius = 1000.0f;

    //draw view ray
    //sf::Vector2 pos = sf::Vector2(player.position.x, player.position.y);
    sf::Vector2 pos = player.position;
    sf::Vector2 end = sf::Vector2(
        (float) (player.position.x + radius * cos(player.rotation * (M_PI / 180.0))),
        (float) (player.position.y + radius * sin(player.rotation * (M_PI / 180.0)))
    );

    pos = Utilities::TransformWorldSpaceToScreenSpace(pos, tileSize, gridSize, windowSize);
    end = Utilities::TransformWorldSpaceToScreenSpace(end, tileSize, gridSize, windowSize);
    Utilities::DrawLine(pos, end, sf::Color::Yellow, window);


    //draw first horizontal intersection
    end = player.GetFirstHorizontalIntersection(tileSize);
    end = Utilities::TransformWorldSpaceToScreenSpace(end, tileSize, gridSize, windowSize);


    Utilities::DrawLine(pos, end, sf::Color::Magenta, window);
    
}