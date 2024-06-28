#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>
#include "Utilities.h"
#include "Player.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <list>

using namespace std;

int defaultFOV = 80;
int raysPerDegree = 8;
Utilities::DisplaySettings settings;
bool isFocus = true;
Utilities::RenderMode mode = Utilities::RenderMode::FIRSTPERSON;

sf::RenderWindow* window;
sf::RenderWindow* window2;
sf::Text debugText;
sf::RectangleShape ceilingRect;
sf::RectangleShape floorRect;

std::vector<std::vector<int>> mapVector;/* = {
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
};*/


void GenerateGrid(sf::RenderWindow* window);
void DrawViews(Player player, sf::RenderWindow* window);
void DrawViews2(Player player, sf::RenderWindow* window);
void UpdateTopDownWindow(Player& player, sf::Clock& clock);
void UpdateFirstPersonWindow(Player& player, sf::Clock& clock);
void DrawGroundAndCeiling(sf::RenderWindow* targetWindow);

int main() {
    settings.tileSize = 50;
    settings.gridSize = 10;
    settings.windowSize = sf::Vector2i(1280, 720);
    mapVector = Utilities::LoadMap();

    window = new sf::RenderWindow(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Raycaster (Top Down)");
    window2 = new sf::RenderWindow(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Raycaster (First Person)");
    
    Player player(settings, 20.0f, 100.0f, defaultFOV, window, mapVector);
    

    sf::Clock clock;
    sf::Font font;
    if (!font.loadFromFile("fonts/ticketing.regular.ttf")) {
        cout << "font not loaded" << endl;
        std::string currentDir = std::filesystem::current_path().string();
        std::cout << "Current working directory: " << currentDir << std::endl;
    }
    
    debugText.setFont(font);
    debugText.setCharacterSize(23);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition(sf::Vector2(10.0f, 10.0f));

    sf::Color gray = sf::Color::Color(128, 128, 128);

    ceilingRect.setPosition(sf::Vector2f(0, 0));
    ceilingRect.setSize(sf::Vector2f(settings.windowSize.x, settings.windowSize.y / 2));
    ceilingRect.setFillColor(gray);

    floorRect.setPosition(sf::Vector2f(0, settings.windowSize.y / 2.0f));
    floorRect.setSize(sf::Vector2f(settings.windowSize.x, settings.windowSize.y / 2));
    floorRect.setFillColor(gray * 0.5f);

    if (mode == Utilities::RenderMode::TOPDOWN) {
        //window = new sf::RenderWindow(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Raycaster (Top Down)");
        while (window->isOpen())
            UpdateTopDownWindow(player, clock);
        
    }
    else if (mode == Utilities::RenderMode::FIRSTPERSON) {
        //window2 = new sf::RenderWindow(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Raycaster (First Person)");
        while (window2->isOpen()) {
            UpdateFirstPersonWindow(player, clock);
        }
    }
    else if (mode == Utilities::RenderMode::DOUBLEVIEW) {

        while (window->isOpen() && window2->isOpen()) {
            UpdateTopDownWindow(player, clock);
            UpdateFirstPersonWindow(player, clock);
        }
    }
    else {
        while (window->isOpen()) {
            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::GainedFocus)
                    isFocus = true;

                if (event.type == sf::Event::LostFocus)
                    isFocus = false;

                if (event.type == sf::Event::Closed)
                    window->close();
            }

            window->clear();

            float columnWidth = 2.0f;
            Utilities::DrawColumn(640, 50, columnWidth, sf::Color::Blue, settings, window);
            //Utilities::DrawColumn(640 + columnWidth, 100, columnWidth, sf::Color::Red, settings, window);

            window->display();
        }
    }
    
    return 0;
}

void UpdateTopDownWindow(Player& player, sf::Clock& clock) {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::GainedFocus)
            isFocus = true;

        if (event.type == sf::Event::LostFocus)
            isFocus = false;

        if (event.type == sf::Event::Closed)
            window->close();
    }
    sf::Time deltaTime = clock.restart();
    window->clear();
    GenerateGrid(window);


    if (isFocus) {
        player.Update(deltaTime.asSeconds()); //todo: this should happen only once if DOUBLEVIEW is enabled
        player.UpdateBodyDisplay();
        //DrawViews(player, window);
        DrawViews2(player, window);
    }

    window->draw(player.body);
    debugText.setString(player.DebugStatistics());
    window->draw(debugText);
    window->display();
}

