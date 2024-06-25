#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>
#include "Utilities.h"
#include "Player.h"
#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;

int FOV = 70;
Utilities::DisplaySettings settings;
bool isFocus = true;
Utilities::RenderMode mode = Utilities::RenderMode::FIRSTPERSON;

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
void DrawViews2(int FOV, Player player, sf::RenderWindow& window);

int main() {
    settings.tileSize = 50;
    settings.gridSize = 10;
    settings.windowSize = sf::Vector2i(1280, 720);

    sf::RenderWindow window(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Raycaster");
    Player player(settings, 20.0f, 100.0f, FOV, &window);

    sf::Clock clock;

    if (mode == Utilities::RenderMode::TOPDOWN) {
        sf::Font font;
        if (!font.loadFromFile("fonts/ticketing.regular.ttf")) {
            cout << "font not loaded" << endl;
            std::string currentDir = std::filesystem::current_path().string();
            std::cout << "Current working directory: " << currentDir << std::endl;
        }
        sf::Text debugText;
        debugText.setFont(font);
        debugText.setCharacterSize(23);
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
                player.Update(deltaTime.asSeconds());
                //DrawViews(player, window);
                DrawViews2(FOV, player, window);
            }

            debugText.setString(player.DebugStatistics());
            window.draw(debugText);
            window.display();
        }
    }
    else if (mode == Utilities::RenderMode::FIRSTPERSON) {
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
            //GenerateGrid(window);

            if (isFocus) {
                player.Update(deltaTime.asSeconds());
                int x = (settings.windowSize.x / 2) - (player.FOV / 2);
                
                for (int i = player.rotation - player.FOV / 2; i < player.rotation + player.FOV / 2; i++) {
                    float rot = i;
                    if (rot < 0)
                        rot += 360;
                    if (rot > 360)
                        rot -= 360;

                    sf::Vector2f intersection = player.GetFirstIntersection(mapVector, rot);
                    float columnHeight = sf::Magnitude(intersection);
                    //Utilities::DrawLine(player.position, intersection, sf::Color::Magenta, window, settings);
                    Utilities::DrawColumn(x, columnHeight, 200, sf::Color::Blue, settings, window);

                    x++;
                }


            }

            window.display();
        }
    }
    else {
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

            window.clear();

            Utilities::DrawColumn(settings.windowSize.x / 2, 50, 10, sf::Color::Blue, settings, window);

            window.display();
        }
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

    Utilities::DrawLine(pos, end, sf::Color::Yellow, window, settings);


    //get intersections
    sf::Vector2f closestHorizIntersect = player.GetFirstHorizontalIntersection(mapVector, player.rotation);
    sf::Vector2f closestVertIntersect = player.GetFirstVerticalIntersection(mapVector, player.rotation);

    if (closestVertIntersect.x < 0 && closestHorizIntersect.x < 0)
        return;

    //if no intersection, set it to be a massive number so its magnitude is always larger than the other vector, and thus the other vector gets chosen
    if (closestHorizIntersect.x < 0)
        closestHorizIntersect = sf::Vector2f(9999, 9999);

    if (closestVertIntersect.x < 0)
        closestVertIntersect = sf::Vector2f(9999, 9999);


    sf::Vector2f closestPoint = sf::Magnitude(closestHorizIntersect - player.position) < sf::Magnitude(closestVertIntersect - player.position) ? closestHorizIntersect : closestVertIntersect;
   // cout << sf::Magnitude(closestHorizIntersect - player.position) << ",  " << sf::Magnitude(closestVertIntersect - player.position) << endl;


    /*Utilities::DrawCircle(closestHorizIntersect, sf::Color::Magenta, settings, &window);
    Utilities::DrawCircle(closestVertIntersect, sf::Color::Magenta, settings, &window);*/

    Utilities::DrawCircle(closestPoint, sf::Color::Green, settings, &window);    
    Utilities::DrawLine(pos, closestPoint, sf::Color::Magenta, window, settings);
}

void DrawViews2(int FOV, Player player, sf::RenderWindow& window) {
    for (int i = player.rotation - player.FOV / 2; i < player.rotation + player.FOV / 2; i++) {
        float rot = i;
        if (rot < 0)
            rot += 360;
        if (rot > 360)
            rot -= 360;

        sf::Vector2f intersection = player.GetFirstIntersection(mapVector, rot);
        Utilities::DrawLine(player.position, intersection, sf::Color::Magenta, window, settings);
    }
}