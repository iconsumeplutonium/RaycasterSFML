#include <SFML/Graphics.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Player.h"
#include <filesystem>
#include "Utilities.h"
#include <vector>

using namespace std;

//int tileSize = 50;
//int gridSize = 10;
//sf::Vector2i windowSize(1280, 720);
Utilities::DisplaySettings settings;
sf::Vector2 playerPos(0, 0);
bool isFocus = true;

std::vector<std::vector<int>> mapVector = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


void GenerateGrid(sf::RenderWindow& window);
void DrawViews(Player player, sf::RenderWindow& window);

int main() {
    settings.tileSize = 50;
    settings.gridSize = 10;
    settings.windowSize = sf::Vector2i(1280, 720);

    sf::RenderWindow window(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Raycaster");
    Player player(settings, 2.0f, 100.0f, &window);
    
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
    tile.setSize(sf::Vector2f(settings.tileSize, settings.tileSize));
    tile.setFillColor(sf::Color::Transparent);
    tile.setOutlineColor(sf::Color::White);
    tile.setOutlineThickness(1.0f);

    for (int tileY = 0; tileY < settings.gridSize; tileY++) {
        for (int tileX = 0; tileX < settings.gridSize; tileX++) {
            tile.setFillColor(mapVector[tileX][tileY] == 0 ? sf::Color::Transparent : sf::Color::Blue);
            
            sf::Vector2 a = Utilities::TransformWorldSpaceToScreenSpace(sf::Vector2((float)tileX * settings.tileSize, (float)tileY * settings.tileSize), settings);
            tile.setPosition(a.x, a.y);
            window.draw(tile);
        }
    }
}

void DrawViews(Player player, sf::RenderWindow& window) {
    sf::Vertex line[2];
    float radius = 1000.0f;

    //draw view ray
    sf::Vector2 pos = player.position;
    sf::Vector2 end = sf::Vector2(
        (float) (player.position.x + radius * cos(player.rotation * (M_PI / 180.0))),
        (float) (player.position.y + radius * sin(player.rotation * (M_PI / 180.0)))
    );

    pos = Utilities::TransformWorldSpaceToScreenSpace(pos, settings);
    end = Utilities::TransformWorldSpaceToScreenSpace(end, settings);
    Utilities::DrawLine(pos, end, sf::Color::Yellow, window);


    //draw first horizontal intersection
    end = player.GetFirstHorizontalIntersection(mapVector);
    if (end.x < 0.0f)
        return;

    end = Utilities::TransformWorldSpaceToScreenSpace(end, settings);

    
    Utilities::DrawLine(pos, end, sf::Color::Magenta, window);
    
}