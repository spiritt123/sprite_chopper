#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "frame.h"
#include "sprite_chopper.h"

sf::Vector2f operator*(const sf::Vector2f &left, const sf::Vector2f &right)
{
    return sf::Vector2f(left.x * right.x, left.y * right.y);
}

void renderRect(sf::RenderWindow &window, const Frame &frame);

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(900, 900), "Frame_chopper");
    window.setFramerateLimit(30);

    std::string filename = "frames.coords";
    SpriteChopper sc(filename);
    sf::Texture texture;
    if (!texture.loadFromFile("sprite.png"))
    {
        // error...
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Vector2f mouse_position; 
    Frame current_rect; 
    bool is_select_area = false;
    
    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2i position = sf::Mouse::getPosition(window);
        sc.updateMousePosition({position.x, position.y});

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();             
                else if (event.key.code == sf::Keyboard::S) sc.save();
                else if (event.key.code == sf::Keyboard::R) sc.load();
                else if (event.key.code == sf::Keyboard::D) sc.deleteSelectedFrames();
                else if (event.key.shift && event.key.code == sf::Keyboard::Up)    sc.moveSizePointInSelectedFrames({0, -1});
                else if (event.key.shift && event.key.code == sf::Keyboard::Down)  sc.moveSizePointInSelectedFrames({0,  1});
                else if (event.key.shift && event.key.code == sf::Keyboard::Left)  sc.moveSizePointInSelectedFrames({-1, 0});
                else if (event.key.shift && event.key.code == sf::Keyboard::Right) sc.moveSizePointInSelectedFrames({ 1, 0});
                else if (event.key.code == sf::Keyboard::Up)    sc.movePositionPointInSelectedFrames({0, -1});
                else if (event.key.code == sf::Keyboard::Down)  sc.movePositionPointInSelectedFrames({0,  1});
                else if (event.key.code == sf::Keyboard::Left)  sc.movePositionPointInSelectedFrames({-1, 0});
                else if (event.key.code == sf::Keyboard::Right) sc.movePositionPointInSelectedFrames({ 1, 0});
                else if (event.key.code == sf::Keyboard::K) sc.moveSelectedFrames({0, -1});
                else if (event.key.code == sf::Keyboard::J) sc.moveSelectedFrames({0,  1});
                else if (event.key.code == sf::Keyboard::H) sc.moveSelectedFrames({-1, 0});
                else if (event.key.code == sf::Keyboard::L) sc.moveSelectedFrames({ 1, 0});
            }
            if (event.type == sf::Event::MouseButtonPressed) sc.mousePressed();
            if (event.type == sf::Event::MouseButtonReleased) sc.mouseReleased();
        }

        window.clear();
        window.draw(sprite);

        if (sc.isSelectStatus())
            renderRect(window, sc.getCurrentFrame());
        for (const auto & s : sc.getFrames())
        {
            renderRect(window, s);
        }

        window.display();
    }

    return 0;
}

void renderRect(sf::RenderWindow &window, const Frame &frame)
{
    sf::Vector2f position(frame.position.x, frame.position.y);
    sf::Vector2f size(frame.size.x, frame.size.y);

    sf::RectangleShape line;
    if (frame.status == STATUS::PASSIVE)
    {
        line.setFillColor(sf::Color::Red);
    }
    else if (frame.status == STATUS::ACTIVE)
    {
        line.setFillColor(sf::Color::Green);
    }


    //up
    line.setPosition(position);
    line.setSize(size * sf::Vector2f(1, 0) + sf::Vector2f(5, 5));
    window.draw(line);

    //left
    line.setPosition(position);
    line.setSize(size * sf::Vector2f(0, 1) + sf::Vector2f(5, 5));
    window.draw(line);

    //down
    line.setPosition(position + size * sf::Vector2f(0, 1));
    line.setSize(size * sf::Vector2f(1, 0) + sf::Vector2f(5, 5));
    window.draw(line);

    //right
    line.setPosition(position + size * sf::Vector2f(1, 0));
    line.setSize(size * sf::Vector2f(0, 1) + sf::Vector2f(5, 5));
    window.draw(line);
}