void UpdateFirstPersonWindow(Player& player, sf::Clock& clock) {
    sf::Event event;
    while (window2->pollEvent(event)) {
        if (event.type == sf::Event::GainedFocus)
            isFocus = true;

        if (event.type == sf::Event::LostFocus)
            isFocus = false;

        if (event.type == sf::Event::Closed)
            window2->close();
    }

    sf::Time deltaTime = clock.restart();
    window2->clear();
    window2->draw(ceilingRect);
    window2->draw(floorRect);
    std::vector<sf::Vertex> vertices;

    if (isFocus) {
        float delta = deltaTime.asSeconds();
        player.Update(delta);

        int numRays = player.FOV * raysPerDegree;
        float columnWidth = ((float)settings.windowSize.x) / numRays;
        if (columnWidth < 2.0f)
            columnWidth = 2.0f;
        float x = 0.0f;

        
        for (float i = player.rotation - player.FOV / 2; i < player.rotation + player.FOV / 2; i += 0.1f) {
            float rot = i;

            float theta = abs(rot - player.rotation);
            if (rot < 0)
                rot += 360;
            if (rot > 360)
                rot -= 360;



            bool wallWasHorizontal;
            sf::Vector2f intersection = player.GetFirstIntersection(mapVector, rot, wallWasHorizontal);

            float columnHeight = (settings.windowSize.y * 50) / (sf::Magnitude(intersection - player.position) * cos(theta * M_PI / 180.0f));
            //Utilities::DrawColumn(x, columnHeight, columnWidth, sf::Color::Color(0, 0, 255) * (wallWasHorizontal ? 0.5f : 1.0f), settings, window2);

            sf::Vertex topLeft, topRight, bottomLeft, bottomRight;
            sf::Color wallColor = sf::Color::Color(0, 0, 255) * (wallWasHorizontal ? 0.5f : 1.0f);

            topLeft.position     = sf::Vector2f(x - (columnWidth / 2), (settings.windowSize.y / 2) + columnHeight / 2);
            topRight.position    = sf::Vector2f(x + (columnWidth / 2), (settings.windowSize.y / 2) + columnHeight / 2);
            bottomLeft.position  = sf::Vector2f(x - (columnWidth / 2), (settings.windowSize.y / 2) - columnHeight / 2);
            bottomRight.position = sf::Vector2f(x + (columnWidth / 2), (settings.windowSize.y / 2) - columnHeight / 2);

            topLeft.color     = wallColor;
            topRight.color    = wallColor;
            bottomLeft.color  = wallColor;
            bottomRight.color = wallColor;

            vertices.push_back(topLeft);
            vertices.push_back(topRight);
            vertices.push_back(bottomLeft);

            vertices.push_back(bottomLeft);
            vertices.push_back(topRight);
            vertices.push_back(bottomRight);

            x += columnWidth;
        }

        window2->draw(&vertices[0], vertices.size(), sf::Triangles);
        debugText.setString(to_string(int(1.0f / delta)));
        window2->draw(debugText);
    }
    
    window2->display();
}

void GenerateGrid(sf::RenderWindow* window) {
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
            window->draw(tile);
        }
    }
}

void DrawViews(Player player, sf::RenderWindow* window) {
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

    Utilities::DrawCircle(closestPoint, sf::Color::Green, settings, window);    
    Utilities::DrawLine(pos, closestPoint, sf::Color::Magenta, window, settings);
}

void DrawViews2(Player player, sf::RenderWindow* window) {
    //draw view ray
    /*float radius = 100.0f;
    sf::Vector2 pos = player.position;
    sf::Vector2 end = sf::Vector2(
        (float)(player.position.x + radius * cos(player.rotation * (M_PI / 180.0))),
        (float)(player.position.y + radius * sin(player.rotation * (M_PI / 180.0)))
    );

    Utilities::DrawLine(pos, end, sf::Color::Yellow, window, settings);*/

    for (float i = player.rotation - player.FOV / 2; i < player.rotation + player.FOV / 2; i += 1.0f / raysPerDegree) {
        float rot = i;
        if (rot < 0)
            rot += 360;
        if (rot > 360)
            rot -= 360;

        bool wallWasHorizontal;
        sf::Vector2f intersection = player.GetFirstIntersection(mapVector, rot, wallWasHorizontal);
        Utilities::DrawLine(player.position, intersection, sf::Color::Magenta, window, settings);
    }
}

void DrawGroundAndCeiling(sf::RenderWindow* targetWindow) {
    

    //targetWindow->draw(ceiling);
    //targetWindow->draw(floor);

}